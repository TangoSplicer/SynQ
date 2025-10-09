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
#include "runtime_heatmap.h"
#include <iostream>
#include <iomanip>

namespace synq {

std::map<std::string, int> RuntimeHeatmap::zones;

void RuntimeHeatmap::mark(const std::string& region, int intensity) {
    zones[region] += intensity;
}

void RuntimeHeatmap::render() {
    std::cout << "[Runtime Heatmap Intensity]\n";
    for (const auto& [region, intensity] : zones) {
        std::cout << "  " << std::setw(15) << std::left << region
                  << ": " << std::string(intensity / 10, '#') << " (" << intensity << ")\n";
    }
}

void RuntimeHeatmap::reset() {
    zones.clear();
}

}