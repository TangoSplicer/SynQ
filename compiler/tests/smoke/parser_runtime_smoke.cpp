#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "compiler/ast.h"
#include "compiler/parser.h"
#include "compiler/runtime.h"

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

std::string write_fixture(const std::string& filename, const std::string& content) {
    const std::string path = "/tmp/" + filename;
    std::ofstream fixture(path);
    fixture << content;
    fixture.close();
    return path;
}

bool parser_accepts_supported_fixture() {
    const std::string path = write_fixture(
        "synq_parser_supported_fixture.synq",
        "// A recovery-parser fixture\n"
        "let theta = 0.5; // variational parameter\n"
        "let endpoint = https://example.invalid;\n"
        "print theta // local-only output\n"
        "delay 0; // zero delay\n"
        "quantum bell_pair // placeholder path\n"
        "ai concise_summary // placeholder path\n");

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());

    if (!require(root != nullptr, "parser accepts supported fixture")) return false;
    auto* program = dynamic_cast<ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser returns ProgramNode")) return false;
    if (!require(program->statements.size() == 6, "parser retains declarations and supported instructions")) return false;

    auto* declaration = dynamic_cast<DeclarationNode*>(program->statements[0]);
    if (!require(declaration != nullptr, "parser creates DeclarationNode")) return false;
    if (!require(declaration->name == "theta" && declaration->value == "0.5", "parser preserves declaration fields")) return false;
    if (!require(declaration->line == 2, "parser records declaration source line")) return false;

    auto* url_declaration = dynamic_cast<DeclarationNode*>(program->statements[1]);
    if (!require(url_declaration != nullptr, "parser retains URL declaration")) return false;
    if (!require(url_declaration->name == "endpoint" && url_declaration->value == "https://example.invalid",
                 "parser preserves non-comment URL text")) return false;
    if (!require(url_declaration->line == 3, "parser records URL declaration source line")) return false;

    const std::string expected_operations[] = {"print", "delay", "ai"};
    const std::string expected_arguments[] = {"theta", "0", "concise_summary"};
    for (std::size_t index = 0; index < 2; ++index) {
        auto* instruction = dynamic_cast<InstructionNode*>(program->statements[index + 2]);
        if (!require(instruction != nullptr, "parser creates InstructionNode")) return false;
        if (!require(instruction->op == expected_operations[index], "parser preserves instruction operation")) return false;
        if (!require(instruction->args.size() == 1 && instruction->args.front() == expected_arguments[index],
                     "parser preserves instruction argument")) return false;
        if (!require(instruction->line == index + 4, "parser records source line")) return false;
    }
    auto* gate = dynamic_cast<QuantumGateNode*>(program->statements[4]);
    if (!require(gate != nullptr, "parser creates a typed quantum gate node")) return false;
    if (!require(gate->kind == QuantumGateKind::BellPair && gate->source_name == "bell_pair" &&
                 !gate->literal_angle.has_value() && gate->qubit_indices.empty() && gate->line == 6,
                 "parser preserves typed bell-pair fields")) return false;
    auto* ai = dynamic_cast<InstructionNode*>(program->statements[5]);
    return require(ai != nullptr && ai->op == expected_operations[2] && ai->args.front() == expected_arguments[2] && ai->line == 7,
                   "parser retains the instruction after a typed quantum node");
}

bool parser_rejects_invalid_fixture() {
    const std::string malformed_path = write_fixture("synq_parser_malformed_fixture.synq", "delay later\n");
    const std::string unsupported_path = write_fixture("synq_parser_unsupported_fixture.synq", "compile program\n");
    const std::string declaration_path = write_fixture("synq_parser_bad_declaration_fixture.synq", "let 9bad = value\n");
    Parser parser;

    std::unique_ptr<ASTNode> malformed(parser.parseFile(malformed_path));
    std::unique_ptr<ASTNode> unsupported(parser.parseFile(unsupported_path));
    std::unique_ptr<ASTNode> malformed_declaration(parser.parseFile(declaration_path));
    std::unique_ptr<ASTNode> missing(parser.parseFile("/tmp/synq_parser_missing_fixture.synq"));
    std::remove(malformed_path.c_str());
    std::remove(unsupported_path.c_str());
    std::remove(declaration_path.c_str());

    return require(malformed == nullptr, "parser rejects non-numeric delay") &&
           require(unsupported == nullptr, "parser rejects unsupported operation") &&
           require(malformed_declaration == nullptr, "parser rejects malformed declaration") &&
           require(missing == nullptr, "parser rejects missing input file");
}

