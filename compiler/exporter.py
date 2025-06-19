# SynQ Commercial Attribution License v1.0
# © 2025 SynQ Contributors. All rights reserved.
# File: exporter.py — Multi-target exporter for SynQ compiler backend

import os
import sys
import hashlib

def export_to_synqvm(ir_code: str, output_path: str):
    # Minimal mock for .synqvm output format
    encoded = ir_code.encode('utf-8')
    header = b"SYNQVM-01\n"
    with open(output_path, "wb") as f:
        f.write(header + encoded)

def export_to_wasm(ir_code: str, output_path: str):
    # Export IR to WASM via external tool (stubbed)
    wasm_code = f"(module ;; WASM stub from SynQ\n  ;; {hashlib.sha256(ir_code.encode()).hexdigest()[:16]}\n)"
    with open(output_path, "w") as f:
        f.write(wasm_code)

def export_to_llvm(ir_code: str, output_path: str):
    # Emit LLVM-like pseudo-IR
    llvm_ir = f"; LLVM IR from SynQ\n@main = global i32 {len(ir_code)}\n"
    with open(output_path, "w") as f:
        f.write(llvm_ir)

def load_ir(file_path: str) -> str:
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"{file_path} not found.")
    with open(file_path, "r") as f:
        return f.read()

def export(file_path: str, target: str):
    ir_code = load_ir(file_path)
    name, _ = os.path.splitext(file_path)

    if target == "synqvm":
        export_to_synqvm(ir_code, f"{name}.synqvm")
        print(f"✅ Exported to {name}.synqvm")
    elif target == "wasm":
        export_to_wasm(ir_code, f"{name}.wasm")
        print(f"✅ Exported to {name}.wasm")
    elif target == "llvm":
        export_to_llvm(ir_code, f"{name}.ll")
        print(f"✅ Exported to {name}.ll")
    else:
        raise ValueError(f"❌ Unknown export target: {target}")