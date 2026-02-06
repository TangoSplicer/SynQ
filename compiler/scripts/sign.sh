#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[SIGN] Signing all plugins and outputs..."
find ../build -type f -name '*.so' -exec synq sign {} \;
find ../plugins -type f -name '*.synq' -exec synq sign {} \;
echo "[SIGN] All signatures complete."