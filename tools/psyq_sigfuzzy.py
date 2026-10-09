"""Fuzzy-match PsyQ object signatures against a PS-X EXE text section.

usage: psyq_sigfuzzy.py EXE SIGDIR OUT [--versions a,b] [--min-score 0.8]

For each object the longest literal runs of the signature anchor candidate positions; the candidate's
score is the fraction of literal signature bytes that agree. Writes "version lib obj vram len score".
Tool for T-0010; signature data is third-party and gitignored (tools/psyq_sigs/).
"""
import argparse
import os
import sys

from psyqsig import TEXT_VRAM, load_text, objects, versions


def runs(parts):
    out = []
    i = 0
    n = len(parts)
    while i < n:
        if parts[i] == "??":
            i += 1
            continue
        j = i
        while j < n and parts[j] != "??":
            j += 1
        out.append((i, j))
        i = j
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("exe")
    ap.add_argument("sigdir")
    ap.add_argument("out")
    ap.add_argument("--versions", default="")
    ap.add_argument("--min-score", type=float, default=0.8)
    a = ap.parse_args()
    text = load_text(a.exe)
    vers = versions(a.sigdir, a.versions)
    nlines = 0
    with open(a.out, "w") as out:
        for v in vers:
            for lib, o in objects(os.path.join(a.sigdir, v)):
                parts = o["sig"].split()
                n = len(parts)
                if n < 16:
                    continue
                lit = [(i, bytes([int(p, 16)])) for i, p in enumerate(parts) if p != "??"]
                rs = sorted(runs(parts), key=lambda r: r[0] - r[1])[:3]
                cands = set()
                for (s, e) in rs:
                    if e - s < 8:
                        continue
                    pat = bytes(int(p, 16) for p in parts[s:e])
                    k = text.find(pat)
                    while k != -1:
                        if (k - s) % 4 == 0 and k - s >= 0:
                            cands.add(k - s)
                        k = text.find(pat, k + 1)
                best = (0, 0)
                for c in cands:
                    if c + n > len(text):
                        continue
                    ok = sum(1 for i, b in lit if text[c + i:c + i + 1] == b)
                    sc = ok / len(lit)
                    if sc > best[0]:
                        best = (sc, c)
                if best[0] >= a.min_score:
                    out.write("%s %s %s 0x%08X %d %.3f\n" % (v, lib + ".LIB", o["name"], TEXT_VRAM + best[1], n, best[0]))
                    nlines += 1
    if nlines == 0:
        sys.exit("psyq_sigfuzzy: no matches")


if __name__ == "__main__":
    main()
