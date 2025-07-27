# src/security/signing_engine.py
# Central Signing API
# License: SynQ Commercial Attribution License v1.0

from .signer_core import generate_signature
from .verifier_core import verify_signature
from .distributed_keyring import get_private_key, get_public_key

def sign_plugin(path: str, user: str) -> str:
    private_key = get_private_key(user)
    signature = generate_signature(path, private_key)
    with open(path + ".sig", "w") as f:
        f.write(signature)
    return signature

def verify_plugin(path: str, user: str) -> bool:
    public_key = get_public_key(user)
    try:
        with open(path + ".sig", "r") as f:
            signature = f.read().strip()
    except FileNotFoundError:
        return False
    return verify_signature(path, public_key, signature)