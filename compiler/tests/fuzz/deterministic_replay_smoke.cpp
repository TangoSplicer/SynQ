#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "compiler/bounded_evaluator.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

constexpr std::size_t kMaxCorpusBytes = 4U * 1024U;
constexpr std::uint64_t kFnvOffsetBasis = 1469598103934665603ULL;
constexpr std::uint64_t kFnvPrime = 1099511628211ULL;

constexpr std::uint32_t kReplaySeeds[] = {
    0x00000001U,
    0x9e3779b9U,
    0x7f4a7c15U,
    0x243f6a88U,
    0xb7e15162U,
    0xdeadbeefU,
    0xc001d00dU,
    0xf00dbabeU,
};

struct ReplayOutcome {
    std::string stage;
    bool accepted = false;
    std::string transcript;
};

struct ReplaySelector {
    std::optional<std::string> case_name;
    std::optional<std::uint32_t> seed;
};

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

std::string trim(std::string value) {
    while (!value.empty() && (value.back() == '\r' || value.back() == ' ' || value.back() == '\t')) {
        value.pop_back();
    }
    std::size_t start = 0;
    while (start < value.size() && (value[start] == ' ' || value[start] == '\t')) {
        ++start;
    }
    return value.substr(start);
}

std::string read_file(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) return {};
    return std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
}

bool read_manifest(const std::filesystem::path& corpus_directory, std::vector<std::string>& entries) {
    const std::filesystem::path manifest_path = corpus_directory / "manifest.txt";
    std::ifstream manifest(manifest_path);
    if (!require(static_cast<bool>(manifest), "opens fixed replay corpus manifest: " + manifest_path.string())) return false;

    std::string line;
    while (std::getline(manifest, line)) {
        const std::string entry = trim(line);
        if (entry.empty() || entry.front() == '#') continue;
        const std::filesystem::path entry_path(entry);
        if (!require(entry_path.filename() == entry_path && entry_path.extension() == ".synq",
                     "manifest uses simple .synq file names only: " + entry)) {
            return false;
        }
        for (const std::string& existing : entries) {
            if (!require(existing != entry, "manifest has no duplicate source: " + entry)) return false;
        }
        entries.push_back(entry);
    }
    return require(!entries.empty(), "manifest has at least one replay source");
}

std::uint32_t next_random(std::uint32_t& state) {
    state ^= state << 13U;
    state ^= state >> 17U;
    state ^= state << 5U;
    return state;
}

char mutation_byte(std::uint32_t& state) {
    return static_cast<char>(0x20U + (next_random(state) % 95U));
}

std::string mutate_source(std::string source, std::uint32_t seed) {
    std::uint32_t state = seed == 0U ? 0x6d2b79f5U : seed;
    if (source.empty()) return std::string(1, mutation_byte(state));

    const std::size_t index = static_cast<std::size_t>(next_random(state)) % source.size();
    const std::uint32_t operation = next_random(state) % 3U;
    if (operation == 0U) {
        source[index] = mutation_byte(state);
    } else if (operation == 1U) {
        source.erase(index, 1U);
    } else if (source.size() < kMaxCorpusBytes) {
        source.insert(source.begin() + static_cast<std::ptrdiff_t>(index), mutation_byte(state));
    } else {
        source[index] = mutation_byte(state);
    }
    return source;
}

void append_diagnostics(std::ostringstream& output, const std::vector<synq::compiler::Diagnostic>& diagnostics) {
    output << diagnostics.size() << ':';
    for (const synq::compiler::Diagnostic& diagnostic : diagnostics) {
        output << diagnostic.code << ':' << synq::compiler::diagnostic_severity_name(diagnostic.severity) << ':'
               << diagnostic.span.line << ':' << diagnostic.span.column_start << ':' << diagnostic.span.column_end << ';';
    }
}

void append_value(std::ostringstream& output, const synq::compiler::BoundedValue& value) {
    output << synq::compiler::bounded_value_kind_name(value.kind) << ':';
    if (value.kind == synq::compiler::BoundedValueKind::Integer) {
        output << value.integer_value;
    } else if (value.kind == synq::compiler::BoundedValueKind::Boolean) {
        output << (value.boolean_value ? "true" : "false");
    } else {
        output << value.string_value.size() << ':' << value.string_value;
    }
}

