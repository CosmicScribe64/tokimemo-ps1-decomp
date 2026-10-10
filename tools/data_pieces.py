#!/usr/bin/env python3
"""Cut the `.data` islands of a splat config into pieces a C file can include (T-9010).

usage: data_pieces.py CONFIG.yaml

A `.data` island is a dot-prefixed `.data` subsegment named like a `c` subsegment, e.g.
`[0x2CD90, .data, RPG_BAT/8014E780]`: the C object provides that range of `.data`, so the
variables a C file defines land at their original addresses (tools/data_island.py adds the
island; wiki/build-system.md, "Data islands"). splat writes the range to
`<asm_path>/data/<name>.data.s`, which the build does not assemble. Everything in it comes
from the C object: variables the C file defines, and for the rest one piece per variable,
included with `INCLUDE_RODATA("<asm_path>/data/<name>.data", D_X);` (asm-processor turns the
line into a dummy `char[]` definition of the same size and splices the bytes in). IDO emits
`.data` in source order, so the lines and the definitions stand in address order.

This script writes the pieces `<asm_path>/data/<name>.data/<symbol>.s`. IDO starts every data
definition on a multiple of 4 (the dummy arrays too), so a piece starts at a 4-aligned symbol
and runs to the next one: a symbol at an unaligned address is a member of the variable before
it and stays a label inside that piece. A piece is written as `.byte` lines (bytes from the
binary named by `target_path`) and `.word EXPR` for each word splat wrote as a symbol
reference, which keeps its relocation; `dlabel` lines keep every symbol defined. Running the
script twice gives the same files.

Fails loudly (exit 1) when an island file is missing, the island does not start on a multiple
of 16 or is not a multiple of 16 long (IDO aligns and pads an object's `.data` to 16), or a
symbol reference is not 4-aligned. Tests: tools/test_data_pieces.py.
"""
import os
import re
import sys

import yaml

SECTION = ".section .data\n"
LINE_RE = re.compile(r"^\s*/\*\s*([0-9A-Fa-f]+)\s+([0-9A-Fa-f]{8})\b[^*]*\*/\s*(\.\w+)\s*(.*?)\s*$")
LABEL_RE = re.compile(r"^\s*dlabel\s+(\w+)")
NUMBER_RE = re.compile(r"^-?(0x[0-9A-Fa-f]+|\d+)$")
SIZES = {".word": 4, ".short": 2, ".half": 2, ".byte": 1}


class PieceError(Exception):
    """The island cannot be cut into pieces exactly."""


def islands(cfg):
    """[(name, rom start, rom end)] of the `.data` subsegments of a splat config."""
    out = []
    for seg in cfg["segments"]:
        if not isinstance(seg, dict):
            continue
        subs = seg.get("subsegments", [])
        rows = []
        for sub in subs:
            if isinstance(sub, dict):
                rows.append((sub["start"], sub["type"], sub.get("name")))
            else:
                rows.append((sub[0], sub[1] if len(sub) > 1 else None, sub[2] if len(sub) > 2 else None))
        nxt = [r[0] for r in rows[1:]]
        end = None
        idx = cfg["segments"].index(seg)
        for later in cfg["segments"][idx + 1:]:
            end = later["start"] if isinstance(later, dict) else later[0]
            break
        nxt.append(end)
        for (start, typ, name), stop in zip(rows, nxt):
            if typ == ".data":
                if stop is None:
                    raise PieceError("%s: island has no end" % name)
                out.append((name, start, stop))
    return out


def parse_island(text):
    """([(vram, symbol)], [(rom, vram, directive, args)]) of a splat data file."""
    labels, lines = [], []
    pending = []
    for line in text.splitlines():
        m = LABEL_RE.match(line)
        if m:
            pending.append(m.group(1))
            continue
        m = LINE_RE.match(line)
        if m:
            rom, vram = int(m.group(1), 16), int(m.group(2), 16)
            labels += [(vram, s) for s in pending]
            pending = []
            lines.append((rom, vram, m.group(3), m.group(4)))
    if pending:
        raise PieceError("labels %s without data" % " ".join(pending))
    return labels, lines


