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