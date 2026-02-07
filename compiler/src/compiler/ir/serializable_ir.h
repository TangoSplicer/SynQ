// MIT License
// Copyright (c) 2025 SynQ Contributors
// 
// Phase 11: Performance & Compilation - Modular Pipeline
// Serializable Intermediate Representation Base Class

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace synq::compiler::ir {

using json = nlohmann::json;

/**
 * @class SerializableIR
 * @brief Base class for all serializable intermediate representations in the compilation pipeline.
 * 
 * Each stage of the compilation pipeline produces an IR that can be:
 * - Serialized to JSON for caching
 * - Deserialized from cache for incremental compilation
 * - Inspected for debugging
 * - Passed to the next stage
 * 
 * This enables:
 * - Incremental compilation (cache unchanged files)
 * - Parallel compilation (independent files)
 * - Debugging (inspect intermediate representations)
 * - Extensibility (add new stages easily)
 */
class SerializableIR {
public:
    virtual ~SerializableIR() = default;

    /**
     * @brief Get the name of this IR type (for logging/debugging)
     * @return IR type name (e.g., "TokenStream", "AST", "TypedAST")
     */
    virtual std::string ir_type() const = 0;

    /**
     * @brief Serialize this IR to JSON format
     * @return JSON string representation
     */
    virtual json serialize() const = 0;

    /**
     * @brief Deserialize this IR from JSON format
     * @param data JSON object to deserialize from
     */
    virtual void deserialize(const json& data) = 0;

    /**
     * @brief Get the source file path this IR was compiled from
     */
    const std::string& get_file_path() const { return file_path; }

    /**
     * @brief Get the hash of the source file (for change detection)
     */
    const std::string& get_file_hash() const { return file_hash; }

    /**
     * @brief Get list of files this IR depends on
     */
    const std::vector<std::string>& get_dependencies() const { return dependencies; }

    /**
     * @brief Get compilation timestamp
     */
    uint64_t get_timestamp() const { return timestamp; }

    /**
     * @brief Set the source file path
     */
    void set_file_path(const std::string& path) { file_path = path; }

    /**
     * @brief Set the source file hash
     */
    void set_file_hash(const std::string& hash) { file_hash = hash; }

    /**
     * @brief Add a dependency
     */
    void add_dependency(const std::string& dep) { dependencies.push_back(dep); }

    /**
     * @brief Set dependencies
     */
    void set_dependencies(const std::vector<std::string>& deps) { dependencies = deps; }

    /**
     * @brief Update timestamp to current time
     */
    void update_timestamp();

protected:
    std::string file_path;                    ///< Source file path
    std::string file_hash;                    ///< SHA256 hash of source file
    std::vector<std::string> dependencies;    ///< Files this IR depends on
    uint64_t timestamp = 0;                   ///< Compilation timestamp (milliseconds since epoch)

    /**
     * @brief Helper to serialize common metadata
     */
    json serialize_metadata() const {
        return json{
            {"file_path", file_path},
            {"file_hash", file_hash},
            {"dependencies", dependencies},
            {"timestamp", timestamp},
            {"ir_type", ir_type()}
        };
    }

    /**
     * @brief Helper to deserialize common metadata
     */
    void deserialize_metadata(const json& data) {
        if (data.contains("file_path")) file_path = data["file_path"];
        if (data.contains("file_hash")) file_hash = data["file_hash"];
        if (data.contains("dependencies")) dependencies = data["dependencies"];
        if (data.contains("timestamp")) timestamp = data["timestamp"];
    }
};

} // namespace synq::compiler::ir
