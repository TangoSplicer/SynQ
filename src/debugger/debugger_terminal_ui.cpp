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