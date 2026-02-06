# Optimizer AI Plugin

Uses RL-like trial-and-evaluation cycles to find the best configuration for a given plugin.

## How it works

- Randomized candidate configs are generated
- Each is scored using `simulate`
- Best result is selected and returned

## Configuration

- `plugin_name`: Target plugin to optimize (e.g. `qcompress`)
- `trials`: Number of simulations to run

## Example

```bash
synq run optimizer_ai.synq --config plugin_name=qcompress trials=20