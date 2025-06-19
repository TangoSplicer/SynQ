#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[DEPLOY] Uploading signed build to target environment..."
rsync -avz ../build/ user@server:/opt/synq/build/
echo "[DEPLOY] Upload complete."