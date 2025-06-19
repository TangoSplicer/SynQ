# src/security/signer_core.py
# SynQ Quantum-Safe Signing Engine Core
# License: SynQ Commercial Attribution License v1.0

import hashlib
import os
from base64 import b64encode

def generate_signature(file_path: str, private_key: str) -> str:
    with open(file_path, "rb") as f:
        data = f.read()
    digest = hashlib.sha3_512(data + private_key.encode()).digest()
    return b64encode(digest).decode("utf-8")