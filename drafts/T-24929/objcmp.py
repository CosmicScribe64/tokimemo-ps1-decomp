#!/usr/bin/env python3
"""Scratch (T-24929): list C-compiled functions whose bytes or relocations differ between two
trees of build objects. Usage: objcmp.py BASE_DIR NEW_DIR  (both mirror build/src)."""
import os
import subprocess
import sys
import re


def funcs(path):
    out = subprocess.run(["mips-linux-gnu-objdump", "-dr", "-z", path], capture_output=True,
                         text=True).stdout
    res, cur, body = {}, None, []
    for line in out.splitlines():
        m = re.match(r"^[0-9a-f]+ <([^>]+)>:$", line)
        if m:
            if cur:
                res[cur] = body
            cur, body = m.group(1), []
            continue
        if cur and line.strip():
            # drop the address column so a shift alone is not a difference... keep it: shifts matter
            line = re.sub(r"^\s*[0-9a-f]+:\s*", "", line)
            line = re.sub(r"\b[0-9a-f]+ <", "<", line)   # absolute target; <func+off> stays
            if "<" in line and "+0x" not in line.split("<", 1)[1] and "jal" in line:
                line = re.sub(r"^[0-9a-f]{8}", "jal-bytes", line)   # addend of a local call
            body.append(line)
    if cur:
        res[cur] = body
    return res


def trim(body):
    """Drop trailing padding nops (the object's last function absorbs the alignment pad)."""
    while body and body[-1].startswith("00000000"):
        body = body[:-1]
    return body


def main():
    base, new = sys.argv[1], sys.argv[2]
    changed, total = [], 0
    for root, _, files in os.walk(base):
        for f in files:
            if not f.endswith(".o"):
                continue
            b = os.path.join(root, f)
            n = os.path.join(new, os.path.relpath(b, base))
            fb = funcs(b)
            fn = funcs(n) if os.path.exists(n) else {}
            for name, body in fb.items():
                total += 1
                if trim(fn.get(name) or []) != trim(body):
                    changed.append("%s %s" % (os.path.relpath(b, base), name))
    for c in sorted(changed):
        print(c)
    print("functions %d changed %d" % (total, len(changed)), file=sys.stderr)


if __name__ == "__main__":
    main()
