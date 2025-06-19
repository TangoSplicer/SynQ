#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[CLEAN] Removing build and temp files..."
rm -rf ../build ../dist ../*.egg-info
find .. -name '__pycache__' -exec rm -rf {} +
echo "[CLEAN] Done."