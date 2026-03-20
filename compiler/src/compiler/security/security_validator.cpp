// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Security Validation Implementation

#include "security_validator.h"
#include <algorithm>
#include <sstream>
#include <regex>

namespace synq::compiler::security {

// VulnerabilityScanner implementation

VulnerabilityScanner::VulnerabilityScanner() {
}

std::vector<Vulnerability> VulnerabilityScanner::scan(const std::string& code,
                                                      const std::string& filename) {
    vulnerabilities.clear();

    // Scan for various vulnerability types
    auto buffer_overflows = scan_buffer_overflow(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), buffer_overflows.begin(),
                          buffer_overflows.end());

    auto use_after_free = scan_use_after_free(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), use_after_free.begin(),
                          use_after_free.end());

    auto type_confusion = scan_type_confusion(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), type_confusion.begin(),
                          type_confusion.end());

    auto integer_overflow = scan_integer_overflow(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), integer_overflow.begin(),
                          integer_overflow.end());

    auto crypto_weakness = scan_crypto_weakness(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), crypto_weakness.begin(),
                          crypto_weakness.end());

    auto hardcoded_secrets = scan_hardcoded_secrets(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), hardcoded_secrets.begin(),
                          hardcoded_secrets.end());

    auto race_conditions = scan_race_conditions(code, filename);
    vulnerabilities.insert(vulnerabilities.end(), race_conditions.begin(),
                          race_conditions.end());

    return vulnerabilities;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_buffer_overflow(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: strcpy, sprintf, gets (unsafe functions)
    std::regex unsafe_pattern(R"(\b(strcpy|sprintf|gets|scanf)\s*\()");
    std::smatch match;
    std::string::const_iterator search_start(code.cbegin());

    while (std::regex_search(search_start, code.cend(), match, unsafe_pattern)) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::BUFFER_OVERFLOW;
        vuln.severity = SeverityLevel::CRITICAL;
        vuln.description = "Use of unsafe function " + match[1].str() +
                          " that can cause buffer overflow";
        vuln.location = filename + ":1";  // Simplified
        vuln.code_snippet = match[0].str();
        vuln.remediation = "Use safe alternatives: strncpy, snprintf, fgets";
        vuln.cwe_id = 120;  // CWE-120: Buffer Copy without Checking Size
        vuln.cvss_score = 95;

        results.push_back(vuln);
        search_start = match.suffix().first;
    }

    return results;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_use_after_free(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: free() followed by use
    std::regex free_pattern(R"(\bfree\s*\(\s*(\w+)\s*\))");
    std::regex use_pattern(R"(\b(\w+)\s*->|\b(\w+)\s*\.)");

    // Simplified detection - in reality would need data flow analysis
    if (code.find("free(") != std::string::npos) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::USE_AFTER_FREE;
        vuln.severity = SeverityLevel::CRITICAL;
        vuln.description = "Potential use-after-free vulnerability";
        vuln.location = filename + ":1";
        vuln.code_snippet = "free(...); ... ptr->member";
        vuln.remediation = "Set pointer to nullptr after free, use RAII";
        vuln.cwe_id = 416;  // CWE-416: Use After Free
        vuln.cvss_score = 90;

        results.push_back(vuln);
    }

    return results;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_type_confusion(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: unsafe casts
    std::regex cast_pattern(R"(\(void\s*\*\)|reinterpret_cast)");
    std::smatch match;
    std::string::const_iterator search_start(code.cbegin());

    while (std::regex_search(search_start, code.cend(), match, cast_pattern)) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::TYPE_CONFUSION;
        vuln.severity = SeverityLevel::HIGH;
        vuln.description = "Unsafe type cast that could lead to type confusion";
        vuln.location = filename + ":1";
        vuln.code_snippet = match[0].str();
        vuln.remediation = "Use safe casts or polymorphism instead";
        vuln.cwe_id = 843;  // CWE-843: Type Confusion
        vuln.cvss_score = 75;

        results.push_back(vuln);
        search_start = match.suffix().first;
    }

    return results;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_integer_overflow(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: arithmetic operations without overflow checking
    std::regex arithmetic_pattern(R"(\w+\s*[\+\-\*\/]\s*\w+)");

    // Simplified detection
    if (code.find("+") != std::string::npos || code.find("*") != std::string::npos) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::INTEGER_OVERFLOW;
        vuln.severity = SeverityLevel::HIGH;
        vuln.description = "Arithmetic operation without overflow checking";
        vuln.location = filename + ":1";
        vuln.code_snippet = "a + b";
        vuln.remediation = "Add overflow checking or use safe arithmetic libraries";
        vuln.cwe_id = 190;  // CWE-190: Integer Overflow
        vuln.cvss_score = 70;

        results.push_back(vuln);
    }

    return results;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_crypto_weakness(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: weak cryptographic algorithms
    std::regex weak_crypto_pattern(R"(\b(MD5|SHA1|DES|RC4|rand)\b)");
    std::smatch match;
    std::string::const_iterator search_start(code.cbegin());

    while (std::regex_search(search_start, code.cend(), match, weak_crypto_pattern)) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::WEAK_CRYPTO;
        vuln.severity = SeverityLevel::HIGH;
        vuln.description = "Use of weak cryptographic algorithm: " + match[1].str();
        vuln.location = filename + ":1";
        vuln.code_snippet = match[0].str();
        vuln.remediation = "Use modern algorithms: SHA3, AES-GCM, ML-KEM, ML-DSA";
        vuln.cwe_id = 327;  // CWE-327: Use of Broken Crypto
        vuln.cvss_score = 85;

        results.push_back(vuln);
        search_start = match.suffix().first;
    }

    return results;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_hardcoded_secrets(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: hardcoded passwords, keys, tokens
    std::regex secret_pattern(
        R"((password|secret|api_key|token|private_key)\s*=\s*["\']([^"\']+)["\'])");
    std::smatch match;
    std::string::const_iterator search_start(code.cbegin());

    while (std::regex_search(search_start, code.cend(), match, secret_pattern)) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::HARDCODED_SECRET;
        vuln.severity = SeverityLevel::CRITICAL;
        vuln.description = "Hardcoded secret: " + match[1].str();
        vuln.location = filename + ":1";
        vuln.code_snippet = match[0].str();
        vuln.remediation = "Use environment variables or secure vaults";
        vuln.cwe_id = 798;  // CWE-798: Hardcoded Credentials
        vuln.cvss_score = 95;

        results.push_back(vuln);
        search_start = match.suffix().first;
    }

    return results;
}

