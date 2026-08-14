#include "classical_expression.h"

#include <cctype>
#include <sstream>
#include <vector>

namespace synq::compiler {

namespace {

bool is_identifier_reference(const std::string& text) {
    if (text.empty()) return false;
    const auto first = static_cast<unsigned char>(text.front());
    if (!(std::isalpha(first) || text.front() == '_')) return false;
    for (char character : text) {
        const auto value = static_cast<unsigned char>(character);
        if (!(std::isalnum(value) || character == '_')) return false;
    }
    return true;
}

bool is_integer_literal(const std::string& text) {
    const std::size_t start = !text.empty() && text.front() == '-' ? 1 : 0;
    if (start == text.size()) return false;
    for (std::size_t index = start; index < text.size(); ++index) {
        if (std::isdigit(static_cast<unsigned char>(text[index])) == 0) return false;
    }
    return true;
}

bool parse_integer_arithmetic_atom(const std::string& source_text,
                                   const SourceSpan& span,
                                   ClassicalIntegerArithmeticExpression& expression) {
    if (is_integer_literal(source_text)) {
        expression = {ClassicalIntegerArithmeticExpressionKind::IntegerLiteral, source_text, span, {}};
        return true;
    }
    if (is_identifier_reference(source_text)) {
        expression = {ClassicalIntegerArithmeticExpressionKind::IdentifierReference, source_text, span, {}};
        return true;
    }
    return false;
}

}  // namespace

bool looks_like_integer_arithmetic_expression(const std::string& source_text) {
    std::istringstream tokens(source_text);
    std::string token;
    while (tokens >> token) {
        if (token == "+" || token == "-" || token == "*") return true;
    }
    return false;
}

bool parse_bounded_integer_arithmetic_expression(const std::string& source_text,
                                                 const SourceSpan& span,
                                                 ClassicalIntegerArithmeticExpression& expression) {
    std::istringstream tokens(source_text);
    std::vector<std::string> words;
    std::string word;
    while (tokens >> word) words.push_back(word);
    if (words.size() != 3 || (words[1] != "+" && words[1] != "-" && words[1] != "*")) return false;

    ClassicalIntegerArithmeticExpression left;
    ClassicalIntegerArithmeticExpression right;
    if (!parse_integer_arithmetic_atom(words[0], span, left) ||
        !parse_integer_arithmetic_atom(words[2], span, right)) {
        return false;
    }
    const ClassicalIntegerArithmeticExpressionKind kind = words[1] == "+"
        ? ClassicalIntegerArithmeticExpressionKind::Add
        : words[1] == "-" ? ClassicalIntegerArithmeticExpressionKind::Subtract
                            : ClassicalIntegerArithmeticExpressionKind::Multiply;
    expression = {kind, source_text, span, {std::move(left), std::move(right)}};
    return true;
}

ClassicalExpression make_classical_expression(const std::string& source_text,
                                               ClassicalLiteralKind literal_kind,
                                               const SourceSpan& span) {
    ClassicalExpression expression;
    expression.source_text = source_text;
    expression.span = span;

    switch (literal_kind) {
        case ClassicalLiteralKind::Integer:
            expression.kind = ClassicalExpressionKind::IntegerLiteral;
            expression.static_type = ClassicalStaticType::Integer;
            break;
        case ClassicalLiteralKind::Decimal:
            expression.kind = ClassicalExpressionKind::DecimalLiteral;
            expression.static_type = ClassicalStaticType::Decimal;
            break;
        case ClassicalLiteralKind::Boolean:
            expression.kind = ClassicalExpressionKind::BooleanLiteral;
            expression.static_type = ClassicalStaticType::Boolean;
            break;
        case ClassicalLiteralKind::QuotedString:
            expression.kind = ClassicalExpressionKind::QuotedStringLiteral;
            expression.static_type = ClassicalStaticType::String;
            break;
        case ClassicalLiteralKind::IntegerArithmeticExpression: {
            ClassicalIntegerArithmeticExpression arithmetic;
            if (parse_bounded_integer_arithmetic_expression(source_text, span, arithmetic)) {
                expression.kind = ClassicalExpressionKind::IntegerArithmeticExpression;
                expression.static_type = ClassicalStaticType::Integer;
                expression.integer_arithmetic = std::move(arithmetic);
                break;
            }
            expression.kind = ClassicalExpressionKind::OpaqueSource;
            expression.static_type = ClassicalStaticType::Unknown;
            break;
        }
        case ClassicalLiteralKind::SourceText:
            if (is_identifier_reference(source_text)) {
                expression.kind = ClassicalExpressionKind::IdentifierReference;
                expression.static_type = ClassicalStaticType::Unknown;
                break;
            }
            expression.kind = ClassicalExpressionKind::OpaqueSource;
            expression.static_type = ClassicalStaticType::Unknown;
            break;
    }
    return expression;
}

const char* classical_static_type_name(ClassicalStaticType type) {
    switch (type) {
        case ClassicalStaticType::Integer: return "Integer";
        case ClassicalStaticType::Decimal: return "Decimal";
        case ClassicalStaticType::Boolean: return "Boolean";
        case ClassicalStaticType::String: return "String";
        case ClassicalStaticType::Unknown: return "Unknown";
    }
    return "Unknown";
}

}  // namespace synq::compiler
