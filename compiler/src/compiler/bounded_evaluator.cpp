#include "bounded_evaluator.h"

#include <charconv>
#include <limits>
#include <map>
#include <utility>

namespace synq::compiler {
namespace {

Diagnostic error(const std::string& code, const SourceSpan& span, std::string message, std::string help) {
    return Diagnostic{code, DiagnosticSeverity::Error, span, std::move(message), std::move(help)};
}

bool parse_integer(const std::string& text, std::int64_t& value) {
    const char* begin = text.data();
    const char* end = begin + text.size();
    const auto parsed = std::from_chars(begin, end, value);
    return parsed.ec == std::errc{} && parsed.ptr == end;
}

struct EvaluationBudget {
    std::size_t max_depth = 0;
    std::size_t max_operations = 0;
    std::size_t operations = 0;
};

bool enter_expression(const SourceSpan& span, std::size_t depth, EvaluationBudget& budget, Diagnostic& diagnostic) {
    if (depth > budget.max_depth) {
        diagnostic = error("SYNQ-E006", span, "bounded constant evaluation exceeds its expression-depth limit",
                           "reduce expression nesting or increase the explicitly configured depth limit");
        return false;
    }
    return true;
}

bool consume_operation(const SourceSpan& span, EvaluationBudget& budget, Diagnostic& diagnostic) {
    if (budget.operations >= budget.max_operations) {
        diagnostic = error("SYNQ-E007", span, "bounded constant evaluation exceeds its operation limit",
                           "reduce expression operations or increase the explicitly configured operation limit");
        return false;
    }
    ++budget.operations;
    return true;
}

bool checked_arithmetic(ClassicalIntegerArithmeticExpressionKind kind, std::int64_t left, std::int64_t right,
                        std::int64_t& result) {
    constexpr auto minimum = std::numeric_limits<std::int64_t>::min();
    constexpr auto maximum = std::numeric_limits<std::int64_t>::max();
    switch (kind) {
        case ClassicalIntegerArithmeticExpressionKind::Add:
            if ((right > 0 && left > maximum - right) || (right < 0 && left < minimum - right)) return false;
            result = left + right;
            return true;
        case ClassicalIntegerArithmeticExpressionKind::Subtract:
            if ((right < 0 && left > maximum + right) || (right > 0 && left < minimum + right)) return false;
            result = left - right;
            return true;
        case ClassicalIntegerArithmeticExpressionKind::Multiply:
            if (left > 0) {
                if ((right > 0 && left > maximum / right) || (right < 0 && right < minimum / left)) return false;
            } else if (left < 0) {
                if ((right > 0 && left < minimum / right) || (right < 0 && left < maximum / right)) return false;
            }
            result = left * right;
            return true;
        default:
            return false;
    }
}

bool evaluate_integer_tree(const ClassicalIntegerArithmeticExpression& expression,
                           const std::map<std::string, BoundedValue>& values,
                           std::int64_t& result, EvaluationBudget& budget,
                           std::size_t depth, Diagnostic& diagnostic) {
    if (!enter_expression(expression.span, depth, budget, diagnostic)) return false;
    if (expression.kind == ClassicalIntegerArithmeticExpressionKind::IntegerLiteral) {
        if (!parse_integer(expression.source_text, result)) {
            diagnostic = error("SYNQ-E004", expression.span, "invalid internal Integer literal in evaluation tree",
                               "use a parser-produced bounded Integer expression tree");
            return false;
        }
        return true;
    }
    if (expression.kind == ClassicalIntegerArithmeticExpressionKind::IdentifierReference) {
        const auto found = values.find(expression.source_text);
        if (found == values.end() || found->second.kind != BoundedValueKind::Integer) {
            diagnostic = error("SYNQ-E003", expression.span,
                               "Integer evaluation reference `" + expression.source_text + "` has no prior evaluated Integer binding",
                               "use an earlier supported Integer declaration");
            return false;
        }
        result = found->second.integer_value;
        return true;
    }
    if (expression.operands.size() != 2) {
        diagnostic = error("SYNQ-E004", expression.span, "invalid internal Integer arithmetic tree shape",
                           "use a parser-produced bounded Integer expression tree");
        return false;
    }
    std::int64_t left = 0;
    std::int64_t right = 0;
    if (!evaluate_integer_tree(expression.operands[0], values, left, budget, depth + 1, diagnostic) ||
        !evaluate_integer_tree(expression.operands[1], values, right, budget, depth + 1, diagnostic) ||
        !consume_operation(expression.span, budget, diagnostic)) return false;
    if (!checked_arithmetic(expression.kind, left, right, result)) {
        diagnostic = error("SYNQ-E005", expression.span, "Integer arithmetic is invalid or overflows int64",
                           "use an in-range bounded Integer expression");
        return false;
    }
    return true;
}

bool evaluate_boolean_tree(const ClassicalBooleanExpression& expression,
                           const std::map<std::string, BoundedValue>& values,
                           bool& result, EvaluationBudget& budget,
                           std::size_t depth, Diagnostic& diagnostic) {
    if (!enter_expression(expression.span, depth, budget, diagnostic)) return false;
    if (expression.kind == ClassicalBooleanExpressionKind::BooleanLiteral) {
        result = expression.boolean_value;
        return true;
    }
    if (expression.kind == ClassicalBooleanExpressionKind::IdentifierReference) {
        const auto found = values.find(expression.source_text);
        if (found == values.end() || found->second.kind != BoundedValueKind::Boolean) {
            diagnostic = error("SYNQ-E003", expression.span,
                               "Boolean evaluation reference `" + expression.source_text + "` has no prior evaluated Boolean binding",
                               "use an earlier supported Boolean declaration");
            return false;
        }
        result = found->second.boolean_value;
        return true;
    }
    const std::size_t expected = expression.kind == ClassicalBooleanExpressionKind::Not ? 1 : 2;
    if (expression.operands.size() != expected) {
        diagnostic = error("SYNQ-E004", expression.span, "invalid internal Boolean expression tree shape",
                           "use a parser-produced bounded Boolean expression tree");
        return false;
    }
    bool left = false;
    if (!evaluate_boolean_tree(expression.operands[0], values, left, budget, depth + 1, diagnostic) ||
        !consume_operation(expression.span, budget, diagnostic)) return false;
    if (expression.kind == ClassicalBooleanExpressionKind::Not) {
        result = !left;
        return true;
    }
    bool right = false;
    if (!evaluate_boolean_tree(expression.operands[1], values, right, budget, depth + 1, diagnostic)) return false;
    result = expression.kind == ClassicalBooleanExpressionKind::And ? left && right : left || right;
    return true;
}

bool evaluate_initializer(const ResolvedHybridDeclaration& declaration,
                          const std::map<std::string, BoundedValue>& values,
                          BoundedValue& value, EvaluationBudget& budget, Diagnostic& diagnostic) {
    const auto& initializer = declaration.declaration.initializer;
    switch (initializer.kind) {
        case ClassicalExpressionKind::IntegerLiteral: {
            std::int64_t parsed = 0;
            if (!parse_integer(initializer.source_text, parsed)) {
                diagnostic = error("SYNQ-E004", initializer.span, "invalid internal Integer literal classification",
                                   "use a parser-produced Integer declaration");
                return false;
            }
            value = BoundedValue{BoundedValueKind::Integer, parsed, false, {}};
            return true;
        }
        case ClassicalExpressionKind::BooleanLiteral:
            value = BoundedValue{BoundedValueKind::Boolean, 0, initializer.source_text == "true", {}};
            return true;
        case ClassicalExpressionKind::QuotedStringLiteral:
            if (initializer.source_text.size() < 2) {
                diagnostic = error("SYNQ-E004", initializer.span, "invalid internal quoted-string classification",
                                   "use a parser-produced quoted string declaration");
                return false;
            }
            value = BoundedValue{BoundedValueKind::String, 0, false,
                                 initializer.source_text.substr(1, initializer.source_text.size() - 2)};
            return true;
        case ClassicalExpressionKind::IdentifierReference: {
            const auto found = values.find(initializer.source_text);
            if (found == values.end()) {
                diagnostic = error("SYNQ-E003", initializer.span,
                                   "evaluation reference `" + initializer.source_text + "` has no prior evaluated binding",
                                   "use an earlier supported declaration");
                return false;
            }
            value = found->second;
            return true;
        }
        case ClassicalExpressionKind::IntegerArithmeticExpression: {
            if (!initializer.integer_arithmetic.has_value()) {
                diagnostic = error("SYNQ-E004", initializer.span, "missing internal Integer arithmetic tree",
                                   "use a parser-produced bounded Integer expression");
                return false;
            }
            std::int64_t parsed = 0;
            if (!evaluate_integer_tree(*initializer.integer_arithmetic, values, parsed, budget, 1, diagnostic)) return false;
            value = BoundedValue{BoundedValueKind::Integer, parsed, false, {}};
            return true;
        }
        case ClassicalExpressionKind::BooleanExpression: {
            if (!initializer.boolean_expression.has_value()) {
                diagnostic = error("SYNQ-E004", initializer.span, "missing internal Boolean expression tree",
                                   "use a parser-produced bounded Boolean expression");
                return false;
            }
            bool parsed = false;
            if (!evaluate_boolean_tree(*initializer.boolean_expression, values, parsed, budget, 1, diagnostic)) return false;
            value = BoundedValue{BoundedValueKind::Boolean, 0, parsed, {}};
            return true;
        }
        case ClassicalExpressionKind::DecimalLiteral:
        case ClassicalExpressionKind::OpaqueSource:
            diagnostic = error("SYNQ-E002", initializer.span,
                               "declaration initializer is outside the bounded constant-evaluation subset",
                               "use Integer, Boolean, quoted String, prior-binding alias, or bounded Alpha expressions");
            return false;
    }
    diagnostic = error("SYNQ-E004", initializer.span, "unknown internal declaration initializer kind",
                       "use a parser-produced supported declaration");
    return false;
}

bool evaluate_state_expression(const ClassicalExpression& expression,
                               const std::map<std::string, BoundedValue>& values,
                               BoundedValue& value, EvaluationBudget& budget, Diagnostic& diagnostic) {
    switch (expression.kind) {
        case ClassicalExpressionKind::IntegerLiteral: {
            std::int64_t parsed = 0;
            if (!parse_integer(expression.source_text, parsed)) {
                diagnostic = error("SYNQ-E010", expression.span, "invalid internal Integer literal in state evaluation",
                                   "use a parser-produced supported state expression");
                return false;
            }
            value = BoundedValue{BoundedValueKind::Integer, parsed, false, {}};
            return true;
        }
        case ClassicalExpressionKind::BooleanLiteral:
            value = BoundedValue{BoundedValueKind::Boolean, 0, expression.source_text == "true", {}};
            return true;
        case ClassicalExpressionKind::QuotedStringLiteral:
            if (expression.source_text.size() < 2) {
                diagnostic = error("SYNQ-E010", expression.span, "invalid internal quoted-string state expression",
                                   "use a parser-produced supported state expression");
                return false;
            }
            value = BoundedValue{BoundedValueKind::String, 0, false,
                                 expression.source_text.substr(1, expression.source_text.size() - 2)};
            return true;
        case ClassicalExpressionKind::IdentifierReference: {
            const auto found = values.find(expression.source_text);
            if (found == values.end()) {
                diagnostic = error("SYNQ-E010", expression.span,
                                   "state evaluation reference has no earlier evaluated binding",
                                   "use an earlier immutable declaration or mutable cell");
                return false;
            }
            value = found->second;
            return true;
        }
        case ClassicalExpressionKind::IntegerArithmeticExpression: {
            if (!expression.integer_arithmetic.has_value()) {
                diagnostic = error("SYNQ-E010", expression.span, "missing internal Integer state expression tree",
                                   "use a parser-produced bounded Integer expression");
                return false;
            }
            std::int64_t parsed = 0;
            if (!evaluate_integer_tree(*expression.integer_arithmetic, values, parsed, budget, 0, diagnostic)) return false;
            value = BoundedValue{BoundedValueKind::Integer, parsed, false, {}};
            return true;
        }
        case ClassicalExpressionKind::BooleanExpression: {
            if (!expression.boolean_expression.has_value()) {
                diagnostic = error("SYNQ-E010", expression.span, "missing internal Boolean state expression tree",
                                   "use a parser-produced bounded Boolean expression");
                return false;
            }
            bool parsed = false;
            if (!evaluate_boolean_tree(*expression.boolean_expression, values, parsed, budget, 0, diagnostic)) return false;
            value = BoundedValue{BoundedValueKind::Boolean, 0, parsed, {}};
            return true;
        }
        case ClassicalExpressionKind::DecimalLiteral:
        case ClassicalExpressionKind::OpaqueSource:
            diagnostic = error("SYNQ-E010", expression.span,
                               "state evaluation expression is outside the bounded U2 subset",
                               "use Boolean, Integer, quoted String, prior-binding alias, or bounded Alpha expressions");
            return false;
    }
    diagnostic = error("SYNQ-E010", expression.span, "unknown internal state expression kind",
                       "use a parser-produced supported state expression");
    return false;
}

}  // namespace

bool BoundedEvaluationResult::ok() const { return evaluation.has_value() && diagnostics.empty(); }

bool BoundedStateEvaluationResult::ok() const { return evaluation.has_value() && diagnostics.empty(); }

const char* bounded_value_kind_name(BoundedValueKind kind) {
    switch (kind) {
        case BoundedValueKind::Integer: return "Integer";
        case BoundedValueKind::Boolean: return "Boolean";
        case BoundedValueKind::String: return "String";
    }
    return "Unknown";
}

BoundedEvaluationResult evaluate_bounded_constants(const ResolvedHybridProgram& program,
                                                   const BoundedEvaluationOptions& options) {
    BoundedEvaluationResult result;
    if (!options.allow_experimental_constant_evaluation) {
        result.diagnostics.push_back(error("SYNQ-E000", {}, "bounded constant evaluation requires explicit opt-in",
                                           "set allow_experimental_constant_evaluation to true after reviewing its limits"));
        return result;
    }
    if (program.nodes.size() > options.max_declarations) {
        result.diagnostics.push_back(error("SYNQ-E001", {}, "bounded constant evaluation exceeds its declaration limit",
                                           "reduce the program to the configured declaration limit"));
        return result;
    }
    BoundedEvaluation evaluation;
    std::map<std::string, BoundedValue> values;
    EvaluationBudget budget{options.max_expression_depth, options.max_operations, 0};
    for (const auto& node : program.nodes) {
        const auto* declaration = std::get_if<ResolvedHybridDeclaration>(&node);
        if (declaration == nullptr) {
            result.diagnostics.push_back(error("SYNQ-E002", {},
                                               "bounded constant evaluation accepts declarations only",
                                               "use the supported CLI export path for quantum statements and keep evaluation inputs declaration-only"));
            return result;
        }
        BoundedValue value;
        Diagnostic diagnostic;
        if (!evaluate_initializer(*declaration, values, value, budget, diagnostic)) {
            result.diagnostics.push_back(std::move(diagnostic));
            return result;
        }
        values.emplace(declaration->declaration.name, value);
        evaluation.bindings.push_back(EvaluatedBinding{declaration->declaration.name, std::move(value),
                                                       declaration->declaration.span});
    }
    result.evaluation = std::move(evaluation);
    return result;
}

BoundedStateEvaluationResult evaluate_bounded_state(const ResolvedHybridProgram& program,
                                                     const BoundedStateEvaluationOptions& options) {
    BoundedStateEvaluationResult result;
    if (!options.allow_experimental_state_evaluation) {
        result.diagnostics.push_back(error("SYNQ-E008", {}, "bounded state evaluation requires explicit opt-in",
                                           "set allow_experimental_state_evaluation to true after reviewing its limits"));
        return result;
    }

    BoundedStateEvaluation evaluation;
    std::map<std::string, BoundedValue> values;
    std::map<std::string, std::size_t> cell_indices;
    EvaluationBudget budget{options.max_expression_depth, options.max_operations, 0};
    std::size_t transitions = 0;
    const auto consume_transition = [&](const SourceSpan& span) -> bool {
        if (transitions >= options.max_state_transitions) {
            result.diagnostics.push_back(error("SYNQ-E009", span,
                                               "bounded state evaluation exceeds its state-transition limit",
                                               "reduce state initializations/writes or increase the explicitly configured limit"));
            return false;
        }
        ++transitions;
        return true;
    };

    for (const ResolvedHybridNode& node : program.nodes) {
        if (const auto* declaration = std::get_if<ResolvedHybridDeclaration>(&node)) {
            BoundedValue value;
            Diagnostic diagnostic;
            if (!evaluate_state_expression(declaration->declaration.initializer, values, value, budget, diagnostic)) {
                result.diagnostics.push_back(std::move(diagnostic));
                return result;
            }
            values.emplace(declaration->declaration.name, std::move(value));
            continue;
        }

        if (const auto* declaration = std::get_if<ResolvedHybridMutableDeclaration>(&node)) {
            if (evaluation.cells.size() >= options.max_state_cells) {
                result.diagnostics.push_back(error("SYNQ-E009", declaration->declaration.span,
                                                   "bounded state evaluation exceeds its mutable-cell limit",
                                                   "reduce mutable cells or increase the explicitly configured limit"));
                return result;
            }
            BoundedValue value;
            Diagnostic diagnostic;
            if (!evaluate_state_expression(declaration->declaration.initializer, values, value, budget, diagnostic)) {
                result.diagnostics.push_back(std::move(diagnostic));
                return result;
            }
            if (!consume_transition(declaration->declaration.span)) return result;
            cell_indices.emplace(declaration->declaration.name, evaluation.cells.size());
            values.emplace(declaration->declaration.name, value);
            evaluation.cells.push_back({declaration->declaration.name, std::move(value), declaration->declaration.span,
                                        declaration->declaration.span});
            continue;
        }

        if (const auto* assignment = std::get_if<ResolvedHybridAssignment>(&node)) {
            const auto cell = cell_indices.find(assignment->assignment.target_name);
            if (cell == cell_indices.end()) {
                result.diagnostics.push_back(error("SYNQ-E010", assignment->assignment.span,
                                                   "state evaluator received an assignment without an active mutable cell",
                                                   "use a resolver-produced assignment to an earlier mutable cell"));
                return result;
            }
            BoundedValue value;
            Diagnostic diagnostic;
            if (!evaluate_state_expression(assignment->assignment.value, values, value, budget, diagnostic)) {
                result.diagnostics.push_back(std::move(diagnostic));
                return result;
            }
            if (value.kind != evaluation.cells[cell->second].value.kind) {
                result.diagnostics.push_back(error("SYNQ-E010", assignment->assignment.span,
                                                   "state evaluator received a static-type-incompatible assignment",
                                                   "use a resolver-produced assignment with the cell's exact static type"));
                return result;
            }
            if (!consume_transition(assignment->assignment.span)) return result;
            values[assignment->assignment.target_name] = value;
            evaluation.cells[cell->second].value = std::move(value);
            evaluation.cells[cell->second].last_write_span = assignment->assignment.span;
            continue;
        }

        result.diagnostics.push_back(error("SYNQ-E010", {},
                                           "bounded state evaluation accepts only immutable declarations, mutable cells, and assignments",
                                           "keep U2 state-evaluation inputs within the documented top-level classical subset"));
        return result;
    }

    result.evaluation = std::move(evaluation);
    return result;
}

}  // namespace synq::compiler
