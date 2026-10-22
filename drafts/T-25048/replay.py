#!/usr/bin/env python3
"""Replay func_80076FDC source controls in Docker without editing production C.

Output is a resolved diagnostic comparison. Use the literal default-C and MAIN
checksum commands in README.md for definitive verification, including tables.
"""
import argparse
from pathlib import Path
import subprocess
import sys

sys.path.insert(0, "tools")
import funcdiff


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("control", choices=("current", "reversed-locals", "empty-use", "local-background", "reused-background"))
    args = parser.parse_args()
    source = Path("src/main/80075320.c").read_text()
    start = source.index("void func_80076FDC(")
    end = source.index("\ns32 func_80077330", start)
    body = source[start:end]
    if args.control in ("local-background", "reused-background"):
        body = Path("drafts/T-25048/%s.c" % args.control).read_text()
    elif args.control == "reversed-locals":
        body = body.replace("    s32 activity;\n    s32 col;", "    s32 col;\n    s32 activity;")
    elif args.control == "empty-use":
        body = body.replace("    D_801230D0 = 0xFF;", "    D_801230D0 = 0xFF;\n    if (D_801230D0) {}", 1)
    output = Path("build/T-25048/replay") / args.control
    output.mkdir(parents=True, exist_ok=True)
    src = output / "full.c"
    obj = output / "full.o"
    src.write_text(source[:start] + body + source[end:])
    subprocess.run([sys.executable, "tools/cc.py", str(src), str(obj), "ido", "5.3"], check=True)
    unit = funcdiff.locate("func_80076FDC", "main")
    expected, error = funcdiff.expected_object(unit)
    if error:
        raise RuntimeError(error)
    verdict = funcdiff.resolved_verdict(str(obj), expected, "func_80076FDC", unit)
    if verdict is None:
        raise RuntimeError("resolved comparison unavailable")
    rows = verdict[1] or []
    (output / "diagnostic.txt").write_text("\n".join(rows) + "\n")
    print("%s: %s, %d diagnostic diff lines (not linked proof)" % (args.control, verdict[0], len(rows)))


if __name__ == "__main__":
    main()