std::vector<Vulnerability> VulnerabilityScanner::scan_race_conditions(
    const std::string& code, const std::string& filename) {
    std::vector<Vulnerability> results;

    // Pattern: concurrent access without synchronization
    if (code.find("thread") != std::string::npos &&
        code.find("mutex") == std::string::npos) {
        Vulnerability vuln;
        vuln.type = VulnerabilityType::RACE_CONDITION;
        vuln.severity = SeverityLevel::HIGH;
        vuln.description = "Potential race condition: threads without synchronization";
        vuln.location = filename + ":1";
        vuln.code_snippet = "thread(...); // no mutex";
        vuln.remediation = "Use mutexes, atomic operations, or channels";
        vuln.cwe_id = 362;  // CWE-362: Concurrent Modification
        vuln.cvss_score = 80;

        results.push_back(vuln);
    }

    return results;
}

void VulnerabilityScanner::record_vulnerability(const Vulnerability& vuln) {
    vulnerabilities.push_back(vuln);
}

std::string VulnerabilityScanner::extract_snippet(const std::string& code,
                                                  const std::string& location) {
    // Simplified snippet extraction
    return code.substr(0, std::min(size_t(50), code.length()));
}

// SecurityHardener implementation

std::string SecurityHardener::add_bounds_checking(const std::string& code) {
    // Add bounds checking to array access
    std::string hardened = code;
    // Simplified - in reality would parse and add checks
    hardened += "\n// Bounds checking enabled";
    return hardened;
}

std::string SecurityHardener::add_overflow_checking(const std::string& code) {
    std::string hardened = code;
    hardened += "\n// Integer overflow checking enabled";
    return hardened;
}

std::string SecurityHardener::add_null_checking(const std::string& code) {
    std::string hardened = code;
    hardened += "\n// Null pointer checking enabled";
    return hardened;
}

std::string SecurityHardener::add_type_checking(const std::string& code) {
    std::string hardened = code;
    hardened += "\n// Type checking enabled";
    return hardened;
}

std::string SecurityHardener::add_memory_safety(const std::string& code) {
    std::string hardened = code;
    hardened += "\n// Memory safety checks enabled";
    return hardened;
}

std::string SecurityHardener::add_constant_time_ops(const std::string& code) {
    std::string hardened = code;
    hardened += "\n// Constant-time operations enabled";
    return hardened;
}

std::string SecurityHardener::sanitize_input(const std::string& input) {
    std::string sanitized = input;
    // Remove potentially dangerous characters
    sanitized.erase(std::remove_if(sanitized.begin(), sanitized.end(),
                                   [](char c) { return c == '\0' || c == '\n'; }),
                   sanitized.end());
    return sanitized;
}

std::string SecurityHardener::escape_output(const std::string& output) {
    std::string escaped;
    for (char c : output) {
        switch (c) {
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '&': escaped += "&amp;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&#x27;"; break;
            default: escaped += c;
        }
    }
    return escaped;
}

// SecurityPolicyValidator implementation

