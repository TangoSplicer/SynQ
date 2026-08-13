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

    InstructionNode(std::string operation, std::vector<std::string> arguments, std::size_t line_number)
        : op(std::move(operation)), args(std::move(arguments)), line(line_number) {}

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

    QuantumGateNode(QuantumGateKind gate_kind,
                    std::string original_name,
                    std::optional<std::string> angle,
                    std::vector<std::size_t> operands,
                    std::size_t line_number)
        : kind(gate_kind),
          source_name(std::move(original_name)),
          literal_angle(std::move(angle)),
          qubit_indices(std::move(operands)),
          line(line_number) {}

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

// A non-evaluating hint for the bounded declaration right-hand side. SourceText
// means the parser preserved accepted source that a later expression/type layer
// must understand; it does not mean that the value was rejected.
enum class ClassicalLiteralKind {
    Integer,
    Decimal,
    Boolean,
    QuotedString,
    SourceText,
};

// A small declaration node for the recovery parser's `let name = value` form.
// The value is retained as source text; expression evaluation is outside this
// recovery profile. Parsed nodes receive a literal hint, while direct legacy
// construction defaults deliberately to SourceText.
class DeclarationNode : public ASTNode {
public:
    std::string name;
    std::string value;
    ClassicalLiteralKind literal_kind = ClassicalLiteralKind::SourceText;
    std::size_t line = 0;

    DeclarationNode(std::string identifier, std::string source_value, std::size_t line_number,
                    ClassicalLiteralKind kind = ClassicalLiteralKind::SourceText)
        : name(std::move(identifier)), value(std::move(source_value)), literal_kind(kind), line(line_number) {}

    std::string toString() override {
        return "let " + name + " = " + value;
    }
};

// (Additional AST node classes like FunctionNode and ClassNode remain future work.)

#endif
