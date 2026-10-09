#!/usr/bin/env python3
"""Cut a rodata "island" for the jump tables of some functions out of a splat config (T-1340).

usage: rodata_island.py CONFIG.yaml C_NAME FUNCTION [FUNCTION...]
       (run after a `splat split` of the current config; C_NAME is the `c` subsegment name,
        e.g. RENSYU or main/80062CD0; the functions are INCLUDE_ASM ones that use jump tables)

IDO writes the rodata of one object as: string literals and constants in source order, then all
jump tables in function order, and the object's rodata is padded to 16 bytes. So the rodata of
one original object is a chunk of the form [early items][jump tables][pad], and the C object
can provide a whole chunk. The script finds the chunk that holds the tables of FUNCTION...
(from the end of the previous run of `jtbl_*` symbols, or the start of the rodata subsegment,
through the last table plus padding up to the next multiple of 16), and splits the rodata
subsegment `[off, rodata, NAME]` of the yaml into

    - [off, rodata, NAME]                              # before (only if not empty)
    - [chunk start, .rodata, C_NAME]                   # island: comes from the C object
    - [chunk end, rodata, NAME_after_<base of C_NAME>] # after

Afterwards tools/rodata_pieces.py (run by the build after splat) hands the island's symbols to
the INCLUDE_ASM function files. The file must hold exactly one object's rodata in the chunk,
which the sha1 check of the build confirms; the script only checks the layout it relies on
(16-aligned start, zero padding) and exits non-zero otherwise.
"""
import os
import re
import sys

import yaml

import rodata_pieces

LINE_RE = re.compile(r"^(\s*- \[)(0x[0-9A-Fa-f]+), rodata, ([\w/]+)\]\s*$")
OFF_RE = re.compile(r"/\* ([0-9A-Fa-f]+) [0-9A-Fa-f]{8}[ *]")
WORD_RE = re.compile(r"/\* ([0-9A-Fa-f]+) [0-9A-Fa-f]{8} [0-9A-Fa-f]{8} \*/\s+\.word\s+(\S+)")


def block_start(body):
    """File offset of the first data line of a symbol block."""
    return int(OFF_RE.search(body).group(1), 16)


def table_words(body):
    """[(offset, operand)] of the `.word` lines of a jump table block."""
    return [(int(o, 16), w) for o, w in WORD_RE.findall(body)]


def is_padded(body):
    """True when the last word of a table block is a zero (the padding of the object's end)."""
    words = table_words(body)
    return bool(words) and words[-1][1] in ("0x00000000", "0")


def find_chunk(blocks, wanted):
    """(start offset, end offset) of the chunk holding the tables `wanted`; blocks is the
    split_blocks() result of one rodata file."""
    syms = [s for s, _ in blocks]
    idx = [syms.index(w) for w in wanted if w in syms]
    if len(idx) != len(wanted):
        raise ValueError("table(s) %s not in this rodata file" % ", ".join(w for w in wanted if w not in syms))
    first, last = min(idx), max(idx)
    # Tables sit at the end of an object, followed by zero padding up to the next multiple of 16
    # (splat counts those zero words as part of the last table). The next object starts after
    # that; before the first table come the early items of the same object.
    for k in range(first, last):
        if is_padded(blocks[k][1]):
            raise ValueError("%s and %s are in different objects (padding between them)"
                             % (syms[first], syms[last]))
    while last + 1 < len(syms) and rodata_pieces.is_late(syms[last + 1]) \
            and not is_padded(blocks[last][1]):
        last += 1
    while first > 0 and rodata_pieces.is_late(syms[first - 1]) \
            and not is_padded(blocks[first - 1][1]):
        first -= 1
    k = first
    while k > 0 and not rodata_pieces.is_late(syms[k - 1]):
        k -= 1
    start = block_start(blocks[k][1])
    words = table_words(blocks[last][1])
    while words and words[-1][1] in ("0x00000000", "0"):
        words.pop()
    if not words:
        raise ValueError("table %s has no entries" % syms[last])
    real_end = words[-1][0] + 4
    end = start + (real_end - start + 15) // 16 * 16
    tail = table_words(blocks[last][1])
    block_end = tail[-1][0] + 4
    if start % 16:
        raise ValueError("chunk start 0x%X is not 16-aligned" % start)
    if end != block_end:
        raise ValueError("padded chunk end 0x%X differs from the end of %s (0x%X)"
                         % (end, syms[last], block_end))
    return start, end


def read_function(asm_path, cname, fn):
    """Text of the splat file of a function, decompiled (matchings/) or not (nonmatchings/)."""
    for kind in ("nonmatchings", "matchings"):
        path = os.path.join(asm_path, kind, cname, fn + ".s")
        if os.path.exists(path):
            return open(path, encoding="utf-8").read()
    sys.exit("rodata_island.py: no splat file for %s in %s/{non,}matchings/%s" % (fn, asm_path, cname))


def main(argv):
    if len(argv) < 4:
        sys.exit(__doc__)
    path, cname, funcs = argv[1], argv[2], argv[3:]
    text = open(path).read()
    cfg = yaml.safe_load(text)
    asm_path = cfg["options"]["asm_path"]
    if re.search(r"\.rodata, %s\]" % re.escape(cname), text):
        sys.exit("rodata_island.py: %s already has an island for %s" % (path, cname))
    wanted = []
    for fn in funcs:
        src = read_function(asm_path, cname, fn)
        found = sorted(set(re.findall(r"%lo\((jtbl_\w+)\)", src)))
        if not found:
            sys.exit("rodata_island.py: %s uses no jump table" % fn)
        wanted += found
    lines = text.splitlines(True)
    for i, line in enumerate(lines):
        m = LINE_RE.match(line)
        if not m:
            continue
        rfile = os.path.join(asm_path, "data", m.group(3) + ".rodata.s")
        if not os.path.exists(rfile):
            continue
        blocks = rodata_pieces.split_blocks(open(rfile, encoding="utf-8").read())
        if wanted[0] in [s for s, _ in blocks]:
            break
    else:
        sys.exit("rodata_island.py: no rodata subsegment holds %s" % wanted[0])
    seg_start = int(m.group(2), 16)
    try:
        start, end = find_chunk(blocks, wanted)
    except ValueError as e:
        sys.exit("rodata_island.py: %s" % e)
    out = []
    if start > seg_start:
        out.append("%s0x%X, rodata, %s]\n" % (m.group(1), seg_start, m.group(3)))
    out.append("%s0x%X, .rodata, %s]\n" % (m.group(1), start, cname))
    out.append("%s0x%X, rodata, %s_after_%s]\n" % (m.group(1), end, m.group(3), cname.split("/")[-1]))
    lines[i] = "".join(out)
    open(path, "w").write("".join(lines))
    print("%s: island %s 0x%X-0x%X" % (path, cname, start, end))


if __name__ == "__main__":
    main(sys.argv)
