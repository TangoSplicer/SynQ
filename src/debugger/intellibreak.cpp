// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "intellibreak.h"
#include <iostream>

namespace synq {

std::map<std::string, IntelliBreak::Predicate> IntelliBreak::predicates;

void IntelliBreak::set(const std::string& id, Predicate condition) {
    predicates[id] = condition;
}

void IntelliBreak::check(const std::map<std::string, std::string>& state) {
    for (const auto& [id, pred] : predicates) {
        if (pred(state)) {
            std::cout << "[IntelliBreak Hit] Condition met at: " << id << "\n";
        }
    }
}

void IntelliBreak::clear() {
    predicates.clear();
}

}