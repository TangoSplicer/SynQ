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

# src/ai/tutor/ai_tutor.py
# Main SynQ AI Tutor Engine
# License: 

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