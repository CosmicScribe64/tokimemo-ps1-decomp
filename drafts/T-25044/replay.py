#!/usr/bin/env python3
"""Replay source-only parameter_up_down controls in Docker; report diagnostic rows.

Literal linked/default checksum proof is separate (README.md). This script never
modifies production sources, headers, build rules, or immutable target inputs.
"""
import argparse
from pathlib import Path
import subprocess
import sys

sys.path.insert(0, "tools")
import funcdiff


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("control", choices=("current", "no-byte-use", "four-row", "padded"))
    args = parser.parse_args()
    source = Path("src/main/800737A0.c").read_text()
    start = source.index("s32 parameter_up_down(")
    end = source.index("\nu8 vacation_day_init", start)
    body = source[start:end]
    if args.control in ("four-row", "padded"):
        body = Path("drafts/T-25044/four-row.c").read_text()
    if args.control == "padded":
        body = body.replace("    u32 h;", "    u32 h;\n    s32 pad;")
    if args.control == "no-byte-use":
        body = body.replace("    if (D_800E6280.unk_0F4.b[1]) {}\n", "")
    output = Path("build/T-25044/replay") / args.control
    output.mkdir(parents=True, exist_ok=True)
    src = output / "full.c"
    obj = output / "full.o"
    src.write_text(source[:start] + body + source[end:])
    subprocess.run([sys.executable, "tools/cc.py", str(src), str(obj), "ido", "5.3"], check=True)
    unit = funcdiff.locate("parameter_up_down", "main")
    expected, error = funcdiff.expected_object(unit)
    if error:
        raise RuntimeError(error)
    verdict = funcdiff.resolved_verdict(str(obj), expected, "parameter_up_down", unit)
    if verdict is None:
        raise RuntimeError("resolved comparison unavailable")
    rows = verdict[1] or []
    (output / "diagnostic.txt").write_text("\n".join(rows) + "\n")
    print("%s: %s, %d diagnostic diff lines (not linked proof)" % (args.control, verdict[0], len(rows)))


if __name__ == "__main__":
    main()
