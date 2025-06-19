// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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