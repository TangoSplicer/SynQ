# src/ai/tutor/ai_tutor.py
# Main SynQ AI Tutor Engine
# License: SynQ Commercial Attribution License v1.0

from .agent_manager import AgentManager
from .tutor_registry import TutorRegistry
from .trace_explainer import explain_trace
from .doc_generator import generate_docs
from .tutorial_builder import generate_tutorial

class AITutor:
    def __init__(self):
        self.agent = AgentManager()
        self.registry = TutorRegistry()

    def handle(self, command: str, args: str = "") -> str:
        action = self.registry.get_action(command)

        if action == "prompt":
            return self.agent.query(args)
        elif action == "explain_trace":
            return explain_trace(args)
        elif action == "generate_docs":
            return generate_docs(args)
        elif action == "generate_tutorial":
            return generate_tutorial(args)
        else:
            return f"[!] Unknown tutor command: {command}"