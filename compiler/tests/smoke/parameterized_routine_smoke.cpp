#include <iostream>
#include <string>

#include "compiler/bounded_simulator.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/openqasm3_exporter.h"
#include "compiler/parser.h"

namespace {

using synq::compiler::BoundedSimulationOptions;
using synq::compiler::Diagnostic;
using synq::compiler::HybridCallableDeclaration;
using synq::compiler::HybridCallableCall;
using synq::compiler::HybridParameterizedRoutineBody;
using synq::compiler::HybridProgram;
using synq::compiler::HybridQubitDeclaration;
using synq::compiler::HybridRoutineFormal;
using synq::compiler::SourceSpan;

bool require(bool condition, const std::string& message) {
    if (!condition) {
        std::cerr << "FAIL: " << message << "\n";
        return false;
    }
    return true;
}

bool has_code(const std::vector<Diagnostic>& diagnostics, const std::string& code) {
    return diagnostics.size() == 1 && diagnostics.front().code == code;
}

bool has_export_text(const std::vector<std::string>& diagnostics, const std::string& text) {
    for (const std::string& diagnostic : diagnostics) {
        if (diagnostic.find(text) != std::string::npos) return true;
    }
    return false;
}

const char* u3_prelude() {
    return
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"named-qubit-register-operands\")]\n"
        "#[experimental(feature = \"callable-declarations\")]\n"
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "#[experimental(feature = \"parameterized-quantum-routines\")]\n";
}

bool parses_lowers_resolves_and_expands_all_documented_signatures() {
    Parser parser;
    const std::string source = std::string(u3_prelude()) +
        "qubit qa[2]\n"
        "qubit qb[2]\n"
        "kernel rotate(angle theta, qubit target) { quantum rz(theta) target }\n"
        "kernel flip(qubit target) { quantum x target }\n"
        "kernel link(qubit control, qubit target) { quantum cx control, target }\n"
        "call rotate(pi/2, qa[0])\n"
        "call flip(qb[1])\n"
        "call link(qa[1], qb[0])\n";
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!require(parsed.ok(), "all three documented parameterized routine signatures parse")) return false;

    const auto* rotate = dynamic_cast<const CallableDeclarationNode*>(parsed.program->statements.at(2));
    const auto* rotate_call = dynamic_cast<const CallableCallNode*>(parsed.program->statements.at(5));
    if (!require(rotate != nullptr && rotate->kind == CallableDeclarationKind::Kernel &&
                     rotate->formals.size() == 2 && rotate->parameterized_body.has_value() &&
                     rotate->parameterized_body->angle_formal == "theta" &&
                     rotate_call != nullptr && rotate_call->arguments.size() == 2 &&
                     rotate_call->arguments.front() == "pi/2",
                 "typed AST retains ordered routine formals, formal-only body, and call actuals")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "parameterized routines lower to Hybrid IR")) return false;
    const auto* hybrid_rotate = std::get_if<HybridCallableDeclaration>(&lowered.program->nodes.at(2));
    const auto* hybrid_rotate_call = std::get_if<HybridCallableCall>(&lowered.program->nodes.at(5));
    if (!require(hybrid_rotate != nullptr && hybrid_rotate->parameterized_body.has_value() &&
                     hybrid_rotate->formals.size() == 2 && hybrid_rotate_call != nullptr &&
                     hybrid_rotate_call->arguments.size() == 2,
                 "Hybrid IR retains routine and invocation provenance without a top-level synthetic gate")) return false;

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "resolver validates all documented U3 routine signatures and named-register actuals")) return false;

    const auto exported = synq::compiler::export_hybrid_openqasm3(*lowered.program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[2] qa;\n"
        "qubit[2] qb;\n"
        "rz(pi/2) qa[0];\n"
        "x qb[1];\n"
        "cx qa[1], qb[0];\n";
    return require(exported.ok() && exported.program == expected,
                   "strict Hybrid export substitutes literal angle and actual registers into one source-ordered gate per call");
}

