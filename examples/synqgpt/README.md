# SynQGPT Plugin

A prompt-chaining engine for large language models (LLMs), designed for embedding intelligent decisions into SynQ workflows.

## Configuration

- `model`: One of `"gpt-4"`, `"gemini"`, `"claude"`, or custom ID
- `prompt`: Text to feed to the LLM

## Example

```bash
synq run synqgpt.synq --config model=gpt-4 prompt="Explain entanglement"