#include <iostream>
#include <string>

#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
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

bool has_code(const std::vector<synq::compiler::Diagnostic>& diagnostics, const std::string& code) {
    return diagnostics.size() == 1 && diagnostics.front().code == code;
}

bool parses_lowers_and_resolves_declaration_only_callables() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\n"
        "fn prepare()\n"
        "kernel ansatz()\n");
    if (!require(parsed.ok(), "Alpha-gated declaration-only functions and kernels parse")) return false;

    const auto* function = dynamic_cast<const CallableDeclarationNode*>(parsed.program->statements.at(0));
    const auto* kernel = dynamic_cast<const CallableDeclarationNode*>(parsed.program->statements.at(1));
    if (!require(function != nullptr && function->kind == CallableDeclarationKind::Function &&
                     function->name == "prepare" && function->span.line == 2,
                 "typed function declaration preserves kind, name, and span")) return false;
    if (!require(kernel != nullptr && kernel->kind == CallableDeclarationKind::Kernel &&
                     kernel->name == "ansatz" && kernel->span.line == 3,
                 "typed kernel declaration preserves kind, name, and span")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "declaration-only callables lower to Hybrid IR")) return false;
    const auto* hybrid_function = std::get_if<synq::compiler::HybridCallableDeclaration>(&lowered.program->nodes.at(0));
    const auto* hybrid_kernel = std::get_if<synq::compiler::HybridCallableDeclaration>(&lowered.program->nodes.at(1));
    if (!require(hybrid_function != nullptr && hybrid_function->kind == CallableDeclarationKind::Function &&
                     hybrid_function->name == "prepare" && hybrid_kernel != nullptr &&
                     hybrid_kernel->kind == CallableDeclarationKind::Kernel && hybrid_kernel->name == "ansatz",
                 "Hybrid IR preserves callable declaration provenance")) return false;

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "name resolver preserves non-executing callable metadata")) return false;
    const auto* resolved_kernel = std::get_if<synq::compiler::HybridCallableDeclaration>(&resolved.program->nodes.at(1));
    if (!require(resolved_kernel != nullptr && resolved_kernel->name == "ansatz",
                 "resolved Hybrid IR retains callable declaration metadata")) return false;

    return require(!synq::compiler::export_hybrid_openqasm3(*lowered.program).ok(),
                   "strict Hybrid OpenQASM export rejects callable declarations rather than dropping them");
}

bool enforces_feature_and_bounded_declaration_shape() {
    Parser parser;
    const auto featureless = parser.parseSourceWithDiagnostics("fn prepare()\n");
    if (!require(!featureless.ok() && has_code(featureless.diagnostics, "SYNQ-P007"),
                 "callable declarations require explicit Alpha opt-in")) return false;

    const auto parameters = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\nfn prepare(value)\n");
    if (!require(!parameters.ok() && has_code(parameters.diagnostics, "SYNQ-P013"),
                 "callable declarations reject parameters")) return false;

    const auto body = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\nkernel ansatz() {}\n");
    if (!require(!body.ok() && has_code(body.diagnostics, "SYNQ-P013"),
                 "callable declarations reject bodies")) return false;

    const auto duplicate = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\nfn same()\nkernel same()\n");
    return require(!duplicate.ok() && has_code(duplicate.diagnostics, "SYNQ-S004"),
                   "functions and kernels share top-level declaration uniqueness");
}

bool lowers_one_bounded_kernel_call_without_runtime_execution() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "#[experimental(feature = \"callable-declarations\")]\n"
        "qubit q[1]\n"
        "kernel prepare() { quantum h q[0] }\n"
        "call prepare()\n");
    if (!require(parsed.ok(), "Alpha-gated bounded one-gate kernel and later call parse")) return false;

    const auto* kernel = dynamic_cast<const CallableDeclarationNode*>(parsed.program->statements.at(1));
    const auto* call = dynamic_cast<const CallableCallNode*>(parsed.program->statements.at(2));
    if (!require(kernel != nullptr && kernel->body != nullptr && kernel->kind == CallableDeclarationKind::Kernel &&
                     call != nullptr && call->name == "prepare",
                 "typed AST preserves bounded kernel body and call provenance")) return false;

    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!require(lowered.ok(), "bounded kernel and call lower to Hybrid IR")) return false;
    if (!require(std::get_if<synq::compiler::HybridCallableDeclaration>(&lowered.program->nodes.at(1)) != nullptr &&
                     std::get_if<synq::compiler::HybridCallableCall>(&lowered.program->nodes.at(2)) != nullptr,
                 "Hybrid IR preserves bounded callable definition and call provenance")) return false;

    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!require(resolved.ok(), "resolver validates bounded kernel resource and earlier call target")) return false;
    const auto exported = synq::compiler::export_hybrid_openqasm3(*lowered.program);
    const std::string expected =
        "OPENQASM 3.0;\n"
        "include \"stdgates.inc\";\n"
        "qubit[1] q;\n"
        "h q[0];\n";
    if (!require(exported.ok() && exported.program == expected,
                 "strict Hybrid exporter expands the bounded call without runtime dispatch")) return false;

    const auto unknown = parser.parseSourceWithDiagnostics(
        "#[experimental(feature = \"callable-declarations\")]\ncall missing()\n");
    if (!require(unknown.ok(), "bounded unknown call remains syntactically typed before resolution")) return false;
    const auto unknown_lowered = synq::compiler::lower_to_hybrid_ir(*unknown.program);
    const auto unknown_resolved = synq::compiler::resolve_hybrid_names(*unknown_lowered.program);
    return require(!unknown_resolved.ok() && has_code(unknown_resolved.diagnostics, "SYNQ-R003"),
                   "resolver rejects a call without an earlier bounded kernel definition");
}

}  // namespace

int main() {
    if (!parses_lowers_and_resolves_declaration_only_callables()) return 1;
    if (!enforces_feature_and_bounded_declaration_shape()) return 1;
    if (!lowers_one_bounded_kernel_call_without_runtime_execution()) return 1;
    std::cout << "SynQ callable declaration smoke test passed\n";
    return 0;
}
