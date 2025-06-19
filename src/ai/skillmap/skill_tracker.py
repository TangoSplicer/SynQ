# SynQ Commercial Attribution License v1.0
# Author: SynQ Contributors — Skill Usage Tracker
# Description: Logs and analyzes plugin/model usage and skill performance

import json
import os
import time
from typing import Dict, List

TRACKER_PATH = "build/skillmap_telemetry.json"

def _load_tracker() -> Dict:
    if os.path.exists(TRACKER_PATH):
        with open(TRACKER_PATH, "r") as f:
            return json.load(f)
    return {}

def _save_tracker(data: Dict):
    with open(TRACKER_PATH, "w") as f:
        json.dump(data, f, indent=2)

def log_plugin_usage(plugin_name: str, variant: str = "default"):
    data = _load_tracker()
    entry = data.setdefault(plugin_name, {}).setdefault(variant, {
        "uses": 0,
        "last_used": 0,
        "speed_avg": 0.0,
        "accuracy_avg": 0.0,
        "hallucination_rate": 0.0
    })
    entry["uses"] += 1
    entry["last_used"] = int(time.time())
    _save_tracker(data)

def log_performance(plugin_name: str, variant: str, speed: float, accuracy: float, halluc: float):
    data = _load_tracker()
    entry = data.setdefault(plugin_name, {}).setdefault(variant, {
        "uses": 0,
        "last_used": 0,
        "speed_avg": 0.0,
        "accuracy_avg": 0.0,
        "hallucination_rate": 0.0
    })
    entry["speed_avg"] = (entry["speed_avg"] * entry["uses"] + speed) / (entry["uses"] + 1)
    entry["accuracy_avg"] = (entry["accuracy_avg"] * entry["uses"] + accuracy) / (entry["uses"] + 1)
    entry["hallucination_rate"] = (entry["hallucination_rate"] * entry["uses"] + halluc) / (entry["uses"] + 1)
    entry["uses"] += 1
    entry["last_used"] = int(time.time())
    _save_tracker(data)

def get_skillmap(plugin_name: str = None) -> Dict:
    data = _load_tracker()
    if plugin_name:
        return data.get(plugin_name, {})
    return data