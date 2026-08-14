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
    if (!rejects_unsupported_hybrid_boundaries()) return 1;
    std::cout << "SynQ Hybrid OpenQASM 3 exporter smoke test passed\n";
    return 0;
}
