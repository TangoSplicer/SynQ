#!/bin/bash
# synq/synq_init.sh

echo "🔁 Initializing SynQ Compiler Environment..."

# Step 1: Setup build
mkdir -p build && cd build
cmake ..
make -j$(nproc)

# Step 2: Run full test suite
echo "🧪 Running test suite..."
./synq_test_runner

# Step 3: Plugin Load Test
echo "🔌 Verifying plugin registration..."
synq plugin list

# Step 4: CLI Startup Test
echo "🚀 Launching SynQ CLI..."
echo "exit" | synq repl

# Step 5: Output Coverage Summary
echo "📈 Dumping coverage reports..."
cd ../tests/tools
g++ coverage_dump.cpp -o dump_core && ./dump_core
g++ coverage_dump_classical.cpp -o dump_classical && ./dump_classical
g++ coverage_dump_quantum.cpp -o dump_quantum && ./dump_quantum

echo "✅ SynQ Initialization Complete"