#!/usr/bin/env python3
"""Hand the rodata of "island" subsegments to the functions that use it (T-1340).

usage: rodata_pieces.py CONFIG.yaml

A C object that contains a `switch` jump table emits it into its own .rodata. To link that table
at the original address, the rodata of the C file is a dot-prefixed `.rodata` subsegment of the
splat config (an "island": the contiguous range of rodata the C object provides, named like the
C file). splat writes the island as one file `<asm_path>/data/<name>.rodata.s`, which the build
does not assemble. Everything in it has to come from the C object instead, in the order of the
original. IDO emits string literals in source order and every jump table after all of them, in
function order; asm-processor reproduces that for INCLUDE_ASM functions from two sections in the
function's .s file, `.rodata` (strings and plain data) and `.late_rodata` (jump tables).

This script appends those sections to the INCLUDE_ASM files
`<asm_path>/nonmatchings/<name>/<function>.s` (splat's output, regenerated on every split):
each island symbol goes to the first function (lowest address) whose code mentions it; symbols
named `jtbl_*` become `.late_rodata`, all others `.rodata`. A function converted to C simply
stops including its .s file, so its tables come from the compiler. Island symbols that no
function of the file mentions are written to `<asm_path>/data/<name>.rodata/<symbol>.s` and
must be placed by hand with `INCLUDE_RODATA("<asm_path>/data/<name>.rodata", <symbol>);` in the
position of the original (a symbol first used by a converted function but also by an
INCLUDE_ASM one fails at link time as undefined). Non-ASCII string characters are re-encoded
to Shift-JIS octal escapes (as tools/asm.py does), because asm-processor assembles these files
without that step. Running the script twice gives the same files.

Limits: only `jtbl_*` symbols are treated as late rodata (float and double constants are
late rodata in IDO too and are not handled; the sha1 check fails if an island holds some).
The length of the island is the length of the object's rodata rounded up to 16 by the compiler.
"""
import os
import re
import sys

import yaml

import asm

MARK = "# T-1340 rodata (rodata_pieces.py)"
SECTION = ".section .rodata\n"
LABEL_RE = re.compile(r"^\s*(?:glabel|dlabel)\s+(\S+)", re.M)


def islands(cfg):
    """Names of the `.rodata` (dot-prefixed) subsegments of every segment of a splat config."""
    out = []
    for seg in cfg["segments"]:
        if not isinstance(seg, dict):
            continue
        for sub in seg.get("subsegments", []):
            typ, name = (sub["type"], sub["name"]) if isinstance(sub, dict) else (sub[1], sub[2])
            if typ == ".rodata":
                out.append(name)
    return out


def split_blocks(text):
    """[(symbol, block text)] of a splat rodata file: a block ends at its `enddlabel` line and
    starts after the previous block, so alignment directives stay with the symbol they precede."""
    blocks = []
    cur = []
    started = False
    for line in text.splitlines():
        if not started:
            if not line.startswith(("nonmatching ", ".align", "dlabel ")):
                continue              # file header: .include, .section, generator comment
            started = True
        cur.append(line)
        if line.startswith("enddlabel "):
            blocks.append((line.split()[1], "\n".join(cur) + "\n"))
            cur = []
            started = False
    if cur and any(line.strip() and not line.startswith(".align") for line in cur):
        raise ValueError("rodata island ends inside a symbol block")
    return blocks


def is_late(sym):
    return sym.startswith("jtbl_")


ZERO_WORD_RE = re.compile(r"\.word\s+(0x0+|0)\s*$")


def clean(sym, body):
    """Block text for assembly: Shift-JIS escapes. For a jump table also no `.align` in front of
    it and no zero words at its end: IDO starts every table on a multiple of 8 itself, and splat
    counts the padding word up to the next table as part of the previous one."""
    body = asm.STRING_LINE.sub(asm.sjis_escape, body)
    if not is_late(sym):
        return body
    lines = [l for l in body.splitlines() if not l.startswith(".align")]
    end = max(i for i, l in enumerate(lines) if l.startswith("enddlabel "))
    while ZERO_WORD_RE.search(lines[end - 1]):
        del lines[end - 1]
        end -= 1
    return "".join(l + "\n" for l in lines)


