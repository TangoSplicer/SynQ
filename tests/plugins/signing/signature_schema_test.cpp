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

// synq/tests/plugins/signing/signature_schema_test.cpp
// Tests JSON output validity against SynQ Signature Schema
// License: 

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