// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// trust_manager.h
#pragma once
#include <string>
#include <vector>
#include <map>

namespace synq::compiler {

  struct TrustCert {
    std::string module_name;
    std::string issuer;
    std::string issued_at;
    std::string fingerprint;
    std::string signature;
  };

  class TrustManager {
  public:
    TrustManager();

    bool is_trusted(const std::string& module_name) const;
    bool verify_fingerprint(const std::string& code, const std::string& expected_fp) const;
    bool validate_signature(const TrustCert& cert) const;

    void load_cert(const std::string& cert_json);
    void log_event(const std::string& event, const std::string& module);

    std::map<std::string, TrustCert> certs;
    std::vector<std::string> trust_log;
  };

}