#include <fstream>
#include <iostream>
#include <string>

#include "compiler/diagnostic.h"
#include "compiler/parser.h"

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

bool has_single_error(const synq::compiler::ParseResult& result, const std::string& code, std::size_t line) {
    return !result.ok() && result.program == nullptr && result.diagnostics.size() == 1 &&
           result.diagnostics.front().code == code &&
           result.diagnostics.front().severity == synq::compiler::DiagnosticSeverity::Error &&
           result.diagnostics.front().span.line == line && !result.diagnostics.front().help.empty();
}

bool parser_reports_structured_recovery_errors() {
    const std::string declaration_path = write_fixture("synq_diagnostic_declaration.synq", "let 9bad = value\n");
    const std::string instruction_path = write_fixture("synq_diagnostic_instruction.synq", "compile program\n");
    const std::string delay_path = write_fixture("synq_diagnostic_delay.synq", "delay later\n");
    const std::string quantum_path = write_fixture("synq_diagnostic_quantum.synq", "quantum h q[-1]\n");
    const std::string annotation_path = write_fixture("synq_diagnostic_annotation.synq", "#[experimental(feature = \"missing-feature\")]\n");
    const std::string gate_path = write_fixture("synq_diagnostic_gate.synq", "quantum rx(pi/2) q[0]\n");
    Parser parser;
    const auto declaration = parser.parseFileWithDiagnostics(declaration_path);
    const auto instruction = parser.parseFileWithDiagnostics(instruction_path);
    const auto delay = parser.parseFileWithDiagnostics(delay_path);
    const auto quantum = parser.parseFileWithDiagnostics(quantum_path);
    const auto annotation = parser.parseFileWithDiagnostics(annotation_path);
    const auto gate = parser.parseFileWithDiagnostics(gate_path);
    std::remove(declaration_path.c_str());
    std::remove(instruction_path.c_str());
    std::remove(delay_path.c_str());
    std::remove(quantum_path.c_str());
    std::remove(annotation_path.c_str());
    std::remove(gate_path.c_str());

    return require(has_single_error(declaration, "SYNQ-P002", 1), "malformed declarations receive SYNQ-P002") &&
           require(has_single_error(instruction, "SYNQ-P003", 1), "unsupported instructions receive SYNQ-P003") &&
           require(has_single_error(delay, "SYNQ-P004", 1), "invalid delays receive SYNQ-P004") &&
           require(has_single_error(quantum, "SYNQ-P005", 1), "malformed quantum operands receive SYNQ-P005") &&
           require(has_single_error(annotation, "SYNQ-P006", 1), "unknown annotations receive SYNQ-P006") &&
           require(has_single_error(gate, "SYNQ-P007", 1), "disabled alpha gates receive SYNQ-P007");
}

bool parser_reports_file_and_success_results() {
    Parser parser;
    const auto missing = parser.parseFileWithDiagnostics("/tmp/synq_diagnostic_missing.synq");
    const std::string valid_path = write_fixture(
        "synq_diagnostic_valid.synq",
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "let theta = 0.5\n"
        "quantum rx(pi/2) q[0]\n");
    const auto valid = parser.parseFileWithDiagnostics(valid_path);
    std::remove(valid_path.c_str());

    if (!require(has_single_error(missing, "SYNQ-P001", 0), "missing files receive SYNQ-P001")) return false;
    if (!require(valid.ok() && valid.diagnostics.empty() && valid.program != nullptr,
                 "valid source returns an owned program without diagnostics")) return false;
    if (!require(valid.program->statements.size() == 2, "feature annotations do not become AST statements")) return false;
    const synq::compiler::Diagnostic diagnostic{
        "SYNQ-P005", synq::compiler::DiagnosticSeverity::Error, {3, 1, 18}, "malformed quantum operands", "use q[0]"};
    const std::string formatted = synq::compiler::format_diagnostic("fixture.synq", diagnostic);
    return require(formatted.find("fixture.synq:3:1: error[SYNQ-P005]") != std::string::npos,
                   "formatted diagnostics include source, span, severity, and code") &&
           require(formatted.find("help: use q[0]") != std::string::npos,
                   "formatted diagnostics include actionable help");
}

}  // namespace

int main() {
    if (!parser_reports_structured_recovery_errors()) return 1;
    if (!parser_reports_file_and_success_results()) return 1;
    std::cout << "SynQ structured diagnostics smoke test passed\n";
    return 0;
}
