# SynQ Export Targets

SynQ supports three official export formats for plugin execution:

---

## 1. `.synqvm`

- Native bytecode format
- Runs in SynQ hybrid execution chain
- Interoperable with plugin sandbox, AI operators

## 2. `.bin` + `.hash`

- Optimized binary
- Includes signed hash (TGIL + QRYPTA)
- Used for trust-bounded plugin bundles and chain deployment

## 3. `.wasm`

- WebAssembly export
- Enables browser previews and embedded visual demos
- Safe to run in isolated sandboxes

---

## CLI Usage

```bash
synq export --input teleport/main.synq --target synqvm
synq export --input teleport/main.synq --target wasm