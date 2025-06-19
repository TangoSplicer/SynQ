# SynQ Commercial Attribution License v1.0
# © 2025 SynQ Contributors. All rights reserved.
# File: execution_graph.py — Distributed Execution Chain (DEC) engine

import json
import os
from typing import List, Dict
from collections import defaultdict

class NodeResult:
    def __init__(self, node_id, success, output, error=None):
        self.node_id = node_id
        self.success = success
        self.output = output
        self.error = error

class ExecutionNode:
    def __init__(self, node_id, command, deps):
        self.id = node_id
        self.command = command
        self.deps = deps

class ExecutionGraph:
    def __init__(self, job_file: str):
        self.nodes: Dict[str, ExecutionNode] = {}
        self.results: Dict[str, NodeResult] = {}
        self.load_graph(job_file)

    def load_graph(self, path: str):
        with open(path, "r") as f:
            if path.endswith(".json"):
                data = json.load(f)
            else:
                import yaml
                data = yaml.safe_load(f)

        for task in data.get("tasks", []):
            node = ExecutionNode(
                node_id=task["id"],
                command=task["run"],
                deps=task.get("depends_on", [])
            )
            self.nodes[node.id] = node

    def run(self):
        executed = set()

        def can_run(node: ExecutionNode):
            return all(dep in self.results and self.results[dep].success for dep in node.deps)

        while len(executed) < len(self.nodes):
            progress = False
            for node in self.nodes.values():
                if node.id in executed:
                    continue
                if can_run(node):
                    print(f"▶️ Executing {node.id}: {node.command}")
                    try:
                        output = os.popen(node.command).read()
                        self.results[node.id] = NodeResult(node.id, True, output)
                        print(f"✅ {node.id} completed.")
                    except Exception as e:
                        self.results[node.id] = NodeResult(node.id, False, "", str(e))
                        print(f"❌ {node.id} failed: {str(e)}")
                    executed.add(node.id)
                    progress = True
            if not progress:
                print("🛑 Circular dependency or unresolved failure detected.")
                break

    def summary(self):
        print("\n📦 Execution Summary:")
        for node_id, result in self.results.items():
            status = "✅ SUCCESS" if result.success else "❌ FAIL"
            print(f"{node_id}: {status}")

def run_chain_file(path: str):
    graph = ExecutionGraph(path)
    graph.run()
    graph.summary()