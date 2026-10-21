#!/usr/bin/env python3
"""Scratch uopt shim for T-24925: cvt_pass pipeline with the OPTN 405 value of the
8th procedure (func_8013CCE4 in ETC/8013BEF0.c) flipped to 1, plus stage dumps.

Same calling convention as uopt_shim.py (--uopt <real> <args...>).
"""
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, "tools")
import cvt_pass

PROC = int(os.environ.get("UCOPTN_PROC", "8"))   # 1-based ENT count = func_8013CCE4


def flip(insns):
    """Set the OPTN 405 record preceding procedure #PROC to val=1."""
    seen = 0
    for k, i in enumerate(insns):
        if i.opc == cvt_pass.OP["optn"] and i.words[1] == cvt_pass.UCO_ZCOPY:
            seen += 1
            if seen == PROC:
                if i.words[2] == 1:
                    return insns, False
                i.words[2] = 1
                return insns, True
    return insns, False


def dump(insns, path):
    with open(path, "wb") as f:
        f.write(cvt_pass.serialize(insns))


def run(argv):
    real, args = argv[1], argv[2:]
    out_dir = os.environ.get("UCODE_DUMP_DIR")
    tag = os.environ.get("UCODE_DUMP_TAG", "u")
    files, i = [], 0
    while i < len(args):
        if args[i] in cvt_pass.VALUED:
            i += 2
        elif args[i].startswith("-"):
            i += 1
        else:
            files.append(i)
            i += 1
    if len(files) < 2:
        return cvt_pass.run_uopt(real, args)
    inp, outp = files[0], files[1]
    with open(args[inp], "rb") as f:
        raw = cvt_pass.parse(f.read())
    w = cvt_pass.widen(raw)
    cpo = cvt_pass.copy_propagation_options(w)
    cpo, flipped = flip(cpo)
    insns, rewritten = cvt_pass.pre(cpo)
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)
        dump(insns, os.path.join(out_dir, "%s_4_uopt_input.uc" % tag))
        sys.stderr.write("flip: OPTN proc %d -> 1: %s\n" % (PROC, flipped))
    fd, tmp = tempfile.mkstemp(prefix="ucflip", suffix=".B")
    with os.fdopen(fd, "wb") as f:
        f.write(cvt_pass.serialize(insns))
    call = list(args)
    call[inp] = tmp
    try:
        rc = subprocess.call([real] + call)
    finally:
        os.unlink(tmp)
    if rc == 0:
        with open(args[outp], "rb") as f:
            out = cvt_pass.parse(f.read())
        if out_dir:
            dump(out, os.path.join(out_dir, "%s_5_uopt_output.uc" % tag))
        if rewritten:
            out = cvt_pass.post(out, rewritten)
        with open(args[outp], "wb") as f:
            f.write(cvt_pass.serialize(out))
    return rc


if __name__ == "__main__":
    sys.exit(run(sys.argv[1:]))