bool parser_accepts_explicit_qubit_operands() {
    const std::string path = write_fixture(
        "synq_parser_explicit_qubits_fixture.synq",
        "quantum h q[3];\n"
        "quantum cx q[3], q[5] // controlled gate\n"
        "quantum bell_pair q[1], q[4]\n");

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());
    auto* program = dynamic_cast<ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser accepts explicit quantum operands")) return false;
    if (!require(program->statements.size() == 3, "parser retains every explicit quantum instruction")) return false;

    const std::vector<QuantumGateKind> expected_kinds = {
        QuantumGateKind::H,
        QuantumGateKind::Cx,
        QuantumGateKind::BellPair,
    };
    const std::vector<std::vector<std::size_t>> expected_operands = {
        {3},
        {3, 5},
        {1, 4},
    };
    for (std::size_t index = 0; index < expected_kinds.size(); ++index) {
        auto* gate = dynamic_cast<QuantumGateNode*>(program->statements[index]);
        if (!require(gate != nullptr, "parser creates a typed quantum gate node")) return false;
        if (!require(gate->kind == expected_kinds[index] && gate->qubit_indices == expected_operands[index] &&
                     !gate->literal_angle.has_value(), "parser preserves typed explicit quantum operands")) return false;
    }
    return true;
}

bool parser_rejects_malformed_qubit_operands() {
    const std::string negative_path = write_fixture("synq_parser_negative_qubit_fixture.synq", "quantum h q[-1]\n");
    const std::string trailing_path = write_fixture("synq_parser_trailing_qubit_fixture.synq", "quantum cx q[0],\n");
    const std::string spacing_path = write_fixture("synq_parser_spacing_qubit_fixture.synq", "quantum h q[0] q[1]\n");
    Parser parser;
    std::unique_ptr<ASTNode> negative(parser.parseFile(negative_path));
    std::unique_ptr<ASTNode> trailing(parser.parseFile(trailing_path));
    std::unique_ptr<ASTNode> spacing(parser.parseFile(spacing_path));
    std::remove(negative_path.c_str());
    std::remove(trailing_path.c_str());
    std::remove(spacing_path.c_str());

    return require(negative == nullptr, "parser rejects negative qubit index") &&
           require(trailing == nullptr, "parser rejects trailing qubit comma") &&
           require(spacing == nullptr, "parser rejects operands without commas");
}

bool parser_accepts_literal_angle_parameters() {
    const std::string path = write_fixture(
        "synq_parser_parameter_fixture.synq",
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "quantum rx(pi/2) q[0];\n"
        "quantum rz(-pi/4) q[3];\n"
        "quantum p(0.125) q[2]\n");

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());
    auto* program = dynamic_cast<ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser accepts literal-angle parameter fixture")) return false;

    const std::vector<QuantumGateKind> expected_kinds = {
        QuantumGateKind::Rx,
        QuantumGateKind::Rz,
        QuantumGateKind::Phase,
    };
    const std::vector<std::string> expected_angles = {"pi/2", "-pi/4", "0.125"};
    const std::vector<std::vector<std::size_t>> expected_operands = {
        {0},
        {3},
        {2},
    };
    if (!require(program->statements.size() == expected_kinds.size(), "parser retains parameterized instructions")) return false;
    for (std::size_t index = 0; index < expected_kinds.size(); ++index) {
        auto* gate = dynamic_cast<QuantumGateNode*>(program->statements[index]);
        if (!require(gate != nullptr && gate->kind == expected_kinds[index] &&
                     gate->literal_angle.has_value() && *gate->literal_angle == expected_angles[index] &&
                     gate->qubit_indices == expected_operands[index],
                     "parser preserves typed parameterized quantum fields")) return false;
    }
    return true;
}

