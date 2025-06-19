// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: src/runtime/runtime.h
#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "../compiler/ast.h"
#include "../../backend/simulator.h"
#include "../../backend/ai_provider.h"

namespace synq {

struct Value {
    enum Type { INT, FLOAT, STRING, QUBIT, CIRCUIT, MODEL, NONE };
    Type type;
    union { int i; double f; } num;
    std::string str;
    void* ptr;
    Value() : type(NONE), ptr(nullptr) {}
    static Value fromInt(int i) { Value v; v.type = INT; v.num.i = i; return v; }
    static Value fromFloat(double f) { Value v; v.type = FLOAT; v.num.f = f; return v; }
    static Value fromString(const std::string& s) { Value v; v.type = STRING; v.str = s; return v; }
};

class ClassicalContext {
public:
    std::map<std::string, Value> variables;
    void set(const std::string& name, const Value& v);
    Value get(const std::string& name) const;
};

class QuantumContext {
private:
    std::shared_ptr<QuantumSimulator> simulator;
public:
    QuantumContext(std::shared_ptr<QuantumSimulator> sim);
    void allocateQubit(const std::string& name);
    void applyGate(const std::string& gate, const std::vector<std::string>& targets);
    std::vector<int> measureAll();
    std::string serializeCircuit() const;
    std::vector<double> statevector() const;
};

class AIRuntimeBridge {
private:
    std::shared_ptr<AIProvider> provider;
public:
    AIRuntimeBridge(std::shared_ptr<AIProvider> prov);
    Value runModel(const std::string& modelName, const std::string& prompt);
};

class Runtime {
private:
    ClassicalContext cctx;
    QuantumContext qctx;
    AIRuntimeBridge ai;

public:
    Runtime(std::shared_ptr<QuantumSimulator> sim, std::shared_ptr<AIProvider> aiProv);
    void execute(const std::shared_ptr<ASTNode>& node);
    Value evaluate(const std::shared_ptr<ASTNode>& node);
};

} // namespace synq