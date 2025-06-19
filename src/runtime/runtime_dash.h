// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_RUNTIME_DASH_H
#define SYNQ_RUNTIME_DASH_H

#include <string>

namespace synq {

class RuntimeDash {
public:
    static void showSystemMetrics();
    static void showCallStack();
    static void showQuantumStats();
};

}

#endif // SYNQ_RUNTIME_DASH_H