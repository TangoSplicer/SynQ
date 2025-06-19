# Classical Cache Plugin

Implements a Least Recently Used (LRU) cache that works across classical and quantum compute layers.

## Features

- Maintains a small memory-efficient cache
- Recomputes values when not found
- Automatically evicts least used keys

## Example

Used in parameter reuse, inference template memory, or fast partial circuit rebuilds.