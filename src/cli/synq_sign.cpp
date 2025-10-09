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

// synq/src/cli/synq_sign.cpp // CLI Tool: synq sign <file> | synq verify <file> <signature> // License: 

#include <iostream> #include <filesystem> #include <string> #include <map> #include "../plugins/signing/sign_engine.h"

using namespace synq::sign;

void print_usage() { std::cout << "\nSynQ Secure Signing CLI\n"; std::cout << "Usage:\n"; std::cout << "  synq sign <file>        Sign a file\n"; std::cout << "  synq verify <file> <signature>  Verify a signed file\n\n"; }

int main(int argc, char* argv[]) { if (argc < 3) { print_usage(); return 1; }

std::string cmd = argv[1];
std::string filepath = argv[2];
SignEngine signer;

if (cmd == "sign") {
    SignatureResult result = signer.sign_file(filepath);
    if (!result.success) {
        std::cerr << "[ERROR] Signing failed. Check file path.\n";
        return 1;
    }
    std::cout << "\n[✓] File signed successfully:\n";
    std::cout << " Signature  : " << result.signature << "\n";
    std::cout << " Public Key : " << result.public_key << "\n";
    std::cout << " Hash       : " << result.signed_hash << "\n\n";
    return 0;
}

if (cmd == "verify" && argc == 4) {
    std::string sig = argv[3];
    VerificationResult verified = signer.verify_file(filepath, sig);
    if (verified.valid) {
        std::cout << "\n[✓] Valid Signature\n";
        std::cout << " Reason: " << verified.reason << "\n\n";
    } else {
        std::cerr << "\n[✗] Invalid Signature\n";
        std::cerr << " Reason: " << verified.reason << "\n\n";
        return 1;
    }
    return 0;
}

print_usage();
return 1;

}

