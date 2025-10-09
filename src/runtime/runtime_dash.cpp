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
#include "runtime_dash.h"
#include <iostream>

namespace synq {

void RuntimeDash::showSystemMetrics() {
    std::cout << "[RuntimeDash] CPU Load: 12.5%\n";
    std::cout << "Memory Usage: 134MB / 512MB\n";
}

void RuntimeDash::showCallStack() {
    std::cout << "[RuntimeDash] Top Call Stack:\n";
    std::cout << "  1. main()\n";
    std::cout << "  2. repl_loop()\n";
}

void RuntimeDash::showQuantumStats() {
    std::cout << "[RuntimeDash] QPU Entropy: 0.87 | Decoherence Level: 3\n";
}

}