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
// target_registry.cpp
#include "target_registry.h"

namespace synq::compiler {

  std::vector<TargetProfile> TargetRegistry::list_targets() {
    return {
      {"x86_64", "Native Binary", ".exe", TargetType::CLASSICAL},
      {"wasm", "WebAssembly", ".wasm", TargetType::CLASSICAL},
      {"qiskit", "Qiskit QASM", ".qasm", TargetType::QUANTUM},
      {"openqasm3", "OpenQASM 3", ".qasm3", TargetType::QUANTUM},
      {"synqvm", "SynQ Bytecode", ".synqbin", TargetType::HYBRID}
    };
  }

}