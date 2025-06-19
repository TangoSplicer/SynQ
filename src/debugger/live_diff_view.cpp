// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "live_diff_view.h"
#include "eval_context.h"
#include <iostream>

namespace synq {

std::map<std::string, LiveDiffView::Snapshot> LiveDiffView::snapshots;

void LiveDiffView::takeSnapshot(const std::string& id) {
    snapshots[id] = EvalContext::variables;
    std::cout << "[LiveDiff] Snapshot '" << id << "' taken with "
              << snapshots[id].size() << " entries.\n";
}

void LiveDiffView::diffSnapshots(const std::string& a, const std::string& b) {
    if (!snapshots.count(a) || !snapshots.count(b)) {
        std::cout << "[LiveDiff] Missing snapshots: " << a << " or " << b << "\n";
        return;
    }

    const auto& sa = snapshots[a];
    const auto& sb = snapshots[b];

    std::cout << "[LiveDiff] Changes from '" << a << "' → '" << b << "':\n";

    for (const auto& [k, va] : sa) {
        if (sb.count(k)) {
            if (va != sb.at(k)) {
                std::cout << "  ~ " << k << ": " << va << " → " << sb.at(k) << "\n";
            }
        } else {
            std::cout << "  - " << k << " removed\n";
        }
    }

    for (const auto& [k, vb] : sb) {
        if (!sa.count(k)) {
            std::cout << "  + " << k << " added = " << vb << "\n";
        }
    }
}

}