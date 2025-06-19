#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[BUILD] Compiling all SynQ targets..."
bash build.sh
synq transpile examples/qpe.synq --target=cpp -o build/qpe.cpp
synq transpile examples/qpe.synq --target=qasm -o build/qpe.qasm
echo "[BUILD] All targets generated."