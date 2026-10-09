"""Match PsyQ object signatures (lab313ru/psx_psyq_signatures JSON) against the .text of a PS-X EXE.

usage: psyq_sigmatch.py EXE SIGDIR [--versions 400,410,...] [--min-len N] [--out FILE]

SIGDIR holds one directory per PsyQ version with <LIB>.json files; each entry has name, sig ("?? " =
relocated byte) and labels. The signature data is third-party and gitignored (tools/psyq_sigs/).
Prints one line per match: version lib object vram length nmatches (--funcs: version lib object func vram length nmatches funcoffset objlen), and per-version byte totals.
"""
import argparse
import json
import os
import re
import sys

TEXT_FILE_OFF = 0x800
TEXT_VRAM = 0x80041000


def compile_sig(sig):
    parts = sig.split()
    pat = b"".join(b"." if p == "??" else re.escape(bytes([int(p, 16)])) for p in parts)
    return re.compile(pat, re.DOTALL), len(parts)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe")
    ap.add_argument("sigdir")
    ap.add_argument("--versions", default="")
    ap.add_argument("--min-len", type=int, default=32)
    ap.add_argument("--out", default="")
    ap.add_argument("--funcs", action="store_true", help="match per named function chunk instead of whole objects")
    a = ap.parse_args()
    data = open(a.exe, "rb").read()
    text = data[TEXT_FILE_OFF:TEXT_FILE_OFF + 0xA2800]
    vers = a.versions.split(",") if a.versions else sorted(os.listdir(a.sigdir))
    out = open(a.out, "w") if a.out else sys.stdout
    for v in vers:
        d = os.path.join(a.sigdir, v)
        if not os.path.isdir(d):
            continue
        tot = 0
        for fn in sorted(os.listdir(d)):
            if not fn.endswith(".json"):
                continue
            lib = fn[:-5]
            for o in json.load(open(os.path.join(d, fn))):
                if "sig" not in o:
                    continue
                if a.funcs:
                    parts = o["sig"].split()
                    fl = [x for x in o["labels"] if not re.match(r"(loc|text|lab|def)_", x["name"])]
                    for k, f in enumerate(fl):
                        end = fl[k + 1]["offset"] if k + 1 < len(fl) else len(parts)
                        rx, n = compile_sig(" ".join(parts[f["offset"]:end]))
                        if n < a.min_len:
                            continue
                        hits = [m.start() for m in rx.finditer(text) if m.start() % 4 == 0]
                        for h in hits:
                            out.write("%s %s %s %s 0x%08X %d %d %d %d\n" % (v, lib, o["name"], f["name"], TEXT_VRAM + h, n, len(hits), f["offset"], len(parts)))
                    continue
                rx, n = compile_sig(o["sig"])
                if n < a.min_len:
                    continue
                hits = [m.start() for m in rx.finditer(text) if m.start() % 4 == 0]
                for h in hits:
                    out.write("%s %s %s 0x%08X %d %d\n" % (v, lib, o["name"], TEXT_VRAM + h, n, len(hits)))
                if len(hits) == 1:
                    tot += n
        out.write("# %s unique-matched bytes %d\n" % (v, tot))
        out.flush()


if __name__ == "__main__":
    main()
