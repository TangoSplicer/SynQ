// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// copilot.h
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class CopilotMode {
    HINT,
    SUGGEST,
    AUTOFIX,
    EXPLAIN,
    WHISPER
};

struct CopilotContext {
    std::string source;
    std::string filepath;
    std::vector<std::string> replHistory;
};

class Copilot {
public:
    Copilot();

    std::string respond(const std::string& input, CopilotMode mode, const CopilotContext& ctx);
    std::string generateHint(const std::string& fragment);
    std::string suggestFix(const std::string& error);
    std::string explainError(const std::string& trace);
    std::string whisper(const std::vector<std::string>& history);

    void injectHintDirective(const std::string& src);

private:
    std::unordered_map<std::string, std::string> hintCache;
    void cacheHints(const std::string& code);
};