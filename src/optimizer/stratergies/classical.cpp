// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

// File: optimizer/strategies/classical.cpp

#include "classical.h"
#include <iostream>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>

namespace synq {
namespace classical {

ParamMap gridSearch(const Objective& f, const ParamMap& init, int max_iters) {
    ParamMap best_params = init;
    double best_score = std::numeric_limits<double>::max();
    int steps = std::max(1, max_iters / init.size());
    double step_size = 0.1;

    for (int step = 0; step < steps; ++step) {
        ParamMap trial = init;
        for (const auto& [k, v] : init) {
            trial[k] = v + step * step_size;
        }
        double result = f(trial);
        if (result < best_score) {
            best_score = result;
            best_params = trial;
        }
    }
    return best_params;
}

ParamMap randomSearch(const Objective& f, const ParamMap& init, int max_iters) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    ParamMap best = init;
    double best_val = std::numeric_limits<double>::max();

    for (int i = 0; i < max_iters; ++i) {
        ParamMap trial;
        for (const auto& [k, v] : init) {
            trial[k] = v + dist(gen);
        }
        double result = f(trial);
        if (result < best_val) {
            best_val = result;
            best = trial;
        }
    }
    return best;
}

ParamMap nelderMeadSearch(const Objective& f, const ParamMap& init, int max_iters) {
    // Placeholder: this should build a simplex and reflect/expand/contract
    std::cout << "[NM] Simplex search placeholder\n";
    return randomSearch(f, init, max_iters); // simulate with randomness for now
}

ParamMap evolutionSearch(const Objective& f, const ParamMap& init, int max_iters) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> mut(0.0, 0.3);

    const int population = 6;
    ParamMap best = init;
    double best_val = std::numeric_limits<double>::max();

    for (int gen_idx = 0; gen_idx < max_iters; ++gen_idx) {
        std::vector<std::pair<double, ParamMap>> scored;

        for (int i = 0; i < population; ++i) {
            ParamMap trial = best;
            for (auto& [k, v] : trial) {
                v += mut(gen);
            }
            double result = f(trial);
            scored.push_back({result, trial});
        }

        std::sort(scored.begin(), scored.end());
        if (!scored.empty() && scored[0].first < best_val) {
            best_val = scored[0].first;
            best = scored[0].second;
        }
    }

    return best;
}

ParamMap bracketedSearch(const Objective& f, const ParamMap& init, int max_iters) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(-0.5, 0.5);

    ParamMap best = init;
    double best_val = std::numeric_limits<double>::max();

    for (int i = 0; i < max_iters; ++i) {
        ParamMap trial;
        for (auto& [k, v] : init) {
            trial[k] = v + dist(gen);
        }

        double score = f(trial) * (1.0 + std::abs(dist(gen))); // weighted penalty
        if (score < best_val) {
            best_val = score;
            best = trial;
        }
    }

    return best;
}

ParamMap echoWaveSearch(const Objective& f, const ParamMap& init, int max_iters) {
    std::random_device rd;
    std::mt19937 gen(rd());
    ParamMap best = init;
    double best_val = std::numeric_limits<double>::max();

    double alpha = 0.2;
    for (int iter = 0; iter < max_iters; ++iter) {
        ParamMap trial;
        for (auto& [k, v] : init) {
            double v0 = f({{k, v}});
            double vp = f({{k, v + alpha}});
            double vm = f({{k, v - alpha}});
            double vpp = f({{k, v + 2 * alpha}});
            double vmm = f({{k, v - 2 * alpha}});

            double echo = (vp - vm) + 0.5 * (vpp - vmm);
            double new_v = v - echo * 0.1; // interference-like update

            trial[k] = new_v;
        }

        double result = f(trial);
        if (result < best_val) {
            best_val = result;
            best = trial;
        }
    }

    std::cout << "[ECHO] Best echo-interference score: " << best_val << "\n";
    return best;
}

} // namespace classical
} // namespace synq