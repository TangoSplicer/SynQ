// synq/tests/plugins/signing/synq_sign_test.cpp
// Unit Test: CLI Interface for synq sign/verify
// License: SynQ Commercial Attribution License v1.0

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

std::string test_file = "testfile.tmp";

void create_test_file() {
    std::ofstream out(test_file);
    out << "This is a file to be signed by SynQ." << std::endl;
    out.close();
    assert(std::filesystem::exists(test_file));
    std::cout << "[*] Created temporary file for signing: " << test_file << "\n";
}

void test_sign_command() {
    std::string cmd = "./synq sign " + test_file + " > sign_output.txt";
    int result = std::system(cmd.c_str());
    assert(result == 0);
    std::ifstream in("sign_output.txt");
    std::string line;
    bool found_sig = false;
    while (std::getline(in, line)) {
        if (line.find("Signature") != std::string::npos) {
            found_sig = true;
            break;
        }
    }
    in.close();
    assert(found_sig);
    std::cout << "[✓] test_sign_command passed.\n";
}

void test_verify_command() {
    std::ifstream in("sign_output.txt");
    std::string line, signature;
    while (std::getline(in, line)) {
        if (line.find("Signature") != std::string::npos) {
            signature = line.substr(line.find(":") + 1);
            break;
        }
    }
    in.close();

    assert(!signature.empty());
    std::string cmd = "./synq verify " + test_file + " " + signature + " > verify_output.txt";
    int result = std::system(cmd.c_str());
    assert(result == 0);

    std::ifstream vout("verify_output.txt");
    std::string content((std::istreambuf_iterator<char>(vout)), std::istreambuf_iterator<char>());
    assert(content.find("Valid Signature") != std::string::npos);
    std::cout << "[✓] test_verify_command passed.\n";
}

void cleanup() {
    std::remove(test_file.c_str());
    std::remove("sign_output.txt");
    std::remove("verify_output.txt");
    std::cout << "[*] Cleanup completed.\n";
}

int main() {
    std::cout << "[*] Running synq_sign CLI tests...\n";
    create_test_file();
    test_sign_command();
    test_verify_command();
    cleanup();
    std::cout << "[✓] All CLI tests passed.\n";
    return 0;
}