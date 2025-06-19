# src/ai/tutor/agent_manager.py
# Agent Backend Switching + Prompt Routing
# License: SynQ Commercial Attribution License v1.0

class AgentManager:
    def __init__(self):
        self.provider = "openai"  # Default, can be dynamic later

    def query(self, prompt: str) -> str:
        if self.provider == "openai":
            return self._query_openai(prompt)
        elif self.provider == "deepseek":
            return self._query_deepseek(prompt)
        else:
            return "[✗] Unknown provider"

    def _query_openai(self, prompt: str) -> str:
        # Simulated response
        return f"[OpenAI GPT-4]: {prompt[:50]}..."

    def _query_deepseek(self, prompt: str) -> str:
        return f"[DeepSeek Chat]: {prompt[:50]}..."