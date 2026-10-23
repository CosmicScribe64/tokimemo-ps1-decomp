#!/usr/bin/env python3
"""Pretty-print an IDO ucode file (cvt_pass record layout) for diffing. T-24925.

Usage: python3 drafts/T-24925/ucpretty.py <file.uc> [<file2.uc> ...]
Prints one line per record; procedures separated by their ENT/END.
SSYM records build a symbol map so ENT lines carry the procedure name.
"""
import struct
import sys

sys.path.insert(0, "tools")
import cvt_pass

MT = {v: k for k, v in
      ((getattr(cvt_pass, n, None), n) for n in ("MT_S", "MT_M", "MT_R"))}


def symbols(insns):
    """symbol number -> name, from SSYM records (name in extra, symnum in words[1])."""
    out = {}
    for i in insns:
        if i.opc == cvt_pass.OP["ssym"] and i.extra:
            out[i.words[1]] = i.extra.rstrip(b"\x00").decode("ascii", "replace")
    return out


def line(idx, i, syms):
    words = i.words
    opc = cvt_pass.OPS[i.opc]
    dt = cvt_pass.DTYPES[i.dtype] if i.dtype < len(cvt_pass.DTYPES) else "?"
    mt = MT.get(i.mtype, str(i.mtype))
    w = " ".join("%x" % x for x in words[1:])
    extra = ""
    if i.opc in (cvt_pass.OP["lod"], cvt_pass.OP["str"], cvt_pass.OP["rlod"],
                 cvt_pass.OP["rlda"], cvt_pass.OP["ilda"], cvt_pass.OP["istr"],
                 cvt_pass.OP["istv"]):
        mtype, block, off, length = i.location()
        if i.mtype == cvt_pass.MT_S:
            extra = " S%s+%#x len=%d" % (syms.get(block, "sym%d" % block), off, length)
        elif i.mtype == cvt_pass.MT_R:
            extra = " R%d+%#x len=%d" % (block, off, length)
        else:
            extra = " m%d,%#x,%#x,len=%d" % (mtype, block, off, length)
        extra = " loc=(%s)" % extra.strip()
    elif i.opc == cvt_pass.OP["vreg"]:
        extra = " temp=(%d,%d)" % (words[1], words[3])
    elif i.opc == cvt_pass.OP["cvt"]:
        src = (words[2] >> 24)
        extra = " %s<-%s" % (dt, cvt_pass.DTYPES[src] if src < len(cvt_pass.DTYPES) else str(src))
    elif i.opc in cvt_pass.HASCONST:
        val = words[4] if len(words) > 4 else None
        if val is not None:
            extra = " const=%#x (%d)" % (val, val if val < 0x80000000 else val - (1 << 32))
    elif i.opc == cvt_pass.OP["optn"]:
        extra = " optn=%d val=%d" % (words[1], words[2])
    elif i.opc == cvt_pass.OP["ent"]:
        extra = " %s" % (syms.get(words[1], "sym%x?" % words[1]) if len(words) > 1 else "")
    elif i.opc == cvt_pass.OP["ssym"] and i.extra:
        extra = " %r" % i.extra.rstrip(b"\x00").decode("ascii", "replace")
    return "%5d %-5s dt=%s mt=%-2s lx=%-4x %s%s" % (idx, opc, dt, mt, i.lexlev, w, extra)


def main(argv):
    for path in argv[1:]:
        with open(path, "rb") as f:
            data = f.read()
        insns = cvt_pass.parse(data)
        syms = symbols(insns)
        print("### %s (%d records)" % (path, len(insns)))
        for idx, i in enumerate(insns):
            if i.opc == cvt_pass.OP["ent"]:
                print("")
            print(line(idx, i, syms))
        print("")


if __name__ == "__main__":
    main(sys.argv)
