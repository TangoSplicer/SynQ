// synq/src/plugins/signing/sign_test.cpp // Signature Engine Test Harness // License: SynQ Commercial Attribution License v1.0

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

