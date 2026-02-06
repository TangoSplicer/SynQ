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
#include "debugger_launcher.h"
#include "debugger_core.h"
#include "debug_trace_tree.h"
#include "error_dive.h"
#include "debugger_ai_assist.h"
#include "symbol_visualizer.h"

#include <iostream>

namespace synq {

void DebuggerLauncher::triggerFromError(const std::string& error) {
    std::cout << "\n[Debugger] Error triggered debugger session\n";
    ErrorDive::dive(error);
    DebuggerAIAssist::explain(error);
    DebuggerCore::enter();
}

void DebuggerLauncher::triggerManual() {
    std::cout << "\n[Debugger] Manual session started\n";
    DebuggerCore::enter();
}

void DebuggerLauncher::triggerBreakpoint(const std::string& func) {
    std::cout << "\n[Debugger] Breakpoint hit in function: " << func << "\n";
    DebugTraceTree::printTrace();
    SymbolVisualizer::showAll();
    DebuggerCore::enter();
}

}