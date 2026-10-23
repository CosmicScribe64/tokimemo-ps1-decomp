#!/usr/bin/env python3
"""T-25151 scratch harness: install a func_801412A4 body variant under the NON_MATCHING guard and run funcdiff.

Usage: python3 drafts/T-25151/GYOZI/func_801412A4/try.py <variant.c> [--keep] [--full]
The variant file holds the whole C definition of func_801412A4 (may include file-scope lines before it).
"""
import subprocess
import sys
from pathlib import Path

SRC = Path("src/ovl/GYOZI/80140C90.c")
INC = 'INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90", func_801412A4);\n'
START = "#ifdef NON_MATCHING /* T-25151 */\n"
END = "#else\n" + INC + "#endif\n"


def install(var):
    cur = SRC.read_text()
    if START in cur:
        i = cur.index(START)
        j = cur.index(END, i) + len(END)
    else:
        i = cur.index(INC)
        j = i + len(INC)
    body = var if var.endswith("\n") else var + "\n"
    SRC.write_text(cur[:i] + START + body + END + cur[j:])


def main():
    install(Path(sys.argv[1]).read_text())
    if "--keep" in sys.argv:
        return 0
    out = subprocess.run(
        ["tools/docker.sh", "python3", "tools/funcdiff.py", "func_801412A4", "--unit", "GYOZI", "--resolve", "--version", "c"],
        capture_output=True, text=True,
    )
    txt = out.stdout + out.stderr
    if "--full" in sys.argv:
        print(txt[-12000:])
    else:
        lines = txt.splitlines()
        rows = [l for l in lines if l.lstrip().startswith(("-@row", "+@row", "-", "+")) and l.startswith("  ")]
        keep = [l for l in lines if "MATCH" in l or "DIFF" in l or "compared" in l or "rror" in l]
        print("\n".join(keep[:8]))
        print("rows:", len(rows))
    return 0


if __name__ == "__main__":
    sys.exit(main())
