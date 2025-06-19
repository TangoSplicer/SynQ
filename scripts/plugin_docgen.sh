#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[DOCGEN] Generating AI-based plugin docs..."
for f in ../src/plugins/*.synq; do
  synq ai docgen "$f" > "${f}.md"
done