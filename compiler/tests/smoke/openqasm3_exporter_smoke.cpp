#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

#include "compiler/ast.h"
#include "compiler/openqasm3_exporter.h"
#include "compiler/parser.h"

namespace {

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

bool exports_supported_kernels_in_order() {
    ProgramNode program;
    program.statements.push_back(new InstructionNode("quantum", {"x"}, 1));
    program.statements.push_back(new InstructionNode("quantum", {"bell_pair"}, 2));
    program.statements.push_back(new InstructionNode("quantum", {"z"}, 3));

    const auto result = synq::compiler::export_openqasm3(program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[2] q;\n"
        "x q[0];\n"
        "h q[0];\n"
        "cx q[0], q[1];\n"
        "z q[0];\n";

    return require(result.ok(), "supported quantum kernels export successfully") &&
           require(result.program == expected, "exported OpenQASM 3 matches the bounded fixture exactly");
}

bool exports_parsed_quantum_fixture() {
    const std::string path = "/tmp/synq_openqasm3_fixture.synq";
    std::ofstream fixture(path);
    fixture << "quantum h // standard gate\n";
    fixture << "quantum y\n";
    fixture.close();

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());
    const auto* program = dynamic_cast<const ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser produces a program for the export fixture")) return false;

    const auto result = synq::compiler::export_openqasm3(*program);
    return require(result.ok(), "parsed supported quantum fixture exports successfully") &&
           require(result.program.find("qubit[1] q;\n") != std::string::npos, "single-qubit export allocates one qubit") &&
           require(result.program.find("h q[0];\ny q[0];\n") != std::string::npos, "parsed instruction order is preserved");
}

bool exports_explicit_qubit_operands() {
    const std::string path = "/tmp/synq_openqasm3_explicit_fixture.synq";
    std::ofstream fixture(path);
    fixture << "quantum h q[3];\n";
    fixture << "quantum cx q[3], q[5];\n";
    fixture << "quantum bell_pair q[1], q[4];\n";
    fixture.close();

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());
    const auto* program = dynamic_cast<const ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser produces an explicit-operand export fixture")) return false;

