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
// File: tests/unit/test_runtime.cpp

#include <iostream>
#include "../../src/runtime/runtime.h"
#include "../../src/compiler/ast.h"

using namespace synq;

// Dummy AI bridge for test
class MockAI : public AIRuntimeBridge {
public:
    Value runModel(const std::string& modelName, const std::string& prompt) override {
        Value v;
        v.type = Value::STRING;
        v.str = "mocked-response";
        return v;
    }
};

int main() {
    std::cout << "[TEST] Runtime Classical Variable Test\n";
    Runtime rt(std::make_unique<MockAI>());

    // Simulate AST node to set and get a variable
    auto root = std::make_shared<ASTNode>();
    root->type = "SetVariable";
    root->token = "x";
    rt.execute(root); // (no real logic for set, placeholder test)

    std::cout << "[PASS] Executed dummy node\n";

    // AI call test
    auto aiNode = std::make_shared<ASTNode>();
    aiNode->type = "RunModel";
    aiNode->children.push_back(std::make_shared<ASTNode>());
    aiNode->children.push_back(std::make_shared<ASTNode>());
    aiNode->children[0]->token = "GPT4";
    aiNode->children[1]->token = "Hello";

    Value result = rt.evaluate(aiNode);
    std::cout << "[AI OUTPUT] " << result.str << "\n";

    if (result.str == "mocked-response") {
        std::cout << "[PASS] AI bridge returned expected result\n";
    } else {
        std::cerr << "[FAIL] AI bridge result mismatch\n";
        return 1;
    }

    return 0;
}