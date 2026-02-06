// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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