void append_constant_result(std::ostringstream& output, const synq::compiler::BoundedEvaluationResult& result) {
    output << "constants:" << (result.ok() ? "ok:" : "error:");
    if (result.evaluation.has_value()) {
        for (const synq::compiler::EvaluatedBinding& binding : result.evaluation->bindings) {
            output << binding.name.size() << ':' << binding.name << ':';
            append_value(output, binding.value);
            output << ';';
        }
    }
    append_diagnostics(output, result.diagnostics);
}

void append_state_result(std::ostringstream& output, const synq::compiler::BoundedStateEvaluationResult& result) {
    output << "state:" << (result.ok() ? "ok:" : "error:");
    if (result.evaluation.has_value()) {
        for (const synq::compiler::EvaluatedStateCell& cell : result.evaluation->cells) {
            output << cell.name.size() << ':' << cell.name << ':';
            append_value(output, cell.value);
            output << ';';
        }
    }
    append_diagnostics(output, result.diagnostics);
}

void append_runtime_result(std::ostringstream& output, const synq::compiler::BoundedRuntimeEvaluationResult& result) {
    output << "runtime:" << (result.ok() ? "ok:" : "error:");
    if (result.evaluation.has_value()) {
        for (const synq::compiler::EvaluatedBinding& binding : result.evaluation->bindings) {
            output << binding.name.size() << ':' << binding.name << ':';
            append_value(output, binding.value);
            output << ';';
        }
    }
    append_diagnostics(output, result.diagnostics);
}

ReplayOutcome replay_pipeline(const std::string& source) {
    ReplayOutcome outcome;
    std::ostringstream transcript;
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    transcript << "parse:" << (parsed.ok() ? "ok:" : "error:");
    append_diagnostics(transcript, parsed.diagnostics);
    if (!parsed.ok()) {
        outcome.stage = "parse";
        outcome.transcript = transcript.str();
        return outcome;
    }

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    transcript << "lower:" << (lowered.ok() ? "ok:" : "error:");
    append_diagnostics(transcript, lowered.diagnostics);
    if (!lowered.ok()) {
        outcome.stage = "lower";
        outcome.transcript = transcript.str();
        return outcome;
    }

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    transcript << "resolve:" << (resolved.ok() ? "ok:" : "error:");
    append_diagnostics(transcript, resolved.diagnostics);
    if (!resolved.ok()) {
        outcome.stage = "resolve";
        outcome.transcript = transcript.str();
        return outcome;
    }

    synq::compiler::BoundedEvaluationOptions constant_options;
    constant_options.allow_experimental_constant_evaluation = true;
    append_constant_result(transcript, synq::compiler::evaluate_bounded_constants(*resolved.program, constant_options));

    synq::compiler::BoundedStateEvaluationOptions state_options;
    state_options.allow_experimental_state_evaluation = true;
    append_state_result(transcript, synq::compiler::evaluate_bounded_state(*resolved.program, state_options));

    synq::compiler::BoundedRuntimeEvaluationOptions runtime_options;
    runtime_options.allow_experimental_runtime_evaluation = true;
    append_runtime_result(transcript, synq::compiler::evaluate_bounded_runtime(*resolved.program, runtime_options));

    outcome.stage = "evaluation";
    outcome.accepted = true;
    outcome.transcript = transcript.str();
    return outcome;
}

std::uint64_t digest(const ReplayOutcome& outcome) {
    std::uint64_t hash = kFnvOffsetBasis;
    const std::string payload = outcome.stage + ':' + (outcome.accepted ? "accepted:" : "rejected:") + outcome.transcript;
    for (const unsigned char byte : payload) {
        hash ^= byte;
        hash *= kFnvPrime;
    }
    return hash;
}

std::string hex_digest(std::uint64_t value) {
    std::ostringstream output;
    output << "0x" << std::hex << std::setfill('0') << std::setw(16) << value;
    return output.str();
}

