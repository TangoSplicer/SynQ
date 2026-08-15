#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>

#include "compiler/bounded_simulator.h"
#include "compiler/hybrid_ir.h"
#include "compiler/name_resolution.h"
#include "compiler/parser.h"

namespace {

bool parse_positive_size(const std::string& text, std::size_t& value) {
    if (text.empty()) return false;
    value = 0;
    for (char character : text) {
        if (character < '0' || character > '9') return false;
        const std::size_t digit = static_cast<std::size_t>(character - '0');
        if (value > (static_cast<std::size_t>(-1) - digit) / 10) return false;
        value = value * 10 + digit;
    }
    return value > 0;
}

}  // namespace

int main(int argc, char** argv) {
    std::size_t iterations = 1000;
    if (argc == 2 && !parse_positive_size(argv[1], iterations)) {
        std::cerr << "usage: synq_benchmark [positive-iterations]\n";
        return 2;
    }
    if (argc > 2) {
        std::cerr << "usage: synq_benchmark [positive-iterations]\n";
        return 2;
    }

    const std::string source =
        "#[experimental(feature = \"qubit-declarations\")]\n"
        "qubit q[2]\n"
        "quantum bell_pair q[0], q[1]\n"
        "measure q[0]\n"
        "measure q[1]\n";
    Parser parser;
    const auto parsed = parser.parseSourceWithDiagnostics(source);
    if (!parsed.ok()) return 3;
    const auto lowered = synq::compiler::lower_to_hybrid_ir(*parsed.program);
    if (!lowered.ok()) return 4;
    const auto resolved = synq::compiler::resolve_hybrid_names(*lowered.program);
    if (!resolved.ok()) return 4;

    synq::compiler::BoundedSimulationOptions options;
    options.allow_experimental_local_simulation = true;
    volatile double sink = 0.0;
    const auto started = std::chrono::steady_clock::now();
    for (std::size_t iteration = 0; iteration < iterations; ++iteration) {
        const auto simulation = synq::compiler::simulate_bounded_quantum(*resolved.program, options);
        if (!simulation.ok() || simulation.simulation->basis_probabilities.size() != 2) return 5;
        sink += simulation.simulation->measurements.front().probability_one;
    }
    const auto elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - started).count();
    const double per_second = static_cast<double>(iterations) / elapsed;
    std::cout << "benchmark = bounded_bell_probability\n"
              << "iterations = " << iterations << "\n"
              << "seconds = " << elapsed << "\n"
              << "simulations_per_second = " << per_second << "\n"
              << "checksum = " << sink << "\n";
    return 0;
}
