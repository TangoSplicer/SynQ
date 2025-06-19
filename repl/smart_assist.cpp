// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// smart_assist.cpp
#include "smart_assist.h"

namespace synq::repl {

  void SmartAssist::load_builtin_docs() {
    docs["qapply"] = "qapply(gate, qubits): Apply a quantum gate.";
    docs["measure"] = "measure(qubit): Collapse and return classical value.";
    docs["let"] = "let x = expr: Assign a value to a variable.";
    docs["macro"] = "macro name() { ... }: Define reusable logic.";
  }

  SmartAssist::SmartAssist() {
    load_builtin_docs();
  }

  std::string SmartAssist::get_doc_for(const std::string& token) {
    auto it = docs.find(token);
    return (it != docs.end()) ? it->second : "No documentation found.";
  }

  std::vector<std::string> SmartAssist::suggest_completions(const std::string& prefix) {
    std::vector<std::string> suggestions;
    for (const auto& [key, _] : docs) {
      if (key.find(prefix) == 0) {
        suggestions.push_back(key);
      }
    }
    return suggestions;
  }

}