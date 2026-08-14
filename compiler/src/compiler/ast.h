// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#ifndef SYNQ_COMPILER_AST_H
#define SYNQ_COMPILER_AST_H

#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "diagnostic.h"

class ASTNode {
public:
    virtual ~ASTNode() {}
       virtual std::string profileQKernels() { return ""; }
       virtual std::string annotateWithAI() { return ""; }
       virtual std::string renderFlowGraph() { return ""; }
       virtual void registerAIHooks(const std::string& hooks) { /* placeholder */ }
       virtual std::string toString() { return ""; }
       virtual std::string toIR() { return ""; }
       virtual std::string toQASM() { return ""; }
       virtual std::string toQIR() { return ""; }
       virtual std::string toJSON() { return ""; }
       virtual std::string toBinaryBlob() { return ""; }
       virtual std::string getName() { return ""; }
};

// AST node for the root of a program (contains a list of statements).
class ProgramNode : public ASTNode {
public:
    std::vector<ASTNode*> statements;
    ~ProgramNode();
};

// A deliberately small instruction node used by the recovery parser profile.
// It represents only the line-oriented instruction grammar documented in parser.cpp;
// it is not a complete SynQ language AST.
class InstructionNode : public ASTNode {
public:
    std::string op;
    std::vector<std::string> args;
    std::size_t line = 0;
    synq::compiler::SourceSpan span;

    InstructionNode(std::string operation, std::vector<std::string> arguments, std::size_t line_number)
        : InstructionNode(std::move(operation), std::move(arguments), line_number, {line_number, 0, 0}) {}

    InstructionNode(std::string operation, std::vector<std::string> arguments, std::size_t line_number,
                    synq::compiler::SourceSpan source_span)
        : op(std::move(operation)), args(std::move(arguments)), line(line_number), span(source_span) {}

    std::string toString() override {
        return op + (args.empty() ? "" : " " + args.front());
    }
};

// Typed representation of the bounded recovery-profile quantum statement.
// `Unknown` preserves a syntactically valid gate name without implying that it
// is supported by a backend or has a defined SynQ semantic meaning.
enum class QuantumGateKind {
    H,
    X,
    Y,
    Z,
    Cx,
    BellPair,
    Rx,
    Ry,
    Rz,
    Phase,
    Unknown,
};

class QuantumGateNode : public ASTNode {
public:
    QuantumGateKind kind;
    std::string source_name;
    std::optional<std::string> literal_angle;
    std::vector<std::size_t> qubit_indices;
    std::size_t line = 0;
    synq::compiler::SourceSpan span;

    QuantumGateNode(QuantumGateKind gate_kind,
                    std::string original_name,
                    std::optional<std::string> angle,
                    std::vector<std::size_t> operands,
                    std::size_t line_number)
        : QuantumGateNode(gate_kind, std::move(original_name), std::move(angle), std::move(operands),
                          line_number, {line_number, 0, 0}) {}

    QuantumGateNode(QuantumGateKind gate_kind,
                    std::string original_name,
                    std::optional<std::string> angle,
                    std::vector<std::size_t> operands,
                    std::size_t line_number,
                    synq::compiler::SourceSpan source_span)
        : kind(gate_kind),
          source_name(std::move(original_name)),
          literal_angle(std::move(angle)),
          qubit_indices(std::move(operands)),
          line(line_number),
          span(source_span) {}

    std::string toString() override {
        std::string text = "quantum " + source_name;
        if (literal_angle.has_value()) text += "(" + *literal_angle + ")";
        for (std::size_t index = 0; index < qubit_indices.size(); ++index) {
            text += index == 0 ? " q[" : ", q[";
            text += std::to_string(qubit_indices[index]) + "]";
        }
        return text;
    }
};

// Typed representation of the bounded recovery-profile quantum-to-classical
// source boundary. It records an indexed qubit but does not model a SynQ result
// value, execution, or classical control flow.
class MeasurementNode : public ASTNode {
public:
    std::size_t qubit_index = 0;
    std::size_t line = 0;
    synq::compiler::SourceSpan span;

    MeasurementNode(std::size_t index, std::size_t line_number)
        : MeasurementNode(index, line_number, {line_number, 0, 0}) {}

    MeasurementNode(std::size_t index, std::size_t line_number,
                    synq::compiler::SourceSpan source_span)
        : qubit_index(index), line(line_number), span(source_span) {}

    std::string toString() override {
        return "measure q[" + std::to_string(qubit_index) + "]";
    }
};

