// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// synq/src/repl/core/synq_repl.cpp

#include <iostream>
#include "repl_shell.cpp"
#include "repl_ai.cpp"
#include "repl_plugin.cpp"
#include "repl_trace.cpp"
#include "repl_state.cpp"

// Commands
#include "../commands/repl_help.cpp"
#include "../commands/repl_history.cpp"
#include "../commands/repl_editor.cpp"
#include "../commands/repl_config.cpp"
#include "../commands/repl_undo.cpp"
#include "../commands/repl_plot.cpp"

int main() {
    std::cout << "🧠 Welcome to the SynQ Hybrid REPL\n";
    std::cout << "Type `help` or `:man` for commands.\n";
    std::cout << "Use `>>>` for blocks, `:@` for AI, `:=` for plugins, `:|` for pipelines\n\n";

    initialize_state();
    start_shell_loop();

    return 0;
}