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

// synq/tests/plugins/mutation/delete_pass_test.cpp
// Tests for the Deletion Pass (code/comment block eliminator)
// License: 

#include "../../../src/plugins/mutation/passes/delete_pass.h"
#include <iostream>
#include <cassert>
#include <string>

void test_basic_comment_removal() {
    std::string input = "int main() {\n  // This is a comment\n  return 0;\n}";
    std::string expected = "int main() {\n\n  return 0;\n}";
    std::string output = synq::mutation::apply_deletion_pass(input);
    assert(output == expected);
    std::cout << "[✓] test_basic_comment_removal passed.\n";
}

void test_multiline_block_removal() {
    std::string input = "int a = 5;\n/*\nThis should be removed\n*/\nint b = 10;";
    std::string expected = "int a = 5;\n\nint b = 10;";
    std::string output = synq::mutation::apply_deletion_pass(input);
    assert(output == expected);
    std::cout << "[✓] test_multiline_block_removal passed.\n";
}

void test_code_block_removal() {
    std::string input = "int unused() {\n  int a = 42;\n  return a;\n}\nint main() {\n  return 0;\n}";
    std::string expected = "int main() {\n  return 0;\n}";
    std::string output = synq::mutation::apply_deletion_pass(input);
    assert(output.find("unused") == std::string::npos);
    std::cout << "[✓] test_code_block_removal passed.\n";
}

int main() {
    std::cout << "[*] Running Deletion Pass Tests...\n";
    test_basic_comment_removal();
    test_multiline_block_removal();
    test_code_block_removal();
    std::cout << "[✓] All Deletion Pass tests passed.\n";
    return 0;
}