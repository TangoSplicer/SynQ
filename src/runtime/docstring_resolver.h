// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DOCSTRING_RESOLVER_H
#define SYNQ_DOCSTRING_RESOLVER_H

#include <string>
#include <map>

namespace synq {

class DocstringResolver {
public:
    static void registerDoc(const std::string& symbol, const std::string& doc);
    static std::string fetch(const std::string& symbol);
    static void listAll();

private:
    static std::map<std::string, std::string> docs;
};

}

#endif // SYNQ_DOCSTRING_RESOLVER_H