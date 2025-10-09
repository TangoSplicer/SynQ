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