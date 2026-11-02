"""Minimal reader for little-endian ELF32 MIPS objects and executables (T-39505).

Sections, the symbol table and REL relocations: what tools/raw_addrs.py and tools/english_link.py
need, without a pyelftools dependency in the Docker image.
"""
import struct

SHT_SYMTAB = 2
SHT_NOBITS = 8
SHT_REL = 9
SHN_UNDEF = 0
SHN_ABS = 0xFFF1
SHN_COMMON = 0xFFF2

R_MIPS_32 = 2
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6


class Section:
    def __init__(self, index, name, sh_type, flags, addr, offset, size, link, info, align, entsize):
        self.index = index
        self.name = name
        self.type = sh_type
        self.flags = flags
        self.addr = addr
        self.offset = offset
        self.size = size
        self.link = link
        self.info = info
        self.align = align
        self.entsize = entsize


class Symbol:
    def __init__(self, name, value, size, info, shndx):
        self.name = name
        self.value = value
        self.size = size
        self.bind = info >> 4
        self.type = info & 0xF
        self.shndx = shndx


class Elf:
    def __init__(self, path):
        self.path = path
        self.data = open(path, "rb").read()
        d = self.data
        if d[:4] != b"\x7fELF" or d[4] != 1 or d[5] != 1:
            raise ValueError("%s: not a little-endian ELF32 file" % path)
        (self.type, _machine, _version, self.entry, _phoff, shoff, _flags, _ehsize, _phentsize,
         _phnum, shentsize, shnum, shstrndx) = struct.unpack_from("<HHIIIIIHHHHHH", d, 16)
        raw = [struct.unpack_from("<IIIIIIIIII", d, shoff + i * shentsize) for i in range(shnum)]
        strtab = raw[shstrndx][4]
        self.sections = [Section(i, self._str(strtab, r[0]), *r[1:]) for i, r in enumerate(raw)]
        self._symbols = None

    def _str(self, table_offset, index):
        end = self.data.index(b"\0", table_offset + index)
        return self.data[table_offset + index:end].decode("latin-1")

    def section(self, name):
        for s in self.sections:
            if s.name == name:
                return s
        return None

    def contents(self, sec):
        if sec.type == SHT_NOBITS:
            return b""
        return self.data[sec.offset:sec.offset + sec.size]

    @property
    def symbols(self):
        if self._symbols is None:
            self._symbols = []
            for s in self.sections:
                if s.type != SHT_SYMTAB:
                    continue
                strtab = self.sections[s.link].offset
                for i in range(s.size // 16):
                    name, value, size, info, _other, shndx = struct.unpack_from(
                        "<IIIBBH", self.data, s.offset + i * 16)
                    self._symbols.append(Symbol(self._str(strtab, name), value, size, info, shndx))
        return self._symbols

    def relocations(self, sec):
        """[(offset, type, symbol)] of the REL section that applies to `sec`."""
        out = []
        for r in self.sections:
            if r.type == SHT_REL and r.info == sec.index:
                for i in range(r.size // 8):
                    offset, info = struct.unpack_from("<II", self.data, r.offset + i * 8)
                    out.append((offset, info & 0xFF, self.symbols[info >> 8]))
        return out