// Typed representation of the first bounded classical-control-flow profile.
// The condition is a parser-validated boolean literal and the owned body is
// limited to one typed quantum gate or measurement statement.
enum class ClassicalControlKind {
    If,
    While,
};

// Alpha control conditions are either parser-validated boolean literals or
// whole identifier references. The latter are resolved and type-checked only
// by the later internal resolver; neither condition form is evaluated here.
enum class ClassicalConditionKind {
    BooleanLiteral,
    IdentifierReference,
    BooleanExpression,
};

// A bounded, non-evaluating Boolean expression tree used only by the Alpha
// control-flow profile. The parser permits literal/identifier leaves, a single
// unary `not`, or one binary `and`/`or`; it deliberately has no precedence,
// parentheses, nested operators, value semantics, or execution behavior.
enum class ClassicalBooleanExpressionKind {
    BooleanLiteral,
    IdentifierReference,
    Not,
    And,
    Or,
};

struct ClassicalBooleanExpression {
    ClassicalBooleanExpressionKind kind = ClassicalBooleanExpressionKind::BooleanLiteral;
    bool boolean_value = false;
    std::string source_text;
    synq::compiler::SourceSpan span;
    std::vector<ClassicalBooleanExpression> operands;
};

// A bounded, non-evaluating integer arithmetic tree for one Alpha-gated
// declaration initializer operator. Parser-created trees have exactly two
// literal/identifier atoms and no precedence, nesting, or runtime semantics.
enum class ClassicalIntegerArithmeticExpressionKind {
    IntegerLiteral,
    IdentifierReference,
    Add,
    Subtract,
    Multiply,
};

struct ClassicalIntegerArithmeticExpression {
    ClassicalIntegerArithmeticExpressionKind kind = ClassicalIntegerArithmeticExpressionKind::IntegerLiteral;
    std::string source_text;
    synq::compiler::SourceSpan span;
    std::vector<ClassicalIntegerArithmeticExpression> operands;
};

struct ClassicalCondition {
    ClassicalConditionKind kind = ClassicalConditionKind::BooleanLiteral;
    bool boolean_value = false;
    std::string source_text;
    synq::compiler::SourceSpan span;
    ClassicalBooleanExpression expression;
};

class ClassicalControlNode : public ASTNode {
public:
    ClassicalControlKind kind;
    ClassicalCondition condition;
    ASTNode* body = nullptr;
    std::size_t line = 0;
    synq::compiler::SourceSpan span;

    ClassicalControlNode(ClassicalControlKind control_kind, ClassicalCondition typed_condition, ASTNode* owned_body,
                         std::size_t line_number, synq::compiler::SourceSpan source_span)
        : kind(control_kind),
          condition(std::move(typed_condition)),
          body(owned_body),
          line(line_number),
          span(source_span) {}

    ~ClassicalControlNode() override { delete body; }

    std::string toString() override {
        return std::string(kind == ClassicalControlKind::If ? "if " : "while ") +
               condition.source_text + " " + (body == nullptr ? "" : body->toString());
    }
};

// A non-evaluating hint for the bounded declaration right-hand side. SourceText
// means the parser preserved accepted source that a later expression/type layer
// must understand; it does not mean that the value was rejected.
enum class ClassicalLiteralKind {
    Integer,
    Decimal,
    Boolean,
    QuotedString,
    IntegerArithmeticExpression,
    SourceText,
};

// A small declaration node for the recovery parser's `let name = value` form.
// The value is retained as source text; expression evaluation is outside this
// recovery profile. Parsed nodes receive a literal or Alpha expression hint,
// while direct legacy construction defaults deliberately to SourceText.
class DeclarationNode : public ASTNode {
public:
    std::string name;
    std::string value;
    ClassicalLiteralKind literal_kind = ClassicalLiteralKind::SourceText;
    std::size_t line = 0;
    synq::compiler::SourceSpan span;

    DeclarationNode(std::string identifier, std::string source_value, std::size_t line_number,
                    ClassicalLiteralKind kind = ClassicalLiteralKind::SourceText)
        : DeclarationNode(std::move(identifier), std::move(source_value), line_number, kind, {line_number, 0, 0}) {}

    DeclarationNode(std::string identifier, std::string source_value, std::size_t line_number,
                    ClassicalLiteralKind kind, synq::compiler::SourceSpan source_span)
        : name(std::move(identifier)), value(std::move(source_value)), literal_kind(kind), line(line_number),
          span(source_span) {}

    std::string toString() override {
        return "let " + name + " = " + value;
    }
};

// (Additional AST node classes like FunctionNode and ClassNode remain future work.)

#endif
