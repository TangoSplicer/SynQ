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
// synq/src/cli/synq_build.cpp

#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    std::cout << "🔧 Building SynQ project...\n";

    std::string build_dir = "./build";
    if (!std::filesystem::exists(build_dir)) {
        std::filesystem::create_directory(build_dir);
    }

    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << "❌ Fork failed.\n";
        exit(1);
    } else if (pid == 0) {
        chdir("build");
        execlp("cmake", "cmake", "..", (char*)NULL);
        std::cerr << "❌ Exec failed.\n";
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (status != 0) {
            std::cerr << "❌ CMake failed.\n";
            exit(1);
        }
    }

    pid = fork();
    if (pid == -1) {
        std::cerr << "❌ Fork failed.\n";
        exit(1);
    } else if (pid == 0) {
        chdir("build");
        execlp("make", "make", "-j$(nproc)", (char*)NULL);
        std::cerr << "❌ Exec failed.\n";
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (status != 0) {
            std::cerr << "❌ Make failed.\n";
            exit(1);
        }
    }

    std::cout << "✅ Build complete.\n";
    return 0;
}