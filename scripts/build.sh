#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[BUILD] Building SynQ via CMake..."
mkdir -p ../build
cd ../build || exit
cmake ..
make -j$(nproc)
echo "[BUILD] Done."