    const auto result = synq::compiler::export_openqasm3(*program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[6] q;\n"
        "h q[3];\n"
        "cx q[3], q[5];\n"
        "h q[1];\n"
        "cx q[1], q[4];\n";
    return require(result.ok(), "explicit operands export successfully") &&
           require(result.program == expected, "explicit operands and cx preserve exact OpenQASM qubit indices");
}

bool rejects_invalid_explicit_operands() {
    ProgramNode program;
    program.statements.push_back(new InstructionNode("quantum", {"cx", "q[0]"}, 7));
    program.statements.push_back(new InstructionNode("quantum", {"h", "q[0]", "q[1]"}, 8));
    program.statements.push_back(new InstructionNode("quantum", {"x", "q[not-an-index]"}, 9));

    const auto result = synq::compiler::export_openqasm3(program);
    return require(!result.ok(), "invalid explicit operand forms fail export") &&
           require(result.program.empty(), "invalid explicit operand forms do not produce partial OpenQASM") &&
           require(result.diagnostics.size() == 3, "invalid explicit operand forms produce three diagnostics") &&
           require(result.diagnostics[0].find("line 7") != std::string::npos, "cx arity error retains source line") &&
           require(result.diagnostics[1].find("line 8") != std::string::npos, "single-qubit arity error retains source line") &&
           require(result.diagnostics[2].find("line 9") != std::string::npos, "invalid operand error retains source line");
}

bool exports_literal_angle_gates() {
    const std::string path = "/tmp/synq_openqasm3_parameter_fixture.synq";
    std::ofstream fixture(path);
    fixture << "#[experimental(feature = \"parameterized-quantum-gates\")]\n";
    fixture << "quantum rx(pi/2) q[0];\n";
    fixture << "quantum ry(-pi/4) q[2];\n";
    fixture << "quantum rz(0.125) q[1];\n";
    fixture << "quantum p(pi) q[3];\n";
    fixture.close();

    Parser parser;
    std::unique_ptr<ASTNode> root(parser.parseFile(path));
    std::remove(path.c_str());
    const auto* program = dynamic_cast<const ProgramNode*>(root.get());
    if (!require(program != nullptr, "parser produces a literal-angle export fixture")) return false;

    const auto result = synq::compiler::export_openqasm3(*program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[4] q;\n"
        "rx(pi/2) q[0];\n"
        "ry(-pi/4) q[2];\n"
        "rz(0.125) q[1];\n"
        "p(pi) q[3];\n";
    return require(result.ok(), "literal-angle gates export successfully") &&
           require(result.program == expected, "literal-angle gates preserve exact OpenQASM source");
}

bool rejects_invalid_parameterized_gates() {
    ProgramNode program;
    program.statements.push_back(new InstructionNode("quantum", {"rx(pi/2)", "q[0]", "q[1]"}, 10));
    program.statements.push_back(new InstructionNode("quantum", {"crx(pi/2)", "q[0]"}, 11));
    program.statements.push_back(new InstructionNode("quantum", {"rz(theta)", "q[0]"}, 12));

    const auto result = synq::compiler::export_openqasm3(program);
    return require(!result.ok(), "invalid parameterized gates fail export") &&
           require(result.program.empty(), "invalid parameterized gates do not produce partial OpenQASM") &&
           require(result.diagnostics.size() == 3, "invalid parameterized gates produce three diagnostics") &&
           require(result.diagnostics[0].find("line 10") != std::string::npos, "parameter arity error retains source line") &&
           require(result.diagnostics[1].find("crx") != std::string::npos, "unsupported parameterized gate identifies gate") &&
           require(result.diagnostics[2].find("rz(theta)") != std::string::npos, "non-literal parameter remains unsupported");
}

bool rejects_unsupported_recovery_statements() {
    ProgramNode program;
    program.statements.push_back(new DeclarationNode("theta", "0.5", 4));
    program.statements.push_back(new InstructionNode("quantum", {"qaoa"}, 5));
    program.statements.push_back(new InstructionNode("print", {"hello"}, 6));

    const auto result = synq::compiler::export_openqasm3(program);
    return require(!result.ok(), "unsupported statements make the exporter fail") &&
           require(result.program.empty(), "failed export does not produce partial OpenQASM") &&
           require(result.diagnostics.size() == 3, "failed export reports every unsupported statement") &&
           require(result.diagnostics[0].find("line 4") != std::string::npos, "declaration diagnostic retains source line") &&
           require(result.diagnostics[1].find("qaoa") != std::string::npos, "kernel diagnostic identifies unsupported kernel") &&
           require(result.diagnostics[2].find("line 6") != std::string::npos, "instruction diagnostic retains source line");
}

bool writes_reference_parser_fixture(const std::string& path) {
    ProgramNode program;
    program.statements.push_back(new InstructionNode("quantum", {"h", "q[3]"}, 1));
    program.statements.push_back(new InstructionNode("quantum", {"cx", "q[3]", "q[5]"}, 2));
    program.statements.push_back(new InstructionNode("quantum", {"bell_pair", "q[1]", "q[4]"}, 3));
    program.statements.push_back(new InstructionNode("quantum", {"rx(pi/2)", "q[0]"}, 4));
    program.statements.push_back(new InstructionNode("quantum", {"ry(-pi/4)", "q[2]"}, 5));
    program.statements.push_back(new InstructionNode("quantum", {"rz(0.125)", "q[1]"}, 6));
    program.statements.push_back(new InstructionNode("quantum", {"p(pi)", "q[3]"}, 7));
    const auto result = synq::compiler::export_openqasm3(program);
    if (!require(result.ok(), "reference-parser fixture exports successfully")) return false;

    std::ofstream fixture(path);
    fixture << result.program;
    return require(static_cast<bool>(fixture), "exporter writes the reference-parser fixture");
}

}  // namespace

int main(int argc, char* argv[]) {
    if (!exports_supported_kernels_in_order()) return 1;
    if (!exports_parsed_quantum_fixture()) return 1;
    if (!exports_explicit_qubit_operands()) return 1;
    if (!rejects_invalid_explicit_operands()) return 1;
    if (!exports_literal_angle_gates()) return 1;
    if (!rejects_invalid_parameterized_gates()) return 1;
    if (!rejects_unsupported_recovery_statements()) return 1;
    if (argc == 2 && !writes_reference_parser_fixture(argv[1])) return 1;
    if (argc > 2) {
        std::cerr << "usage: synq_openqasm3_exporter_smoke [reference-fixture-path]\n";
        return 2;
    }

    std::cout << "SynQ OpenQASM 3 exporter smoke test passed\n";
    return 0;
}
