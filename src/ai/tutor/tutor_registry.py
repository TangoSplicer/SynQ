# src/ai/tutor/tutor_registry.py
# Maps CLI-friendly tutor commands to handler types
# License: SynQ Commercial Attribution License v1.0

class TutorRegistry:
    def __init__(self):
        self.commands = {
            "how": "prompt",
            "why": "prompt",
            "explain": "prompt",
            "fix": "prompt",
            "scan": "generate_docs",
            "trace": "explain_trace",
            "tutorial": "generate_tutorial"
        }

    def get_action(self, command: str) -> str:
        return self.commands.get(command, "prompt")