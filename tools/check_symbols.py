#!/usr/bin/env python3
"""Reject overlay symbols in the shared main symbol file (T-9400).

Every overlay reads config/symbol_addrs.txt and shares VRAM with other overlays.
Addresses >= 0x80132000 belong in config/symbols_<OVL>.txt instead.
Usage (Docker): python3 tools/check_symbols.py [config/symbol_addrs.txt]
"""
import argparse
import re
from pathlib import Path

OVERLAY_VRAM = 0x80132000


def check(text, path="config/symbol_addrs.txt"):
    """Diagnostics for active assignments at overlay addresses, including renamed symbols."""
    text = re.sub(r"/\*.*?\*/", lambda m: "\n" * m.group().count("\n"), text, flags=re.S)
    problems = []
    for num, line in enumerate(text.splitlines(), 1):
        line = line.split("//", 1)[0].split("#", 1)[0]
        m = re.match(r"^\s*(\w+)\s*=\s*(0[xX][0-9a-fA-F]+|[0-9]+)\s*;", line)
        if m and int(m.group(2), 16 if m.group(2).lower().startswith("0x") else 10) >= OVERLAY_VRAM:
            problems.append("%s:%d: %s = %s is an overlay symbol; move it to config/symbols_<OVL>.txt"
                            % (path, num, m.group(1), m.group(2)))
    return problems


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("path", nargs="?", default="config/symbol_addrs.txt")
    a = ap.parse_args(argv)
    problems = check(Path(a.path).read_text(), a.path)
    for problem in problems:
        print(problem)
    if not problems:
        print("symbols OK")
    return int(bool(problems))


if __name__ == "__main__":
    raise SystemExit(main())
