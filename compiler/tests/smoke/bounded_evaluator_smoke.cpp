#include <iostream>
#include <string>

#include "compiler/bounded_evaluator.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
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

bool evaluate_source(const std::string& source, synq::compiler::BoundedEvaluationResult& result,
                     std::size_t max_declarations = 64, std::size_t max_depth = 16,
                     std::size_t max_operations = 128) {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!parsed.ok()) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!lowered.ok()) return false;
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!resolved.ok()) return false;
    synq::compiler::BoundedEvaluationOptions options;
    options.allow_experimental_constant_evaluation = true;
    options.max_declarations = max_declarations;
    options.max_expression_depth = max_depth;
    options.max_operations = max_operations;
    result = synq::compiler::evaluate_bounded_constants(*resolved.program, options);
    return true;
}

bool evaluates_supported_declaration_subset() {
    synq::compiler::BoundedEvaluationResult result;
    if (!require(evaluate_source(
                     "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
                     "#[experimental(feature = \"classical-control-flow\")]\n"
                     "let seed = 12\n"
                     "let alias = seed\n"
                     "let total = seed + 3\n"
                     "let product = total * 2\n"
                     "let enabled = true\n"
                     "let disabled = not enabled\n"
                     "let ready = enabled or disabled\n"
                     "let title = \"SynQ\"\n",
                     result),
                 "supported evaluator fixture parses, lowers, and resolves")) return false;
    if (!require(result.ok() && result.evaluation->bindings.size() == 8,
                 "bounded evaluator returns eight deterministic bindings")) return false;
    const auto& bindings = result.evaluation->bindings;
    return require(bindings[0].value.integer_value == 12 && bindings[1].value.integer_value == 12 &&
                       bindings[2].value.integer_value == 15 && bindings[3].value.integer_value == 30 &&
                       bindings[4].value.kind == synq::compiler::BoundedValueKind::Boolean &&
                       bindings[4].value.boolean_value && !bindings[5].value.boolean_value &&
                       bindings[6].value.boolean_value && bindings[7].value.string_value == "SynQ",
                   "literals, aliases, typed Boolean expressions, and checked Integer arithmetic evaluate in source order");
}

bool enforces_explicit_opt_in_and_limits() {
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics("let value = 1\n");
    if (!require(parsed.ok(), "featureless literal declaration parses")) return false;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    synq::compiler::BoundedEvaluationOptions disabled;
    const auto disabled_result = synq::compiler::evaluate_bounded_constants(*resolved.program, disabled);
    if (!require(!disabled_result.ok() && has_code(disabled_result.diagnostics, "SYNQ-E000"),
                 "evaluation requires explicit API opt-in")) return false;

    synq::compiler::BoundedEvaluationResult limit_result;
    if (!require(evaluate_source("let first = 1\nlet second = 2\n", limit_result, 1),
                 "limit fixture parses, lowers, and resolves")) return false;
    return require(!limit_result.ok() && has_code(limit_result.diagnostics, "SYNQ-E001"),
                   "evaluation enforces a configured declaration limit");
}

bool enforces_expression_budgets() {
    synq::compiler::BoundedEvaluationResult operation_result;
    if (!require(evaluate_source(
                     "#[experimental(feature = \"classical-control-flow\")]\n"
                     "let enabled = true\n"
                     "let disabled = not enabled\n",
                     operation_result, 64, 16, 0),
                 "operation-budget fixture parses, lowers, and resolves")) return false;
    if (!require(!operation_result.ok() && has_code(operation_result.diagnostics, "SYNQ-E007"),
                 "evaluation rejects a Boolean operation beyond the configured budget")) return false;

    synq::compiler::BoundedEvaluationResult depth_result;
    if (!require(evaluate_source(
                     "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
                     "let seed = 1\n"
                     "let total = seed + 2\n",
                     depth_result, 64, 1),
                 "depth-budget fixture parses, lowers, and resolves")) return false;
    return require(!depth_result.ok() && has_code(depth_result.diagnostics, "SYNQ-E006"),
                   "evaluation rejects an expression beyond the configured depth limit");
}

bool rejects_unsupported_or_overflowing_semantics() {
    synq::compiler::BoundedEvaluationResult decimal_result;
    if (!require(evaluate_source("let ratio = 1.5\n", decimal_result), "decimal fixture parses, lowers, and resolves")) return false;
    if (!require(!decimal_result.ok() && has_code(decimal_result.diagnostics, "SYNQ-E002"),
                 "decimal evaluation is rejected rather than assigned invented semantics")) return false;

    synq::compiler::BoundedEvaluationResult quantum_result;
    if (!require(evaluate_source("quantum h q[0]\n", quantum_result), "quantum fixture parses, lowers, and resolves")) return false;
    if (!require(!quantum_result.ok() && has_code(quantum_result.diagnostics, "SYNQ-E002"),
                 "quantum statements are rejected by constant evaluation")) return false;

    synq::compiler::BoundedEvaluationResult overflow_result;
    if (!require(evaluate_source(
                     "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
                     "let maximum = 9223372036854775807\n"
                     "let overflow = maximum + 1\n",
                     overflow_result),
                 "overflow fixture parses, lowers, and resolves")) return false;
    if (!require(!overflow_result.ok() && has_code(overflow_result.diagnostics, "SYNQ-E005"),
                 "addition overflow is rejected deterministically")) return false;

    synq::compiler::BoundedEvaluationResult multiplication_overflow_result;
    if (!require(evaluate_source(
                     "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
                     "let maximum = 9223372036854775807\n"
                     "let overflow = maximum * 2\n",
                     multiplication_overflow_result),
                 "multiplication overflow fixture parses, lowers, and resolves")) return false;
    if (!require(!multiplication_overflow_result.ok() && has_code(multiplication_overflow_result.diagnostics, "SYNQ-E005"),
                 "multiplication overflow is rejected deterministically")) return false;

    synq::compiler::BoundedEvaluationResult subtraction_overflow_result;
    if (!require(evaluate_source(
                     "#[experimental(feature = \"integer-arithmetic-expressions\")]\n"
                     "let minimum = -9223372036854775808\n"
                     "let overflow = minimum - 1\n",
                     subtraction_overflow_result),
                 "subtraction overflow fixture parses, lowers, and resolves")) return false;
    return require(!subtraction_overflow_result.ok() && has_code(subtraction_overflow_result.diagnostics, "SYNQ-E005"),
                   "subtraction overflow is rejected deterministically");
}

}  // namespace

int main() {
    if (!evaluates_supported_declaration_subset()) return 1;
    if (!enforces_explicit_opt_in_and_limits()) return 1;
    if (!enforces_expression_budgets()) return 1;
    if (!rejects_unsupported_or_overflowing_semantics()) return 1;
    std::cout << "SynQ bounded evaluator smoke test passed\n";
    return 0;
}
