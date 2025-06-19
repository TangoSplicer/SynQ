// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_LIVE_MUTATOR_H
#define SYNQ_LIVE_MUTATOR_H

#include <string>

namespace synq {

class LiveMutator {
public:
    static void patchVariable(const std::string& var, const std::string& val);
    static void patchFunction(const std::string& name, const std::string& code);
};

}

#endif // SYNQ_LIVE_MUTATOR_H