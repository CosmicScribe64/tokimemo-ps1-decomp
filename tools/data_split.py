#!/usr/bin/env python3
"""Cut the `.data` of a compiled C object into the sections of its data islands (T-25005).

IDO writes all of an object's data to one `.data` section, and a linker cannot place parts of one
input section at different addresses. When an object owns several `.data` ranges (yaml `.data`
islands of the same name, tools/data_pieces.py), tools/object_layout.py calls `split()` after
cc.py: the bytes from the start symbol of each range up to the next range form one part, and
each part goes to the section the yaml gives its range (`.data` or `.data_<VRAM>`), which the
splat linker script places at the range's address.

The object's `.data` must be exactly those parts back to back, in some order (IDO emits data in
source order, so the C file decides it), each as long as its range. Symbols move with their part.
A relocation against the `.data` section symbol (IDO's references to unnamed data such as the
template of a local aggregate initialiser) whose addend lies in a moved part is retargeted to the
part's section symbol with the addend made relative to the part: R_MIPS_32 words and
R_MIPS_HI16/LO16 pairs (one HI16 may serve several LO16s, which must then stay in one part).
Relocations inside a moved part go to a new `.rel.<section>`. Anything else against `.data` fails.
No byte of an instruction changes except the immediates of such retargeted pairs, which the
linker resolves to the same final address.
"""
import struct

from trailing_pad import SHT_REL, Elf, read_symbols

STT_SECTION = 3
R_MIPS_32, R_MIPS_HI16, R_MIPS_LO16 = 2, 5, 6
SYM = struct.Struct("<IIIBBH")


class SplitError(Exception):
    """The object's .data cannot be cut into the configured parts exactly."""


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def add_name(elf, name):
    strtab = elf.sections[elf.e_shstrndx]["data"]
    off = len(strtab)
    strtab += name.encode() + b"\0"
    return off


def parts_of(elf, data_index, regions):
    """[(lo, hi, section name)] in object order for regions [(section, start symbol, size)]."""
    syms = read_symbols(elf, elf.find(".symtab"))
    size = len(elf.sections[data_index]["data"])
    found = []
    for section, start, length in regions:
        offs = [s[1] for s in syms if s[0] == start and s[5] == data_index and s[3] & 15 != STT_SECTION]
        if len(offs) != 1:
            raise SplitError("start symbol %s of %s is not in the object's .data" % (start, section))
        found.append((offs[0], offs[0] + length, section))
    found.sort()
    pos = 0
    for lo, hi, section in found:
        if lo != pos:
            raise SplitError(".data part %s starts at +0x%X, expected +0x%X (parts must be back to back)"
                             % (section, lo, pos))
        pos = hi
    if pos != size:
        raise SplitError(".data is 0x%X bytes, its parts 0x%X" % (size, pos))
    if [p[2] for p in found].count(".data") != 1:
        raise SplitError("exactly one part must stay in .data")
    return found


