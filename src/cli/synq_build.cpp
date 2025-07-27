// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

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