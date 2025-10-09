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
// backend_wasm.cpp
#include "codegen_backend.h"
#include <sstream>
#include <memory>

namespace synq::compiler {

  class BackendWASM : public CodegenBackend {
  public:
    std::string generate(const ASTNode& root, const TargetProfile& profile) override {
      std::ostringstream out;
      out << "(module\n";
      out << "  ;; SynQ → WebAssembly Output\n";
      out << "  (func (export \"main\")\n";
      out << "    i32.const 42 ;; simulate compiled AST\n";
      out << "    drop\n";
      out << "  )\n";
      out << ")";
      return out.str();
    }
  };

  std::shared_ptr<CodegenBackend> make_backend_wasm() {
    return std::make_shared<BackendWASM>();
  }

}