#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[FORMAT] Auto-formatting all source files..."
black ../src
clang-format -i $(find ../src -name '*.cpp')
echo "[FORMAT] Complete."