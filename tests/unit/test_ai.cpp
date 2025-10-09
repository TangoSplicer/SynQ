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
// File: tests/unit/test_ai.cpp

#include <iostream>
#include "../../src/runtime/runtime.h"

using namespace synq;

// Mock implementation
class StaticMockAI : public AIRuntimeBridge {
public:
    Value runModel(const std::string& modelName, const std::string& prompt) override {
        if (prompt.find("quantum") != std::string::npos) {
            return Value::fromString("Quantum mechanics is the study of...");
        }
        return Value::fromString("Default mock output.");
    }
};

int main() {
    std::cout << "[TEST] AI Runtime Invocation Test\n";

    Runtime rt(std::make_unique<StaticMockAI>());

    auto aiCall = std::make_shared<ASTNode>();
    aiCall->type = "RunModel";

    auto model = std::make_shared<ASTNode>();
    auto prompt = std::make_shared<ASTNode>();
    model->token = "GPT";
    prompt->token = "Tell me about quantum entanglement.";

    aiCall->children.push_back(model);
    aiCall->children.push_back(prompt);

    Value v = rt.evaluate(aiCall);

    if (v.str.find("quantum") != std::string::npos) {
        std::cout << "[PASS] AI returned quantum-related explanation\n";
    } else {
        std::cerr << "[FAIL] Unexpected AI response: " << v.str << "\n";
        return 1;
    }

    return 0;
}