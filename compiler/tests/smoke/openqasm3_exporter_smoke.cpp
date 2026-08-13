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

}  // namespace

int main() {
    if (!exports_supported_kernels_in_order()) return 1;
    if (!exports_parsed_quantum_fixture()) return 1;
    if (!rejects_unsupported_recovery_statements()) return 1;

    std::cout << "SynQ OpenQASM 3 exporter smoke test passed\n";
    return 0;
}
