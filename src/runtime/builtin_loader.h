// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_BUILTIN_LOADER_H
#define SYNQ_BUILTIN_LOADER_H

#include <string>
#include <map>

namespace synq {

class BuiltinLoader {
public:
    static void registerBuiltin(const std::string& name, const std::string& doc);
    static std::string describe(const std::string& name);
    static void list();

private:
    static std::map<std::string, std::string> builtins;
};

}

#endif // SYNQ_BUILTIN_LOADER_H