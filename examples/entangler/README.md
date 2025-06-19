# Entangler Plugin

Simulates quantum entanglement patterns including GHZ, W-state, and cluster state.

## Configuration

- `pattern`: `"ghz"`, `"w"`, or `"cluster"`
- `qubits`: Number of qubits to entangle

## Usage

```bash
synq run entangler.synq --config pattern=ghz qubits=5