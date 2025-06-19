// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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