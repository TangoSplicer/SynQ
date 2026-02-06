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


# Author: SynQ Contributors
# Description: SynQHUB Plugin Federation System – Remote Publishing and Validation

import os
import json
import hashlib
import requests
from typing import Dict

HUB_URL = "https://synqhub.io/api/v1"
LOCAL_PLUGIN_DIR = "build/plugins/"
MANIFEST_FILE = "plugin.json"

def set_hub_url(url: str):
    global HUB_URL
    HUB_URL = url

def _load_plugin_manifest(plugin_path: str) -> Dict:
    manifest_path = os.path.join(plugin_path, MANIFEST_FILE)
    if not os.path.exists(manifest_path):
        raise FileNotFoundError(f"Missing manifest at {manifest_path}")
    with open(manifest_path, "r") as f:
        return json.load(f)

def _create_zip_archive(plugin_name: str) -> str:
    path = os.path.join(LOCAL_PLUGIN_DIR, plugin_name)
    archive_path = os.path.join(path, f"{plugin_name}.zip")
    import shutil
    shutil.make_archive(archive_path.replace(".zip", ""), 'zip', path)
    return archive_path

def publish_plugin_to_synqhub(plugin_name: str, team: str = "default"):
    path = os.path.join(LOCAL_PLUGIN_DIR, plugin_name)
    manifest = _load_plugin_manifest(path)
    archive_path = _create_zip_archive(plugin_name)

    files = {'file': open(archive_path, 'rb')}
    metadata = {
        "team": team,
        "plugin": plugin_name,
        "fingerprint": _fingerprint_plugin(archive_path)
    }

    print(f"Publishing {plugin_name} to SynQHUB as team:{team}")
    response = requests.post(f"{HUB_URL}/upload", files=files, data=metadata, verify=True)

    if response.status_code != 200:
        raise Exception(f"Failed to publish plugin: {response.text}")
    print(f"✅ Plugin '{plugin_name}' published to SynQHUB.")

def pull_plugin_from_synqhub(plugin_name: str, team: str = "default"):
    print(f"Pulling plugin {plugin_name} from team:{team}")
    response = requests.get(f"{HUB_URL}/plugin/{team}/{plugin_name}", verify=True)

    if response.status_code != 200:
        raise ValueError(f"Failed to pull plugin: {response.text}")

    save_path = os.path.join(LOCAL_PLUGIN_DIR, plugin_name + ".zip")
    with open(save_path, "wb") as f:
        f.write(response.content)

    # Extract
    import zipfile
    with zipfile.ZipFile(save_path, "r") as zip_ref:
        extract_to = os.path.join(LOCAL_PLUGIN_DIR, plugin_name)
        zip_ref.extractall(extract_to)

    print(f"✅ Pulled and extracted {plugin_name} into {extract_to}")

def sync_synqhub_metadata():
    print("🔄 Syncing plugin metadata from SynQHUB...")
    response = requests.get(f"{HUB_URL}/metadata", verify=True)
    if response.status_code != 200:
        raise Exception("Failed to sync metadata.")
    metadata = response.json()
    with open("build/synqhub_metadata.json", "w") as f:
        json.dump(metadata, f, indent=2)
    print("✅ Metadata synced.")

def _fingerprint_plugin(path: str) -> str:
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()[:16]

def validate_remote_signature(plugin_name: str):
    local_path = os.path.join(LOCAL_PLUGIN_DIR, plugin_name, MANIFEST_FILE)
    remote_path = f"{HUB_URL}/plugin/{plugin_name}/signature"

    if not os.path.exists(local_path):
        raise FileNotFoundError("Plugin manifest not found.")

    local_manifest = _load_plugin_manifest(os.path.join(LOCAL_PLUGIN_DIR, plugin_name))
    response = requests.get(remote_path, verify=True)

    if response.status_code != 200:
        raise ValueError("Failed to fetch remote signature.")

    remote_sig = response.json().get("signature")
    local_sig = hashlib.sha256(json.dumps(local_manifest).encode()).hexdigest()

    if remote_sig == local_sig:
        print("✅ Signature match. Plugin verified.")
        return True
    else:
        print("❌ Signature mismatch.")
        return False