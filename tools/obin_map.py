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

Usage: python3 tools/obin_map.py [--write]
Without --write it prints the statistics only. With --write it (re)writes
config/symbol_addrs_obin.txt (splat format, high confidence, main exe only) and
config/obin_renames.txt (old new, same set). Paths are the defaults below. The ELF comes from a normal build
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
MAIN_BSS_LIMIT = 0x8012C000   # splat data symbols end here (main bss ends at 0x8012B538)
OVL_BASE = 0x80132000

ELF = "build/SLPM_86.053.elf"
SDK = "config/symbol_addrs_sdk.txt"
OUT_SYMS = "config/symbol_addrs_obin.txt"
OUT_RENAMES = "config/obin_renames.txt"
Entry = collections.namedtuple("Entry", "addr size name")


def read_elf_syms(path):
    """Return [(addr, size, name, is_code)] for the named symbols of an ELF32 LE file."""
    with open(path, "rb") as f:
        d = f.read()
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", d, 0x2E)
    secs = [struct.unpack_from("<10I", d, shoff + shentsize * i) for i in range(shnum)]
    out = []
    for s in secs:
        if s[1] != 2:   # SHT_SYMTAB
            continue
        strtab = secs[s[6]]
        for k in range(s[5] // 16):
            nm, val, size, _, _, shndx = struct.unpack_from("<IIIBBH", d, s[4] + 16 * k)
            if shndx == 0 or shndx >= 0xFF00:
                continue
            e = d.index(b"\0", strtab[4] + nm)
            name = d[strtab[4] + nm:e].decode("ascii")
            if name and "." not in name and not name.startswith("_MACRO"):
                out.append((val, size, name, bool(secs[shndx][2] & 4)))   # SHF_EXECINSTR
    return out


def read_sdk(path):
    r = {}
    with open(path) as f:
        for line in f:
            if not line.startswith("//") and "=" in line:
                n, a = line.split("=", 1)
                r[n.strip()] = int(a.split(";")[0].strip(), 16)
    return r


def gap_sizes(pairs):
    """[(addr, name)] sorted by address -> [Entry] with size = gap to the next address (0 if last)."""
    return [Entry(a, (pairs[k + 1][0] - a) if k + 1 < len(pairs) else 0, n)
            for k, (a, n) in enumerate(pairs)]


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
    """O: [Entry], F: [Entry] sorted. anchors: [(oi,fj)]. Returns {oi: fj}."""
    res = {}
    bounds = [(-1, -1)] + anchors + [(len(O), len(F))]
    for (i0, j0), (i1, j1) in zip(bounds, bounds[1:]):
        oi = list(range(i0 + 1, i1))
        fj = list(range(j0 + 1, j1))
        if oi and fj:
            for x, y in align([O[i].size for i in oi], [F[j].size for j in fj]):
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
        return O[k].size == F[l].size and l - j == k - i and k - i == q - p
    out = {}
    for p, i in enumerate(ids):
        j = res[i]
        if O[i].size != F[j].size:
            out[i] = "low"
            continue
        lo = hi = p
        while lo > 0 and O[ids[lo - 1]].size == F[res[ids[lo - 1]]].size and same(lo - 1, lo):
            lo -= 1
        while hi + 1 < len(ids) and O[ids[hi + 1]].size == F[res[ids[hi + 1]]].size and same(hi, hi + 1):
            hi += 1
        run = hi - lo + 1
        sizes = {O[ids[x]].size for x in range(lo, hi + 1)}
        if run >= RUN_HIGH and len(sizes) >= 3:
            out[i] = "high"
        elif run >= RUN_MED:
            out[i] = "med"
        else:
            out[i] = "low"
    return out


def write_outputs(syms_path, rens_path, parts, taken):
    """parts: [(tag, res, cls, O, F, type_hint)]; writes only high pairs whose target is a placeholder."""
    seen, lines, rens = set(), [], []
    for tag, R, C, OO, FF, hint in parts:
        lines.append("// ---- %s ----" % tag)
        for i in sorted(R):
            name, target = OO[i].name, FF[R[i]]
            if C[i] != "high" or not target.name.startswith(("func_", "D_", "jtbl_")):
                continue
            if name in seen or name in taken:
                continue
            seen.add(name)
            extra = " ".join(x for x in (hint, "size:0x%X" % target.size if target.size else "") if x)
            lines.append("%s = 0x%08X; // %s" % (name, target.addr, extra))
            rens.append("%s %s" % (target.name, name))
    with open(syms_path, "w") as f:
        f.write("// Generated once by tools/obin_map.py (T-0201), now maintained by hand (T-0600).\n"
                "// HYPOTHESES, not facts: names from the O.BIN developer-build symbol table, aligned to\n"
                "// SLPM_86.053 by function size (identical size in a run of >= %d identical-size\n"
                "// neighbours). Calibration and spot checks: wiki/obin.md. Applied to the build by T-0600;\n"
                "// the renames are done, so --write on an applied tree yields an empty list: edit this file by hand now.\n" % RUN_HIGH)
        f.write("\n".join(lines) + "\n")
    with open(rens_path, "w") as f:
        f.write("# old new  (generated by tools/obin_map.py; hypotheses, apply through the splat symbol files)\n")
        f.write("\n".join(rens) + "\n")
    return len(rens)


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--write", action="store_true")
    a = ap.parse_args()
    try:
        with open(obin_syms.DEFAULT, "rb") as f:
            _, syms = obin_syms.parse(f.read())
        elf = read_elf_syms(ELF)
        sdk = read_sdk(SDK)
    except (OSError, ValueError, struct.error) as e:
        print("obin_map: %s" % e, file=sys.stderr)
        return 1

    # O.BIN code symbols (sized by gap) vs main-exe functions.
    O = gap_sizes([(s.addr, s.name) for s in syms if s.sc != 2 and s.addr < CODE_LIMIT])
    funcs = {}
    for val, size, name, code in elf:
        if code and size and val < MAIN_TEXT_END:
            funcs.setdefault(val, Entry(val, size, name))
    F = [funcs[v] for v in sorted(funcs)]
    oidx = {e.name: i for i, e in enumerate(O)}
    fidx = {e.addr: j for j, e in enumerate(F)}
    anch_all = [(oidx[n], fidx[ad]) for n, ad in sdk.items() if n in oidx and ad in fidx]
    chain = lis_chain(anch_all)
    res = map_region(O, F, chain)
    cls = classify(res, O, F)
    for i, j in anch_all:       # PsyQ-named pairs are exact by construction
        res[i], cls[i] = j, "sdk"

    # Data/bss: same alignment against the splat data symbols (no PsyQ anchors).
    OD = gap_sizes([(s.addr, s.name) for s in syms if CODE_LIMIT <= s.addr < OVL_BASE])
    dsyms = {}
    for val, _, name, code in elf:
        if not code and MAIN_TEXT_END <= val < MAIN_BSS_LIMIT and name.startswith(("D_", "jtbl_")):
            dsyms.setdefault(val, name)
    FD = gap_sizes([(v, dsyms[v]) for v in sorted(dsyms)])
    dres = map_region(OD, FD, [])
    dcls = classify(dres, OD, FD)

    novl = sum(1 for s in syms if s.addr >= OVL_BASE)
    print("O.BIN symbols %d: main code-side %d (aligned %d), main data/bss-side %d (aligned %d), overlay-side %d" %
          (len(syms), len(O), len(res), len(OD), len(dres), novl))
    print("main-exe functions %d; PsyQ anchors %d (ordered chain %d)" % (len(F), len(anch_all), len(chain)))
    print("code confidence:", dict(collections.Counter(cls.values())),
          "| data confidence:", dict(collections.Counter(dcls.values())))

    if a.write:
        taken = {e.name for e in F} | {n for _, _, n, _ in elf}
        n = write_outputs(OUT_SYMS, OUT_RENAMES,
                          [("code (high confidence)", res, cls, O, F, "type:func"),
                           ("data (high confidence, uncalibrated)", dres, dcls, OD, FD, "")], taken)
        print("wrote %d symbols/renames" % n)
    return 0


if __name__ == "__main__":
    sys.exit(main())