def function_files(folder):
    """{path: (vram, text)} of the splat function files in `folder`, with the appended rodata of
    an earlier run removed."""
    out = {}
    for fn in sorted(os.listdir(folder)):
        if not fn.endswith(".s"):
            continue
        path = os.path.join(folder, fn)
        text = open(path, encoding="utf-8").read()
        text = text.split(MARK)[0].rstrip("\n") + "\n"
        m = re.search(r"/\* [0-9A-Fa-f]+ ([0-9A-Fa-f]{8}) ", text)
        if m and "glabel" in text:
            out[path] = (int(m.group(1), 16), text)
    return out


def distribute(blocks, funcs):
    """({path: [(late, symbol, body)]}, [unowned symbols]): each symbol to the lowest function
    that mentions it."""
    owned = {}
    free = []
    ordered = sorted(funcs.items(), key=lambda kv: kv[1][0])
    for sym, body in blocks:
        pat = re.compile(r"\b%s\b" % re.escape(sym))
        for path, (_vram, text) in ordered:
            if pat.search(text):
                owned.setdefault(path, []).append((is_late(sym), sym, clean(sym, body)))
                break
        else:
            free.append((sym, clean(sym, body)))
    return owned, free


def render(items):
    """Appended text for one function: `.rodata` blocks, then `.late_rodata` blocks."""
    out = [MARK, "\n"]
    early = [b for late, _s, b in items if not late]
    late = [b for l, _s, b in items if l]
    if early:
        out.append(SECTION + "\n" + "\n".join(early))
    if late:
        out.append(".section .late_rodata\n\n" + "\n".join(late))
    return "".join(out)


def process(src, folder, piece_dir):
    """Rewrite the function files of `folder` from the island file `src`; write the unowned
    symbols to `piece_dir`. Returns (owned count, unowned symbols)."""
    with open(src, encoding="utf-8") as f:
        blocks = split_blocks(f.read())
    if not blocks:
        raise ValueError("%s: no symbol blocks" % src)
    funcs = function_files(folder)
    owned, free = distribute(blocks, funcs)
    for path, (_vram, text) in funcs.items():
        with open(path, "w", encoding="utf-8") as f:
            f.write(text + (("\n" + render(owned[path])) if path in owned else ""))
    os.makedirs(piece_dir, exist_ok=True)
    for fn in os.listdir(piece_dir):
        os.remove(os.path.join(piece_dir, fn))
    for sym, body in free:
        with open(os.path.join(piece_dir, sym + ".s"), "w", encoding="utf-8") as f:
            f.write(SECTION + "\n" + body)
    return sum(len(v) for v in owned.values()), [s for s, _ in free]


def main(argv):
    if len(argv) != 2:
        sys.exit(__doc__)
    with open(argv[1]) as f:
        cfg = yaml.safe_load(f)
    asm_path = cfg["options"]["asm_path"]
    for name in islands(cfg):
        src = os.path.join(asm_path, "data", name + ".rodata.s")
        folder = os.path.join(asm_path, "nonmatchings", name)
        if not os.path.exists(src):
            sys.exit("rodata_pieces.py: %s not written by splat" % src)
        if not os.path.isdir(folder):
            os.makedirs(folder)       # every function of the file is C: all symbols are pieces
        n, free = process(src, folder, os.path.join(asm_path, "data", name + ".rodata"))
        print("%s: %d symbols into functions, %d unowned%s"
              % (src, n, len(free), (" (" + " ".join(free) + ")") if free else ""))


if __name__ == "__main__":
    main(sys.argv)