bool enforces_u3_feature_gates_and_parse_boundaries() {
    Parser parser;
    const auto missing_callable = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "#[experimental(feature = \"parameterized-quantum-routines\")]\n"
        "kernel flip(qubit target) { quantum x target }\n");
    if (!require(!missing_callable.ok() && has_code(missing_callable.diagnostics, "SYNQ-P007"),
                 "parameterized routine declarations require callable-declarations opt-in")) return false;

    const auto missing_routines = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\n"
        "#[experimental(feature = \"parameterized-quantum-gates\")]\n"
        "kernel flip(qubit target) { quantum x target }\n");
    if (!require(!missing_routines.ok() && has_code(missing_routines.diagnostics, "SYNQ-P007"),
                 "parameterized routine declarations require their dedicated Alpha opt-in")) return false;

    const auto missing_gates = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\n"
        "#[experimental(feature = \"parameterized-quantum-routines\")]\n"
        "kernel rotate(angle theta, qubit target) { quantum rz(theta) target }\n");
    if (!require(!missing_gates.ok() && has_code(missing_gates.diagnostics, "SYNQ-P007"),
                 "angle-bearing parameterized routines require parameterized-quantum-gates opt-in")) return false;

    const auto malformed_formals = parser.parseSourceWithDiagnostics(
        std::string(u3_prelude()) + "kernel bad(angle theta, angle phi, qubit target) { quantum rz(theta) target }\n");
    if (!require(!malformed_formals.ok() && has_code(malformed_formals.diagnostics, "SYNQ-P017"),
                 "duplicate angle formals use the reserved parameterized-routine declaration diagnostic")) return false;

    const auto captured_body = parser.parseSourceWithDiagnostics(
        std::string(u3_prelude()) + "kernel bad(qubit target) { quantum x q[0] }\n");
    if (!require(!captured_body.ok() && has_code(captured_body.diagnostics, "SYNQ-P017"),
                 "routine bodies reject captured global qubit operands")) return false;

    const auto unsupported_body = parser.parseSourceWithDiagnostics(
        std::string(u3_prelude()) + "kernel bad(qubit target) { quantum bell_pair target }\n");
    if (!require(!unsupported_body.ok() && has_code(unsupported_body.diagnostics, "SYNQ-P017"),
                 "routine bodies reject unsupported one-gate shapes")) return false;

    const auto malformed_call = parser.parseSourceWithDiagnostics(
        std::string(u3_prelude()) + "call rotate(pi/2, )\n");
    return require(!malformed_call.ok() && has_code(malformed_call.diagnostics, "SYNQ-P018"),
                   "malformed parameterized routine calls use the reserved invocation diagnostic");
}

bool rejects_invalid_u3_call_resolution() {
    Parser parser;
    const std::string base = std::string(u3_prelude()) +
        "qubit qa[1]\n"
        "qubit qb[1]\n"
        "kernel rotate(angle theta, qubit target) { quantum rz(theta) target }\n"
        "kernel link(qubit control, qubit target) { quantum cx control, target }\n";
    const auto check = [&parser, &base](const std::string& call, const std::string& code, const std::string& message) {
        const auto parsed = parser.parseSourceWithDiagnostics(base + call);
        if (!require(parsed.ok(), message + " parses before static resolution")) return false;
        const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
        if (!require(lowered.ok(), message + " lowers before static resolution")) return false;
        const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
        return require(!resolved.ok() && has_code(resolved.diagnostics, code), message);
    };
    if (!check("call missing(q[0])\n", "SYNQ-R004", "unknown parameterized routine targets are rejected")) return false;
    if (!check("call rotate(pi/2)\n", "SYNQ-R005", "wrong parameterized routine arity is rejected")) return false;
    if (!check("call rotate(q[0], q[0])\n", "SYNQ-R005", "wrong parameterized routine actual kind/order is rejected")) return false;
    if (!check("call link(qa[0], qa[0])\n", "SYNQ-R006", "duplicate physical two-qubit actuals are rejected")) return false;
    return check("call rotate(pi/2, qa[1])\n", "SYNQ-Q002", "out-of-range parameterized routine actual operands are rejected");
}

