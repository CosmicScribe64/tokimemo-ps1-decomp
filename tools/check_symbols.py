#!/usr/bin/env python3
"""Check the splat symbol files of every unit (T-9400, T-27002).

1. No overlay symbol in the shared main file: every overlay reads config/symbol_addrs.txt and shares
   VRAM with other overlays, so addresses >= 0x80132000 belong in config/symbols_<OVL>.txt.
2. With no path argument, also the checks of tools/symtab.py (SymbolTable.problems) over the symbol
   files of every yaml: one name per address and one address per name in every unit, no main name
   inside EVENT's range (0x800F6000-0x80126000) in a file EVENT loads, one name per main address
   across units, no two names that differ only in case.

Usage (Docker): python3 tools/check_symbols.py [--stamp OUT] [config/symbol_addrs.txt]
--stamp writes OUT only when it is missing, so a ninja rule with restat keeps the C objects that
depend on it from rebuilding after a symbol file edit that changes nothing here.
"""
import argparse
import os
import re
from pathlib import Path

import symtab

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
    ap.add_argument("path", nargs="?")
    ap.add_argument("--root", default=".")
    ap.add_argument("--stamp", help="write this file when the check passes (only when missing)")
    a = ap.parse_args(argv)
    path = a.path or os.path.join(a.root, symtab.SHARED_FILE)
    problems = check(Path(path).read_text(), a.path or symtab.SHARED_FILE)
    if a.path is None:
        problems += symtab.SymbolTable(a.root).problems()
    for problem in problems:
        print(problem)
    if not problems:
        print("symbols OK")
        if a.stamp and not os.path.exists(a.stamp):
            Path(a.stamp).write_text("symbols OK\n")
    return int(bool(problems))


if __name__ == "__main__":
    raise SystemExit(main())
