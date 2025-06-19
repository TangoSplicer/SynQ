#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[MUTATE] Applying AI mutation to all plugins..."
find ../src/plugins -name '*.synq' -exec synq mutate {} \;
echo "[MUTATE] Mutation complete."