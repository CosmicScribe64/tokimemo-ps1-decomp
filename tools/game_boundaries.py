#!/usr/bin/env python3
"""Find the source-file (translation unit) boundaries of the game code of SLPM_86.053 (T-0012).

Usage: python3 tools/game_boundaries.py [--yaml] [--exe disc/files/SLPM_86.053]
Needs asm/ (run ninja or splat once; only the function list and sizes are read from it).

Evidence, all derived from the original bytes:
 1. Alignment padding. The original linker aligns the .text of every object to 16 bytes, while
    functions inside one object follow each other with no padding. A run of zero words between two
    functions whose end is the next 16-byte boundary is therefore an object boundary
    (BOUNDARY). A gap that is longer than the padding needed, or does not end 16-aligned, is
    reported as AMBIGUOUS and is not used (fewer, larger files).
    Objects whose size is a multiple of 16 leave no padding, so the files found are unions of one
    or more original objects.
 2. Rodata object starts: zero runs of 5+ bytes that end 16-aligned inside the game .rodata,
    mapped to the functions referencing the strings/jump tables before and after. A boundary
    candidate must lie between those two functions (consistency check for evidence 1).
 3. Data/bss ordering: data and bss symbols referenced from one file only are laid out in file
    order; the script reports the fraction of address-adjacent pairs that respect the file order.
 4. Call graph: calls inside a file versus across files.
--yaml prints the splat `c` subsegments for config/SLPM_86.053.yaml.
"""
import argparse
import bisect
import glob
import re
import struct
import sys

TEXT_VRAM = 0x80041000
FILE_OFF = 0x800
GAME_END = 0x80086810            # DecDCTReset, first SDK object (T-0010)
RO = (0x800AF340, 0x800B3220)
DATA_BSS = (0x800B3220, 0x8012B538)


# Boundaries not proven by padding but supported by two independent lines of evidence:
# 0x800420D0 is the PS-X EXE entry point (16-aligned, start of the startup code) and the string
# at 0x800AF370 used by the code after it starts a new rodata object (zero run before it ends
# 16-aligned), which needs a boundary between 0x800412E0 and 0x80042134.
INFERRED = [0x800420D0]


def load_functions(root):
    """[(addr, size)] of the game functions from the splat `nonmatching` headers."""
    funcs = {}
    for f in glob.glob(root + "/asm/*matchings/**/func_*.s", recursive=True):
        head = open(f, errors="replace").read(400)
        m = re.search(r"nonmatching (func_([0-9A-F]{8})), (0x[0-9A-F]+|\d+)", head)
        if m:
            a = int(m.group(2), 16)
            if TEXT_VRAM <= a < GAME_END:
                funcs[a] = int(m.group(3), 0)
    return sorted(funcs.items())


class Exe:
    def __init__(self, path):
        self.d = open(path, "rb").read()

    def word(self, vram):
        o = vram - TEXT_VRAM + FILE_OFF
        return struct.unpack("<I", self.d[o:o + 4])[0]

    def bytes(self, lo, hi):
        return self.d[lo - TEXT_VRAM + FILE_OFF:hi - TEXT_VRAM + FILE_OFF]


