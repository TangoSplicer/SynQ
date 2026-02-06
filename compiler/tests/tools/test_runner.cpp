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

// synq/tests/tools/test_runner.cpp
// Centralized SynQ Test Runner (Classical, Quantum, AI, Plugins)
// License: 

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

void run_test(const std::string& path) {
    std::cout << "\n[ RUN  ] " << path << std::endl;
    int code = std::system(path.c_str());
    if (code == 0)
        std::cout << "[ PASS ] " << path << "\n";
    else
        std::cerr << "[ FAIL ] " << path << " (exit code " << code << ")\n";
}

int main() {
    std::vector<std::string> tests = {
        "./tests/lang/lexer_test",
        "./tests/lang/parser_test",
        "./tests/lang/ast_test",
        "./tests/lang/type_test",
        "./tests/lang/transpile_test",
        "./tests/runtime/vm_test",
        "./tests/runtime/memory_test",
        "./tests/runtime/stack_test",
        "./tests/quantum/grover_test",
        "./tests/quantum/qiso_test",
        "./tests/quantum/qpela_test",
        "./tests/quantum/qebet_test",
        "./tests/quantum/qsim_test",
        "./tests/ai/ai_test",
        "./tests/plugins/fuzz/fuzz_test",
        "./tests/plugins/mutation/mutate_test",
        "./tests/plugins/mutation/delete_pass_test",
        "./tests/plugins/signing/synq_sign_test",
        "./tests/plugins/signing/signature_schema_test",
        "./tests/plugins/chain/chain_test",
        "./tests/plugins/inspect/inspect_test",
        "./tests/plugins/debug/debug_test",
        "./tests/plugins/skillmap/skillmap_test",
        "./tests/repl/inspect_plugin_test",
        "./tests/repl/inspect_memory_test"
        "./tests/simulation/test_simulation_results.synq"
    };

    std::cout << "\n=== SynQ Universal Test Runner ===\n";
    for (const auto& bin : tests) {
        if (fs::exists(bin)) {
            run_test(bin);
        } else {
            std::cerr << "[ SKIP ] " << bin << " not found\n";
        }
    }

    std::cout << "\n[✓] All available tests executed.\n";
    return 0;
}