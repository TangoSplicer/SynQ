// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Security Validation Framework

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace synq::compiler::security {

/**
 * @enum VulnerabilityType
 * @brief Types of security vulnerabilities
 */
enum class VulnerabilityType {
    // Memory vulnerabilities
    BUFFER_OVERFLOW,
    USE_AFTER_FREE,
    DOUBLE_FREE,
    MEMORY_LEAK,
    STACK_OVERFLOW,

    // Type vulnerabilities
    TYPE_CONFUSION,
    INTEGER_OVERFLOW,
    INTEGER_UNDERFLOW,
    NULL_POINTER_DEREFERENCE,

    // Logic vulnerabilities
    RACE_CONDITION,
    DEADLOCK,
    LOGIC_ERROR,
    UNINITIALIZED_VARIABLE,

    // Cryptographic vulnerabilities
    WEAK_CRYPTO,
    INSUFFICIENT_RANDOMNESS,
    INSECURE_RANDOM,
    TIMING_LEAK,

    // Input validation vulnerabilities
    SQL_INJECTION,
    COMMAND_INJECTION,
    PATH_TRAVERSAL,
    INVALID_INPUT,

    // Other vulnerabilities
    HARDCODED_SECRET,
    INSECURE_DESERIALIZATION,
    MISSING_BOUNDS_CHECK,
    UNSAFE_CAST,
};

/**
 * @enum SeverityLevel
 * @brief Severity level of vulnerability
 */
enum class SeverityLevel {
    CRITICAL,   // Immediate exploitation risk
    HIGH,       // Significant risk
    MEDIUM,     // Moderate risk
    LOW,        // Minor risk
    INFO,       // Informational
};

/**
 * @struct Vulnerability
 * @brief Represents a detected vulnerability
 */
struct Vulnerability {
    VulnerabilityType type;
    SeverityLevel severity;
    std::string description;
    std::string location;  // File:line
    std::string code_snippet;
    std::string remediation;
    uint64_t cwe_id;  // Common Weakness Enumeration ID
    uint64_t cvss_score;  // CVSS v3.1 score (0-100)
};

/**
 * @struct SecurityPolicy
 * @brief Security policy for validation
 */
struct SecurityPolicy {
    std::string name;
    std::vector<VulnerabilityType> blocked_vulnerabilities;
    std::vector<SeverityLevel> blocked_severities;
    uint64_t max_cvss_score;
    bool require_code_review;
    bool require_security_audit;
};

/**
 * @class VulnerabilityScanner
 * @brief Scans code for security vulnerabilities
 */
class VulnerabilityScanner {
public:
    VulnerabilityScanner();

    /**
     * @brief Scan code for vulnerabilities
     * @param code Code to scan
     * @param filename Source file name
     * @return Vector of detected vulnerabilities
     */
    std::vector<Vulnerability> scan(const std::string& code, const std::string& filename);

    /**
     * @brief Scan for buffer overflow vulnerabilities
     */
    std::vector<Vulnerability> scan_buffer_overflow(const std::string& code,
                                                    const std::string& filename);

    /**
     * @brief Scan for use-after-free vulnerabilities
     */
    std::vector<Vulnerability> scan_use_after_free(const std::string& code,
                                                   const std::string& filename);

    /**
     * @brief Scan for type confusion vulnerabilities
     */
    std::vector<Vulnerability> scan_type_confusion(const std::string& code,
                                                   const std::string& filename);

    /**
     * @brief Scan for integer overflow vulnerabilities
     */
    std::vector<Vulnerability> scan_integer_overflow(const std::string& code,
                                                     const std::string& filename);

    /**
     * @brief Scan for cryptographic weaknesses
     */
    std::vector<Vulnerability> scan_crypto_weakness(const std::string& code,
                                                    const std::string& filename);

    /**
     * @brief Scan for hardcoded secrets
     */
    std::vector<Vulnerability> scan_hardcoded_secrets(const std::string& code,
                                                      const std::string& filename);

    /**
     * @brief Scan for race conditions
     */
    std::vector<Vulnerability> scan_race_conditions(const std::string& code,
                                                    const std::string& filename);

    /**
     * @brief Get all detected vulnerabilities
     */
    const std::vector<Vulnerability>& get_vulnerabilities() const {
        return vulnerabilities;
    }

    /**
     * @brief Clear all detected vulnerabilities
     */
    void clear() { vulnerabilities.clear(); }

private:
    std::vector<Vulnerability> vulnerabilities;

    /**
     * @brief Record a vulnerability
     */
    void record_vulnerability(const Vulnerability& vuln);

    /**
     * @brief Extract code snippet
     */
    std::string extract_snippet(const std::string& code, const std::string& location);
};

/**
 * @class SecurityHardener
 * @brief Applies security hardening to code
 */
class SecurityHardener {
public:
    /**
     * @brief Add bounds checking to array access
     * @param code Code to harden
     * @return Hardened code
     */
    static std::string add_bounds_checking(const std::string& code);

    /**
     * @brief Add integer overflow checking
     * @param code Code to harden
     * @return Hardened code
     */
    static std::string add_overflow_checking(const std::string& code);

    /**
     * @brief Add null pointer checking
     * @param code Code to harden
     * @return Hardened code
     */
    static std::string add_null_checking(const std::string& code);

    /**
     * @brief Add type checking
     * @param code Code to harden
     * @return Hardened code
     */
    static std::string add_type_checking(const std::string& code);

    /**
     * @brief Add memory safety checks
     * @param code Code to harden
     * @return Hardened code
     */
    static std::string add_memory_safety(const std::string& code);

    /**
     * @brief Add constant-time operations
     * @param code Code to harden
     * @return Hardened code
     */
    static std::string add_constant_time_ops(const std::string& code);

    /**
     * @brief Sanitize input
     * @param input Input to sanitize
     * @return Sanitized input
     */
    static std::string sanitize_input(const std::string& input);

    /**
     * @brief Escape output
     * @param output Output to escape
     * @return Escaped output
     */
    static std::string escape_output(const std::string& output);
};

/**
 * @class SecurityPolicyValidator
 * @brief Validates code against security policies
 */
class SecurityPolicyValidator {
public:
    explicit SecurityPolicyValidator(const SecurityPolicy& policy);

    /**
     * @brief Validate code against policy
     * @param code Code to validate
     * @param vulnerabilities Detected vulnerabilities
     * @return true if code complies with policy
     */
    bool validate(const std::string& code,
                 const std::vector<Vulnerability>& vulnerabilities);

    /**
     * @brief Check if vulnerability is allowed
     */
    bool is_allowed(const Vulnerability& vuln) const;

    /**
     * @brief Get policy violations
     */
    const std::vector<std::string>& get_violations() const { return violations; }

    /**
     * @brief Get compliance score (0-100)
     */
    uint32_t get_compliance_score() const;

private:
    SecurityPolicy policy;
    std::vector<std::string> violations;

    void record_violation(const std::string& violation);
};

/**
 * @class SecurityAudit
 * @brief Comprehensive security audit
 */
class SecurityAudit {
public:
    struct AuditResult {
        bool passed;
        uint32_t compliance_score;
        std::vector<Vulnerability> vulnerabilities;
        std::vector<std::string> recommendations;
    };

    explicit SecurityAudit(const SecurityPolicy& policy);

    /**
     * @brief Perform security audit
     * @param code Code to audit
     * @param filename Source file name
     * @return Audit result
     */
    AuditResult audit(const std::string& code, const std::string& filename);

    /**
     * @brief Generate audit report
     */
    std::string generate_report(const AuditResult& result) const;

private:
    SecurityPolicy policy;
    VulnerabilityScanner scanner;
    SecurityPolicyValidator validator;

    /**
     * @brief Generate recommendations
     */
    std::vector<std::string> generate_recommendations(
        const std::vector<Vulnerability>& vulnerabilities);
};

/**
 * @class SecureCodeingStandards
 * @brief Enforces secure coding standards
 */
class SecureCodeingStandards {
public:
    /**
     * @brief Check for OWASP Top 10 compliance
     */
    static bool check_owasp_compliance(const std::string& code);

    /**
     * @brief Check for CWE compliance
     */
    static bool check_cwe_compliance(const std::string& code);

    /**
     * @brief Check for CERT compliance
     */
    static bool check_cert_compliance(const std::string& code);

    /**
     * @brief Check for MISRA compliance (C/C++)
     */
    static bool check_misra_compliance(const std::string& code);

    /**
     * @brief Get secure coding guidelines
     */
    static std::vector<std::string> get_guidelines();

    /**
     * @brief Suggest fixes for violations
     */
    static std::vector<std::string> suggest_fixes(const Vulnerability& vuln);
};

} // namespace synq::compiler::security
