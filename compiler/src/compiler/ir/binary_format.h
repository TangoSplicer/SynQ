// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Custom Binary Format
// Optimized for speed and size of IR serialization

#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <cstring>

namespace synq::compiler::ir {

/**
 * @class BinaryFormat
 * @brief Custom binary format specification for SynQ IR serialization
 * 
 * Format Overview:
 * ┌─────────────────────────────────────────────────────────────┐
 * │ Header (16 bytes)                                           │
 * ├─────────────────────────────────────────────────────────────┤
 * │ Magic: "SYNQ" (4 bytes)                                     │
 * │ Version: u32 (4 bytes)                                      │
 * │ IR Type: u32 (4 bytes) [0=Parsed, 1=TypedOpt, 2=LLVM]      │
 * │ Flags: u32 (4 bytes)                                        │
 * ├─────────────────────────────────────────────────────────────┤
 * │ Metadata Section                                            │
 * │ - File hash (32 bytes SHA256)                               │
 * │ - Timestamp (u64)                                           │
 * │ - Dependency count (u32)                                    │
 * │ - Dependencies (variable length)                            │
 * ├─────────────────────────────────────────────────────────────┤
 * │ Data Section (IR-specific)                                  │
 * │ - Variable length, format depends on IR type                │
 * ├─────────────────────────────────────────────────────────────┤
 * │ Checksum (4 bytes CRC32)                                    │
 * └─────────────────────────────────────────────────────────────┘
 * 
 * Design Goals:
 * - Minimal overhead (16-byte header)
 * - Fast parsing (no complex structures)
 * - Compact representation (5-10x smaller than JSON)
 * - Streaming support (can read incrementally)
 * - Version compatibility (can handle format changes)
 */

class BinaryWriter {
public:
    /**
     * @brief Write u8 (1 byte)
     */
    void write_u8(uint8_t value) {
        buffer.push_back(value);
    }

    /**
     * @brief Write u16 (2 bytes, little-endian)
     */
    void write_u16(uint16_t value) {
        buffer.push_back(static_cast<uint8_t>(value & 0xFF));
        buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    }

    /**
     * @brief Write u32 (4 bytes, little-endian)
     */
    void write_u32(uint32_t value) {
        buffer.push_back(static_cast<uint8_t>(value & 0xFF));
        buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
        buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
        buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
    }

    /**
     * @brief Write u64 (8 bytes, little-endian)
     */
    void write_u64(uint64_t value) {
        write_u32(static_cast<uint32_t>(value & 0xFFFFFFFF));
        write_u32(static_cast<uint32_t>((value >> 32) & 0xFFFFFFFF));
    }

    /**
     * @brief Write i32 (4 bytes, little-endian, two's complement)
     */
    void write_i32(int32_t value) {
        write_u32(static_cast<uint32_t>(value));
    }

    /**
     * @brief Write f32 (4 bytes, IEEE 754)
     */
    void write_f32(float value) {
        uint32_t bits;
        std::memcpy(&bits, &value, sizeof(float));
        write_u32(bits);
    }

    /**
     * @brief Write f64 (8 bytes, IEEE 754)
     */
    void write_f64(double value) {
        uint64_t bits;
        std::memcpy(&bits, &value, sizeof(double));
        write_u64(bits);
    }

    /**
     * @brief Write string (u32 length + data)
     */
    void write_string(const std::string& str) {
        write_u32(static_cast<uint32_t>(str.length()));
        buffer.insert(buffer.end(), str.begin(), str.end());
    }

    /**
     * @brief Write raw bytes
     */
    void write_bytes(const uint8_t* data, size_t length) {
        buffer.insert(buffer.end(), data, data + length);
    }

    /**
     * @brief Get the binary buffer
     */
    const std::vector<uint8_t>& get_buffer() const {
        return buffer;
    }

    /**
     * @brief Get buffer size
     */
    size_t size() const {
        return buffer.size();
    }

    /**
     * @brief Clear buffer
     */
    void clear() {
        buffer.clear();
    }

private:
    std::vector<uint8_t> buffer;
};

class BinaryReader {
public:
    explicit BinaryReader(const std::vector<uint8_t>& data) : data(data), pos(0) {}

    /**
     * @brief Read u8 (1 byte)
     */
    uint8_t read_u8() {
        if (pos >= data.size()) throw std::runtime_error("BinaryReader: read past end");
        return data[pos++];
    }

    /**
     * @brief Read u16 (2 bytes, little-endian)
     */
    uint16_t read_u16() {
        uint8_t b0 = read_u8();
        uint8_t b1 = read_u8();
        return (static_cast<uint16_t>(b1) << 8) | b0;
    }

    /**
     * @brief Read u32 (4 bytes, little-endian)
     */
    uint32_t read_u32() {
        uint8_t b0 = read_u8();
        uint8_t b1 = read_u8();
        uint8_t b2 = read_u8();
        uint8_t b3 = read_u8();
        return (static_cast<uint32_t>(b3) << 24) |
               (static_cast<uint32_t>(b2) << 16) |
               (static_cast<uint32_t>(b1) << 8) |
               b0;
    }

