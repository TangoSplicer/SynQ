// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_ERRORWHISPERER_H
#define SYNQ_ERRORWHISPERER_H

#include <string>
#include <vector>
#include <map>

namespace synq {

class ErrorWhisperer {
public:
    static void recordError(const std::string& message, const std::string& file, int line);
    static std::string explainLastError();
    static void suggestFixes();
    static void clear();

private:
    struct ErrorEntry {
        std::string message;
        std::string file;
        int line;
    };

    static std::vector<ErrorEntry> history;
};

}

#endif // SYNQ_ERRORWHISPERER_H