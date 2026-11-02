#!/usr/bin/env python3
"""Find addresses the linker does not resolve (T-39505, wiki/raw-addresses.md).

A free relink (tools/english_link.py) moves code and data. An address survives that only if the
linker writes it: a relocation against a symbol defined in a section. This scan lists what does
not survive, for every object of one link (from its map):
  word   a 4-byte-aligned data word in a movable range with no R_MIPS_32 relocation
  hi/lo  a `lui` with no R_MIPS_HI16 relocation whose value (with the first matching low part)
         lies in a movable range
  jal/j  a jump with no R_MIPS_26 relocation whose target lies in a movable range
  abs    a relocation against a symbol the link scripts define as a number (`X = 0x...;`)

Usage (in Docker):
  raw_addrs.py MAP [--range LO:HI ...] [--scripts LD ...] [--json OUT]
Default ranges: the main executable image and bss (0x80041000-0x8012B540, end included) and the
overlay slot (0x80132000-0x80162000, end included).
"""
import argparse
import json
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402

DEFAULT_RANGES = [(0x80041000, 0x8012B540), (0x80132000, 0x80162000)]
MAP_INPUT = re.compile(r"^ (\.\w+)\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (build/\S+\.o)$")
ABS_DEF = re.compile(r"^\s*(?:PROVIDE\()?\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*\)?\s*;")
LOW_OPS = {0x09: "addiu", 0x0D: "ori", 0x20: "lb", 0x21: "lh", 0x23: "lw", 0x24: "lbu", 0x25: "lhu",
           0x28: "sb", 0x29: "sh", 0x2B: "sw", 0x32: "lwc2", 0x3A: "swc2", 0x22: "lwl", 0x26: "lwr",
           0x2A: "swl", 0x2E: "swr"}


def in_ranges(v, ranges):
    return any(lo <= v <= hi for lo, hi in ranges)


def placed_sections(map_path):
    """{(object, section): address} of the input sections with contents in a link map."""
    out = {}
    pending = None
    for line in open(map_path):
        line = line.rstrip("\n")
        m = re.match(r"^ (\.\w+)$", line)
        if m:
            pending = m.group(1)
            continue
        m = MAP_INPUT.match(line)
        if not m and pending:
            m2 = re.match(r"^\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (build/\S+\.o)$", line)
            if m2:
                m = (pending, m2.group(1), m2.group(2), m2.group(3))
        elif m:
            m = m.groups()
        pending = None
        if m and int(m[2], 16) > 0:
            out.setdefault((m[3], m[0]), int(m[1], 16))
    return out


def abs_definitions(scripts):
    out = {}
    for path in scripts:
        for line in open(path):
            m = ABS_DEF.match(line)
            if m:
                out[m.group(1)] = int(m.group(2), 16)
    return out


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def scan_text(data, relocs, ranges):
    hi_relocs = {o for o, t, _s in relocs if t == elf32.R_MIPS_HI16}
    j_relocs = {o for o, t, _s in relocs if t == elf32.R_MIPS_26}
    lo_relocs = {o for o, t, _s in relocs if t == elf32.R_MIPS_LO16}
    n = len(data) // 4
    words = struct.unpack_from("<%dI" % n, data)
    for i, w in enumerate(words):
        op = w >> 26
        off = i * 4
        if op in (2, 3) and off not in j_relocs:
            target = 0x80000000 | ((w & 0x03FFFFFF) << 2)
            if in_ranges(target, ranges):
                yield off, ("jal" if op == 3 else "j"), target, None
        elif op == 0x0F and off not in hi_relocs:
            rt = (w >> 16) & 31
            hi = (w & 0xFFFF) << 16
            low = None
            for k in range(i + 1, min(n, i + 40)):
                x = words[k]
                xop = x >> 26
                if xop in LOW_OPS and (x >> 21) & 31 == rt and k * 4 not in lo_relocs:
                    imm = x & 0xFFFF
                    low = (hi | imm) if xop == 0x0D else (hi + sext16(imm)) & 0xFFFFFFFF
                    low = (low, LOW_OPS[xop], k * 4)
                    break
                # rt overwritten by another instruction: stop
                if xop == 0x0F and (x >> 16) & 31 == rt:
                    break
                if xop == 0 and (x >> 11) & 31 == rt and x & 0x3F not in (0x08, 0x09):
                    break
            value = low[0] if low else hi
            if in_ranges(value, ranges) or (low is None and in_ranges(hi, ranges)):
                yield off, "hi/lo", value, low


def scan_object(path, placed, ranges, absdefs):
    elf = elf32.Elf(path)
    found = []
    for sec in elf.sections:
        if sec.type in (elf32.SHT_NOBITS, elf32.SHT_REL, elf32.SHT_SYMTAB) or not sec.flags & 2:
            continue  # only allocated sections with contents
        base = placed.get((path, sec.name))
        data = elf.contents(sec)
        relocs = elf.relocations(sec)
        for off, _t, sym in relocs:
            if sym.shndx == elf32.SHN_UNDEF and sym.name in absdefs and in_ranges(absdefs[sym.name], ranges):
                found.append({"object": path, "section": sec.name, "offset": off, "kind": "abs",
                              "value": absdefs[sym.name], "symbol": sym.name,
                              "address": None if base is None else base + off})
        if sec.flags & 4:  # SHF_EXECINSTR
            for off, kind, value, low in scan_text(data, relocs, ranges):
                f = {"object": path, "section": sec.name, "offset": off, "kind": kind, "value": value,
                     "address": None if base is None else base + off}
                if low:
                    f["low"] = "%s at +0x%X" % (low[1], low[2])
                found.append(f)
        else:
            w_relocs = {o for o, t, _s in relocs if t == elf32.R_MIPS_32}
            for off in range(0, len(data) - 3, 4):
                v = struct.unpack_from("<I", data, off)[0]
                if off not in w_relocs and in_ranges(v, ranges):
                    found.append({"object": path, "section": sec.name, "offset": off, "kind": "word",
                                  "value": v, "address": None if base is None else base + off})
    return found


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("map")
    ap.add_argument("--range", action="append", default=[])
    ap.add_argument("--scripts", nargs="*", default=[])
    ap.add_argument("--json")
    a = ap.parse_args(argv)
    ranges = [tuple(int(x, 16) for x in r.split(":")) for r in a.range] or DEFAULT_RANGES
    placed = placed_sections(a.map)
    absdefs = abs_definitions(a.scripts)
    objects = sorted({o for o, _s in placed})
    found = []
    for o in objects:
        found += scan_object(o, placed, ranges, absdefs)
    for f in found:
        print("%-5s %s %s+0x%X @%s = 0x%08X%s%s" % (
            f["kind"], f["object"], f["section"], f["offset"],
            "?" if f["address"] is None else "%08X" % f["address"], f["value"],
            " " + f["symbol"] if "symbol" in f else "", " (" + f["low"] + ")" if "low" in f else ""))
    if a.json:
        with open(a.json, "w") as fh:
            json.dump(found, fh, indent=1)


if __name__ == "__main__":
    main(sys.argv[1:])
