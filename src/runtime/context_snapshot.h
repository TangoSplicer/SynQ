// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_CONTEXT_SNAPSHOT_H
#define SYNQ_CONTEXT_SNAPSHOT_H

#include <string>

namespace synq {

class ContextSnapshot {
public:
    static void save(const std::string& filename);
    static void restore(const std::string& filename);
};

}

#endif // SYNQ_CONTEXT_SNAPSHOT_H