// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/commands/repl_help.cpp

#include <iostream>

void show_repl_help() {
    std::cout << "📘 SynQ REPL Command Index:\n\n";

    std::cout << "🧪 Core:\n";
    std::cout << "  help, :reset, :reload, :snapshot, :exit\n\n";

    std::cout << "🔍 Inspect:\n";
    std::cout << "  :inspect mem, qreg, trace, plugin, ai, heat\n\n";

    std::cout << "🧠 AI:\n";
    std::cout << "  :@ [prompt]        → Ask AI question\n";
    std::cout << "  :why               → Explain last step\n\n";

    std::cout << "⚙️ Plugin:\n";
    std::cout << "  := load NAME       → Load plugin\n";
    std::cout << "  := mutate current  → Evolve plugin\n\n";

    std::cout << "📚 Extensions:\n";
    std::cout << "  :edit              → Edit last expression\n";
    std::cout << "  :history           → View REPL history\n";
    std::cout << "  :config            → Show config\n";
    std::cout << "  :who               → List all vars\n";
    std::cout << "  :undo              → Undo last action\n";
    std::cout << "  :plot              → Visualize states\n";
    std::cout << "  :man <cmd>         → Detailed help\n";
}