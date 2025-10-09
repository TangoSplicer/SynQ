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

// synq/src/plugins/signing/sign_test.cpp // Signature Engine Test Harness // License: 

#include "sign_engine.h" #include <iostream> #include <fstream> #include <chrono> #include <ctime>

using namespace synq::sign;

int main(int argc, char* argv[]) { if (argc < 2) { std::cerr << "Usage: sign_test <file>\n"; return 1; }

std::string target_file = argv[1];
SignEngine engine;

std::cout << "[+] Hashing and signing: " << target_file << "\n";
auto result = engine.sign_file(target_file);
if (!result.success) {
    std::cerr << "[!] ERROR: Signing failed.\n";
    return 1;
}

std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
std::ofstream log("signature_output.log");
log << "{\n";
log << "  \"filename\": \"" << target_file << "\",\n";
log << "  \"signature\": \"" << result.signature << "\",\n";
log << "  \"public_key\": \"" << result.public_key << "\",\n";
log << "  \"signed_hash\": \"" << result.signed_hash << "\",\n";
log << "  \"timestamp\": \"" << std::ctime(&now) << "\"\n";
log << "}\n";
log.close();

std::cout << "[✓] Signature written to log.\n";

std::cout << "[+] Verifying...\n";
auto verify = engine.verify_file(target_file, result.signature);
std::cout << (verify.valid ? "[✓] Signature Verified" : "[✗] Signature Rejected") << ": " << verify.reason << "\n";

return verify.valid ? 0 : 1;

}

