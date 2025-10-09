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
// synq/src/plugins/mutation/mutation_passes/insert_pass.cpp
// InsertPass - Adds mutation hooks or logs
// (c) 2025 SynQ Project. License: Paid Commercial Use Only

#include "insert_pass.h"
#include <regex>

namespace synq {
namespace mutation {

std::string InsertPass::apply(const std::string& source) {
    std::string result = source;
    std::regex pattern("int main\(.*\) *\{");
    result = std::regex_replace(result, pattern,
        "int main() {\n    // [Mutation] Inserted log\n    std::cout << \"[Mutation] Starting main\\n\";\n");
    return result;
}

} // namespace mutation
} // namespace synq
