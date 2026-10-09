"""Match PsyQ object signatures (lab313ru/psx_psyq_signatures JSON) against the .text of a PS-X EXE.

usage: psyq_sigmatch.py EXE SIGDIR [--versions 400,410,...] [--min-len N] [--funcs] [--out FILE]

SIGDIR holds one directory per PsyQ version with <LIB>.json files; each entry has name, sig ("?? " =
relocated byte) and labels. The signature data is third-party and gitignored (tools/psyq_sigs/).
Prints one line per match: version lib object vram length nmatches (--funcs: version lib object func vram length nmatches funcoffset objlen), and per-version byte totals.
"""
import argparse
import os
import re
import sys

from psyqsig import TEXT_VRAM, load_text, objects, versions


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
    text = load_text(a.exe)
    vers = versions(a.sigdir, a.versions)
    out = open(a.out, "w") if a.out else sys.stdout
    nlines = 0
    for v in vers:
        tot = 0
        for lib, o in objects(os.path.join(a.sigdir, v)):
            parts = o["sig"].split()
            if a.funcs:
                fl = [x for x in o["labels"] if not re.match(r"(loc|text|lab|def)_", x["name"])]
                chunks = []
                for k, f in enumerate(fl):
                    end = fl[k + 1]["offset"] if k + 1 < len(fl) else len(parts)
                    chunks.append((f["name"], f["offset"], parts[f["offset"]:end]))
            else:
                chunks = [(None, 0, parts)]
            for fname, off, cp in chunks:
                rx, n = compile_sig(" ".join(cp))
                if n < a.min_len:
                    continue
                hits = [m.start() for m in rx.finditer(text) if m.start() % 4 == 0]
                for h in hits:
                    if a.funcs:
                        out.write("%s %s %s %s 0x%08X %d %d %d %d\n" % (v, lib, o["name"], fname, TEXT_VRAM + h, n, len(hits), off, len(parts)))
                    else:
                        out.write("%s %s %s 0x%08X %d %d\n" % (v, lib, o["name"], TEXT_VRAM + h, n, len(hits)))
                    nlines += 1
                if len(hits) == 1 and not a.funcs:
                    tot += n
        out.write("# %s unique-matched bytes %d\n" % (v, tot))
        out.flush()
    if nlines == 0:
        sys.exit("psyq_sigmatch: no matches")


if __name__ == "__main__":
    main()
