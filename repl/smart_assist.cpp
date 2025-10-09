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