    /**
     * @brief Read u64 (8 bytes, little-endian)
     */
    uint64_t read_u64() {
        uint32_t lo = read_u32();
        uint32_t hi = read_u32();
        return (static_cast<uint64_t>(hi) << 32) | lo;
    }

    /**
     * @brief Read i32 (4 bytes, little-endian, two's complement)
     */
    int32_t read_i32() {
        return static_cast<int32_t>(read_u32());
    }

    /**
     * @brief Read f32 (4 bytes, IEEE 754)
     */
    float read_f32() {
        uint32_t bits = read_u32();
        float value;
        std::memcpy(&value, &bits, sizeof(float));
        return value;
    }

    /**
     * @brief Read f64 (8 bytes, IEEE 754)
     */
    double read_f64() {
        uint64_t bits = read_u64();
        double value;
        std::memcpy(&value, &bits, sizeof(double));
        return value;
    }

    /**
     * @brief Read string (u32 length + data)
     */
    std::string read_string() {
        uint32_t length = read_u32();
        if (pos + length > data.size()) throw std::runtime_error("BinaryReader: read past end");
        std::string str(data.begin() + pos, data.begin() + pos + length);
        pos += length;
        return str;
    }

    /**
     * @brief Read raw bytes
     */
    std::vector<uint8_t> read_bytes(size_t length) {
        if (pos + length > data.size()) throw std::runtime_error("BinaryReader: read past end");
        std::vector<uint8_t> result(data.begin() + pos, data.begin() + pos + length);
        pos += length;
        return result;
    }

    /**
     * @brief Get current position
     */
    size_t tell() const {
        return pos;
    }

    /**
     * @brief Seek to position
     */
    void seek(size_t new_pos) {
        if (new_pos > data.size()) throw std::runtime_error("BinaryReader: seek past end");
        pos = new_pos;
    }

    /**
     * @brief Check if at end
     */
    bool at_end() const {
        return pos >= data.size();
    }

    /**
     * @brief Get remaining bytes
     */
    size_t remaining() const {
        return data.size() - pos;
    }

private:
    const std::vector<uint8_t>& data;
    size_t pos;
};

/**
 * @class BinaryFormatHeader
 * @brief Header for binary IR files
 */
struct BinaryFormatHeader {
    static constexpr const char* MAGIC = "SYNQ";
    static constexpr uint32_t VERSION = 1;

    enum class IRType : uint32_t {
        PARSED = 0,
        TYPED_OPTIMIZED = 1,
        LLVM = 2
    };

    enum class Flags : uint32_t {
        NONE = 0,
        DEBUG_INFO = 1 << 0,
        OPTIMIZED = 1 << 1,
        INCREMENTAL = 1 << 2
    };

    IRType ir_type;
    uint32_t flags;
    std::string file_hash;      // SHA256 hash (32 bytes)
    uint64_t timestamp;
    std::vector<std::string> dependencies;

    /**
     * @brief Write header to binary
     */
    void write(BinaryWriter& writer) const {
        // Magic
        writer.write_bytes(reinterpret_cast<const uint8_t*>(MAGIC), 4);
        
        // Version
        writer.write_u32(VERSION);
        
        // IR Type
        writer.write_u32(static_cast<uint32_t>(ir_type));
        
        // Flags
        writer.write_u32(flags);
        
        // File hash (SHA256, 32 bytes)
        if (file_hash.length() == 64) {  // Hex string
            writer.write_string(file_hash);
        } else {
            writer.write_string("");
        }
        
        // Timestamp
        writer.write_u64(timestamp);
        
        // Dependencies
        writer.write_u32(static_cast<uint32_t>(dependencies.size()));
        for (const auto& dep : dependencies) {
            writer.write_string(dep);
        }
    }

    /**
     * @brief Read header from binary
     */
    static BinaryFormatHeader read(BinaryReader& reader) {
        BinaryFormatHeader header;
        
        // Magic
        std::vector<uint8_t> magic = reader.read_bytes(4);
        if (std::string(magic.begin(), magic.end()) != MAGIC) {
            throw std::runtime_error("BinaryFormatHeader: Invalid magic number");
        }
        
        // Version
        uint32_t version = reader.read_u32();
        if (version != VERSION) {
            throw std::runtime_error("BinaryFormatHeader: Unsupported version");
        }
        
        // IR Type
        header.ir_type = static_cast<IRType>(reader.read_u32());
        
        // Flags
        header.flags = reader.read_u32();
        
        // File hash
        header.file_hash = reader.read_string();
        
        // Timestamp
        header.timestamp = reader.read_u64();
        
        // Dependencies
        uint32_t dep_count = reader.read_u32();
        for (uint32_t i = 0; i < dep_count; ++i) {
            header.dependencies.push_back(reader.read_string());
        }
        
        return header;
    }
};

/**
 * @brief Calculate CRC32 checksum
 */
uint32_t crc32(const uint8_t* data, size_t length);

/**
 * @brief Calculate SHA256 hash (hex string)
 */
std::string sha256(const std::string& data);

} // namespace synq::compiler::ir
