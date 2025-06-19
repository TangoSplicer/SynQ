// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SHADOW_RUNTIME_H
#define SYNQ_SHADOW_RUNTIME_H

#include <string>
#include <vector>

namespace synq {

class ShadowRuntime {
public:
    static void recordFailedPath(const std::string& code);
    static std::string suggestRecovery();
    static void printFailedPaths();

private:
    static std::vector<std::string> failedPaths;
};

}

#endif // SYNQ_SHADOW_RUNTIME_H