def scan(exe, addr, size):
    """Data addresses (lui/addiu/ori/load/store pairs) and jal targets of one function."""
    regs, refs, calls = {}, [], []
    for i in range(size // 4):
        x = exe.word(addr + 4 * i)
        op, rs, rt, imm = x >> 26, (x >> 21) & 31, (x >> 16) & 31, x & 0xFFFF
        si = imm - 0x10000 if imm & 0x8000 else imm
        if op == 0xF:
            regs[rt] = imm << 16
        elif op == 9 and rs in regs:
            regs[rt] = (regs[rs] + si) & 0xFFFFFFFF
            refs.append(regs[rt])
        elif op == 0xD and rs in regs:
            regs[rt] = regs[rs] | imm
            refs.append(regs[rt])
        elif op in (0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x28, 0x29, 0x2A, 0x2B, 0x2E) and rs in regs:
            refs.append((regs[rs] + si) & 0xFFFFFFFF)
            if op < 0x26:
                regs.pop(rt, None)
        elif op == 3:
            calls.append(((x & 0x3FFFFFF) << 2) | 0x80000000)
        elif op in (8, 0xA, 0xB, 0xC, 0xE):
            regs.pop(rt, None)
        elif op == 0:
            regs.pop((x >> 11) & 31, None)
    return refs, calls


def padding_boundaries(exe, funcs):
    """(boundaries, ambiguous): object starts proven by alignment padding, and unusable gaps."""
    bounds, amb = [], []
    for (a, s), (b, _) in zip(funcs, funcs[1:]):
        end = a + s
        if end == b:
            continue
        zero = all(exe.word(x) == 0 for x in range(end, b, 4))
        need = (-end) % 16
        if zero and b % 16 == 0 and b - end == need:
            bounds.append(b)
        else:
            amb.append((end, b))
    return bounds, amb


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--exe", default="disc/files/SLPM_86.053")
    ap.add_argument("--root", default=".")
    ap.add_argument("--yaml", action="store_true")
    a = ap.parse_args()
    exe = Exe(a.exe)
    funcs = load_functions(a.root)
    if not funcs:
        sys.exit("game_boundaries.py: no asm/*matchings/**/func_*.s found (run ninja first)")
    bounds, amb = padding_boundaries(exe, funcs)
    starts = sorted([funcs[0][0]] + bounds + INFERRED)
    if a.yaml:
        for s in starts:
            print("      - { start: 0x%X, type: c, name: main/%08X }"
                  % (s - TEXT_VRAM + FILE_OFF, s))
        return 0

    print("functions: %d, alignment-padding boundaries: %d, inferred: %s, files: %d"
          % (len(funcs), len(bounds), ["%08X" % i for i in INFERRED], len(starts)))
    print("ambiguous gaps (not used): " + ", ".join("0x%08X-0x%08X" % g for g in amb))
    ends = starts[1:] + [GAME_END]
    fidx = lambda addr: bisect.bisect_right(starts, addr) - 1

    scans = {ad: scan(exe, ad, s) for ad, s in funcs}
    print("\nfile       size   funcs")
    for s, e in zip(starts, ends):
        print("%08X %6X %5d" % (s, e - s, sum(1 for ad, _ in funcs if s <= ad < e)))

    # 2. rodata object starts
    lo, hi = RO
    blob = exe.bytes(lo, hi)
    refs_ro = {}
    for ad, _ in funcs:
        for x in scans[ad][0]:
            if lo <= x < hi:
                refs_ro.setdefault(x, set()).add(ad)
    print("\nrodata object starts (zero run >= 5 ending 16-aligned, referenced exactly):")
    ok = tot = 0
    for m in re.finditer(rb"\x00{5,}", blob):
        r = lo + m.end()
        if r % 16 or r not in refs_ro:
            continue
        before = [f for x, fs in refs_ro.items() if x < r for f in fs]
        after = [f for x, fs in refs_ro.items() if x >= r for f in fs]
        if not before or max(before) >= min(after):
            continue
        cand = [b for b in starts if max(before) < b <= min(after)]
        tot += 1
        ok += bool(cand)
        print("  %08X between funcs %08X and %08X -> boundary candidates %s"
              % (r, max(before), min(after), ["%08X" % c for c in cand] or "NONE (hidden boundary)"))
    print("  consistent with a proven boundary: %d of %d" % (ok, tot))

    # 3. data/bss ordering of single-file symbols
    owner = {}
    for ad, _ in funcs:
        for x in scans[ad][0]:
            if DATA_BSS[0] <= x < DATA_BSS[1]:
                owner.setdefault(x, set()).add(fidx(ad))
    excl = sorted((x, next(iter(f))) for x, f in owner.items() if len(f) == 1)
    pairs = list(zip(excl, excl[1:]))
    good = sum(1 for (x, f), (y, g) in pairs if f <= g)
    print("\ndata/bss symbols used by one file only: %d of %d; address-adjacent pairs in file order: %d of %d (%.0f%%)"
          % (len(excl), len(owner), good, len(pairs), 100.0 * good / max(1, len(pairs))))

    # 4. calls
    fs = {ad for ad, _ in funcs}
    intra = cross = 0
    callers = {}
    for ad, _ in funcs:
        for t in scans[ad][1]:
            if t in fs:
                callers.setdefault(t, set()).add(fidx(ad))
                if fidx(t) == fidx(ad):
                    intra += 1
                else:
                    cross += 1
    only_own = sum(1 for t, c in callers.items() if c == {fidx(t)})
    print("\ncalls between game functions: %d inside a file, %d across files; %d of %d called functions are called only from their own file"
          % (intra, cross, only_own, len(callers)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
