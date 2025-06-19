// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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