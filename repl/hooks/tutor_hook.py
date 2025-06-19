# src/repl/hooks/tutor_hook.py
# Hook for integrating AI tutor in REPL runtime
# License: SynQ Commercial Attribution License v1.0

from src.ai.tutor.ai_tutor import AITutor

def tutor_hook(input_line: str) -> str:
    if input_line.startswith("tutor "):
        tutor = AITutor()
        args = input_line[len("tutor "):]
        return tutor.handle("how", args)
    return ""