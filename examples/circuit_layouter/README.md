# Circuit Layouter Plugin

Optimizes a list of quantum gates by evaluating and minimizing execution cost using hybrid logic.

## How it works

Each gate is scored:
- `H`: 1
- `T`: 2
- `CNOT`: 3
- Others: 5+

The plugin sorts and applies gates to reduce total execution cost.

## Configuration

Pass a list of gate operations and their targets.

## Example

```bash
synq run circuit_layouter.synq