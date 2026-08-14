#ifndef SYNQ_COMPILER_CLASSICAL_EXPRESSION_H
#define SYNQ_COMPILER_CLASSICAL_EXPRESSION_H

#include <optional>
#include <string>

#include "ast.h"

namespace synq::compiler {

// A bounded internal expression classification. It describes source already
// accepted by the recovery parser; it does not evaluate that source.
enum class ClassicalExpressionKind {
    IntegerLiteral,
    DecimalLiteral,
    BooleanLiteral,
    QuotedStringLiteral,
    IdentifierReference,
    IntegerArithmeticExpression,
    OpaqueSource,
};

enum class ClassicalStaticType {
    Integer,
    Decimal,
    Boolean,
    String,
    Unknown,
};

struct ClassicalExpression {
    ClassicalExpressionKind kind = ClassicalExpressionKind::OpaqueSource;
    ClassicalStaticType static_type = ClassicalStaticType::Unknown;
    std::string source_text;
    SourceSpan span;
    std::optional<ClassicalIntegerArithmeticExpression> integer_arithmetic;
};

bool looks_like_integer_arithmetic_expression(const std::string& source_text);
bool parse_bounded_integer_arithmetic_expression(const std::string& source_text,
                                                 const SourceSpan& span,
                                                 ClassicalIntegerArithmeticExpression& expression);
ClassicalExpression make_classical_expression(const std::string& source_text,
                                               ClassicalLiteralKind literal_kind,
                                               const SourceSpan& span);
const char* classical_static_type_name(ClassicalStaticType type);

}  // namespace synq::compiler

#endif
