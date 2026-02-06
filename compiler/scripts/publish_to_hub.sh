#!/bin/bash
set -e

PLUGIN_DIR=$1
VERSION=$2

echo "🔁 Preparing release for $PLUGIN_DIR (version $VERSION)"
synq run --input synq/scripts/prepare_release.synq --project "$PLUGIN_DIR" --version "$VERSION"

echo "☁️ Publishing to SynQHub..."
synq publish --remote --input "$PLUGIN_DIR/release_$VERSION.synqbundle.zip"

echo "✅ Upload complete."