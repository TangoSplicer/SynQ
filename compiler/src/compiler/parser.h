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
#ifndef SYNQ_COMPILER_PARSER_H
#define SYNQ_COMPILER_PARSER_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ast.h"
#include "diagnostic.h"
#include "feature_gate.h"

namespace synq::compiler {

struct ParseResult {
    std::unique_ptr<ProgramNode> program;
    std::vector<Diagnostic> diagnostics;

    bool ok() const {
        if (program == nullptr) return false;
        for (const Diagnostic& diagnostic : diagnostics) {
            if (diagnostic.severity == DiagnosticSeverity::Error) return false;
        }
        return true;
    }

    std::unique_ptr<ProgramNode> take_program() {
        return std::move(program);
    }
};

}  // namespace synq::compiler

class Parser {
public:
    Parser();

    // Enables a compiler-registered experimental feature for this parser
    // instance. Source annotations can opt in for a single parsed file too.
    bool enableExperimentalFeature(const std::string& feature_name);

    // Parses a source file and retains structured errors for C ABI and future
    // editor/binding callers. The program is present only when `ok()` is true.
    synq::compiler::ParseResult parseFileWithDiagnostics(const std::string& filename);

    // Parse a source file and return the AST root node (or nullptr on error).
    // This compatibility wrapper renders structured diagnostics to stderr.
    ASTNode* parseFile(const std::string& filename);

private:
    synq::compiler::FeatureRegistry configured_features_;
};

#endif