bool parser_rejects_unsupported_parameter_forms() {
    const std::string empty_path = write_fixture("synq_parser_empty_parameter_fixture.synq", "quantum rx() q[0]\n");
    const std::string variable_path = write_fixture("synq_parser_variable_parameter_fixture.synq", "quantum rx(theta) q[0]\n");
    const std::string missing_operand_path = write_fixture("synq_parser_missing_parameter_operand_fixture.synq", "quantum rx(pi/2)\n");
    const std::string two_operand_path = write_fixture("synq_parser_two_parameter_operand_fixture.synq", "quantum rx(pi/2) q[0], q[1]\n");
    Parser parser;
    std::unique_ptr<ASTNode> empty(parser.parseFile(empty_path));
    std::unique_ptr<ASTNode> variable(parser.parseFile(variable_path));
    std::unique_ptr<ASTNode> missing_operand(parser.parseFile(missing_operand_path));
    std::unique_ptr<ASTNode> two_operands(parser.parseFile(two_operand_path));
    std::remove(empty_path.c_str());
    std::remove(variable_path.c_str());
    std::remove(missing_operand_path.c_str());
    std::remove(two_operand_path.c_str());

    return require(empty == nullptr, "parser rejects empty parameter list") &&
           require(variable == nullptr, "parser rejects variable parameter expression") &&
           require(missing_operand == nullptr, "parser rejects parameterized gate without operand") &&
           require(two_operands == nullptr, "parser rejects parameterized gate with two operands");
}

bool parser_enforces_experimental_feature_gate() {
    const std::string disabled_path = write_fixture(
        "synq_parser_disabled_experimental_fixture.synq",
        "quantum rx(pi/2) q[0]\n");
    const std::string unknown_path = write_fixture(
        "synq_parser_unknown_experimental_fixture.synq",
        "#[experimental(feature = \"missing-feature\")]\n"
        "quantum h q[0]\n");
    const std::string configured_path = write_fixture(
        "synq_parser_configured_experimental_fixture.synq",
        "quantum rx(pi/2) q[0]\n");
    Parser parser;
    std::unique_ptr<ASTNode> disabled(parser.parseFile(disabled_path));
    std::unique_ptr<ASTNode> unknown(parser.parseFile(unknown_path));
    parser.enableExperimentalFeature("parameterized-quantum-gates");
    std::unique_ptr<ASTNode> configured(parser.parseFile(configured_path));
    std::remove(disabled_path.c_str());
    std::remove(unknown_path.c_str());
    std::remove(configured_path.c_str());

    return require(disabled == nullptr, "parser rejects a disabled alpha feature") &&
           require(unknown == nullptr, "parser rejects an unknown feature annotation") &&
           require(configured != nullptr, "parser accepts an explicitly configured alpha feature");
}

bool runtime_executes_supported_and_fallback_paths() {
    synq::compiler::Program program{
        {
            {"print", {"fixture output"}},
            {"delay", {"0"}},
            {"quantum", {"bell_pair"}},
            {"ai", {"summarize"}},
            {"unknown", {}},
        },
    };

    std::ostringstream output;
    auto* original_buffer = std::cout.rdbuf(output.rdbuf());
    synq::compiler::Runtime runtime;
    runtime.initialize();
    runtime.execute(program);
    std::cout.rdbuf(original_buffer);

    const std::string text = output.str();
    return require(text.find("Runtime initialization complete") != std::string::npos,
                   "runtime initializes") &&
           require(text.find("Executing SynQ Program") != std::string::npos,
                   "runtime begins execution") &&
           require(text.find("[print] fixture output") != std::string::npos,
                   "runtime executes print instruction") &&
           require(text.find("[delay] 0ms") != std::string::npos,
                   "runtime executes zero-duration delay") &&
           require(text.find("[quantum] Invoking quantum kernel") != std::string::npos,
                   "runtime reaches quantum placeholder path") &&
           require(text.find("[ai] Querying AI agent") != std::string::npos,
                   "runtime reaches AI placeholder path") &&
           require(text.find("[warn] Unknown instruction: unknown") != std::string::npos,
                   "runtime reports unknown instruction") &&
           require(text.find("Execution complete") != std::string::npos,
                   "runtime completes execution");
}

}  // namespace

int main() {
    if (!parser_accepts_supported_fixture()) return 1;
    if (!parser_rejects_invalid_fixture()) return 1;
    if (!parser_accepts_explicit_qubit_operands()) return 1;
    if (!parser_rejects_malformed_qubit_operands()) return 1;
    if (!parser_accepts_literal_angle_parameters()) return 1;
    if (!parser_rejects_unsupported_parameter_forms()) return 1;
    if (!parser_enforces_experimental_feature_gate()) return 1;
    if (!runtime_executes_supported_and_fallback_paths()) return 1;

    std::cout << "SynQ parser and runtime smoke test passed\n";
    return 0;
}
