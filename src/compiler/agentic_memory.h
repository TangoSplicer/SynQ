// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_AGENTIC_MEMORY_H
#define SYNQ_AGENTIC_MEMORY_H

#include <string>
#include <vector>
#include <map>
#include <optional>

namespace synq {

    // Memory entry representing an observation, action, or note
    struct MemoryEntry {
        std::string timestamp;
        std::string type;         // e.g., "observation", "plan", "error", "tool_call"
        std::string content;
        std::optional<std::string> label;
        std::optional<std::string> embedding_id;  // optional reference to vector store
    };

    // Core memory store that supports scoped, persistent agent memory
    class AgenticMemory {
    public:
        AgenticMemory(std::string agent_id);

        void record(const std::string& type, const std::string& content,
                    const std::optional<std::string>& label = std::nullopt);

        std::vector<MemoryEntry> retrieve_by_type(const std::string& type) const;
        std::optional<MemoryEntry> retrieve_by_label(const std::string& label) const;

        std::string export_summary() const;
        void clear();

    private:
        std::string agent_id_;
        std::vector<MemoryEntry> log_;
        std::string current_time() const;
    };

}  // namespace synq

#endif  // SYNQ_AGENTIC_MEMORY_H