// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_STEP_CONTROLLER_H
#define SYNQ_STEP_CONTROLLER_H

#include <string>

namespace synq {

class StepController {
public:
    enum Mode { NONE, INTO, OVER, OUT };

    static void stepInto();
    static void stepOver();
    static void stepOut();
    static Mode currentMode();
    static void clear();

private:
    static Mode stepMode;
};

}

#endif // SYNQ_STEP_CONTROLLER_H