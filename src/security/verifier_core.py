# src/security/verifier_core.py
# SynQ Signature Verifier
# License: SynQ Commercial Attribution License v1.0

from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding, rsa
from cryptography.hazmat.primitives.serialization import load_pem_public_key
from base64 import b64decode

def verify_signature(file_path: str, public_key_path: str, signature: str) -> bool:
    with open(public_key_path, "rb") as key_file:
        public_key = load_pem_public_key(key_file.read())

    with open(file_path, "rb") as f:
        data = f.read()

    try:
        public_key.verify(
            b64decode(signature),
            data,
            padding.PSS(
                mgf=padding.MGF1(hashes.SHA256()),
                salt_length=padding.PSS.MAX_LENGTH
            ),
            hashes.SHA256()
        )
        return True
    except Exception:
        return False