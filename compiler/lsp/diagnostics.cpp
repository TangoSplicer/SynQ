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
// diagnostics.cpp
#include "diagnostics.h"
#include <regex>

using json = nlohmann::json;

namespace synq::lsp {

  json run_diagnostics(const std::string& source) {
    json results = json::array();
    int line_number = 0;
    std::istringstream stream(source);
    std::string line;

    while (std::getline(stream, line)) {
      if (line.find(";;") != std::string::npos) {
        results.push_back({
          {"range", {
            {"start", {{"line", line_number}, {"character", 0}}},
            {"end",   {{"line", line_number}, {"character", (int)line.length()}}}
          }},
          {"message", "Double semicolon is not valid syntax"},
          {"severity", 2}
        });
      }
      line_number++;
    }

    return results;
  }

}