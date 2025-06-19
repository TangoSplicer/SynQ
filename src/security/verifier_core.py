# src/security/verifier_core.py
# SynQ Signature Verifier
# License: SynQ Commercial Attribution License v1.0

import hashlib
from base64 import b64decode

def verify_signature(file_path: str, public_key: str, signature: str) -> bool:
    with open(file_path, "rb") as f:
        data = f.read()
    digest = hashlib.sha3_512(data + public_key.encode()).digest()
    return digest == b64decode(signature)