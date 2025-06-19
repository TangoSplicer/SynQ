#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[BENCH] Benchmarking plugin runtimes..."
for f in ../src/plugins/*.synq; do
  echo "[PLUGIN] $f"
  synq bench --plugin "$f"
done