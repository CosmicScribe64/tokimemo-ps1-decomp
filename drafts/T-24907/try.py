#!/usr/bin/env python3
"""T-24907 scratch harness: install a func_80140E0C body variant, run funcdiff, print the diff.

Usage: python3 drafts/T-24907/try.py <variant-file> [--keep]
The variant file holds the full body of func_80140E0C (the lines between `#ifdef NON_MATCHING`
and the matching `#else`). Nothing else in the file is touched.
"""
import subprocess
import sys
from pathlib import Path

SRC = Path("src/ovl/GYOZI/80140C90.c")
START = "#ifdef NON_MATCHING\n"
END = "#else\nINCLUDE_ASM(\"asm/ovl/GYOZI/nonmatchings/GYOZI/80140C90\", func_80140E0C);\n#endif\n"


def main():
    var = Path(sys.argv[1]).read_text()
    cur = SRC.read_text()
    i = cur.index(START)
    j = cur.index(END, i)
    new = cur[: i + len(START)] + (var if var.endswith("\n") else var + "\n") + cur[j:]
    SRC.write_text(new)
    if "--keep" in sys.argv:
        return 0
    out = subprocess.run(
        ["tools/docker.sh", "python3", "tools/funcdiff.py", "--resolve", "GYOZI:func_80140E0C"],
        capture_output=True, text=True,
    )
    lines = [l for l in out.stdout.splitlines() if "DIFF" in l or "MATCH" in l or l.startswith(("  -", "  +"))]
    print("\n".join(lines) if lines else out.stdout[-1500:] + out.stderr[-1000:])
    return 0


if __name__ == "__main__":
    sys.exit(main())
