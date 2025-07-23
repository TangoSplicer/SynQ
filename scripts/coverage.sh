#!/bin/bash
# synq/scripts/coverage.sh
# SynQ Unified Coverage Collector
# License: SynQ Commercial Attribution License v1.0

echo "=== SynQ Test Coverage Suite ==="

# Clean up previous coverage data
rm -rf coverage
mkdir -p coverage/cpp

# C++ Coverage
echo "--- Generating C++ coverage report ---"
PYBIND11_CMAKE_DIR=$(python3 -m pybind11 --cmakedir)
cmake -D SYNQ_ENABLE_COVERAGE=ON -D CMAKE_PREFIX_PATH=${PYBIND11_CMAKE_DIR} -B build -S .
cmake --build build
(cd build && ctest)
lcov --capture --directory . --output-file coverage/cpp/coverage.info
genhtml coverage/cpp/coverage.info --output-directory coverage/cpp/html

echo "=== Coverage reports generated ==="
echo "C++ report: coverage/cpp/html/index.html"
