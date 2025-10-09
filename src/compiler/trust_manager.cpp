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
// trust_manager.cpp
#include "trust_manager.h"
#include <nlohmann/json.hpp>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

namespace synq::compiler {

  TrustManager::TrustManager() {}

  bool TrustManager::is_trusted(const std::string& module_name) const {
    return certs.count(module_name) > 0;
  }

  bool TrustManager::verify_fingerprint(const std::string& code, const std::string& expected_fp) const {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)code.c_str(), code.length(), hash);
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
      oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return expected_fp == oss.str();
  }

  bool TrustManager::validate_signature(const TrustCert& cert) const {
    // Placeholder: Use RSA/Ed25519 signature verification later
    return !cert.signature.empty();  // Simulate success
  }

  void TrustManager::load_cert(const std::string& cert_json) {
    auto j = json::parse(cert_json);
    TrustCert cert;
    cert.module_name = j["module_name"];
    cert.issuer = j["issuer"];
    cert.issued_at = j["issued_at"];
    cert.fingerprint = j["fingerprint"];
    cert.signature = j["signature"];
    certs[cert.module_name] = cert;
    log_event("CERT_LOADED", cert.module_name);
  }

  void TrustManager::log_event(const std::string& event, const std::string& module) {
    trust_log.push_back(event + ":" + module);
  }

}