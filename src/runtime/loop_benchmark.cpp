// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

#include "loop_benchmark.h"
#include <iostream>
#include <chrono>

namespace synq {

void LoopBenchmark::run(int iterations) {
    using clock = std::chrono::high_resolution_clock;
    auto start = clock::now();

    volatile int dummy = 0;
    for (int i = 0; i < iterations; ++i) {
        dummy += i;
    }

    auto end = clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "[Benchmark] Looped " << iterations << " times in " << ms << "ms.\n";
}

}