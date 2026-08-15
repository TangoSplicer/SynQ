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

bool checked_arithmetic(ClassicalIntegerArithmeticExpressionKind kind, std::int64_t left, std::int64_t right,
                        std::int64_t& result) {
    __int128 raw = 0;
    switch (kind) {
        case ClassicalIntegerArithmeticExpressionKind::Add:
            raw = static_cast<__int128>(left) + static_cast<__int128>(right);
            break;
        case ClassicalIntegerArithmeticExpressionKind::Subtract:
            raw = static_cast<__int128>(left) - static_cast<__int128>(right);
            break;
        case ClassicalIntegerArithmeticExpressionKind::Multiply:
            raw = static_cast<__int128>(left) * static_cast<__int128>(right);
            break;
        default:
            return false;
    }
    if (raw < std::numeric_limits<std::int64_t>::min() || raw > std::numeric_limits<std::int64_t>::max()) return false;
    result = static_cast<std::int64_t>(raw);
    return true;
}

bool evaluate_integer_tree(const ClassicalIntegerArithmeticExpression& expression,
                           const std::map<std::string, BoundedValue>& values,
                           std::int64_t& result, Diagnostic& diagnostic) {
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
                           "use a parser-produced one-operator Integer expression tree");
        return false;
    }

    std::int64_t left = 0;
    std::int64_t right = 0;
    if (!evaluate_integer_tree(expression.operands[0], values, left, diagnostic) ||
        !evaluate_integer_tree(expression.operands[1], values, right, diagnostic)) {
        return false;
    }
    if (!checked_arithmetic(expression.kind, left, right, result)) {
        diagnostic = error("SYNQ-E005", expression.span, "Integer arithmetic is invalid or overflows int64",
                           "use an in-range one-operator Integer expression");
        return false;
    }
    return true;
}

bool evaluate_initializer(const ResolvedHybridDeclaration& declaration,
                          const std::map<std::string, BoundedValue>& values,
                          BoundedValue& value, Diagnostic& diagnostic) {
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
            if (!evaluate_integer_tree(*initializer.integer_arithmetic, values, parsed, diagnostic)) return false;
            value = BoundedValue{BoundedValueKind::Integer, parsed, false, {}};
            return true;
        }
        case ClassicalExpressionKind::DecimalLiteral:
        case ClassicalExpressionKind::OpaqueSource:
            diagnostic = error("SYNQ-E002", initializer.span,
                               "declaration initializer is outside the bounded constant-evaluation subset",
                               "use Integer, Boolean, quoted String, prior-binding alias, or opted-in bounded Integer arithmetic");
            return false;
    }
    diagnostic = error("SYNQ-E004", initializer.span, "unknown internal declaration initializer kind",
                       "use a parser-produced supported declaration");
    return false;
}

}  // namespace

bool BoundedEvaluationResult::ok() const { return evaluation.has_value() && diagnostics.empty(); }

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
        if (!evaluate_initializer(*declaration, values, value, diagnostic)) {
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

}  // namespace synq::compiler
