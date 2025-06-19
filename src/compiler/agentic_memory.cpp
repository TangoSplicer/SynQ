// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "agentic_memory.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace synq {

    AgenticMemory::AgenticMemory(std::string agent_id) : agent_id_(std::move(agent_id)) {}

    void AgenticMemory::record(const std::string& type, const std::string& content,
                               const std::optional<std::string>& label) {
        MemoryEntry entry;
        entry.timestamp = current_time();
        entry.type = type;
        entry.content = content;
        entry.label = label;
        log_.push_back(entry);
    }

    std::vector<MemoryEntry> AgenticMemory::retrieve_by_type(const std::string& type) const {
        std::vector<MemoryEntry> filtered;
        for (const auto& entry : log_) {
            if (entry.type == type)
                filtered.push_back(entry);
        }
        return filtered;
    }

    std::optional<MemoryEntry> AgenticMemory::retrieve_by_label(const std::string& label) const {
        for (const auto& entry : log_) {
            if (entry.label && entry.label.value() == label)
                return entry;
        }
        return std::nullopt;
    }

    std::string AgenticMemory::export_summary() const {
        std::ostringstream oss;
        oss << "Memory Summary for Agent [" << agent_id_ << "]\n";
        for (const auto& entry : log_) {
            oss << "[" << entry.timestamp << "] "
                << entry.type << ": "
                << entry.content;
            if (entry.label)
                oss << " (label: " << entry.label.value() << ")";
            oss << "\n";
        }
        return oss.str();
    }

    void AgenticMemory::clear() {
        log_.clear();
    }

    std::string AgenticMemory::current_time() const {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

} // namespace synq