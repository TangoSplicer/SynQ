# src/ai/tutor/trace_explainer.py
# Trace analysis with AI annotation
# License: SynQ Commercial Attribution License v1.0

def explain_trace(trace_file: str) -> str:
    try:
        with open(trace_file, "r") as f:
            lines = f.readlines()
        summary = "[Trace Analysis]\n"
        for idx, line in enumerate(lines):
            summary += f"{idx+1:02d}: {line.strip()}  <-- [ok]\n"
        return summary
    except Exception as e:
        return f"[✗] Failed to load trace: {e}"