bool preserves_zero_parameter_behavior_and_rejects_simulation() {
    Parser parser;
    const auto zero_parameter = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"callable-declarations\")]\n"
        "qubit q[1]\n"
        "kernel prepare() { quantum h q[0] }\n"
        "call prepare()\n");
    if (!require(zero_parameter.ok(), "existing zero-parameter kernel syntax remains accepted")) return false;
    const auto zero_lowered = synq::compiler::lower_to_hybrid_ir(*zero_parameter.program);
    const auto zero_resolved = synq::compiler::resolve_hybrid_names(*zero_lowered.program);
    const auto zero_exported = synq::compiler::export_hybrid_openqasm3(*zero_lowered.program);
    if (!require(zero_resolved.ok() && zero_exported.ok() && zero_exported.program.find("h q[0];") != std::string::npos,
                 "zero-parameter bounded kernels retain their earlier strict Hybrid lowering")) return false;

    const auto u3 = parser.parseSourceWithDiagnostics(std::string(u3_prelude()) +
        "qubit q[1]\n"
        "kernel flip(qubit target) { quantum x target }\n"
        "call flip(q[0])\n");
    if (!require(u3.ok(), "simulator-boundary U3 fixture parses")) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*u3.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "simulator-boundary U3 fixture resolves")) return false;
    BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    const auto simulation = synq::compiler::simulate_bounded_quantum(*resolved.program, options);
    return require(!simulation.ok() && has_code(simulation.diagnostics, "SYNQ-SIM002"),
                   "bounded simulator explicitly rejects Alpha parameterized routine nodes");
}

bool bounds_export_expansion_and_rejects_unexpanded_nodes() {
    Parser parser;
    std::string declaration_limit = u3_prelude();
    for (std::size_t index = 0; index < 33; ++index) {
        declaration_limit += "kernel f" + std::to_string(index) + "(qubit target) { quantum x target }\n";
    }
    const auto too_many_declarations = parser.parseSourceWithDiagnostics(declaration_limit);
    if (!require(too_many_declarations.ok(), "declaration-limit fixture parses")) return false;
    const auto declaration_lowered = synq::compiler::lower_to_hybrid_ir(*too_many_declarations.program);
    const auto declaration_exported = synq::compiler::export_hybrid_openqasm3(*declaration_lowered.program);
    if (!require(!declaration_exported.ok() && has_export_text(declaration_exported.diagnostics, "SYNQ-H003"),
                 "strict Hybrid export rejects more than 32 U3 declarations")) return false;

    std::string call_limit = std::string(u3_prelude()) +
        "qubit q[1]\n"
        "kernel f(qubit target) { quantum x target }\n";
    for (std::size_t index = 0; index < 129; ++index) call_limit += "call f(q[0])\n";
    const auto too_many_calls = parser.parseSourceWithDiagnostics(call_limit);
    if (!require(too_many_calls.ok(), "call-limit fixture parses")) return false;
    const auto call_lowered = synq::compiler::lower_to_hybrid_ir(*too_many_calls.program);
    const auto call_resolved = synq::compiler::resolve_hybrid_names(*call_lowered.program);
    if (!require(call_resolved.ok(), "call-limit fixture resolves")) return false;
    const auto call_exported = synq::compiler::export_hybrid_openqasm3(*call_lowered.program);
    if (!require(!call_exported.ok() && has_export_text(call_exported.diagnostics, "SYNQ-H003"),
                 "strict Hybrid export rejects more than 128 U3 calls")) return false;

    HybridProgram malformed;
    malformed.nodes.emplace_back(HybridQubitDeclaration{"q", 1, SourceSpan{1, 1, 1}});
    malformed.nodes.emplace_back(HybridCallableDeclaration{
        CallableDeclarationKind::Kernel,
        "bad",
        std::nullopt,
        {HybridRoutineFormal{RoutineFormalKind::Qubit, "target"}},
        HybridParameterizedRoutineBody{QuantumGateKind::Cx, "cx", std::nullopt, {"target", "target"}, SourceSpan{2, 1, 2}},
        SourceSpan{2, 1, 2}});
    const auto malformed_exported = synq::compiler::export_hybrid_openqasm3(malformed);
    return require(!malformed_exported.ok() && has_export_text(malformed_exported.diagnostics, "unresolved or malformed"),
                   "strict Hybrid exporter rejects an unexpanded malformed U3 declaration rather than emitting a runtime definition");
}

}  // namespace

int main() {
    if (!parses_lowers_resolves_and_expands_all_documented_signatures()) return 1;
    if (!enforces_u3_feature_gates_and_parse_boundaries()) return 1;
    if (!rejects_invalid_u3_call_resolution()) return 1;
    if (!preserves_zero_parameter_behavior_and_rejects_simulation()) return 1;
    if (!bounds_export_expansion_and_rejects_unexpanded_nodes()) return 1;
    std::cout << "SynQ parameterized routine smoke test passed\n";
    return 0;
}
