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

# src/security/signing_engine.py
# Central Signing API
# License: 

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