bool parse_seed(const std::string& text, std::uint32_t& value) {
    try {
        std::size_t parsed = 0;
        const unsigned long raw = std::stoul(text, &parsed, 0);
        if (parsed != text.size() || raw > 0xffffffffUL) return false;
        value = static_cast<std::uint32_t>(raw);
        return true;
    } catch (...) {
        return false;
    }
}

bool parse_selector(int argc, char** argv, ReplaySelector& selector) {
    for (int index = 2; index < argc; ++index) {
        const std::string argument = argv[index];
        if (argument == "--case") {
            if (!require(index + 1 < argc && !selector.case_name.has_value(),
                         "--case is supplied exactly once with a corpus file name")) {
                return false;
            }
            selector.case_name = argv[++index];
        } else if (argument == "--seed") {
            if (!require(index + 1 < argc && !selector.seed.has_value(),
                         "--seed is supplied exactly once with a uint32 value")) {
                return false;
            }
            std::uint32_t seed = 0;
            if (!require(parse_seed(argv[++index], seed), "--seed parses as an exact uint32 value")) return false;
            selector.seed = seed;
        } else {
            return require(false, "unknown replay argument: " + argument);
        }
    }
    return require(selector.case_name.has_value() == selector.seed.has_value(),
                   "--case and --seed are either both absent or both supplied");
}

bool replay_input(const std::string& case_name, const std::string& seed_label, const std::string& source) {
    const ReplayOutcome first = replay_pipeline(source);
    const ReplayOutcome second = replay_pipeline(source);
    const std::uint64_t first_digest = digest(first);
    const std::uint64_t second_digest = digest(second);
    if (!require(first_digest == second_digest && first.stage == second.stage && first.accepted == second.accepted,
                 "deterministic replay outcome for " + case_name + " seed=" + seed_label)) {
        return false;
    }
    std::cout << "PASS case=" << case_name << " seed=" << seed_label << " stage=" << first.stage
              << " digest=" << hex_digest(first_digest) << "\n";
    return true;
}

bool replay_case(const std::filesystem::path& corpus_directory, const std::string& case_name,
                 const std::optional<std::uint32_t>& selected_seed) {
    const std::filesystem::path source_path = corpus_directory / case_name;
    if (!require(std::filesystem::is_regular_file(source_path), "manifest source exists: " + source_path.string())) return false;
    const std::string source = read_file(source_path);
    if (!require(!source.empty(), "corpus source is readable and nonempty: " + case_name)) return false;
    if (!require(source.size() <= kMaxCorpusBytes, "corpus source is within 4 KiB bound: " + case_name)) return false;
    if (!require(source.find('\0') == std::string::npos, "corpus source contains no NUL byte: " + case_name)) return false;

    if (!replay_input(case_name, "base", source)) return false;
    if (selected_seed.has_value()) {
        return replay_input(case_name, "0x" + [&]() {
                                std::ostringstream output;
                                output << std::hex << *selected_seed;
                                return output.str();
                            }(),
                            mutate_source(source, *selected_seed));
    }
    for (const std::uint32_t seed : kReplaySeeds) {
        std::ostringstream label;
        label << "0x" << std::hex << seed;
        if (!replay_input(case_name, label.str(), mutate_source(source, seed))) return false;
    }
    return true;
}

}  // namespace

int main(int argc, char** argv) {
    if (!require(argc >= 2, "usage: synq_deterministic_replay_smoke <corpus-dir> [--case file.synq --seed uint32]")) return 1;

    ReplaySelector selector;
    if (!parse_selector(argc, argv, selector)) return 1;

    const std::filesystem::path corpus_directory = argv[1];
    std::vector<std::string> entries;
    if (!read_manifest(corpus_directory, entries)) return 1;

    bool selected_case_found = !selector.case_name.has_value();
    for (const std::string& case_name : entries) {
        if (selector.case_name.has_value() && *selector.case_name != case_name) continue;
        selected_case_found = true;
        if (!replay_case(corpus_directory, case_name, selector.seed)) return 1;
    }
    if (!require(selected_case_found, "selected --case appears exactly in the fixed manifest")) return 1;

    std::cout << "SynQ deterministic parser/runtime replay smoke test passed\n";
    return 0;
}
