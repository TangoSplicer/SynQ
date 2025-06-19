#!/bin/bash
# synq/scripts/coverage.sh
# SynQ Unified Coverage Collector
# License: SynQ Commercial Attribution License v1.0

echo "=== SynQ Test Coverage Suite ==="

BUILD_DIR="./build"
COVERAGE_LOG="./coverage_report.log"
DUMP_BIN="./tests/tools/coverage_dump"

# Build the test coverage aggregator if not already built
if [ ! -f "$DUMP_BIN" ]; then
    echo "[*] Building coverage_dump.cpp..."
    g++ -std=c++17 -O2 -o "$DUMP_BIN" ./tests/tools/coverage_dump.cpp
    if [ $? -ne 0 ]; then
        echo "[✗] Build failed: coverage_dump.cpp"
        exit 1
    fi
fi

# Execute the aggregator
echo "[*] Running coverage dump..."
"$DUMP_BIN"

# Display summary
if [ -f "$COVERAGE_LOG" ]; then
    echo
    echo "=== Coverage Summary ==="
    cat "$COVERAGE_LOG"
else
    echo "[✗] Coverage log not generated."
fi