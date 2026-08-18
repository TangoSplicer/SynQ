#include <iostream>
#include <string>

#include "compiler/hybrid_ir.h"
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

synq::compiler::HybridProgram lower_fixture(const std::string& source, bool& ok) {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!parsed.ok()) {
        ok = false;
        return {};
    }
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    ok = lowered.ok();
    return lowered.program.value_or(synq::compiler::HybridProgram{});
}

bool exports_explicit_typed_hybrid_subset() {
    bool lowered_ok = false;
    const auto program = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "qubit q[2]\n"
        "quantum h q[1]\n"
        "measure q[0]\n", lowered_ok);
    if (!require(lowered_ok, "typed Hybrid OpenQASM fixture lowers")) return false;

    const auto exported = synq::compiler::export_hybrid_openqasm3(program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[2] q;\n"
        "bit[2] c;\n"
        "h q[1];\n"
        "c[0] = measure q[0];\n";
    return require(exported.ok(), "explicit typed Hybrid subset exports") &&
           require(exported.program == expected, "Hybrid OpenQASM output respects the explicit q declaration size");
}

bool exports_explicit_named_register_hybrid_subset() {
    bool lowered_ok = false;
    const auto program = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"named-qubit-register-operands\")]\n"
        "qubit data[2]\n"
        "qubit ancilla[1]\n"
        "quantum h data[0]\n"
        "quantum cx data[0], ancilla[0]\n"
        "measure data[1]\n"
        "measure ancilla[0]\n", lowered_ok);
    if (!require(lowered_ok, "typed named-register Hybrid fixture lowers")) return false;

    const auto exported = synq::compiler::export_hybrid_openqasm3(program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[2] data;\n"
        "qubit[1] ancilla;\n"
        "bit[2] c_data;\n"
        "bit[1] c_ancilla;\n"
        "h data[0];\n"
        "cx data[0], ancilla[0];\n"
        "c_data[1] = measure data[1];\n"
        "c_ancilla[0] = measure ancilla[0];\n";
    return require(exported.ok(), "explicit named-register Hybrid subset exports") &&
           require(exported.program == expected,
                   "Hybrid OpenQASM preserves declared register names, bounds, and measurement storage");
}