def split(data, regions):
    """ELF bytes with .data cut into the parts of regions [(section, start symbol, size)]."""
    elf = Elf(data)
    di = elf.find(".data")
    st = elf.find(".symtab")
    parts = parts_of(elf, di, regions)
    old = elf.sections[di]
    # section index of every part: .data keeps its index, the others are appended
    index = []
    for lo, hi, name in parts:
        if name == ".data":
            index.append(di)
            continue
        elf.sections.append(dict(old, name=add_name(elf, name), sname=name, data=bytearray(old["data"][lo:hi])))
        index.append(len(elf.sections) - 1)
    new_secs = [i for i in index if i != di]

    def part(off):
        for k, (lo, hi, _n) in enumerate(parts):
            if lo <= off < hi:
                return k
        raise SplitError(".data offset +0x%X is outside every part" % off)

    # symbols: locals, then one section symbol per new section, then globals
    symsec = elf.sections[st]
    nlocal = symsec["info"]
    syms = [list(SYM.unpack_from(symsec["data"], o)) for o in range(0, len(symsec["data"]), 16)]
    data_syms = {i for i, s in enumerate(syms) if s[5] == di and s[3] & 15 == STT_SECTION}
    for s in syms:
        if s[5] == di and s[3] & 15 != STT_SECTION:
            k = part(s[1])
            s[1] -= parts[k][0]
            s[5] = index[k]
    added = [[0, 0, 0, STT_SECTION, 0, i] for i in new_secs]
    sec_sym = {i: nlocal + n for n, i in enumerate(new_secs)}
    if any(i >= nlocal for i in data_syms):
        raise SplitError(".data section symbol is not local")
    syms = syms[:nlocal] + added + syms[nlocal:]

    def renum(i):
        return i + len(added) if i >= nlocal else i

    # relocations: retarget addends into moved parts, then cut .rel.data
    for rsec in [s for s in elf.sections if s["type"] == SHT_REL]:
        if rsec["link"] != st:
            raise SplitError("%s does not use .symtab" % rsec["sname"])
        target = elf.sections[rsec["info"]]["data"]
        rel = [list(struct.unpack_from("<II", rsec["data"], o)) for o in range(0, len(rsec["data"]), 8)]
        pending, group = [], {}
        for e in rel:
            sym, typ = e[1] >> 8, e[1] & 0xFF
            e[1] = (renum(sym) << 8) | typ
            if sym in data_syms:   # a local: same index after renumbering
                retarget(e, sym, typ, target, parts, index, di, sec_sym, pending, group)
        if pending:
            raise SplitError("%s: R_MIPS_HI16 against .data without R_MIPS_LO16" % rsec["sname"])
        rsec["data"] = bytearray(b"".join(struct.pack("<II", *e) for e in rel))
        rsec["rel"] = rel
    rdata = [s for s in elf.sections if s["type"] == SHT_REL and s["info"] == di]
    for rsec in rdata:
        keep = []
        moved = {i: [] for i in new_secs}
        for off, info in rsec["rel"]:
            k = part(off)
            if index[k] == di:
                keep.append((off - parts[k][0], info))
            else:
                moved[index[k]].append((off - parts[k][0], info))
        rsec["data"] = bytearray(b"".join(struct.pack("<II", *e) for e in keep))
        for i, entries in moved.items():
            if entries:
                name = ".rel" + elf.sections[i]["sname"]
                elf.sections.append(dict(rsec, name=add_name(elf, name), sname=name, info=i,
                                         data=bytearray(b"".join(struct.pack("<II", *e) for e in entries))))
    for rsec in elf.sections:
        rsec.pop("rel", None)
    k0 = index.index(di)
    old["data"] = bytearray(old["data"][parts[k0][0]:parts[k0][1]])
    symsec["data"] = bytearray(b"".join(SYM.pack(*s) for s in syms))
    symsec["info"] = nlocal + len(added)
    return elf.to_bytes()


def retarget(e, sym, typ, target, parts, index, di, sec_sym, pending, group):
    """Rewrite relocation e = [offset, info] against the .data section symbol in place."""
    def where(addend):
        for k, (lo, hi, _n) in enumerate(parts):
            if lo <= addend < hi:
                return k
        raise SplitError("addend .data+0x%X is outside every part" % addend)

    def move(k):
        return index[k] != di or parts[k][0] != 0

    def symbol(k):
        return sym if index[k] == di else sec_sym[index[k]]

    if typ == R_MIPS_32:
        a = struct.unpack_from("<I", target, e[0])[0]
        k = where(a)
        if move(k):
            struct.pack_into("<I", target, e[0], a - parts[k][0])
            e[1] = (symbol(k) << 8) | typ
    elif typ == R_MIPS_HI16:
        pending.append(e)
    elif typ == R_MIPS_LO16:
        word = struct.unpack_from("<I", target, e[0])[0]
        if pending:
            hi = struct.unpack_from("<I", target, pending[0][0])[0] & 0xFFFF
            a = (hi << 16) + sext16(word & 0xFFFF)
            k = where(a)
            new = a - parts[k][0]
            hi_new = ((new + 0x8000) >> 16) & 0xFFFF
            for h in pending:
                hw = struct.unpack_from("<I", target, h[0])[0]
                if hw & 0xFFFF != hi:
                    raise SplitError("R_MIPS_HI16 group with different immediates at 0x%X" % h[0])
                if move(k):
                    struct.pack_into("<I", target, h[0], (hw & 0xFFFF0000) | hi_new)
                    h[1] = (symbol(k) << 8) | R_MIPS_HI16
            group[sym] = (k, hi, hi_new)
            del pending[:]
        elif sym in group:
            k0, hi, hi_new = group[sym]
            a = (hi << 16) + sext16(word & 0xFFFF)
            k = where(a)
            if k != k0:
                raise SplitError("R_MIPS_LO16 at 0x%X shares a HI16 with another .data part" % e[0])
        else:
            raise SplitError("R_MIPS_LO16 at 0x%X against .data without R_MIPS_HI16" % e[0])
        k, hi, hi_new = group[sym]
        a = (hi << 16) + sext16(word & 0xFFFF)
        if move(k):
            lo = a - parts[k][0] - (hi_new << 16)
            if not -0x8000 <= lo < 0x8000:
                raise SplitError("R_MIPS_LO16 at 0x%X out of range after the split" % e[0])
            struct.pack_into("<I", target, e[0], (word & 0xFFFF0000) | (lo & 0xFFFF))
            e[1] = (symbol(k) << 8) | typ
    else:
        raise SplitError("relocation type %d against .data at 0x%X is not supported" % (typ, e[0]))
