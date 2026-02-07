// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Dependency Tracker

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstdint>

namespace synq::compiler::dependency {

/**
 * @class DependencyGraph
 * @brief Tracks file dependencies for incremental compilation
 * 
 * Features:
 * - Tracks which files depend on which
 * - Detects transitive dependencies
 * - Identifies files that need recompilation
 * - Persists to disk for cache validation
 * 
 * Usage:
 * ```cpp
 * DependencyGraph graph;
 * graph.add_dependency("main.synq", "utils.synq");
 * graph.add_dependency("main.synq", "math.synq");
 * 
 * auto changed = graph.get_changed_files({"utils.synq"});
 * // Returns: ["utils.synq", "main.synq"] (main depends on utils)
 * ```
 */
class DependencyGraph {
public:
    /**
     * @brief Add a dependency: file A depends on file B
     */
    void add_dependency(const std::string& file, const std::string& depends_on) {
        dependencies[file].insert(depends_on);
        dependents[depends_on].insert(file);
    }

    /**
     * @brief Get all files that file A depends on
     */
    const std::unordered_set<std::string>& get_dependencies(const std::string& file) const {
        auto it = dependencies.find(file);
        if (it != dependencies.end()) {
            return it->second;
        }
        static const std::unordered_set<std::string> empty;
        return empty;
    }

    /**
     * @brief Get all files that depend on file A
     */
    const std::unordered_set<std::string>& get_dependents(const std::string& file) const {
        auto it = dependents.find(file);
        if (it != dependents.end()) {
            return it->second;
        }
        static const std::unordered_set<std::string> empty;
        return empty;
    }

    /**
     * @brief Get all files that need recompilation given changed files
     * 
     * This includes:
     * - The changed files themselves
     * - All files that depend on changed files (transitive)
     */
    std::unordered_set<std::string> get_files_to_recompile(
        const std::unordered_set<std::string>& changed_files) const {
        
        std::unordered_set<std::string> to_recompile = changed_files;
        std::vector<std::string> queue(changed_files.begin(), changed_files.end());
        
        while (!queue.empty()) {
            std::string file = queue.back();
            queue.pop_back();
            
            const auto& deps = get_dependents(file);
            for (const auto& dependent : deps) {
                if (to_recompile.find(dependent) == to_recompile.end()) {
                    to_recompile.insert(dependent);
                    queue.push_back(dependent);
                }
            }
        }
        
        return to_recompile;
    }

    /**
     * @brief Clear all dependencies
     */
    void clear() {
        dependencies.clear();
        dependents.clear();
    }

    /**
     * @brief Get all tracked files
     */
    std::unordered_set<std::string> get_all_files() const {
        std::unordered_set<std::string> files;
        for (const auto& [file, _] : dependencies) {
            files.insert(file);
        }
        for (const auto& [file, _] : dependents) {
            files.insert(file);
        }
        return files;
    }

    /**
     * @brief Serialize to binary format
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserialize from binary format
     */
    static DependencyGraph deserialize(const std::vector<uint8_t>& data);

private:
    // file -> set of files it depends on
    std::unordered_map<std::string, std::unordered_set<std::string>> dependencies;
    
    // file -> set of files that depend on it
    std::unordered_map<std::string, std::unordered_set<std::string>> dependents;
};

/**
 * @class FileHasher
 * @brief Detects file changes using content hashing
 */
class FileHasher {
public:
    /**
     * @brief Compute hash of file content
     */
    static std::string compute_hash(const std::string& content);

    /**
     * @brief Check if file has changed since last build
     */
    bool has_changed(const std::string& file_path, const std::string& current_hash) {
        auto it = file_hashes.find(file_path);
        if (it == file_hashes.end()) {
            // New file
            file_hashes[file_path] = current_hash;
            return true;
        }
        
        bool changed = it->second != current_hash;
        if (changed) {
            it->second = current_hash;
        }
        return changed;
    }

    /**
     * @brief Update hash for file
     */
    void update_hash(const std::string& file_path, const std::string& hash) {
        file_hashes[file_path] = hash;
    }

    /**
     * @brief Get hash for file
     */
    std::string get_hash(const std::string& file_path) const {
        auto it = file_hashes.find(file_path);
        if (it != file_hashes.end()) {
            return it->second;
        }
        return "";
    }

    /**
     * @brief Clear all hashes
     */
    void clear() {
        file_hashes.clear();
    }

    /**
     * @brief Serialize to binary
     */
    std::vector<uint8_t> serialize() const;

    /**
     * @brief Deserialize from binary
     */
    static FileHasher deserialize(const std::vector<uint8_t>& data);

private:
    std::unordered_map<std::string, std::string> file_hashes;
};

/**
 * @class CompilationCache
 * @brief Manages IR caching with dependency tracking
 */
class CompilationCache {
public:
    explicit CompilationCache(const std::string& cache_dir = ".synq_cache");

    /**
     * @brief Check if file has valid cache
     */
    bool has_valid_cache(const std::string& file_path, const std::string& file_hash) const;

    /**
     * @brief Get cached IR data
     */
    std::vector<uint8_t> get_cached(const std::string& file_path) const;

    /**
     * @brief Store IR data in cache
     */
    void cache(const std::string& file_path, const std::vector<uint8_t>& data);

    /**
     * @brief Get dependency graph
     */
    DependencyGraph& get_dependency_graph() {
        return dependency_graph;
    }

    /**
     * @brief Get file hasher
     */
    FileHasher& get_file_hasher() {
        return file_hasher;
    }

    /**
     * @brief Clear all caches
     */
    void clear();

    /**
     * @brief Load cache metadata from disk
     */
    void load_metadata();

    /**
     * @brief Save cache metadata to disk
     */
    void save_metadata();

private:
    std::string cache_dir;
    DependencyGraph dependency_graph;
    FileHasher file_hasher;
    std::unordered_map<std::string, std::vector<uint8_t>> memory_cache;
};

} // namespace synq::compiler::dependency
