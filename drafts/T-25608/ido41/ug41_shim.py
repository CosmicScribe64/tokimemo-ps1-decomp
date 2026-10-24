#!/usr/bin/env python3
"""ugen shim for the T-25608 check of the block-copy state rules against IDO 4.1's ugen.

Installed through tools/cc.py `ido_frame_env(..., extra_shims={"ugen": this file})`, so the
5.3 driver calls it as `ug41_shim.py --ugen /opt/ido/5.3/ugen <ugen args...>`.

UG41_MODE selects the ugen that runs:
  41    IDO 4.1's ugen (tools/local-compilers/ido4.1, gitignored) under qemu-irix-4.0, as
        tools/ido_eval.py runs it; its `.option pic 1` record is set to 0 for IDO 5.3's as1
        (tools/ido_eval.py PIC_OPTION, T-20700)
  raw   IDO 5.3's ugen without a pass
  PATH  a tools/ugen_pass.py variant (the build's, or a candidate rule's)
UG41_FORCE (mode 41 only), e.g. `2:-FFF`: before 4.1's ugen runs, tools/ugen_pass.py's F gadget
(`Uistr` of $zero to -0x7777, the sentinel `sb` is stripped from the binasm afterwards) is put
before each Umov marked F of the procedure with that index among the procedures that have a Umov
(0-based, file order); `-` leaves a Umov alone. In 4.1 an indirect store sets state F, so
comparing the output with and without the gadget shows whether the copy was F already.
UG41_SAVE, if set, is a directory that receives a copy of the ugen input ucode as <n>.O.
Runs in the tokimemo-decomp-ido52 image (amd64, /opt/u2404 for qemu-irix-4.0).
"""
import os
import shutil
import subprocess
import sys
import tempfile

ROOT = "/work"
sys.path.insert(0, os.path.join(ROOT, "tools"))
import ido_eval  # noqa: E402
import ugen_pass  # noqa: E402


def force_f(path, spec):
    """Rewrite the ucode file `path` with F gadgets per UG41_FORCE `spec` (`index:marks`)."""
    want, marks = spec.split(":")
    with open(path, "rb") as f:
        insns = ugen_pass.parse_ucode(f.read())
    procs = [(s, e) for s, e in ugen_pass._procedures(insns)
             if any(insns[k].opc == ugen_pass.OP["mov"] for k in range(s, e))]
    start, end = procs[int(want)]
    movs = [k for k in range(start, end) if insns[k].opc == ugen_pass.OP["mov"]]
    if len(movs) != len(marks):
        sys.exit("ug41_shim.py: procedure %s has %d Umov, UG41_FORCE gives %d" % (want, len(movs), len(marks)))
    at = {ugen_pass._mov_insert_index(insns, start, k) for k, m in zip(movs, marks) if m == "F"}
    out = []
    for k, ins in enumerate(insns):
        if k in at:
            out.extend(ugen_pass._gadget_f())
        out.append(ins)
    with open(path, "wb") as f:
        f.write(ugen_pass.serialize_ucode(out))
    return bool(at)


def ucode_input(args):
    """Index of the input ucode file in ugen's argument list."""
    i = 0
    while i < len(args):
        if args[i] in ("-G", "-o", "-t", "-temp", "-f", "-l", "-Olimit"):
            i += 2
        elif args[i].startswith("-"):
            i += 1
        else:
            return i
    sys.exit("ug41_shim.py: no ucode input in %r" % args)


def main(argv):
    if len(argv) < 3 or argv[1] != "--ugen":
        sys.exit(__doc__)
    real, args = argv[2], argv[3:]
    mode = os.environ.get("UG41_MODE", "raw")
    save = os.environ.get("UG41_SAVE")
    if save:
        os.makedirs(save, exist_ok=True)
        n = len([f for f in os.listdir(save) if f.endswith(".O")])
        shutil.copy(args[ucode_input(args)], os.path.join(save, "%d.O" % n))
    if mode == "raw":
        sys.exit(subprocess.call([real] + args))
    if mode != "41":
        sys.exit(subprocess.call(["python3", mode, "--ugen", real] + args))
    tmp = tempfile.mkdtemp(prefix="ug41")
    try:
        args = list(args)
        forced = False
        if os.environ.get("UG41_FORCE"):
            i = ucode_input(args)
            shutil.copy(args[i], os.path.join(tmp, "in.O"))
            args[i] = os.path.join(tmp, "in.O")
            forced = force_f(args[i], os.environ["UG41_FORCE"])
        pre, root = ido_eval.qemu("4.1", tmp)
        rc = subprocess.call(pre + [os.path.join(root, "usr/lib/ugen")] + args)
        if rc:
            sys.exit(rc)
        out = args[args.index("-o") + 1]
        with open(out, "rb") as f:
            data = f.read()
        if forced:
            data = ugen_pass.strip_sentinel_binasm(data)
        with open(out, "wb") as f:
            f.write(ido_eval.set_pic_option(data, 0))
    finally:
        shutil.rmtree(tmp)


if __name__ == "__main__":
    main(sys.argv)
