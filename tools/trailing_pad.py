#!/usr/bin/env python3
"""Trailing-padding pass (T-1310): put the original's words after each C function back.

Problem. The original link left zero words (nops) after the last function of every object,
because it aligned each object's .text to 16. Our overlays (and a few mid-file spots) are one
C object holding several original objects, so those words sit in the middle of .text. A
function that is still INCLUDE_ASM carries its own trailing words (splat puts them after its
`endlabel`), but once the function is ported to C they vanish and shift everything after.
asm-processor cannot stand in for them: it needs at least two instructions per block, so a
single trailing nop (end address = 12 mod 16, 124 of the overlay cases) cannot be an
INCLUDE_ASM stub; stubs for 2 or more nops were a per-site source line (T-0700).

Rule (uniform, no per-function control, no source markers). After tools/cc.py compiled an
object, for every function symbol that the C source defines (that is, not named by an
INCLUDE_ASM line, which already holds its words), look up the function's disassembly
`<name>.s` that splat wrote under asm/ (matchings/ or nonmatchings/ of the source file's
segment). The words after its `endlabel` are the trailing padding of the original function;
insert exactly that many zero words at the end of the compiled function
(symbol value + size). The C stays ordinary: the pass reproduces what the original
linker/assembler did and nothing else.

Mechanics. A zero-word insertion in the middle of .text moves later code, so the pass edits
the ELF relocatable: it shifts symbol values and the .text section symbol size, shifts REL
offsets in .text, and fixes the in-place addends of relocations that point at the .text
section symbol (R_MIPS_32 and R_MIPS_26). It fails loudly (SystemExit) on anything it cannot
transform exactly: a function symbol without size, a symbol inside the function,
HI16/LO16 (or other) relocations against the section symbol, trailing lines that are not
nops, an `.s` without `endlabel`.

Tests with synthetic input: tools/test_trailing_pad.py. Docs: wiki/toolchain.md.
"""
import os
import re
import struct
import sys

import srcscan

RODATA_MARK = "# T-1340 rodata (rodata_pieces.py)"  # rodata_pieces.MARK (that module imports yaml)

SHT_SYMTAB = 2
SHT_REL = 9
SHT_NOBITS = 8
STT_SECTION = 3
STT_FUNC = 2
R_MIPS_32 = 2
R_MIPS_26 = 4

ENDLABEL_RE = re.compile(r"^(endlabel|\.end)\b")
NOP_RE = re.compile(r"^\s*/\*[^*]*\*/\s+nop\s*$")


def fail(msg):
    sys.exit("trailing_pad.py: " + msg)


class Elf:
    """Minimal ELF32 little-endian relocatable object: parse, edit sections, write."""

    HDR = struct.Struct("<16sHHIIIIIHHHHHH")
    SHDR = struct.Struct("<IIIIIIIIII")

    def __init__(self, data):
        if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
            fail("not an ELF32 little-endian object")
        (self.ident, self.e_type, self.e_machine, self.e_version, self.e_entry, self.e_phoff,
         e_shoff, self.e_flags, self.e_ehsize, self.e_phentsize, self.e_phnum,
         self.e_shentsize, e_shnum, self.e_shstrndx) = self.HDR.unpack_from(data, 0)
        if self.e_type != 1 or self.e_phnum:
            fail("not a relocatable object")
        self.sections = []
        for i in range(e_shnum):
            f = self.SHDR.unpack_from(data, e_shoff + i * self.e_shentsize)
            sec = dict(zip(("name", "type", "flags", "addr", "offset", "size", "link", "info",
                            "align", "entsize"), f))
            sec["data"] = (bytearray() if sec["type"] in (0, SHT_NOBITS)
                           else bytearray(data[sec["offset"]:sec["offset"] + sec["size"]]))
            self.sections.append(sec)
        strtab = self.sections[self.e_shstrndx]["data"]
        for sec in self.sections:
            end = strtab.index(b"\0", sec["name"])
            sec["sname"] = bytes(strtab[sec["name"]:end]).decode()

    def find(self, name):
        found = [i for i, s in enumerate(self.sections) if s["sname"] == name]
        if len(found) != 1:
            fail("expected exactly one %s section, found %d" % (name, len(found)))
        return found[0]

    def to_bytes(self):
        out = bytearray(self.e_ehsize)
        for sec in self.sections:
            if sec["type"] in (0, SHT_NOBITS):
                continue
            align = max(sec["align"], 1)
            out += b"\0" * (-len(out) % align)
            sec["offset"] = len(out)
            sec["size"] = len(sec["data"])
            out += sec["data"]
        out += b"\0" * (-len(out) % 4)
        shoff = len(out)
        for sec in self.sections:
            out += self.SHDR.pack(sec["name"], sec["type"], sec["flags"], sec["addr"],
                                  sec["offset"], sec["size"], sec["link"], sec["info"],
                                  sec["align"], sec["entsize"])
        self.HDR.pack_into(out, 0, self.ident, self.e_type, self.e_machine, self.e_version,
                           self.e_entry, self.e_phoff, shoff, self.e_flags, self.e_ehsize,
                           self.e_phentsize, self.e_phnum, self.e_shentsize,
                           len(self.sections), self.e_shstrndx)
        return bytes(out)


