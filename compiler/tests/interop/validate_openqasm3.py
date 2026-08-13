#!/usr/bin/env python3
"""Validate a generated OpenQASM 3 fixture with the reference Python parser."""

from pathlib import Path
import sys


def main() -> int:
    if len(sys.argv) != 2:
        print("usage: validate_openqasm3.py <generated-openqasm3-file>", file=sys.stderr)
        return 2

    fixture = Path(sys.argv[1])
    if not fixture.is_file():
        print(f"missing generated OpenQASM fixture: {fixture}", file=sys.stderr)
        return 2

    try:
        import openqasm3
    except ImportError as error:
        print("openqasm3[parser] is required for reference validation", file=sys.stderr)
        print(error, file=sys.stderr)
        return 2

    source = fixture.read_text(encoding="utf-8")
    try:
        openqasm3.parse(source)
    except Exception as error:  # The reference parser exposes implementation-specific exception types.
        print(f"reference OpenQASM 3 parser rejected {fixture}: {error}", file=sys.stderr)
        return 1

    print(f"reference OpenQASM 3 parser accepted {fixture}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
