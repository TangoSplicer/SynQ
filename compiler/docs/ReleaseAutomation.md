✅ File: synq/docs/ReleaseAutomation.md

📘 Developer & CI Instructions

# SynQ Release Automation

This guide explains how to prepare and publish official plugin releases.

---

## Steps

1. Validate
2. Score
3. Export (.bin)
4. Sign
5. Bundle
6. Publish

---

## CLI Release

```bash
synq run --input synq/scripts/prepare_release.synq --project my_plugin/ --version v1.2.3


---

Publish to SynQHub

./synq/scripts/publish_to_hub.sh my_plugin v1.2.3


---

Output

release_vX.X.X.synqbundle.zip

release_meta.json


Includes signed .bin, export logs, scorecard


---

GitHub Action Integration

jobs:
  release:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - run: docker build -t synq .
      - run: docker run synq bash synq/scripts/publish_to_hub.sh my_plugin vX.Y.Z


---

© 2025 SynQ Project — Release System

---

### ✅ File: `synq/tests/unit/test_release.synq`  
🧪 **Tests Export + Sign + Package**

```synq
test release_automation:

    uses compiler.export.exporter as exporter
    uses signing.signer as signer
    uses io.fs as fs

    case "export and sign plugin":
        let base = "examples/plugins/teleport/"
        let main = base + "main.synq"
        exporter.export_plugin(main, "bin")
        signer.sign_plugin(replace_ext(main, ".bin"))
        assert fs.exists(base + "main.bin.sig")

    case "validate bundle output":
        let meta = fs.read_json("examples/plugins/teleport/release_meta.json")
        assert meta["signed"]
        assert meta["score"] > 50

end
