// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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