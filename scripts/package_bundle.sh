#!/bin/bash
# © 2025 SynQ Project. All rights reserved.
# This file is part of the proprietary SynQ language platform.
# Commercial use, redistribution, or modification is strictly prohibited
# without explicit written permission. See LICENSE.txt for full terms.

echo "[PACKAGE] Bundling binaries and docs..."
tar czf synq_bundle.tar.gz ../build ../docs ../LICENSE.txt
echo "[PACKAGE] Created synq_bundle.tar.gz"