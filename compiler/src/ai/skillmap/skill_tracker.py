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