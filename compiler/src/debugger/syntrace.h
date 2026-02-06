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
#ifndef SYNQ_SYNTRACE_H
#define SYNQ_SYNTRACE_H

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace synq {

struct TraceFrame {
    std::string functionName;
    std::string file;
    int line;
    std::map<std::string, std::string> locals;
    std::string quantumStateSnapshot;
};

class SynTrace {
public:
    static void enter(const std::string& function, const std::string& file, int line);
    static void exit();
    static void snapshot(const std::string& var, const std::string& val);
    static void quantumState(const std::string& snapshot);
    static void printTrace();
    static void clear();

private:
    static std::vector<TraceFrame> callStack;
};

} // namespace synq

#endif // SYNQ_SYNTRACE_H