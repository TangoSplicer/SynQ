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
#include "docstring_resolver.h"
#include <iostream>

namespace synq {

std::map<std::string, std::string> DocstringResolver::docs;

void DocstringResolver::registerDoc(const std::string& symbol, const std::string& doc) {
    docs[symbol] = doc;
}

std::string DocstringResolver::fetch(const std::string& symbol) {
    return docs.count(symbol) ? docs[symbol] : "[No documentation]";
}

void DocstringResolver::listAll() {
    std::cout << "[Docstrings]\n";
    for (const auto& [sym, doc] : docs) {
        std::cout << "  " << sym << " - " << doc << "\n";
    }
}

}