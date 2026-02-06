// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include "live_diff_view.h"
#include "../runtime/eval_context.h"
#include <iostream>

namespace synq {

std::map<std::string, LiveDiffView::Snapshot> LiveDiffView::snapshots;

void LiveDiffView::takeSnapshot(const std::string& id) {
    snapshots[id] = EvalContext::getVariables();
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