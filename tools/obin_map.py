#!/usr/bin/env python3
"""Map O.BIN symbol names onto SLPM_86.053 and write reviewable generated files.

O.BIN carries no code for the main program (only its symbol names and addresses),
so the match is structural: the symbols of both sides are sorted by address,
each O.BIN symbol gets a size (gap to the next symbol), and the two size
sequences are aligned (Needleman-Wunsch). Hard anchors are the PsyQ names
already recovered by signature matching (config/symbol_addrs_sdk.txt) that O.BIN
also names; they split the address space into monotonic segments. An aligned pair
is "high" confidence only if its size is identical and it sits in a run of at
least RUN_HIGH consecutive identical-size pairs with at least 3 distinct sizes.
See wiki/obin.md for the method, the calibration on the PsyQ names and the stats.

Usage: python3 tools/obin_map.py [--obin PATH] [--elf build/SLPM_86.053.elf]
           [--sdk config/symbol_addrs_sdk.txt] [--write]
Without --write it prints the statistics only. With --write it (re)writes
config/symbol_addrs_obin.txt (splat format, high confidence, main exe only) and
config/obin_renames.txt (old new, same set). The ELF comes from a normal build
(`ninja`); it is only used for function addresses and sizes.
"""
import argparse
import bisect
import collections
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import obin_syms  # noqa: E402

RUN_HIGH = 4
RUN_MED = 2
CODE_LIMIT = 0x800C6000   # O.BIN: below this are code symbols, above data/bss
MAIN_TEXT_END = 0x800B3220
OVL_BASE = 0x80132000


def read_elf_syms(path):
    """Return list of (addr, size, name, is_code) for named symbols of an ELF32 LE file."""
    d = open(path, "rb").read()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    secs = [struct.unpack_from("<10I", d, shoff + shentsize * i) for i in range(shnum)]
    out = []
    for s in secs:
        if s[1] != 2:   # SHT_SYMTAB
            continue
        strtab = secs[s[6]]
        for k in range(s[5] // 16):
            nm, val, size, info, other, shndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * k)
            if shndx == 0 or shndx >= 0xFF00:
                continue
            e = d.index(b"\0", strtab[4] + nm)
            name = d[strtab[4] + nm:e].decode("ascii")
            if not name or "." in name or name.startswith("_MACRO"):
                continue
            code = bool(secs[shndx][2] & 4)   # SHF_EXECINSTR
            out.append((val, size, name, code))
    return out


def read_sdk(path):
    r = {}
    for line in open(path):
        if line.startswith("//") or "=" not in line:
            continue
        n, a = line.split("=", 1)
        r[n.strip()] = int(a.split(";")[0].strip(), 16)
    return r


def lis_chain(pairs):
    """Longest chain of (i, j) strictly increasing in both coordinates."""
    pairs = sorted(pairs)
    tails, prev = [], {}
    for k, (_, j) in enumerate(pairs):
        p = bisect.bisect_left([pairs[t][1] for t in tails], j)
        if p > 0:
            prev[k] = tails[p - 1]
        if p == len(tails):
            tails.append(k)
        else:
            tails[p] = k
    k, chain = tails[-1], []
    while True:
        chain.append(pairs[k])
        if k not in prev:
            break
        k = prev[k]
    return chain[::-1]


def align(osz, fsz):
    """Needleman-Wunsch over two size lists; returns [(oi, fj)]."""
    n, m = len(osz), len(fsz)
    S = [[0] * (m + 1) for _ in range(n + 1)]
    T = [[0] * (m + 1) for _ in range(n + 1)]
    for i in range(1, n + 1):
        S[i][0], T[i][0] = -i, 1
    for j in range(1, m + 1):
        S[0][j], T[0][j] = -j, 2
    for i in range(1, n + 1):
        a = osz[i - 1]
        for j in range(1, m + 1):
            b = fsz[j - 1]
            if a == b:
                sc = 3
            else:
                sc = 0.3 if b and 0.5 <= a / b <= 2 else -0.7
            best, t = S[i - 1][j - 1] + sc, 0
            if S[i - 1][j] - 1 > best:
                best, t = S[i - 1][j] - 1, 1
            if S[i][j - 1] - 1 > best:
                best, t = S[i][j - 1] - 1, 2
            S[i][j], T[i][j] = best, t
    i, j, out = n, m, []
    while i > 0 or j > 0:
        t = T[i][j]
        if i > 0 and j > 0 and t == 0:
            out.append((i - 1, j - 1))
            i, j = i - 1, j - 1
        elif i > 0 and (j == 0 or t == 1):
            i -= 1
        else:
            j -= 1
    return out[::-1]


def map_region(O, F, anchors):
    """O: [(addr,name,size)], F: [(addr,size,name)] sorted. anchors: [(oi,fj)]. Returns {oi: fj}."""
    res = {}
    bounds = [(-1, -1)] + anchors + [(len(O), len(F))]
    for (i0, j0), (i1, j1) in zip(bounds, bounds[1:]):
        oi = list(range(i0 + 1, i1))
        fj = list(range(j0 + 1, j1))
        if oi and fj:
            for x, y in align([O[i][2] for i in oi], [F[j][1] for j in fj]):
                res[oi[x]] = fj[y]
    for i, j in anchors:
        res[i] = j
    return res