bool lowers_bounded_if_gates_and_rejects_wider_control() {
    bool literal_ok = false;
    const auto literal_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[1]\n"
        "if true then quantum h q[0]\n", literal_ok);
    if (!require(literal_ok, "literal-if Hybrid fixture lowers")) return false;
    const auto literal_export = synq::compiler::export_hybrid_openqasm3(literal_if);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[1] q;\n"
        "if (true) h q[0];\n";
    if (!require(literal_export.ok() && literal_export.program == expected,
                 "strict Hybrid export lowers one literal-if typed gate body")) return false;

    bool identifier_ok = false;
    const auto identifier_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let enabled = true\n"
        "qubit q[1]\n"
        "if enabled then quantum h q[0]\n", identifier_ok);
    const auto identifier_export = synq::compiler::export_hybrid_openqasm3(identifier_if);
    const std::string identifier_expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[1] q;\n"
        "bool synq_bool_enabled = true;\n"
        "if (synq_bool_enabled) h q[0];\n";
    if (!require(identifier_ok && identifier_export.ok() && identifier_export.program == identifier_expected,
                 "strict Hybrid export lowers one earlier Boolean-literal declaration identifier if condition")) return false;

    bool negated_identifier_ok = false;
    const auto negated_identifier_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let enabled = false\n"
        "qubit q[1]\n"
        "if not enabled then quantum h q[0]\n", negated_identifier_ok);
    const auto negated_identifier_export = synq::compiler::export_hybrid_openqasm3(negated_identifier_if);
    const std::string negated_identifier_expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[1] q;\n"
        "bool synq_bool_enabled = false;\n"
        "if (!synq_bool_enabled) h q[0];\n";
    if (!require(negated_identifier_ok && negated_identifier_export.ok() &&
                 negated_identifier_export.program == negated_identifier_expected,
                 "strict Hybrid export lowers not over one earlier Boolean-literal declaration identifier")) return false;

    bool negated_true_ok = false;
    const auto negated_true_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[1]\n"
        "if not true then quantum h q[0]\n", negated_true_ok);
    const auto negated_true_export = synq::compiler::export_hybrid_openqasm3(negated_true_if);
    const std::string negated_true_expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[1] q;\n"
        "if (false) h q[0];\n";
    if (!require(negated_true_ok && negated_true_export.ok() &&
                 negated_true_export.program == negated_true_expected,
                 "strict Hybrid export folds not true into a literal false condition without target storage")) return false;

    bool negated_false_ok = false;
    const auto negated_false_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[1]\n"
        "if not false then quantum h q[0]\n", negated_false_ok);
    const auto negated_false_export = synq::compiler::export_hybrid_openqasm3(negated_false_if);
    const std::string negated_false_expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[1] q;\n"
        "if (true) h q[0];\n";
    if (!require(negated_false_ok && negated_false_export.ok() &&
                 negated_false_export.program == negated_false_expected,
                 "strict Hybrid export folds not false into a literal true condition without target storage")) return false;

    bool alias_ok = false;
    const auto alias_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "let enabled = ready\n"
        "qubit q[1]\n"
        "if enabled then quantum h q[0]\n", alias_ok);
    if (!require(alias_ok && !synq::compiler::export_hybrid_openqasm3(alias_if).ok(),
                 "strict Hybrid export rejects identifier conditions backed by declaration aliases")) return false;

    bool negated_alias_ok = false;
    const auto negated_alias_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "let enabled = ready\n"
        "qubit q[1]\n"
        "if not enabled then quantum h q[0]\n", negated_alias_ok);
    if (!require(negated_alias_ok && !synq::compiler::export_hybrid_openqasm3(negated_alias_if).ok(),
                 "strict Hybrid export rejects negated identifier conditions backed by declaration aliases")) return false;

    bool expression_ok = false;
    const auto expression_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "let ready = true\n"
        "let fallback = false\n"
        "qubit q[1]\n"
        "if ready or fallback then quantum h q[0]\n", expression_ok);
    if (!require(expression_ok && !synq::compiler::export_hybrid_openqasm3(expression_if).ok(),
                 "strict Hybrid export rejects Boolean-expression if conditions")) return false;

    bool while_ok = false;
    const auto while_control = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[1]\n"
        "while true do quantum h q[0]\n", while_ok);
    if (!require(while_ok && !synq::compiler::export_hybrid_openqasm3(while_control).ok(),
                 "strict Hybrid export rejects while-loop lowering")) return false;

    bool measurement_ok = false;
    const auto measurement_if = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"classical-control-flow\")]\n"
        "qubit q[1]\n"
        "if false then measure q[0]\n", measurement_ok);
    return require(measurement_ok && !synq::compiler::export_hybrid_openqasm3(measurement_if).ok(),
                   "strict Hybrid export retains the literal-if measurement boundary");
}

bool rejects_unsupported_hybrid_boundaries() {
    bool no_declaration_ok = false;
    const auto no_declaration = lower_fixture("quantum h q[0]\n", no_declaration_ok);
    if (!require(no_declaration_ok, "legacy fixture lowers for strict Hybrid export rejection")) return false;
    if (!require(!synq::compiler::export_hybrid_openqasm3(no_declaration).ok(),
                 "Hybrid export rejects missing explicit default declaration")) return false;

    bool out_of_range_ok = false;
    const auto out_of_range = lower_fixture(
        "#[experimental(feature = \"qubit-declarations\")]\nqubit q[1]\nquantum h q[1]\n", out_of_range_ok);
    if (!require(out_of_range_ok, "out-of-range fixture lowers before exporter validation")) return false;
    if (!require(!synq::compiler::export_hybrid_openqasm3(out_of_range).ok(),
                 "Hybrid export rejects operands outside explicit default declaration")) return false;

    bool named_measurement_ok = false;
    const auto named_measurement = lower_fixture("measure q[0] as observed\n", named_measurement_ok);
    if (!require(named_measurement_ok, "named measurement fixture lowers for Hybrid export rejection")) return false;
    if (!require(!synq::compiler::export_hybrid_openqasm3(named_measurement).ok(),
                 "Hybrid export rejects named measurement result declarations")) return false;

    bool control_ok = false;
    const auto control = lower_fixture(
        "#[experimental(feature = \"classical-control-flow\")]\nif true then quantum h q[0]\n", control_ok);
    return require(control_ok && !synq::compiler::export_hybrid_openqasm3(control).ok(),
                   "Hybrid export rejects unlowered typed control nodes");
}

}  // namespace

int main() {
    if (!exports_explicit_typed_hybrid_subset()) return 1;
    if (!exports_explicit_named_register_hybrid_subset()) return 1;
    if (!lowers_bounded_if_gates_and_rejects_wider_control()) return 1;
    if (!rejects_unsupported_hybrid_boundaries()) return 1;
    std::cout << "SynQ Hybrid OpenQASM 3 exporter smoke test passed\n";
    return 0;
}
