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