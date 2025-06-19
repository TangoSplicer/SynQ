// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_SHELL_UTILS_H
#define SYNQ_SHELL_UTILS_H

#include <string>

namespace synq {

class ShellUtils {
public:
    static std::string execute(const std::string& command);
};

}

#endif // SYNQ_SHELL_UTILS_H