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