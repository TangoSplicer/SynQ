// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_ERROR_DIVE_H
#define SYNQ_ERROR_DIVE_H

#include <string>

namespace synq {

class ErrorDive {
public:
    static void dive(const std::string& errorMsg);
};

}

#endif // SYNQ_ERROR_DIVE_H