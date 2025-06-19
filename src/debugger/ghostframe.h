// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_GHOSTFRAME_H
#define SYNQ_GHOSTFRAME_H

#include <vector>
#include <map>
#include <string>

namespace synq {

struct GhostFrame {
    int tickId;
    std::map<std::string, std::string> state;
};

class GhostFrame {
public:
    static void record(int tick, const std::map<std::string, std::string>& state);
    static void rewind(int tick);
    static void play();
    static void reset();

private:
    static std::vector<GhostFrame> frameLog;
};

}

#endif // SYNQ_GHOSTFRAME_H