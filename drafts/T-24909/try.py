#!/usr/bin/env python3
"""Inject a draft body into the T-24909 guard and run funcdiff --resolve.

Usage: try.py BODY [LABEL]
Prints the differing-row count and the first differing row.
"""
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SRC = ROOT / "src/ovl/ETC/80146400.c"
BEGIN = "/* T-24909 guard BEGIN */"
END = "/* T-24909 guard END */"


def inject(body_path):
    body = Path(body_path).read_text()
    if not body.endswith("\n"):
        body += "\n"
    text = SRC.read_text()
    a = text.index(BEGIN)
    b = text.index(END) + len(END)
    new = (BEGIN + "\n#ifdef NON_MATCHING\n" + body + "#else\n"
           + 'INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/80146400", func_8014790C);\n'
           + "#endif " + END)
    SRC.write_text(text[:a] + new + text[b:])


def main():
    body = sys.argv[1]
    label = sys.argv[2] if len(sys.argv) > 2 else Path(body).stem
    inject(body)
    out = subprocess.run(
        ["tools/docker.sh", "python3", "tools/funcdiff.py", "--resolve", "ETC:func_8014790C"],
        cwd=ROOT, capture_output=True, text=True).stdout
    rows = [l for l in out.splitlines() if l.startswith("  -") or l.startswith("  +")]
    dest = ROOT / "drafts/T-24909/runs" / (label + ".txt")
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_text(out)
    verdict = "MATCH" if re.search(r": MATCH\b", out) else "%d rows" % len(rows)
    first = rows[0].strip() if rows else ""
    print("%-24s %s  %s" % (label, verdict, first))


if __name__ == "__main__":
    main()
