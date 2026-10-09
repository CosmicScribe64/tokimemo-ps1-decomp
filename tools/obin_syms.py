#!/usr/bin/env python3
"""Parse disc/files/CDROM/EXEDIR/O.BIN (little-endian MIPS ECOFF) and print its symbols.

O.BIN is a stripped ECOFF object (magic 0x0162) whose only symbol data is the
external symbol table of the mdebug symbolic header (see wiki/obin.md). The
file is read at run time from disc/; nothing from it is stored in the repo.

Usage: python3 tools/obin_syms.py [--file PATH] [--format tsv|json|sections]
Output (tsv): ADDR NAME KIND SC SIZE
  KIND  ECOFF symbol type (Global, Proc, ...), SC storage class (Text, Data, Abs).
  SIZE  from the procedure descriptor when present, else the gap to the next
        higher symbol address (prefixed "~", an upper bound), else "-".
Exits non-zero if the file is not the expected ECOFF layout.
"""
import argparse
import json
import struct
import sys

DEFAULT = "disc/files/CDROM/EXEDIR/O.BIN"
ST = {0: "Nil", 1: "Global", 2: "Static", 3: "Param", 4: "Local", 5: "Label",
      6: "Proc", 7: "Block", 8: "End", 9: "Member", 10: "Typedef", 11: "File",
      14: "StaticProc", 26: "Constant"}
SC = {0: "Nil", 1: "Text", 2: "Data", 3: "Bss", 4: "Register", 5: "Abs",
      6: "Undefined", 7: "CdbLocal", 8: "Bits", 9: "CdbSystem", 10: "RegImage",
      11: "Info", 12: "UserStruct", 13: "SData", 14: "SBss", 15: "RData",
      16: "Var", 17: "Common", 18: "SCommon", 19: "VarRegister", 20: "Variant",
      21: "SUndefined", 22: "Init", 23: "BasedVar", 24: "XData", 25: "PData",
      26: "Fini", 27: "NonGp"}
HDRR_NAMES = ("ilineMax cbLine cbLineOffset idnMax cbDnOffset ipdMax cbPdOffset "
              "isymMax cbSymOffset ioptMax cbOptOffset iauxMax cbAuxOffset issMax "
              "cbSsOffset issExtMax cbSsExtOffset ifdMax cbFdOffset crfd "
              "cbRfdOffset iextMax cbExtOffset").split()


def parse(data):
    """Return (filehdr dict, aouthdr dict, sections list, symbols list)."""
    magic, nscns, timdat, symptr, nsyms, opthdr, flags = struct.unpack_from("<HHIIIHH", data, 0)
    if magic != 0x0162 or opthdr != 0x38:
        raise ValueError("not a little-endian MIPS ECOFF with a 0x38 optional header")
    fh = dict(magic=magic, nscns=nscns, timdat=timdat, symptr=symptr, nsyms=nsyms,
              opthdr=opthdr, flags=flags)
    amagic, vstamp, tsize, dsize, bsize, entry, text_start, data_start, bss_start = \
        struct.unpack_from("<HHIIIIIII", data, 20)
    gprmask, cprmask0, cprmask1, cprmask2, cprmask3, gp_value = \
        struct.unpack_from("<6I", data, 20 + 32)
    ah = dict(magic=amagic, vstamp=vstamp, tsize=tsize, dsize=dsize, bsize=bsize,
              entry=entry, text_start=text_start, data_start=data_start,
              bss_start=bss_start, gprmask=gprmask, gp_value=gp_value)
    secs = []
    for i in range(nscns):
        o = 20 + opthdr + 40 * i
        name, paddr, vaddr, size, scnptr, relptr, lnnoptr, nreloc, nlnno, sflags = \
            struct.unpack_from("<8sIIIIIIHHI", data, o)
        secs.append(dict(name=name.rstrip(b"\0").decode("ascii"), vaddr=vaddr,
                         size=size, scnptr=scnptr, flags=sflags))
    h = dict(zip(HDRR_NAMES, struct.unpack_from("<23i", data, symptr + 4)))
    hmagic = struct.unpack_from("<H", data, symptr)[0]
    if hmagic != 0x7009:
        raise ValueError("bad symbolic header magic %#x" % hmagic)
    fh["hdrr"] = h
    syms = []
    for i in range(h["iextMax"]):
        flg, ifd, iss, value, bits = struct.unpack_from("<hhiiI", data, h["cbExtOffset"] + 16 * i)
        name_off = h["cbSsExtOffset"] + iss
        end = data.index(b"\0", name_off)
        st, sc = bits & 63, (bits >> 6) & 31
        syms.append(dict(name=data[name_off:end].decode("ascii"),
                         addr=value & 0xFFFFFFFF, st=st, sc=sc, index=bits >> 12,
                         size=None, derived=False))
    # Procedure descriptors give exact bounds for the few procs that have them.
    pd = {}
    for i in range(h["ipdMax"]):
        adr, isym = struct.unpack_from("<Ii", data, h["cbPdOffset"] + 52 * i)
        pd[adr] = isym
    syms.sort(key=lambda s: (s["addr"], s["name"]))
    for n, s in enumerate(syms):
        nxt = next((t["addr"] for t in syms[n + 1:] if t["addr"] > s["addr"]), None)
        if nxt is not None:
            s["size"], s["derived"] = nxt - s["addr"], True
    return fh, ah, secs, syms


def kind(s):
    return ST.get(s["st"], str(s["st"])), SC.get(s["sc"], str(s["sc"]))


def main():
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--file", default=DEFAULT)
    ap.add_argument("--format", choices=("tsv", "json", "sections"), default="tsv")
    a = ap.parse_args()
    try:
        with open(a.file, "rb") as f:
            data = f.read()
        fh, ah, secs, syms = parse(data)
    except (OSError, ValueError, struct.error) as e:
        print("obin_syms: %s" % e, file=sys.stderr)
        return 1
    if a.format == "sections":
        for s in secs:
            print("%-8s vaddr=%08x size=%05x fileoff=%05x flags=%08x" %
                  (s["name"], s["vaddr"], s["size"], s["scnptr"], s["flags"]))
        print("symbols=%d" % len(syms))
    elif a.format == "json":
        json.dump([dict(s, kind=kind(s)[0], sc_name=kind(s)[1]) for s in syms], sys.stdout)
    else:
        for s in syms:
            k, c = kind(s)
            size = "-" if s["size"] is None else ("~%d" % s["size"])
            print("%08x\t%s\t%s\t%s\t%s" % (s["addr"], s["name"], k, c, size))
    return 0


if __name__ == "__main__":
    sys.exit(main())
