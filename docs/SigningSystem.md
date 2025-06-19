// SynQ Commercial Attribution License v1.0
// © 2025 SynQ Contributors. All rights reserved.
// This file is part of the SynQ programming ecosystem.

✅ docs/SigningSystem.md

# SynQ Distributed Plugin Signing System

## Overview

SynQ includes a built-in signing and verification framework designed to:
- Securely sign plugins with quantum-safe hashes
- Verify author identity during execution or cloud sync
- Support multi-developer teams with federated keys

## Commands

```bash
synq sign --sign myplugin.synq dev1
synq sign --verify myplugin.synq dev1

Internals

Uses SHA3-512 hash with user-specific key material

Distributed keyring supports multi-author signatures

Future support: post-quantum lattice signature schemes


Files

.sig files are placed alongside source

distributed_keyring.py handles user trust


---