def relocations(lines):
    """{vram: expression} of the words splat wrote as symbol references."""
    out = {}
    for _rom, vram, directive, args in lines:
        if directive == ".word" and not NUMBER_RE.match(args):
            if vram % 4:
                raise PieceError("symbol reference %s at unaligned %08X" % (args, vram))
            out[vram] = args
    return out


def pieces(labels, relocs, blob, rom0, vram0, size):
    """[(symbol, text)] for an island of `size` bytes at vram0 whose bytes start at blob[rom0]."""
    if vram0 % 16 or size % 16:
        raise PieceError("island %08X+%X is not 16-aligned" % (vram0, size))
    if not labels or labels[0][0] != vram0:
        raise PieceError("island %08X does not start with a symbol" % vram0)
    starts = [(v, s) for v, s in labels if v % 4 == 0]
    out = []
    for k, (v, sym) in enumerate(starts):
        end = starts[k + 1][0] if k + 1 < len(starts) else vram0 + size
        body = [SECTION, "\n"]
        inner = sorted(x for x in labels if v <= x[0] < end)
        pos = v
        chunk = []

        def flush():
            if chunk:
                body.append("    .byte %s\n" % ", ".join("0x%02X" % b for b in chunk))
                del chunk[:]
        while pos < end:
            for lv, ls in inner:
                if lv == pos:
                    flush()
                    body.append("dlabel %s\n" % ls)
            if pos in relocs:
                flush()
                body.append("    .word %s\n" % relocs[pos])
                pos += 4
                continue
            chunk.append(blob[rom0 + pos - vram0])
            if len(chunk) == 16:
                flush()
            pos += 1
        flush()
        out.append((sym, "".join(body)))
    return out


def process(island_path, blob, rom0, rom1, piece_dir):
    """Write the pieces of one island; returns ([piece symbols], [(vram, label)])."""
    with open(island_path, encoding="utf-8") as f:
        labels, lines = parse_island(f.read())
    if not lines:
        raise PieceError("%s: no data lines" % island_path)
    vram0 = lines[0][1] - (lines[0][0] - rom0)
    out = pieces(labels, relocations(lines), blob, rom0, vram0, rom1 - rom0)
    os.makedirs(piece_dir, exist_ok=True)
    for fn in os.listdir(piece_dir):
        os.remove(os.path.join(piece_dir, fn))
    for sym, text in out:
        with open(os.path.join(piece_dir, sym + ".s"), "w", encoding="utf-8") as f:
            f.write(text)
    return [s for s, _ in out], labels


def provide_text(labels):
    """Linker script lines that define every island label the objects do not define: a C
    definition replaces a piece, and the names inside it (`D_8015EE34` in `D_8015EE30[4]`)
    are still used by the asm of other objects."""
    return "".join("PROVIDE(%s = 0x%08X);\n" % (n, v) for v, n in sorted(labels))


def syms_path(opts, base):
    """The linker script fragment this script writes for a config (always, empty without islands)."""
    ld = opts["ld_script_path"]
    return os.path.join(base, ld[:-3] + "_data_syms.ld" if ld.endswith(".ld") else ld + "_data_syms.ld")


def main(argv):
    if len(argv) != 2:
        sys.exit(__doc__)
    with open(argv[1]) as f:
        cfg = yaml.safe_load(f)
    opts = cfg["options"]
    base = os.path.normpath(os.path.join(os.path.dirname(argv[1]), opts.get("base_path", ".")))
    asm_path = os.path.join(base, opts["asm_path"])
    labels = []
    try:
        found = islands(cfg)
        blob = None
        for name, r0, r1 in found:
            if blob is None:
                with open(os.path.join(base, opts["target_path"]), "rb") as f:
                    blob = f.read()
            src = os.path.join(asm_path, "data", name + ".data.s")
            if not os.path.exists(src):
                raise PieceError("%s not written by splat" % src)
            syms, lab = process(src, blob, r0, r1, os.path.join(asm_path, "data", name + ".data"))
            labels += lab
            print("%s: %d data pieces" % (src, len(syms)))
    except PieceError as e:
        sys.exit("data_pieces.py: %s" % e)
    out = syms_path(opts, base)
    os.makedirs(os.path.dirname(out), exist_ok=True)
    with open(out, "w") as f:
        f.write("/* data island labels (T-9010), tools/data_pieces.py */\n" + provide_text(labels))


if __name__ == "__main__":
    main(sys.argv)
