// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "debugger_core.h"
#include <iostream>
#include <sstream>

namespace synq {

std::map<std::string, bool> DebuggerCore::breakpoints;
std::vector<std::string> DebuggerCore::commandHistory;

void DebuggerCore::enter() {
    std::string line;
    std::cout << "\n[Debugger] Entering paused session (type 'help' for commands)\n";
    while (true) {
        std::cout << "(dbg)> ";
        std::getline(std::cin, line);
        if (line == "exit" || line == "continue") break;
        runCommand(line);
    }
}

void DebuggerCore::defineBreakpoint(const std::string& fn) {
    breakpoints[fn] = true;
    std::cout << "[Debugger] Breakpoint set at: " << fn << "\n";
}

void DebuggerCore::removeBreakpoint(const std::string& fn) {
    breakpoints.erase(fn);
    std::cout << "[Debugger] Breakpoint removed from: " << fn << "\n";
}

bool DebuggerCore::isBreakpoint(const std::string& fn) {
    return breakpoints.count(fn) > 0;
}

void DebuggerCore::runCommand(const std::string& cmd) {
    commandHistory.push_back(cmd);
    std::istringstream iss(cmd);
    std::string word;
    iss >> word;

    if (word == "help") {
        printHelp();
    } else if (word == "break") {
        std::string fn;
        iss >> fn;
        defineBreakpoint(fn);
    } else if (word == "unbreak") {
        std::string fn;
        iss >> fn;
        removeBreakpoint(fn);
    } else if (word == "bt") {
        std::cout << "[Backtrace] (Stubbed) - Call stack unavailable in core layer.\n";
    } else if (word == "history") {
        for (const auto& c : commandHistory)
            std::cout << " - " << c << "\n";
    } else {
        std::cout << "[Debugger] Unknown command: " << cmd << "\n";
    }
}

void DebuggerCore::printHelp() {
    std::cout << "\n[Debugger Commands]\n"
              << "  help           Show this help message\n"
              << "  break <func>   Set breakpoint at function\n"
              << "  unbreak <func> Remove breakpoint from function\n"
              << "  bt             Print backtrace\n"
              << "  history        Show entered debugger commands\n"
              << "  continue       Resume execution\n"
              << "  exit           Leave debugger\n";
}

}