// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "step_controller.h"

namespace synq {

StepController::Mode StepController::stepMode = StepController::NONE;

void StepController::stepInto() {
    stepMode = INTO;
}

void StepController::stepOver() {
    stepMode = OVER;
}

void StepController::stepOut() {
    stepMode = OUT;
}

StepController::Mode StepController::currentMode() {
    return stepMode;
}

void StepController::clear() {
    stepMode = NONE;
}

}