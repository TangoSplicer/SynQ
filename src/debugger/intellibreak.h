// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_INTELLIBREAK_H
#define SYNQ_INTELLIBREAK_H

#include <string>
#include <functional>
#include <map>

namespace synq {

class IntelliBreak {
public:
    using Predicate = std::function<bool(const std::map<std::string, std::string>&)>;

    static void set(const std::string& id, Predicate condition);
    static void check(const std::map<std::string, std::string>& state);
    static void clear();

private:
    static std::map<std::string, Predicate> predicates;
};

}

#endif // SYNQ_INTELLIBREAK_H