// MIT License
// 
// Copyright (c) 2025 SynQ Contributors
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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