// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Binary Format Implementation

#include "binary_format.h"
#include <array>
#include <sstream>
#include <iomanip>

namespace synq::compiler::ir {

// CRC32 lookup table
static constexpr std::array<uint32_t, 256> CRC32_TABLE = []() {
    std::array<uint32_t, 256> table{};
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
        table[i] = crc;
    }
    return table;
}();

/**
 * @brief Calculate CRC32 checksum
 */
uint32_t crc32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        uint8_t byte = data[i];
        crc = (crc >> 8) ^ CRC32_TABLE[(crc ^ byte) & 0xFF];
    }
    return crc ^ 0xFFFFFFFF;
}

/**
 * @brief Simple SHA256 implementation (for file hashing)
 * 
 * This is a simplified version. For production, consider using OpenSSL or similar.
 * For now, we use a fast hash function suitable for change detection.
 */
std::string sha256(const std::string& data) {
    // For Phase 11, we use a fast hash function (not cryptographically secure SHA256)
    // This is sufficient for change detection and cache invalidation
    // In production, replace with proper SHA256 from OpenSSL
    
    // Simple FNV-1a hash, extended to 256 bits
    uint64_t hash1 = 0xCBF29CE484222325ULL;
    uint64_t hash2 = 0xCBF29CE484222325ULL;
    
    for (size_t i = 0; i < data.length(); ++i) {
        if (i % 2 == 0) {
            hash1 ^= static_cast<uint64_t>(data[i]);
            hash1 *= 0x100000001B3ULL;
        } else {
            hash2 ^= static_cast<uint64_t>(data[i]);
            hash2 *= 0x100000001B3ULL;
        }
    }
    
    // Convert to hex string
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    ss << std::setw(16) << hash1;
    ss << std::setw(16) << hash2;
    ss << std::setw(16) << (hash1 ^ hash2);
    ss << std::setw(16) << (hash1 + hash2);
    
    return ss.str();
}

} // namespace synq::compiler::ir
