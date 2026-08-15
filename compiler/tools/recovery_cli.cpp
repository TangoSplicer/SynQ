#include <fstream>
#include <iostream>
#include <optional>
#include <string>

#include "compiler/bounded_evaluator.h"
#include "compiler/diagnostic.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/openqasm3_exporter.h"
#include "compiler/parser.h"

namespace {

enum class Mode {
    Validate,
    EmitOpenQasm,
    EvaluateConstants,
};

struct Command {
    Mode mode = Mode::Validate;
    std::string source_path;
    std::optional<std::string> output_path;
    std::size_t max_declarations = 64;
};

void print_help(std::ostream& output) {
    output << "SynQ recovery-profile command line\n\n"
           << "Usage:\n"
           << "  synqc <source.synq> --validate\n"
           << "  synqc <source.synq> --emit-openqasm [--out <file.qasm>]\n"
           << "  synqc <source.synq> --eval-constants [--max-declarations <n>]\n\n"
           << "Modes:\n"
           << "  --validate        Parse, lower, and resolve the documented bounded profile.\n"
           << "  --emit-openqasm   Emit the supported AST OpenQASM 3 source subset.\n"
           << "  --eval-constants  Explicitly run bounded declaration-only constant evaluation.\n\n"
           << "This command does not execute quantum programs, simulate circuits, submit jobs,\n"
           << "run legacy runtime components, or evaluate general SynQ source.\n";
}

bool parse_positive_size(const std::string& text, std::size_t& value) {
    if (text.empty()) return false;
    value = 0;
    for (char character : text) {
        if (character < '0' || character > '9') return false;
        const std::size_t digit = static_cast<std::size_t>(character - '0');
        if (value > (static_cast<std::size_t>(-1) - digit) / 10) return false;
        value = value * 10 + digit;
    }
    return value > 0;
}

bool parse_command(int argc, char** argv, Command& command, std::string& error) {
    if (argc < 2) {
        error = "a source file and one mode are required";
        return false;
    }
    command.source_path = argv[1];
    bool selected_mode = false;
    for (int index = 2; index < argc; ++index) {
        const std::string argument = argv[index];
        if (argument == "--validate") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::Validate;
            selected_mode = true;
        } else if (argument == "--emit-openqasm") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::EmitOpenQasm;
            selected_mode = true;
        } else if (argument == "--eval-constants") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::EvaluateConstants;
            selected_mode = true;
        } else if (argument == "--out") {
            if (++index >= argc || command.output_path.has_value()) { error = "--out requires one output path"; return false; }
            command.output_path = argv[index];
        } else if (argument == "--max-declarations") {
            if (++index >= argc || !parse_positive_size(argv[index], command.max_declarations)) {
                error = "--max-declarations requires a positive whole number";
                return false;
            }
        } else {
            error = "unknown argument: " + argument;
            return false;
        }
    }
    if (!selected_mode) {
        error = "select one of --validate, --emit-openqasm, or --eval-constants";
        return false;
    }
    if (command.output_path.has_value() && command.mode != Mode::EmitOpenQasm) {
        error = "--out is supported only with --emit-openqasm";
        return false;
    }
    if (command.max_declarations != 64 && command.mode != Mode::EvaluateConstants) {
        error = "--max-declarations is supported only with --eval-constants";
        return false;
    }
    return true;
}

int render_diagnostics(const std::string& source_path, const std::vector<synq::compiler::Diagnostic>& diagnostics,
                       int exit_code) {
    for (const auto& diagnostic : diagnostics) {
        std::cerr << synq::compiler::format_diagnostic(source_path, diagnostic) << "\n";
    }
    return exit_code;
}

void print_value(const synq::compiler::EvaluatedBinding& binding) {
    std::cout << binding.name << " = " << synq::compiler::bounded_value_kind_name(binding.value.kind) << ":";
    switch (binding.value.kind) {
        case synq::compiler::BoundedValueKind::Integer:
            std::cout << binding.value.integer_value;
            break;
        case synq::compiler::BoundedValueKind::Boolean:
            std::cout << (binding.value.boolean_value ? "true" : "false");
            break;
        case synq::compiler::BoundedValueKind::String:
            std::cout << '"' << binding.value.string_value << '"';
            break;
    }
    std::cout << "\n";
}

}  // namespace

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        print_help(std::cout);
        return 0;
    }

    Command command;
    std::string argument_error;
    if (!parse_command(argc, argv, command, argument_error)) {
        std::cerr << "synqc: usage error: " << argument_error << "\n\n";
        print_help(std::cerr);
        return 2;
    }

    Parser parser;
    const auto parsed = parser.parseFileWithDiagnostics(command.source_path);
    if (!parsed.ok()) return render_diagnostics(command.source_path, parsed.diagnostics, 3);

    if (command.mode == Mode::EmitOpenQasm) {
        const auto exported = synq::compiler::export_openqasm3(*parsed.program);
        if (!exported.ok()) {
            for (const auto& diagnostic : exported.diagnostics) {
                std::cerr << command.source_path << ": error[openqasm-export]: " << diagnostic << "\n";
            }
            return 5;
        }
        if (command.output_path.has_value()) {
            std::ofstream output(*command.output_path, std::ios::binary);
            if (!output) {
                std::cerr << "synqc: error: cannot write " << *command.output_path << "\n";
                return 6;
            }
            output << exported.program;
            if (!output) {
                std::cerr << "synqc: error: failed while writing " << *command.output_path << "\n";
                return 6;
            }
        } else {
            std::cout << exported.program;
        }
        return 0;
    }

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!lowered.ok()) return render_diagnostics(command.source_path, lowered.diagnostics, 4);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!resolved.ok()) return render_diagnostics(command.source_path, resolved.diagnostics, 4);

    if (command.mode == Mode::Validate) {
        std::cout << "synqc: valid bounded recovery-profile program: " << command.source_path << "\n";
        return 0;
    }

    synq::compiler::BoundedEvaluationOptions options;
    options.allow_experimental_constant_evaluation = true;
    options.max_declarations = command.max_declarations;
    const auto evaluation = synq::compiler::evaluate_bounded_constants(*resolved.program, options);
    if (!evaluation.ok()) return render_diagnostics(command.source_path, evaluation.diagnostics, 5);
    for (const auto& binding : evaluation.evaluation->bindings) print_value(binding);
    return 0;
}