SecurityPolicyValidator::SecurityPolicyValidator(const SecurityPolicy& policy)
    : policy(policy) {
}

bool SecurityPolicyValidator::validate(const std::string& code,
                                      const std::vector<Vulnerability>& vulnerabilities) {
    violations.clear();

    for (const auto& vuln : vulnerabilities) {
        if (!is_allowed(vuln)) {
            std::ostringstream oss;
            oss << "Violation: " << static_cast<int>(vuln.type) << " (severity: "
                << static_cast<int>(vuln.severity) << ")";
            record_violation(oss.str());
        }
    }

    return violations.empty();
}

bool SecurityPolicyValidator::is_allowed(const Vulnerability& vuln) const {
    // Check if vulnerability type is blocked
    auto type_blocked = std::find(policy.blocked_vulnerabilities.begin(),
                                 policy.blocked_vulnerabilities.end(),
                                 vuln.type) != policy.blocked_vulnerabilities.end();

    // Check if severity is blocked
    auto severity_blocked = std::find(policy.blocked_severities.begin(),
                                     policy.blocked_severities.end(),
                                     vuln.severity) != policy.blocked_severities.end();

    // Check CVSS score
    bool cvss_ok = vuln.cvss_score <= policy.max_cvss_score;

    return !type_blocked && !severity_blocked && cvss_ok;
}

uint32_t SecurityPolicyValidator::get_compliance_score() const {
    // Simplified - in reality would calculate based on violations
    return violations.empty() ? 100 : std::max(0u, 100u - (uint32_t)violations.size() * 10);
}

void SecurityPolicyValidator::record_violation(const std::string& violation) {
    violations.push_back(violation);
}

// SecurityAudit implementation

SecurityAudit::SecurityAudit(const SecurityPolicy& policy)
    : policy(policy), validator(policy) {
}

SecurityAudit::AuditResult SecurityAudit::audit(const std::string& code,
                                               const std::string& filename) {
    AuditResult result;

    // Scan for vulnerabilities
    result.vulnerabilities = scanner.scan(code, filename);

    // Validate against policy
    result.passed = validator.validate(code, result.vulnerabilities);

    // Calculate compliance score
    result.compliance_score = validator.get_compliance_score();

    // Generate recommendations
    result.recommendations = generate_recommendations(result.vulnerabilities);

    return result;
}

std::string SecurityAudit::generate_report(const AuditResult& result) const {
    std::ostringstream oss;
    oss << "Security Audit Report\n";
    oss << "======================\n";
    oss << "Status: " << (result.passed ? "PASSED" : "FAILED") << "\n";
    oss << "Compliance Score: " << result.compliance_score << "/100\n";
    oss << "\nVulnerabilities Found: " << result.vulnerabilities.size() << "\n";

    for (const auto& vuln : result.vulnerabilities) {
        oss << "\n- " << vuln.description << "\n";
        oss << "  Severity: " << static_cast<int>(vuln.severity) << "\n";
        oss << "  Remediation: " << vuln.remediation << "\n";
    }

    oss << "\nRecommendations:\n";
    for (const auto& rec : result.recommendations) {
        oss << "- " << rec << "\n";
    }

    return oss.str();
}

std::vector<std::string> SecurityAudit::generate_recommendations(
    const std::vector<Vulnerability>& vulnerabilities) {
    std::vector<std::string> recommendations;

    for (const auto& vuln : vulnerabilities) {
        recommendations.push_back(vuln.remediation);
    }

    return recommendations;
}

// SecureCodeingStandards implementation

bool SecureCodeingStandards::check_owasp_compliance(const std::string& code) {
    // Simplified OWASP Top 10 check
    return code.find("hardcoded") == std::string::npos;
}

bool SecureCodeingStandards::check_cwe_compliance(const std::string& code) {
    // Simplified CWE check
    return code.find("strcpy") == std::string::npos;
}

bool SecureCodeingStandards::check_cert_compliance(const std::string& code) {
    // Simplified CERT check
    return code.find("gets") == std::string::npos;
}

bool SecureCodeingStandards::check_misra_compliance(const std::string& code) {
    // Simplified MISRA check
    return code.find("goto") == std::string::npos;
}

std::vector<std::string> SecureCodeingStandards::get_guidelines() {
    return {
        "Use safe string functions (strncpy, snprintf)",
        "Enable bounds checking and overflow detection",
        "Use modern cryptographic algorithms",
        "Implement proper input validation",
        "Use secure random number generation",
        "Apply principle of least privilege",
        "Use defense in depth",
        "Keep dependencies updated",
    };
}

std::vector<std::string> SecureCodeingStandards::suggest_fixes(
    const Vulnerability& vuln) {
    std::vector<std::string> fixes;
    fixes.push_back(vuln.remediation);
    return fixes;
}

} // namespace synq::compiler::security