def classify(res, O, F):
    """Return {oi: 'high'|'med'|'low'}; run = consecutive same-diagonal identical-size pairs."""
    ids = sorted(res)
    def same(p, q):
        i, j = ids[p], res[ids[p]]
        k, l = ids[q], res[ids[q]]
        return O[k][2] == F[l][1] and l - j == k - i and k - i == q - p
    out = {}
    for p, i in enumerate(ids):
        j = res[i]
        if O[i][2] != F[j][1]:
            out[i] = "low"
            continue
        lo = hi = p
        while lo > 0 and O[ids[lo - 1]][2] == F[res[ids[lo - 1]]][1] and same(lo - 1, lo):
            lo -= 1
        while hi + 1 < len(ids) and O[ids[hi + 1]][2] == F[res[ids[hi + 1]]][1] and same(hi, hi + 1):
            hi += 1
        run = hi - lo + 1
        sizes = {O[ids[x]][2] for x in range(lo, hi + 1)}
        if run >= RUN_HIGH and len(sizes) >= 3:
            out[i] = "high"
        elif run >= RUN_MED:
            out[i] = "med"
        else:
            out[i] = "low"
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--obin", default=obin_syms.DEFAULT)
    ap.add_argument("--elf", default="build/SLPM_86.053.elf")
    ap.add_argument("--sdk", default="config/symbol_addrs_sdk.txt")
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--out-syms", default="config/symbol_addrs_obin.txt")
    ap.add_argument("--out-renames", default="config/obin_renames.txt")
    a = ap.parse_args()
    try:
        fh, ah, secs, syms = obin_syms.parse(open(a.obin, "rb").read())
        elf = read_elf_syms(a.elf)
    except (OSError, ValueError, struct.error) as e:
        print("obin_map: %s" % e, file=sys.stderr)
        return 1
    sdk = read_sdk(a.sdk)

    # O.BIN side: code symbols (sized by gap) vs main-exe functions.
    O = [(s["addr"], s["name"], s["size"] or 0) for s in syms
         if s["sc"] != 2 and s["addr"] < CODE_LIMIT]
    funcs = {}
    for val, size, name, code in elf:
        if code and size and val < MAIN_TEXT_END:
            funcs.setdefault(val, (size, name))
    F = [(v, s, n) for v, (s, n) in sorted(funcs.items())]
    oidx = {n: i for i, (_, n, _) in enumerate(O)}
    fidx = {v: j for j, (v, _, _) in enumerate(F)}
    anch_all = [(oidx[n], fidx[ad]) for n, ad in sdk.items() if n in oidx and ad in fidx]
    chain = lis_chain(anch_all)
    res = map_region(O, F, chain)
    cls = classify(res, O, F)
    for i, j in anch_all:       # PsyQ-named pairs are exact by construction
        res[i] = j
        cls[i] = "sdk"
    inv = [(O[i][1], hex(O[i][0]), hex(F[j][0])) for i, j in anch_all if (i, j) not in set(chain)]

    # Data/bss side: same alignment against the splat data symbols (no PsyQ anchors).
    OD = [(s["addr"], s["name"], s["size"] or 0) for s in syms
          if CODE_LIMIT <= s["addr"] < OVL_BASE]
    dsyms = {}
    for val, size, name, code in elf:
        if not code and MAIN_TEXT_END <= val < 0x8012C000 and name.startswith(("D_", "jtbl_")):
            dsyms.setdefault(val, name)
    da = sorted(dsyms)
    FD = [(v, (da[k + 1] - v if k + 1 < len(da) else 0), dsyms[v]) for k, v in enumerate(da)]
    dres = map_region(OD, FD, [])
    dcls = classify(dres, OD, FD)

    stats = collections.Counter(cls.values())
    dstats = collections.Counter(dcls.values())
    nsyms = len(syms)
    novl = sum(1 for s in syms if s["addr"] >= OVL_BASE)
    print("O.BIN symbols %d: main code-side %d (aligned %d), main data/bss-side %d (aligned %d), overlay-side %d" %
          (nsyms, len(O), len(res), len(OD), len(dres), novl))
    print("main-exe functions %d; PsyQ anchors %d (chain %d, order inversions %d)" %
          (len(F), len(anch_all), len(chain), len(inv)))
    print("code confidence:", dict(stats), "| data confidence:", dict(dstats))
    print("same address and size:", sum(1 for i in res if O[i][0] == F[res[i]][0] and O[i][2] == F[res[i]][1]))

    if a.write:
        taken = {n for _, _, n in F} | {n for _, _, n, _ in elf}
        seen, lines, rens = set(), [], []
        for tag, R, C, OO, FF, kind in (("code", res, cls, O, F, "type:func"),
                                        ("data", dres, dcls, OD, FD, "")):
            lines.append("// ---- %s (high confidence) ----" % tag)
            for i in sorted(R):
                if C[i] != "high":
                    continue
                name = OO[i][1]
                faddr, fsize, fname = FF[R[i]]
                if not fname.startswith(("func_", "D_", "jtbl_")) or name in seen or name in taken:
                    continue
                seen.add(name)
                extra = (kind + " " if kind else "") + ("size:0x%X" % fsize if fsize else "")
                lines.append("%s = 0x%08X; // %s" % (name, faddr, extra.strip()))
                rens.append("%s %s" % (fname, name))
        with open(a.out_syms, "w") as f:
            f.write("// Generated by tools/obin_map.py (T-0201); do not edit by hand.\n"
                    "// Names from the O.BIN developer-build symbol table mapped onto SLPM_86.053 by\n"
                    "// size-sequence alignment, high confidence only (identical size in a run of >= %d\n"
                    "// identical-size neighbours). Method and calibration: wiki/obin.md. Not yet applied.\n" % RUN_HIGH)
            f.write("\n".join(lines) + "\n")
        with open(a.out_renames, "w") as f:
            f.write("# old new  (generated by tools/obin_map.py; apply through the splat symbol files)\n")
            f.write("\n".join(rens) + "\n")
        print("wrote %d symbols, %d renames" % (len(rens), len(rens)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
