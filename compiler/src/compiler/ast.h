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

// A small declaration node for the recovery parser's `let name = value` form.
// The value is retained as source text; expression evaluation is outside this
// recovery profile.
class DeclarationNode : public ASTNode {
public:
    std::string name;
    std::string value;
    std::size_t line = 0;

    DeclarationNode(std::string identifier, std::string source_value, std::size_t line_number)
        : name(std::move(identifier)), value(std::move(source_value)), line(line_number) {}

    std::string toString() override {
        return "let " + name + " = " + value;
    }
};

// (Additional AST node classes like FunctionNode and ClassNode remain future work.)

#endif
