#!/usr/bin/env python3
"""Parse disc/files/CDROM/EXEDIR/O.BIN (little-endian MIPS ECOFF) and print its symbols.

O.BIN is a stripped ECOFF whose only symbol data is the external symbol table of
the mdebug symbolic header (see wiki/obin.md). The file is read at run time from
disc/; nothing from it is stored in the repo.

Usage: python3 tools/obin_syms.py [--file PATH] [--sections]
Output: ADDR NAME KIND SC SIZE, tab separated. SIZE is "~N", the gap to the next
higher symbol address (an upper bound; O.BIN stores no sizes), or "-" for the last.
--sections prints the ECOFF section table instead.
Exits non-zero if the file is not the expected ECOFF layout.
"""
import argparse
import collections
import struct
import sys

DEFAULT = "disc/files/CDROM/EXEDIR/O.BIN"
ST = {1: "Global", 6: "Proc"}
SC = {1: "Text", 2: "Data", 5: "Abs"}
HDRR_NAMES = ("ilineMax cbLine cbLineOffset idnMax cbDnOffset ipdMax cbPdOffset "
              "isymMax cbSymOffset ioptMax cbOptOffset iauxMax cbAuxOffset issMax "
              "cbSsOffset issExtMax cbSsExtOffset ifdMax cbFdOffset crfd "
              "cbRfdOffset iextMax cbExtOffset").split()

Symbol = collections.namedtuple("Symbol", "name addr st sc size")
Section = collections.namedtuple("Section", "name vaddr size scnptr flags")


def parse(data):
    """Return (sections, symbols); symbols are sorted by address, size is the gap or None."""
    magic, nscns, _, symptr, _, opthdr, _ = struct.unpack_from("<HHIIIHH", data, 0)
    if magic != 0x0162 or opthdr != 0x38:
        raise ValueError("not a little-endian MIPS ECOFF with a 0x38 optional header")
    secs = []
    for i in range(nscns):
        name, _, vaddr, size, scnptr, _, _, _, _, flags = struct.unpack_from(
            "<8sIIIIIIHHI", data, 20 + opthdr + 40 * i)
        secs.append(Section(name.rstrip(b"\0").decode("ascii"), vaddr, size, scnptr, flags))
    if struct.unpack_from("<H", data, symptr)[0] != 0x7009:
        raise ValueError("bad symbolic header magic")
    h = dict(zip(HDRR_NAMES, struct.unpack_from("<23i", data, symptr + 4)))
    raw = []
    for i in range(h["iextMax"]):
        _, _, iss, value, bits = struct.unpack_from("<hhiII", data, h["cbExtOffset"] + 16 * i)
        start = h["cbSsExtOffset"] + iss
        name = data[start:data.index(b"\0", start)].decode("ascii")
        raw.append((value, name, bits & 63, (bits >> 6) & 31))
    raw.sort()
    syms = []
    for n, (addr, name, st, sc) in enumerate(raw):
        nxt = next((r[0] for r in raw[n + 1:] if r[0] > addr), None)
        syms.append(Symbol(name, addr, st, sc, None if nxt is None else nxt - addr))
    return secs, syms


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--file", default=DEFAULT)
    ap.add_argument("--sections", action="store_true")
    a = ap.parse_args()
    try:
        with open(a.file, "rb") as f:
            secs, syms = parse(f.read())
    except (OSError, ValueError, struct.error) as e:
        print("obin_syms: %s" % e, file=sys.stderr)
        return 1
    if a.sections:
        for s in secs:
            print("%-8s vaddr=%08x size=%05x fileoff=%05x flags=%08x" % s)
        return 0
    for s in syms:
        print("%08x\t%s\t%s\t%s\t%s" % (s.addr, s.name, ST.get(s.st, s.st), SC.get(s.sc, s.sc),
                                       "-" if s.size is None else "~%d" % s.size))
    return 0


if __name__ == "__main__":
    sys.exit(main())