def read_symbols(elf, symtab):
    """[[name, value, size, info, other, shndx, name_offset]] of the symbol table."""
    sec = elf.sections[symtab]
    strtab = elf.sections[sec["link"]]["data"]
    syms = []
    for off in range(0, len(sec["data"]), 16):
        name, value, size, info, other, shndx = struct.unpack_from("<IIIBBH", sec["data"], off)
        end = strtab.index(b"\0", name)
        syms.append([bytes(strtab[name:end]).decode(), value, size, info, other, shndx, name])
    return syms


def text_functions(path):
    """{name: (value, size)} of the FUNC symbols defined in .text of the object `path`."""
    with open(path, "rb") as f:
        elf = Elf(f.read())
    text = elf.find(".text")
    syms = read_symbols(elf, elf.find(".symtab"))
    return {s[0]: (s[1], s[2]) for s in syms if s[5] == text and s[3] & 0xf == STT_FUNC}


def insert_zero_words(data, pads):
    """Return ELF object bytes with `pads` ({function name: zero words}) inserted after each
    named function (at symbol value + size), all offsets, symbols and relocations fixed."""
    elf = Elf(data)
    text = elf.find(".text")
    symtab = elf.find(".symtab")
    syms = read_symbols(elf, symtab)
    tsec = elf.sections[text]
    tlen = len(tsec["data"])
    # The assembler rounds .text up to 16 after the last function; those zeros belong to no
    # function (cc.py's pad_text puts the original's alignment back), so drop them first.
    code_end = max([s[1] + s[2] for s in syms if s[5] == text and s[3] & 0xf != STT_SECTION],
                   default=0)
    if code_end < tlen:
        if any(tsec["data"][code_end:]):
            fail("non-zero bytes after the last symbol")
        tlen = code_end
        del tsec["data"][code_end:]

    edits = {}                                  # insertion offset -> byte count
    for name, words in pads.items():
        if words <= 0:
            fail("%s: pad of %d words" % (name, words))
        hit = [s for s in syms if s[0] == name and s[5] == text and s[3] & 0xf == STT_FUNC]
        if len(hit) != 1:
            fail("%s: expected one function symbol in .text, found %d" % (name, len(hit)))
        value, size = hit[0][1], hit[0][2]
        if size == 0:
            fail("%s: function symbol has no size" % name)
        at = value + size
        if at % 4 or at > tlen:
            fail("%s: bad function end 0x%x" % (name, at))
        if at in edits:
            fail("%s: two pads at 0x%x" % (name, at))
        for s in syms:
            if s[5] == text and value < s[1] < at:
                fail("%s: symbol %s inside the function" % (name, s[0]))
        edits[at] = 4 * words
    cuts = sorted(edits)

    def shift(x):
        return x + sum(edits[c] for c in cuts if x >= c)

    new = bytearray()
    prev = 0
    for c in cuts:
        new += tsec["data"][prev:c] + b"\0" * edits[c]
        prev = c
    new += tsec["data"][prev:]
    tsec["data"] = new

    ssec = elf.sections[symtab]
    for i, s in enumerate(syms):
        if s[5] != text:
            continue
        if s[3] & 0xf == STT_SECTION:
            s[2] = len(new)
        else:
            s[1] = shift(s[1])
        struct.pack_into("<IIIBBH", ssec["data"], 16 * i, s[6], s[1], s[2], s[3], s[4], s[5])

    for sec in elf.sections:
        if sec["type"] != SHT_REL:
            continue
        target = sec["info"]
        tdata = elf.sections[target]["data"]
        for off in range(0, len(sec["data"]), 8):
            r_offset, r_info = struct.unpack_from("<II", sec["data"], off)
            rsym, rtype = r_info >> 8, r_info & 0xff
            if target == text:
                r_offset = shift(r_offset)
                struct.pack_into("<II", sec["data"], off, r_offset, r_info)
            s = syms[rsym] if rsym else None
            if s is None or s[5] != text or s[3] & 0xf != STT_SECTION:
                continue                        # symbol-relative: the symbol itself moved
            if rtype == R_MIPS_32:
                w, = struct.unpack_from("<I", tdata, r_offset)
                struct.pack_into("<I", tdata, r_offset, shift(w))
            elif rtype == R_MIPS_26:
                w, = struct.unpack_from("<I", tdata, r_offset)
                tgt = shift((w & 0x3ffffff) << 2)
                if tgt >> 28:
                    fail("jump target out of range after padding")
                struct.pack_into("<I", tdata, r_offset, (w & ~0x3ffffff) | (tgt >> 2))
            else:
                fail("relocation type %d against the .text section symbol at 0x%x "
                     "(only R_MIPS_32 and R_MIPS_26 are supported)" % (rtype, r_offset))
    return elf.to_bytes()


