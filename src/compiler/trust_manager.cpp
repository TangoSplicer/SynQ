// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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