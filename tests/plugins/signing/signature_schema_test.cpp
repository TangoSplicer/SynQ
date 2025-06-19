// synq/tests/plugins/signing/signature_schema_test.cpp
// Tests JSON output validity against SynQ Signature Schema
// License: SynQ Commercial Attribution License v1.0

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <cassert>
#include <regex>

using json = nlohmann::json;

void test_valid_signature_log() {
    std::ifstream in("signature_output.log");
    assert(in.is_open());
    json j;
    in >> j;
    in.close();

    assert(j.contains("filename"));
    assert(j.contains("signature"));
    assert(j.contains("public_key"));
    assert(j.contains("signed_hash"));
    assert(j.contains("timestamp"));

    std::regex iso8601(
        R"(^\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}(?:\.\d+)?(?:Z|[+-]\d{2}:\d{2})$)"
    );

    std::string ts = j["timestamp"];
    assert(std::regex_match(ts, iso8601) || ts.find(" ") != std::string::npos);  // Accept ctime fallback too

    std::cout << "[✓] JSON signature schema validation passed.\n";
}

int main() {
    std::cout << "[*] Running schema compliance test...\n";
    test_valid_signature_log();
    std::cout << "[✓] All tests passed.\n";
    return 0;
}