def pad_text_to(path, align):
    """Zero-pad .text of the object `path` to a multiple of `align` bytes.

    Done on the ELF directly: `objcopy --update-section .text=` with a longer section silently
    drops every relocation of the object (found in T-1310), which pad_text must not do.
    """
    with open(path, "rb") as f:
        elf = Elf(f.read())
    tsec = elf.sections[elf.find(".text")]
    extra = -len(tsec["data"]) % align
    if not extra:
        return
    tsec["data"] += b"\0" * extra
    symtab = elf.find(".symtab")
    for i, s in enumerate(read_symbols(elf, symtab)):
        if s[5] == elf.find(".text") and s[3] & 0xf == STT_SECTION:
            struct.pack_into("<I", elf.sections[symtab]["data"], 16 * i + 8, len(tsec["data"]))
    with open(path, "wb") as f:
        f.write(elf.to_bytes())


def trailing_words(path):
    """Number of nop words after the `endlabel` line of a splat function file. The rodata that
    tools/rodata_pieces.py appends after a marker line (T-1340) is not part of the function."""
    with open(path) as f:
        lines = f.read().split(RODATA_MARK)[0].splitlines()
    ends = [i for i, l in enumerate(lines) if ENDLABEL_RE.match(l)]
    if not ends:
        fail("%s: no endlabel line" % path)
    tail = [l for l in lines[ends[-1] + 1:] if l.strip()]
    for l in tail:
        if not NOP_RE.match(l):
            fail("%s: trailing line is not a nop: %s" % (path, l.strip()))
    return len(tail)


def asm_dirs(src):
    """Directories of splat function files for the C source `src`, or [] (srcscan's path rule;
    src/main/<addr>.c, src/ovl/<NAME>.c and the per-object src/ovl/<NAME>/<addr>.c, T-0500)."""
    return srcscan.asm_dirs_of_source(src)


def find_asm(src, name, root="."):
    for d in asm_dirs(src):
        path = os.path.join(root, d, name + ".s")
        if os.path.exists(path):
            return path
    return None


def pad_functions(obj, src, include_asm_names, root="."):
    """Apply the rule to the compiled object `obj` of source `src`; return the .s files read."""
    used = []
    pads = {}
    for name in text_functions(obj):
        if name in include_asm_names:
            continue
        path = find_asm(src, name, root)
        if path is None:
            continue                            # static helper or SDK-only symbol: no original words
        used.append(path)
        words = trailing_words(path)
        if words:
            pads[name] = words
    if pads:
        with open(obj, "rb") as f:
            data = f.read()
        with open(obj, "wb") as f:
            f.write(insert_zero_words(data, pads))
    return used
