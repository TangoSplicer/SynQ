# SynQ Commercial Attribution License v1.0
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

def _load_plugin_manifest(plugin_path: str) -> Dict:
    manifest_path = os.path.join(plugin_path, MANIFEST_FILE)
    if not os.path.exists(manifest_path):
        raise FileNotFoundError(f"Missing manifest at {manifest_path}")
    with open(manifest_path, "r") as f:
        return json.load(f)

def publish_plugin_to_synqhub(plugin_name: str, team: str = "default"):
    path = os.path.join(LOCAL_PLUGIN_DIR, plugin_name)
    manifest = _load_plugin_manifest(path)
    archive_path = os.path.join(path, f"{plugin_name}.zip")

    # Zip plugin folder
    import shutil
    shutil.make_archive(archive_path.replace(".zip", ""), 'zip', path)

    files = {'file': open(archive_path, 'rb')}
    metadata = {
        "team": team,
        "plugin": plugin_name,
        "fingerprint": _fingerprint_plugin(archive_path)
    }

    print(f"Publishing {plugin_name} to SynQHUB as team:{team}")
    response = requests.post(f"{HUB_URL}/upload", files=files, data=metadata)

    if response.status_code == 200:
        print(f"✅ Plugin '{plugin_name}' published to SynQHUB.")
    else:
        print(f"❌ Failed to publish plugin: {response.text}")

def pull_plugin_from_synqhub(plugin_name: str, team: str = "default"):
    print(f"Pulling plugin {plugin_name} from team:{team}")
    response = requests.get(f"{HUB_URL}/plugin/{team}/{plugin_name}")

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
    response = requests.get(f"{HUB_URL}/metadata")
    if response.status_code == 200:
        metadata = response.json()
        with open("build/synqhub_metadata.json", "w") as f:
            json.dump(metadata, f, indent=2)
        print("✅ Metadata synced.")
    else:
        print("❌ Failed to sync metadata.")

def _fingerprint_plugin(path: str) -> str:
    with open(path, "rb") as f:
        return hashlib.sha256(f.read()).hexdigest()[:16]

def validate_remote_signature(plugin_name: str):
    local_path = os.path.join(LOCAL_PLUGIN_DIR, plugin_name, MANIFEST_FILE)
    remote_path = f"{HUB_URL}/plugin/{plugin_name}/signature"

    if not os.path.exists(local_path):
        raise FileNotFoundError("Plugin manifest not found.")

    local_manifest = _load_plugin_manifest(os.path.join(LOCAL_PLUGIN_DIR, plugin_name))
    response = requests.get(remote_path)

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