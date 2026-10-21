#!/usr/bin/env python3
"""Scratch uopt shim for T-24925: cvt_pass's exact pipeline with ucode stage dumps.

Called by cc.py's ido_frame_env wrapper as:
    python3 uopt_shim.py --uopt /opt/ido/5.3/uopt <uopt args...>

Dumps (UCODE_DUMP_DIR, tag = UCODE_DUMP_TAG) one file per stage:
    1_raw_cfe      ucode as cfe wrote it (uopt input)
    2_widen        after cvt_pass.widen (K&R widening put back)
    3_cpo          after cvt_pass.copy_propagation_options (OPTN 405 records)
    4_uopt_input   after cvt_pass.pre (what the real uopt reads)
    5_uopt_output  the real uopt's output, before post
    6_post         after cvt_pass.post (what ugen reads; only when rewritten)
"""
import os
import subprocess
import sys
import tempfile

sys.path.insert(0, "tools")
import cvt_pass


def dump(insns, path):
    with open(path, "wb") as f:
        f.write(cvt_pass.serialize(insns))


def run(argv):
    real, args = argv[1], argv[2:]
    out_dir = os.environ.get("UCODE_DUMP_DIR")
    tag = os.environ.get("UCODE_DUMP_TAG", "u")

    def path_of(name):
        return os.path.join(out_dir, "%s_%s.uc" % (tag, name))

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
    if out_dir:
        os.makedirs(out_dir, exist_ok=True)
        dump(raw, path_of("1_raw_cfe"))
    w = cvt_pass.widen(raw)
    if out_dir:
        dump(w, path_of("2_widen"))
    cpo = cvt_pass.copy_propagation_options(w)
    if out_dir:
        dump(cpo, path_of("3_cpo"))
    insns, rewritten = cvt_pass.pre(cpo)
    if out_dir:
        dump(insns, path_of("4_uopt_input"))
    fd, tmp = tempfile.mkstemp(prefix="ucdump", suffix=".B")
    with os.fdopen(fd, "wb") as f:
        f.write(cvt_pass.serialize(insns))
    call = list(args)
    call[inp] = tmp
    try:
        rc = subprocess.call([real] + call)
    finally:
        os.unlink(tmp)
    if rc == 0 and out_dir:
        with open(args[outp], "rb") as f:
            uopt_out = cvt_pass.parse(f.read())
        dump(uopt_out, path_of("5_uopt_output"))
        if rewritten:
            out = cvt_pass.post(uopt_out, rewritten)
            with open(args[outp], "wb") as f:
                f.write(cvt_pass.serialize(out))
            dump(out, path_of("6_post"))
    elif rc == 0 and rewritten:
        with open(args[outp], "rb") as f:
            out = cvt_pass.post(cvt_pass.parse(f.read()), rewritten)
        with open(args[outp], "wb") as f:
            f.write(cvt_pass.serialize(out))
    return rc


if __name__ == "__main__":
    sys.exit(run(sys.argv[1:]))
