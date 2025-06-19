// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_RUNTIME_HEATMAP_H
#define SYNQ_RUNTIME_HEATMAP_H

#include <string>
#include <map>

namespace synq {

class RuntimeHeatmap {
public:
    static void mark(const std::string& region, int intensity);
    static void render();
    static void reset();

private:
    static std::map<std::string, int> zones;
};

}

#endif // SYNQ_RUNTIME_HEATMAP_H