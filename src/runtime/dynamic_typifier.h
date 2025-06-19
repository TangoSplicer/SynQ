// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_DYNAMIC_TYPIFIER_H
#define SYNQ_DYNAMIC_TYPIFIER_H

#include <string>
#include <map>

namespace synq {

class DynamicTypifier {
public:
    static void analyze(const std::string& expr);
    static void suggestType(const std::string& var);
private:
    static std::map<std::string, std::string> typeHints;
};

}

#endif // SYNQ_DYNAMIC_TYPIFIER_H