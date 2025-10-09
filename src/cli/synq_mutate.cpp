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
// synq/src/cli/synq_mutate.cpp // CLI Entry Point for Plugin Mutation // (c) 2025 SynQ Project. License: Paid Commercial Use Only

#include <iostream>
#include <filesystem>
#include "../plugins/mutation/mutate_engine.h"

using namespace synq::mutation;

int main(int argc, char* argv[]) { if (argc < 2) { std::cerr << "Usage: synq mutate <plugin_file.synq>\n"; return 1; }

std::string path = argv[1];
if (!std::filesystem::exists(path)) {
    std::cerr << "Error: File not found: " << path << "\n";
    return 1;
}

MutateEngine engine;
MutationResult result = engine.run(path);

std::cout << "===== Mutation Logs =====\n";
for (const auto& log : result.logs) {
    std::cout << log << "\n";
}

std::cout << "\n===== Mutated Plugin Output =====\n";
std::cout << result.mutated_code << "\n";

return 0;

}

