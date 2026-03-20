// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 12: Security Hardening - Cryptographic Primitives

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <memory>

namespace synq::compiler::security {

/**
 * @enum CryptoAlgorithm
 * @brief Cryptographic algorithms
 */
enum class CryptoAlgorithm {
    // Quantum-safe key encapsulation
    ML_KEM_512,      // NIST FIPS 203 - 512-bit security
    ML_KEM_768,      // NIST FIPS 203 - 768-bit security
    ML_KEM_1024,     // NIST FIPS 203 - 1024-bit security

    // Quantum-safe digital signatures
    ML_DSA_44,       // NIST FIPS 204 - 44-byte signature
    ML_DSA_65,       // NIST FIPS 204 - 65-byte signature
    ML_DSA_87,       // NIST FIPS 204 - 87-byte signature

    // Symmetric encryption
    AES_128_GCM,     // AES-128 with GCM mode
    AES_256_GCM,     // AES-256 with GCM mode
    CHACHA20_POLY1305, // ChaCha20-Poly1305

    // Hash functions
    SHA3_256,        // SHA3-256
    SHA3_512,        // SHA3-512
    BLAKE3,          // BLAKE3
};

/**
 * @struct CryptoKey
 * @brief Cryptographic key
 */
struct CryptoKey {
    CryptoAlgorithm algorithm;
    std::vector<uint8_t> key_material;
    uint64_t key_id;
    uint64_t created_at;
    uint64_t expires_at;
    bool is_secret;
};

/**
 * @struct CryptoSignature
 * @brief Digital signature
 */
struct CryptoSignature {
    std::vector<uint8_t> signature;
    CryptoAlgorithm algorithm;
    uint64_t timestamp;
    std::string signer_id;
};

/**
 * @class QuantumSafeKeyEncapsulation
 * @brief Quantum-safe key encapsulation mechanism (KEM)
 * 
 * Implements NIST FIPS 203 ML-KEM for quantum-safe key establishment.
 * Provides resistance against quantum computer attacks.
 */
class QuantumSafeKeyEncapsulation {
public:
    explicit QuantumSafeKeyEncapsulation(CryptoAlgorithm algorithm);

    /**
     * @brief Generate a new key pair
     * @return Pair of (public_key, secret_key)
     */
    std::pair<CryptoKey, CryptoKey> generate_keypair();

    /**
     * @brief Encapsulate a shared secret
     * @param public_key Recipient's public key
     * @return Pair of (ciphertext, shared_secret)
     */
    std::pair<std::vector<uint8_t>, std::vector<uint8_t>> encapsulate(
        const CryptoKey& public_key);

    /**
     * @brief Decapsulate a shared secret
     * @param ciphertext Encapsulated ciphertext
     * @param secret_key Recipient's secret key
     * @return Shared secret
     */
    std::vector<uint8_t> decapsulate(const std::vector<uint8_t>& ciphertext,
                                     const CryptoKey& secret_key);

    /**
     * @brief Get algorithm name
     */
    std::string get_algorithm_name() const;

private:
    CryptoAlgorithm algorithm;
};

/**
 * @class QuantumSafeDigitalSignature
 * @brief Quantum-safe digital signature scheme
 * 
 * Implements NIST FIPS 204 ML-DSA for quantum-safe digital signatures.
 * Provides authentication and non-repudiation.
 */
class QuantumSafeDigitalSignature {
public:
    explicit QuantumSafeDigitalSignature(CryptoAlgorithm algorithm);

    /**
     * @brief Generate a new key pair
     * @return Pair of (public_key, secret_key)
     */
    std::pair<CryptoKey, CryptoKey> generate_keypair();

    /**
     * @brief Sign a message
     * @param message Message to sign
     * @param secret_key Signer's secret key
     * @param signer_id Signer identifier
     * @return Digital signature
     */
    CryptoSignature sign(const std::string& message, const CryptoKey& secret_key,
                        const std::string& signer_id);

    /**
     * @brief Verify a signature
     * @param message Original message
     * @param signature Signature to verify
     * @param public_key Signer's public key
     * @return true if signature is valid
     */
    bool verify(const std::string& message, const CryptoSignature& signature,
               const CryptoKey& public_key);

    /**
     * @brief Get algorithm name
     */
    std::string get_algorithm_name() const;

private:
    CryptoAlgorithm algorithm;
};

/**
 * @class SymmetricEncryption
 * @brief Symmetric encryption with authenticated encryption
 * 
 * Provides authenticated encryption with associated data (AEAD).
 * Ensures confidentiality and authenticity.
 */
class SymmetricEncryption {
public:
    explicit SymmetricEncryption(CryptoAlgorithm algorithm);

