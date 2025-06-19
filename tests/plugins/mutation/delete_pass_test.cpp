// synq/tests/plugins/mutation/delete_pass_test.cpp
// Tests for the Deletion Pass (code/comment block eliminator)
// License: SynQ Commercial Attribution License v1.0

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