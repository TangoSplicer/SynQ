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
#ifndef SYNQ_COMPILER_AI_TOOLKIT_H
#define SYNQ_COMPILER_AI_TOOLKIT_H

#include <string>
#include <vector>

namespace synq {
namespace compiler {
namespace ai {

class AIToolkit {
public:
    explicit AIToolkit(const std::string& filepath);
    
    void analyzeErrors();
    void suggestFixes();
    void optimizeCode();
    void quantizeCode();
    void rewriteWithPersona(const std::string& style);
    void annotateCode();
    
    void enablePrivacyMode(bool enable);
    
private:
    std::string filepath;
    std::string code;
    bool privacyMode;

    std::string readCode();
    void sendToLLM(const std::string& prompt);
};

} // namespace ai
} // namespace compiler
} // namespace synq

#endif // SYNQ_COMPILER_AI_TOOLKIT_H