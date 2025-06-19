// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#ifndef SYNQ_LIVE_DIFF_VIEW_H
#define SYNQ_LIVE_DIFF_VIEW_H

#include <string>
#include <map>

namespace synq {

class LiveDiffView {
public:
    using Snapshot = std::map<std::string, std::string>;

    static void takeSnapshot(const std::string& id);
    static void diffSnapshots(const std::string& a, const std::string& b);
private:
    static std::map<std::string, Snapshot> snapshots;
};

}

#endif // SYNQ_LIVE_DIFF_VIEW_H