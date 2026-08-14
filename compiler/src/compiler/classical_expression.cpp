#include "classical_expression.h"

#include <cctype>

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

}  // namespace

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
        case ClassicalLiteralKind::SourceText:
            expression.kind = is_identifier_reference(source_text)
                ? ClassicalExpressionKind::IdentifierReference
                : ClassicalExpressionKind::OpaqueSource;
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
