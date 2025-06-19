#!/bin/bash
# synq/scripts/install_tools.sh
# Sets up completions and aliases for SynQ CLI
# © 2025 SynQ Project

echo "🔧 Installing SynQ developer tooling..."

TOOL_DIR="$(cd "$(dirname "$0")" && pwd)"
BASHRC="$HOME/.bashrc"
ZSHRC="$HOME/.zshrc"

echo "📁 Tool directory: $TOOL_DIR"

# Bash setup
if [ -f "$BASHRC" ]; then
    grep -q 'synq.bash' "$BASHRC" || echo "source $TOOL_DIR/completions/synq.bash" >> "$BASHRC"
    grep -q 'synq.aliases' "$BASHRC" || echo "source $TOOL_DIR/completions/synq.aliases" >> "$BASHRC"
    echo "✅ SynQ bash completions and aliases installed in $BASHRC"
fi

# ZSH setup
if [ -f "$ZSHRC" ]; then
    grep -q 'synq.zsh' "$ZSHRC" || echo "source $TOOL_DIR/completions/synq.zsh" >> "$ZSHRC"
    grep -q 'synq.aliases' "$ZSHRC" || echo "source $TOOL_DIR/completions/synq.aliases" >> "$ZSHRC"
    echo "✅ SynQ ZSH completions and aliases installed in $ZSHRC"
fi

echo "✅ Done. Please restart your terminal or run: source ~/.bashrc or ~/.zshrc"