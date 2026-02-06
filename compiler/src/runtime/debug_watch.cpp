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
#include "debug_watch.h"
#include <iostream>
#include <set>

namespace synq {

static std::set<std::string> watchedVars;

void DebugWatch::watch(const std::string& variable) {
    watchedVars.insert(variable);
    std::cout << "[DebugWatch] Watching: " << variable << "\n";
}

void DebugWatch::unwatch(const std::string& variable) {
    watchedVars.erase(variable);
    std::cout << "[DebugWatch] Unwatched: " << variable << "\n";
}

void DebugWatch::list() {
    std::cout << "[DebugWatch] Current watches:\n";
    for (const auto& var : watchedVars) {
        std::cout << " - " << var << "\n";
    }
}

}