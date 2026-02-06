# MIT License
#
# Copyright (c) 2025 SynQ Contributors
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


# Author: SynQ Contributors — Federated Signing Keyring System
# Description: Implements multi-signature keyring with quorum enforcement

import json
import os
import hashlib
from typing import Dict, List, Tuple

KEYRING_PATH = "build/keyring_registry.json"
SIGNATURES_PATH = "build/plugin_signatures.json"

# Public key metadata entry
class KeyMetadata:
    def __init__(self, fingerprint, pubkey, owner):
        self.fingerprint = fingerprint
        self.pubkey = pubkey
        self.owner = owner

# Signature record
class Signature:
    def __init__(self, plugin_id, fingerprint, signature_hex):
        self.plugin_id = plugin_id
        self.fingerprint = fingerprint
        self.signature = signature_hex

# Load or initialize registry
def load_registry() -> Dict[str, KeyMetadata]:
    if os.path.exists(KEYRING_PATH):
        with open(KEYRING_PATH, "r") as f:
            raw = json.load(f)
            return {k: KeyMetadata(**v) for k, v in raw.items()}
    return {}

def save_registry(registry: Dict[str, KeyMetadata]):
    with open(KEYRING_PATH, "w") as f:
        json.dump({k: v.__dict__ for k, v in registry.items()}, f, indent=2)

def register_key(fingerprint: str, pubkey: str, owner: str):
    registry = load_registry()
    registry[fingerprint] = KeyMetadata(fingerprint, pubkey, owner)
    save_registry(registry)

def list_keys() -> List[KeyMetadata]:
    return list(load_registry().values())

# Load or initialize plugin signatures
def load_signatures() -> Dict[str, List[Signature]]:
    if os.path.exists(SIGNATURES_PATH):
        with open(SIGNATURES_PATH, "r") as f:
            raw = json.load(f)
            return {k: [Signature(**s) for s in v] for k, v in raw.items()}
    return {}

def save_signatures(sigs: Dict[str, List[Signature]]):
    with open(SIGNATURES_PATH, "w") as f:
        json.dump({k: [s.__dict__ for s in v] for k, v in sigs.items()}, f, indent=2)

def sign_artifact(plugin_id: str, private_key: str) -> str:
    fingerprint = hashlib.sha256(private_key.encode()).hexdigest()[:16]
    signature = hashlib.sha256((plugin_id + private_key).encode()).hexdigest()
    sig = Signature(plugin_id, fingerprint, signature)
    all_sigs = load_signatures()
    all_sigs.setdefault(plugin_id, []).append(sig)
    save_signatures(all_sigs)
    return fingerprint

# Threshold store
QUORUM_CONFIG_PATH = "build/quorum_config.json"

def set_quorum_threshold(plugin_id: str, required: int, total: int):
    conf = {}
    if os.path.exists(QUORUM_CONFIG_PATH):
        with open(QUORUM_CONFIG_PATH, "r") as f:
            conf = json.load(f)
    conf[plugin_id] = {"required": required, "total": total}
    with open(QUORUM_CONFIG_PATH, "w") as f:
        json.dump(conf, f, indent=2)

def get_quorum_threshold(plugin_id: str) -> Tuple[int, int]:
    if os.path.exists(QUORUM_CONFIG_PATH):
        with open(QUORUM_CONFIG_PATH, "r") as f:
            conf = json.load(f)
            if plugin_id in conf:
                return conf[plugin_id]["required"], conf[plugin_id]["total"]
    return 0, 0

def verify_quorum(plugin_id: str) -> bool:
    required, total = get_quorum_threshold(plugin_id)
    if required == 0:
        return False
    sigs = load_signatures().get(plugin_id, [])
    fingerprints = {s.fingerprint for s in sigs}
    return len(fingerprints) >= required