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
// hover_info.cpp
#include "hover_info.h"
#include <map>
#include <sstream>
#include <vector>

namespace synq::lsp {

  static std::map<std::string, std::string> doc_map = {
    {"let", "Defines a new immutable variable"},
    {"mut", "Defines a mutable variable"},
    {"qapply", "Applies a quantum gate to the target qubit(s)"},
    {"measure", "Collapses a qubit and returns its classical value"}
  };

  std::string extract_token(const std::string& line, int pos) {
    int start = line.find_last_of(" \t", pos);
    int end = line.find_first_of(" \t();", pos);
    return line.substr(start == std::string::npos ? 0 : start + 1,
                       (end == std::string::npos ? line.size() : end) - (start + 1));
  }

  std::string get_hover_info(const std::string& code, int line, int char_pos) {
    std::istringstream iss(code);
    std::string current_line;
    for (int i = 0; i <= line && std::getline(iss, current_line); ++i) {
      if (i == line) {
        std::string token = extract_token(current_line, char_pos);
        auto it = doc_map.find(token);
        return it != doc_map.end() ? it->second : "No documentation found";
      }
    }
    return "";
  }

}