    /**
     * @brief Generate a random key
     * @return Cryptographic key
     */
    CryptoKey generate_key();

    /**
     * @brief Encrypt data
     * @param plaintext Data to encrypt
     * @param key Encryption key
     * @param associated_data Additional authenticated data (optional)
     * @return Ciphertext with nonce and authentication tag
     */
    std::vector<uint8_t> encrypt(const std::string& plaintext, const CryptoKey& key,
                                const std::string& associated_data = "");

    /**
     * @brief Decrypt data
     * @param ciphertext Data to decrypt
     * @param key Decryption key
     * @param associated_data Additional authenticated data (optional)
     * @return Plaintext if valid, empty string if authentication fails
     */
    std::string decrypt(const std::vector<uint8_t>& ciphertext, const CryptoKey& key,
                       const std::string& associated_data = "");

    /**
     * @brief Get algorithm name
     */
    std::string get_algorithm_name() const;

private:
    CryptoAlgorithm algorithm;
};

/**
 * @class CryptographicHash
 * @brief Cryptographic hash functions
 * 
 * Provides collision-resistant hash functions.
 * Used for integrity verification and key derivation.
 */
class CryptographicHash {
public:
    explicit CryptographicHash(CryptoAlgorithm algorithm);

    /**
     * @brief Hash data
     * @param data Data to hash
     * @return Hash digest
     */
    std::vector<uint8_t> hash(const std::string& data);

    /**
     * @brief Hash file
     * @param filename File to hash
     * @return Hash digest
     */
    std::vector<uint8_t> hash_file(const std::string& filename);

    /**
     * @brief Verify hash
     * @param data Data to verify
     * @param expected_hash Expected hash value
     * @return true if hash matches
     */
    bool verify(const std::string& data, const std::vector<uint8_t>& expected_hash);

    /**
     * @brief Get algorithm name
     */
    std::string get_algorithm_name() const;

    /**
     * @brief Get hash size in bytes
     */
    size_t get_hash_size() const;

private:
    CryptoAlgorithm algorithm;
};

/**
 * @class SideChannelResistance
 * @brief Side-channel attack resistance
 * 
 * Implements countermeasures against side-channel attacks:
 * - Timing attacks
 * - Power analysis attacks
 * - Cache timing attacks
 */
class SideChannelResistance {
public:
    /**
     * @brief Constant-time comparison
     * @param a First value
     * @param b Second value
     * @return true if equal (constant time)
     */
    static bool constant_time_compare(const std::vector<uint8_t>& a,
                                     const std::vector<uint8_t>& b);

    /**
     * @brief Constant-time memory wipe
     * @param ptr Pointer to memory
     * @param size Size of memory
     */
    static void secure_wipe(void* ptr, size_t size);

    /**
     * @brief Add random delay to prevent timing attacks
     * @param min_ms Minimum delay in milliseconds
     * @param max_ms Maximum delay in milliseconds
     */
    static void add_random_delay(uint32_t min_ms, uint32_t max_ms);

    /**
     * @brief Mask sensitive data
     * @param data Data to mask
     * @param mask Random mask
     * @return Masked data
     */
    static std::vector<uint8_t> mask_data(const std::vector<uint8_t>& data,
                                         const std::vector<uint8_t>& mask);

    /**
     * @brief Unmask sensitive data
     * @param masked_data Masked data
     * @param mask Random mask
     * @return Unmasked data
     */
    static std::vector<uint8_t> unmask_data(const std::vector<uint8_t>& masked_data,
                                           const std::vector<uint8_t>& mask);
};

/**
 * @class CryptoValidator
 * @brief Validates cryptographic operations
 */
class CryptoValidator {
public:
    /**
     * @brief Validate key strength
     * @param key Key to validate
     * @return true if key meets security requirements
     */
    static bool validate_key_strength(const CryptoKey& key);

    /**
     * @brief Validate signature format
     * @param signature Signature to validate
     * @return true if signature format is valid
     */
    static bool validate_signature_format(const CryptoSignature& signature);

    /**
     * @brief Check for weak algorithms
     * @param algorithm Algorithm to check
     * @return true if algorithm is considered weak
     */
    static bool is_weak_algorithm(CryptoAlgorithm algorithm);

    /**
     * @brief Get recommended algorithm
     * @return Recommended quantum-safe algorithm
     */
    static CryptoAlgorithm get_recommended_algorithm();
};

} // namespace synq::compiler::security
