// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// copilot.cpp
#include "copilot.h"
#include <iostream>

Copilot::Copilot() {}

std::string Copilot::respond(const std::string& input, CopilotMode mode, const CopilotContext& ctx) {
    switch (mode) {
        case CopilotMode::HINT: return generateHint(input);
        case CopilotMode::SUGGEST: return suggestFix(input);
        case CopilotMode::AUTOFIX: return "[autofix TBD]";
        case CopilotMode::EXPLAIN: return explainError(input);
        case CopilotMode::WHISPER: return whisper(ctx.replHistory);
        default: return "🤖 Copilot: Unknown mode";
    }
}

std::string Copilot::generateHint(const std::string& fragment) {
    if (hintCache.count(fragment)) return hintCache[fragment];
    std::string hint = "💡 Try simplifying: " + fragment;
    hintCache[fragment] = hint;
    return hint;
}

std::string Copilot::suggestFix(const std::string& error) {
    return "🔧 Based on error, consider revising syntax near: " + error;
}

std::string Copilot::explainError(const std::string& trace) {
    return "📚 Error traceback explained: " + trace;
}

std::string Copilot::whisper(const std::vector<std::string>& history) {
    if (history.empty()) return "🤫 Nothing to whisper.";
    return "🧠 I noticed you're repeating: '" + history.back() + "'. Try rephrasing?";
}

void Copilot::injectHintDirective(const std::string& src) {
    std::cout << "🪄 Injected hint tags from source...\n";
    cacheHints(src);
}

void Copilot::cacheHints(const std::string& code) {
    hintCache["@copilot hint:loop"] = "🔁 Use 'for ... in' or while depending on exit strategy.";
    hintCache["@copilot hint:quantum"] = "🧪 Ensure qubit allocation is balanced.";
}