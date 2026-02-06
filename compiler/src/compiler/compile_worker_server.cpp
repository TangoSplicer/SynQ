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
// compile_worker_server.cpp
#include "compiler_job.h"
#include <iostream>
#include <thread>

using namespace synq::compiler;

int main() {
  std::cout << "👷 Worker Server Ready. Listening for compilation jobs...\n";

  while (true) {
    CompilerJob job("example", "int main(){}", {"-O2"});
    std::cout << "🛠️  Compiling job: " << job.name << " with flags:";
    for (const auto& flag : job.flags) std::cout << " " << flag;
    std::cout << "\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "✅ Job complete: " << job.name << "\n";

    break; // Simulated single-job run
  }

  return 0;
}