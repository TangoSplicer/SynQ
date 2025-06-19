// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debugger_terminal_ui.h"
#include "debugger_core.h"
#include "symbol_visualizer.h"
#include "debug_trace_tree.h"
#include "live_diff_view.h"

#include <iostream>

namespace synq {

void DebuggerTerminalUI::startSession() {
    std::string input;

    std::cout << "\n╭────────────────────────────────────╮\n";
    std::cout << "│         SynQ Interactive Debugger │\n";
    std::cout << "╰────────────────────────────────────╯\n";
    std::cout << "[UI] Press `:q` to exit. Type `?` for help.\n\n";

    while (true) {
        std::cout << "debug-ui > ";
        std::getline(std::cin, input);

        if (input == ":q") break;
        else if (input == "vars") SymbolVisualizer::showAll();
        else if (input == "trace") DebugTraceTree::printTrace();
        else if (input == "diff") {
            std::string a, b;
            std::cout << "  snapshot A: "; std::getline(std::cin, a);
            std::cout << "  snapshot B: "; std::getline(std::cin, b);
            LiveDiffView::diffSnapshots(a, b);
        }
        else if (input == "?") {
            std::cout << "\n[Available Debug Commands]\n";
            std::cout << "  vars        Show all variables\n";
            std::cout << "  trace       Print function trace\n";
            std::cout << "  diff        Compare two snapshots\n";
            std::cout << "  :q          Exit debugger\n";
        }
        else {
            DebuggerCore::runCommand(input);
        }
    }

    std::cout << "[UI] Debugger session ended.\n";
}

}