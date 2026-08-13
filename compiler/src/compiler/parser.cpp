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
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include "parser.h"

namespace {

std::string trim(const std::string& value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char ch) {
        return std::isspace(ch) != 0;
    }).base();
    return first >= last ? "" : std::string(first, last);
}

bool is_non_negative_integer(const std::string& value) {
    return !value.empty() && std::all_of(value.begin(), value.end(), [](unsigned char ch) {
        return std::isdigit(ch) != 0;
    });
}

bool is_identifier(const std::string& value) {
    if (value.empty() || !(std::isalpha(static_cast<unsigned char>(value.front())) != 0 || value.front() == '_')) {
        return false;
    }
    return std::all_of(value.begin() + 1, value.end(), [](unsigned char ch) {
        return std::isalnum(ch) != 0 || ch == '_';
    });
}

std::string strip_comment(const std::string& value) {
    // Recovery-profile comment rule: `//` begins a comment only at the start
    // of a trimmed line or when preceded by whitespace. This preserves values
    // such as `https://example.invalid` while accepting ordinary inline notes.
    std::size_t marker = value.find("//");
    while (marker != std::string::npos) {
        if (marker == 0 || std::isspace(static_cast<unsigned char>(value[marker - 1])) != 0) {
            return trim(value.substr(0, marker));
        }
        marker = value.find("//", marker + 2);
    }
    return trim(value);
}

}  // namespace

ASTNode* Parser::parseFile(const std::string& filename) {
    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return nullptr;
    }

    std::cout << "Parsing " << filename << "..." << std::endl;
    ProgramNode* root = new ProgramNode();

    // Recovery-profile grammar: one statement per line, with an optional
    // trailing semicolon. Supported statements are `let <identifier> = <value>`
    // plus the instructions `print <text>`, `delay <non-negative milliseconds>`,
    // `quantum <kernel>`, and `ai <prompt>`. Declaration values are preserved as
    // source text; expression parsing is intentionally out of scope. Blank lines
    // and `//` comments at line start or after whitespace are ignored. The latter
    // rule deliberately preserves `//` in unquoted source text such as URLs.
    std::string raw_line;
    std::size_t line_number = 0;
    while (std::getline(infile, raw_line)) {
        ++line_number;
        std::string line = strip_comment(trim(raw_line));
        if (line.empty()) {
            continue;
        }
        if (line.back() == ';') {
            line = trim(line.substr(0, line.size() - 1));
        }

        std::istringstream tokens(line);
        std::string operation;
        tokens >> operation;
        std::string argument;
        std::getline(tokens, argument);
        argument = trim(argument);

        if (operation == "let") {
            const auto assignment = argument.find('=');
            const std::string identifier = assignment == std::string::npos ? "" : trim(argument.substr(0, assignment));
            const std::string value = assignment == std::string::npos ? "" : trim(argument.substr(assignment + 1));
            if (!is_identifier(identifier) || value.empty()) {
                std::cerr << "Error: malformed declaration at " << filename
                          << ":" << line_number << std::endl;
                delete root;
                return nullptr;
            }
            root->statements.push_back(new DeclarationNode(identifier, value, line_number));
            continue;
        }

        const bool known_instruction = operation == "print" || operation == "delay" ||
                                       operation == "quantum" || operation == "ai";
        if (!known_instruction || argument.empty() ||
            (operation == "delay" && !is_non_negative_integer(argument))) {
            std::cerr << "Error: unsupported or malformed instruction at " << filename
                      << ":" << line_number << std::endl;
            delete root;
            return nullptr;
        }

        root->statements.push_back(new InstructionNode(operation, {argument}, line_number));
    }

    return root;
}
