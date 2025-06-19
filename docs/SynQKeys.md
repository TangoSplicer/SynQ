
---

📘 File: docs/FederatedKeys.md

# 🔐 SynQ Federated Signing Keyring

## Overview

The SynQ Federated Signing Keyring system enables **multi-party digital validation** for plugins, pipelines, and compiler components.

This system ensures that:
- Trust is distributed across a group of known signers
- No single developer can publish a plugin without quorum approval
- Artifacts are cryptographically verified during compilation and execution

---

## ✍️ Key Concepts

| Term | Description |
|------|-------------|
| **Fingerprint** | Short SHA-256 identifier of a private/public key |
| **Quorum** | Minimum number of signatures required (e.g. 3-of-5) |
| **Plugin ID** | Unique identifier of the plugin artifact |
| **Signature** | SHA-256 hash of plugin ID signed with private key |

---

## 📂 Files and Paths

| File | Purpose |
|------|---------|
| `build/keyring_registry.json` | Stores trusted signers and owners |
| `build/plugin_signatures.json` | Holds per-plugin signatures |
| `build/quorum_config.json` | Defines required threshold for plugins |

---

## 🔑 Registering Keys

To add a public key to the trusted registry:

```python
from distributed_keyring import register_key

register_key(fingerprint="abc123...", pubkey="MIIBIj...", owner="Alice")


---

📝 Signing Plugins

To generate a signature for a plugin:

sign_artifact("my_plugin", private_key="...secret...")


---

✅ Checking Quorum

Via CLI:

synq quorum check my_plugin

Output:

Quorum for 'my_plugin': 3 of 5 signatures
✅ Quorum met.


---

⚙️ Setting Quorum Requirements

Set thresholds (e.g. 3-of-5):

synq quorum set my_plugin 3 5


---

📜 REPL Extension

Within REPL:

> skillmap my_plugin
> quorum_status my_plugin

(Coming soon)


---

💼 Use Cases

Federated governance over sensitive plugins

Signed releases with chain-of-trust

Prevent rogue uploads to SynQHub



---

🛡 Future Upgrades

GPG + post-quantum key formats

Remote quorum sync over IPFS or SynQHub



---

📌 Summary

The Federated Signing Keyring makes plugin and compiler signing:

Transparent ✅

Trustworthy ✅

Distributed ✅



