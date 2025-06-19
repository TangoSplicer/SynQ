// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/runtime/runtime.cpp

#include "runtime.h"
#include <iostream>
#include "../../plugins/registry.h"
#include "../../plugins/ai_plugin.h"

namespace synq {

// --- ClassicalContext ---
void ClassicalContext::set(const std::string& name, const Value& v) {
    variables[name] = v;
}

Value ClassicalContext::get(const std::string& name) const {
    auto it = variables.find(name);
    return (it != variables.end()) ? it->second : Value();
}

// --- QuantumContext ---
QuantumContext::QuantumContext(std::shared_ptr<QuantumSimulator> sim)
    : simulator(sim) {}

void QuantumContext::allocateQubit(const std::string& name) {
    simulator->allocateQubit(name);
}

void QuantumContext::applyGate(const std::string& gate, const std::vector<std::string>& targets) {
    simulator->applyGate(gate, targets);
}

std::vector<int> QuantumContext::measureAll() {
    return simulator->measureAll();
}

std::string QuantumContext::serializeCircuit() const {
    return simulator->serialize();
}

std::vector<double> QuantumContext::statevector() const {
    return simulator->getStatevector();
}

// --- AIRuntimeBridge ---
AIRuntimeBridge::AIRuntimeBridge(std::shared_ptr<AIProvider> prov)
    : provider(prov) {}

Value AIRuntimeBridge::runModel(const std::string& modelName, const std::string& prompt) {
    std::string output = provider->run(modelName, prompt);
    return Value::fromString(output);
}

// --- Runtime ---
Runtime::Runtime(std::shared_ptr<QuantumSimulator> sim, std::shared_ptr<AIProvider> aiProv)
    : qctx(sim), ai(aiProv) {}

void Runtime::execute(const std::shared_ptr<ASTNode>& node) {
    evaluate(node); // Top-level
}

Value Runtime::evaluate(const std::shared_ptr<ASTNode>& node) {
    if (node->type == "RunModel") {
        std::string model = node->children[0]->token;
        std::string prompt = node->children[1]->token;
        return ai.runModel(model, prompt);
    }

    if (node->type == "UsePlugin") {
        std::string plugin_name = node->token;
        PluginRegistry::instance().runPlugin(plugin_name);
        return Value::fromString("Plugin loaded");
    }

    if (node->type == "RunPluginModel") {
        std::string plugin_name = node->children[0]->token;
        std::string prompt = node->children[1]->token;

        auto& registry = PluginRegistry::instance();
        if (!registry.isLoaded(plugin_name)) {
            std::cerr << "[PLUGIN ERROR] Not loaded: " << plugin_name << "\n";
            return Value::fromString("[Plugin Error]");
        }

        auto plugin = registry.get<AIProviderPlugin>(plugin_name);
        if (!plugin) {
            std::cerr << "[PLUGIN ERROR] Type mismatch: not AIProvider\n";
            return Value::fromString("[Type Error]");
        }

        return Value::fromString(plugin->runModel(plugin_name, prompt));
    }

    return Value::fromInt(42);
}

} // namespace synq