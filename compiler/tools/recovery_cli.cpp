#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "compiler/bounded_evaluator.h"
#include "compiler/bounded_simulator.h"
#include "compiler/diagnostic.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/openqasm3_exporter.h"
#include "compiler/parser.h"

namespace {

enum class Mode {
    Validate,
    EmitOpenQasm,
    EmitHybridOpenQasm,
    InspectSemantics,
    EvaluateConstants,
    EvaluateState,
    Simulate,
};

struct Command {
    Mode mode = Mode::Validate;
    std::string source_path;
    std::optional<std::string> output_path;
    std::size_t max_declarations = 64;
    std::size_t max_qubits = 10;
    std::size_t max_operations = 1024;
    std::size_t max_state_cells = 64;
    std::size_t max_state_transitions = 128;
    std::size_t max_expression_depth = 16;
    std::size_t max_state_operations = 128;
    bool has_max_operations = false;
};

void print_help(std::ostream& output) {
    output << "SynQ recovery-profile command line\n\n"
           << "Usage:\n"
           << "  synqc <source.synq> --validate\n"
           << "  synqc <source.synq> --emit-openqasm [--out <file.qasm>]\n"
           << "  synqc <source.synq> --emit-openqasm-hybrid [--out <file.qasm>]\n"
           << "  synqc <source.synq> --inspect-semantics\n"
           << "  synqc <source.synq> --eval-constants [--max-declarations <n>]\n"
           << "  synqc <source.synq> --eval-state [--max-state-cells <n>] [--max-state-transitions <n>] [--max-expression-depth <n>] [--max-operations <n>]\n"
           << "  synqc <source.synq> --simulate [--max-qubits <n>] [--max-operations <n>]\n\n"
           << "Modes:\n"
           << "  --validate        Parse, lower, and resolve the documented bounded profile.\n"
           << "  --emit-openqasm   Emit the supported AST OpenQASM 3 source subset.\n"
           << "  --emit-openqasm-hybrid  Emit strict Hybrid IR OpenQASM with explicit q[n].\n"
           << "  --inspect-semantics  Render resolved top-level binding metadata without evaluation.\n"
           << "  --eval-constants  Explicitly run bounded declaration-only constant evaluation.\n"
           << "  --eval-state      Explicitly run bounded top-level mutable-cell evaluation.\n"
           << "  --simulate        Explicitly calculate deterministic bounded local probabilities.\n\n"
           << "This command does not submit jobs,\n"
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
        } else if (argument == "--emit-openqasm-hybrid") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::EmitHybridOpenQasm;
            selected_mode = true;
        } else if (argument == "--inspect-semantics") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::InspectSemantics;
            selected_mode = true;
        } else if (argument == "--eval-constants") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::EvaluateConstants;
            selected_mode = true;
        } else if (argument == "--eval-state") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::EvaluateState;
            selected_mode = true;
        } else if (argument == "--simulate") {
            if (selected_mode) { error = "select exactly one mode"; return false; }
            command.mode = Mode::Simulate;
            selected_mode = true;
        } else if (argument == "--out") {
            if (++index >= argc || command.output_path.has_value()) { error = "--out requires one output path"; return false; }
            command.output_path = argv[index];
        } else if (argument == "--max-declarations") {
            if (++index >= argc || !parse_positive_size(argv[index], command.max_declarations)) {
                error = "--max-declarations requires a positive whole number";
                return false;
            }
        } else if (argument == "--max-qubits") {
            if (++index >= argc || !parse_positive_size(argv[index], command.max_qubits)) {
                error = "--max-qubits requires a positive whole number";
                return false;
            }
        } else if (argument == "--max-operations") {
            std::size_t parsed = 0;
            if (++index >= argc || !parse_positive_size(argv[index], parsed)) {
                error = "--max-operations requires a positive whole number";
                return false;
            }
            command.max_operations = parsed;
            command.max_state_operations = parsed;
            command.has_max_operations = true;
        } else if (argument == "--max-state-cells") {
            if (++index >= argc || !parse_positive_size(argv[index], command.max_state_cells)) {
                error = "--max-state-cells requires a positive whole number";
                return false;
            }
        } else if (argument == "--max-state-transitions") {
            if (++index >= argc || !parse_positive_size(argv[index], command.max_state_transitions)) {
                error = "--max-state-transitions requires a positive whole number";
                return false;
            }
        } else if (argument == "--max-expression-depth") {
            if (++index >= argc || !parse_positive_size(argv[index], command.max_expression_depth)) {
                error = "--max-expression-depth requires a positive whole number";
                return false;
            }
        } else {
            error = "unknown argument: " + argument;
            return false;
        }
    }
    if (!selected_mode) {
        error = "select one of --validate, --emit-openqasm, --emit-openqasm-hybrid, --inspect-semantics, --eval-constants, --eval-state, or --simulate";
        return false;
    }
    if (command.output_path.has_value() && command.mode != Mode::EmitOpenQasm &&
        command.mode != Mode::EmitHybridOpenQasm) {
        error = "--out is supported only with an OpenQASM export mode";
        return false;
    }
    if (command.max_declarations != 64 && command.mode != Mode::EvaluateConstants) {
        error = "--max-declarations is supported only with --eval-constants";
        return false;
    }
    if ((command.max_qubits != 10 || (command.has_max_operations && command.mode == Mode::Simulate)) &&
        command.mode != Mode::Simulate) {
        error = "--max-qubits and --max-operations are supported only with --simulate";
        return false;
    }
    if ((command.max_state_cells != 64 || command.max_state_transitions != 128 ||
         command.max_expression_depth != 16 ||
         (command.has_max_operations && command.mode == Mode::EvaluateState)) &&
        command.mode != Mode::EvaluateState) {
        error = "--max-state-cells, --max-state-transitions, and --max-expression-depth are supported only with --eval-state";
        return false;
    }
    if (command.has_max_operations && command.mode != Mode::Simulate && command.mode != Mode::EvaluateState) {
        error = "--max-operations is supported only with --simulate or --eval-state";
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

void print_state_cell(const synq::compiler::EvaluatedStateCell& cell) {
    std::cout << "cell " << cell.name << " = " << synq::compiler::bounded_value_kind_name(cell.value.kind) << ":";
    switch (cell.value.kind) {
        case synq::compiler::BoundedValueKind::Integer:
            std::cout << cell.value.integer_value;
            break;
        case synq::compiler::BoundedValueKind::Boolean:
            std::cout << (cell.value.boolean_value ? "true" : "false");
            break;
        case synq::compiler::BoundedValueKind::String:
            std::cout << '"' << cell.value.string_value << '"';
            break;
    }
    std::cout << " | declared line " << cell.declaration_span.line
              << " | last write line " << cell.last_write_span.line << "\n";
}

std::string basis_label(std::size_t basis, std::size_t qubits) {
    std::string label;
    label.reserve(qubits);
    for (std::size_t index = qubits; index > 0; --index) {
        label.push_back((basis & (std::size_t{1} << (index - 1))) == 0 ? '0' : '1');
    }
    return label;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        print_help(std::cout);
        return 0;
    }
    if (argc == 2 && std::string(argv[1]) == "--version") {
        std::cout << "synqc " << SYNQ_RECOVERY_CLI_VERSION << "\n";
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

    if (command.mode == Mode::EmitHybridOpenQasm) {
        const auto exported = synq::compiler::export_hybrid_openqasm3(*lowered.program);
        if (!exported.ok()) {
            for (const auto& diagnostic : exported.diagnostics) {
                std::cerr << command.source_path << ": error[hybrid-openqasm-export]: " << diagnostic << "\n";
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

    if (command.mode == Mode::Validate) {
        std::cout << "synqc: valid bounded recovery-profile program: " << command.source_path << "\n";
        return 0;
    }

    if (command.mode == Mode::InspectSemantics) {
        std::cout << synq::compiler::render_semantic_environment(*resolved.program);
        return 0;
    }

    if (command.mode == Mode::Simulate) {
        synq::compiler::BoundedSimulationOptions options;
        options.allow_experimental_local_simulation = true;
        options.max_qubits = command.max_qubits;
        options.max_operations = command.max_operations;
        const auto simulation = synq::compiler::simulate_bounded_quantum(*resolved.program, options);
        if (!simulation.ok()) return render_diagnostics(command.source_path, simulation.diagnostics, 5);
        std::cout << "qubits = " << simulation.simulation->qubit_count << "\n";
        for (const auto& register_info : simulation.simulation->registers) {
            std::cout << "register " << register_info.name << "[" << register_info.qubit_count
                      << "] physical_offset = " << register_info.physical_offset << "\n";
        }
        for (const auto& basis : simulation.simulation->basis_probabilities) {
            std::cout << "basis |" << basis_label(basis.basis_index, simulation.simulation->qubit_count)
                      << "> probability = " << basis.probability << "\n";
        }
        for (const auto& measurement : simulation.simulation->measurements) {
            std::cout << "measurement " << measurement.register_name << "[" << measurement.register_index
                      << "] probability_one = "
                      << measurement.probability_one << "\n";
        }
        return 0;
    }

    if (command.mode == Mode::EvaluateState) {
        synq::compiler::BoundedStateEvaluationOptions options;
        options.allow_experimental_state_evaluation = true;
        options.max_state_cells = command.max_state_cells;
        options.max_state_transitions = command.max_state_transitions;
        options.max_expression_depth = command.max_expression_depth;
        options.max_operations = command.max_state_operations;
        const auto evaluation = synq::compiler::evaluate_bounded_state(*resolved.program, options);
        if (!evaluation.ok()) return render_diagnostics(command.source_path, evaluation.diagnostics, 5);
        for (const auto& cell : evaluation.evaluation->cells) print_state_cell(cell);
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
