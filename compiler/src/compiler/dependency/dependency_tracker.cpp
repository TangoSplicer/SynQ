// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Dependency Tracker Implementation

#include "dependency_tracker.h"
#include "../ir/binary_format.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

namespace synq::compiler::dependency {

// ============================================================================
// DependencyGraph Implementation
// ============================================================================

std::vector<uint8_t> DependencyGraph::serialize() const {
    ir::BinaryWriter writer;
    
    // Write dependencies
    writer.write_u32(static_cast<uint32_t>(dependencies.size()));
    for (const auto& [file, deps] : dependencies) {
        writer.write_string(file);
        writer.write_u32(static_cast<uint32_t>(deps.size()));
        for (const auto& dep : deps) {
            writer.write_string(dep);
        }
    }
    
    return writer.get_buffer();
}

DependencyGraph DependencyGraph::deserialize(const std::vector<uint8_t>& data) {
    ir::BinaryReader reader(data);
    DependencyGraph graph;
    
    uint32_t file_count = reader.read_u32();
    for (uint32_t i = 0; i < file_count; ++i) {
        std::string file = reader.read_string();
        uint32_t dep_count = reader.read_u32();
        for (uint32_t j = 0; j < dep_count; ++j) {
            std::string dep = reader.read_string();
            graph.add_dependency(file, dep);
        }
    }
    
    return graph;
}

// ============================================================================
// FileHasher Implementation
// ============================================================================

std::string FileHasher::compute_hash(const std::string& content) {
    // Use SHA256 from binary_format module
    return ir::sha256(content);
}

std::vector<uint8_t> FileHasher::serialize() const {
    ir::BinaryWriter writer;
    
    writer.write_u32(static_cast<uint32_t>(file_hashes.size()));
    for (const auto& [file, hash] : file_hashes) {
        writer.write_string(file);
        writer.write_string(hash);
    }
    
    return writer.get_buffer();
}

FileHasher FileHasher::deserialize(const std::vector<uint8_t>& data) {
    ir::BinaryReader reader(data);
    FileHasher hasher;
    
    uint32_t count = reader.read_u32();
    for (uint32_t i = 0; i < count; ++i) {
        std::string file = reader.read_string();
        std::string hash = reader.read_string();
        hasher.file_hashes[file] = hash;
    }
    
    return hasher;
}

// ============================================================================
// CompilationCache Implementation
// ============================================================================

CompilationCache::CompilationCache(const std::string& cache_dir)
    : cache_dir(cache_dir) {
    
    // Create cache directory if it doesn't exist
    try {
        fs::create_directories(cache_dir);
    } catch (...) {
        // Directory might already exist
    }
    
    // Load existing metadata
    load_metadata();
}

bool CompilationCache::has_valid_cache(
    const std::string& file_path,
    const std::string& file_hash) const {
    
    // Check if file hash matches cached hash
    std::string cached_hash = file_hasher.get_hash(file_path);
    if (cached_hash.empty() || cached_hash != file_hash) {
        return false;
    }
    
    // Check if cache file exists
    std::string cache_file = cache_dir + "/" + std::to_string(std::hash<std::string>{}(file_path)) + ".cache";
    return fs::exists(cache_file);
}

std::vector<uint8_t> CompilationCache::get_cached(const std::string& file_path) const {
    // Check memory cache first
    auto it = memory_cache.find(file_path);
    if (it != memory_cache.end()) {
        return it->second;
    }
    
    // Load from disk
    std::string cache_file = cache_dir + "/" + std::to_string(std::hash<std::string>{}(file_path)) + ".cache";
    std::ifstream file(cache_file, std::ios::binary);
    if (!file) {
        return {};
    }
    
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    std::vector<uint8_t> data(size);
    file.read(reinterpret_cast<char*>(data.data()), size);
    
    return data;
}

void CompilationCache::cache(const std::string& file_path, const std::vector<uint8_t>& data) {
    // Store in memory cache
    memory_cache[file_path] = data;
    
    // Write to disk
    std::string cache_file = cache_dir + "/" + std::to_string(std::hash<std::string>{}(file_path)) + ".cache";
    std::ofstream file(cache_file, std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
    }
}

void CompilationCache::clear() {
    memory_cache.clear();
    
    // Clear disk cache
    try {
        for (const auto& entry : fs::directory_iterator(cache_dir)) {
            if (entry.path().extension() == ".cache") {
                fs::remove(entry);
            }
        }
    } catch (...) {
        // Ignore errors
    }
}

void CompilationCache::load_metadata() {
    // Load dependency graph
    std::string dep_file = cache_dir + "/dependencies.bin";
    if (fs::exists(dep_file)) {
        std::ifstream file(dep_file, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            std::vector<uint8_t> data(size);
            file.read(reinterpret_cast<char*>(data.data()), size);
            
            try {
                dependency_graph = DependencyGraph::deserialize(data);
            } catch (...) {
                // Ignore deserialization errors
            }
        }
    }
    
    // Load file hashes
    std::string hash_file = cache_dir + "/hashes.bin";
    if (fs::exists(hash_file)) {
        std::ifstream file(hash_file, std::ios::binary);
        if (file) {
            file.seekg(0, std::ios::end);
            size_t size = file.tellg();
            file.seekg(0, std::ios::beg);
            
            std::vector<uint8_t> data(size);
            file.read(reinterpret_cast<char*>(data.data()), size);
            
            try {
                file_hasher = FileHasher::deserialize(data);
            } catch (...) {
                // Ignore deserialization errors
            }
        }
    }
}

void CompilationCache::save_metadata() {
    // Save dependency graph
    std::string dep_file = cache_dir + "/dependencies.bin";
    auto dep_data = dependency_graph.serialize();
    std::ofstream dep_out(dep_file, std::ios::binary);
    if (dep_out) {
        dep_out.write(reinterpret_cast<const char*>(dep_data.data()), dep_data.size());
    }
    
    // Save file hashes
    std::string hash_file = cache_dir + "/hashes.bin";
    auto hash_data = file_hasher.serialize();
    std::ofstream hash_out(hash_file, std::ios::binary);
    if (hash_out) {
        hash_out.write(reinterpret_cast<const char*>(hash_data.data()), hash_data.size());
    }
}

} // namespace synq::compiler::dependency
