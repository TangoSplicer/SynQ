# State Tracer Plugin

Provides a full trace of the quantum state vector during circuit execution.

## Features

- `trace_state` captures the entire system at key checkpoints
- Useful for debugging or training

## Output

Each checkpoint logs the state vector to the trace stream, visible in `synq monitor` and log overlays.

## Example

```bash
synq run state_tracer.synq