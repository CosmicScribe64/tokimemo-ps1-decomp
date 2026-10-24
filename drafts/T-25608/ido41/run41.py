#!/usr/bin/env python3
"""Compile a C file with the build's IDO 5.3 front end and uopt and a chosen ugen (T-25608).

Usage (tokimemo-decomp-ido52 image, cwd /work):
  python3 drafts/T-25608/ido41/run41.py [--asmproc [--pad-src C]] [--keep DIR] SRC MODE...
MODE is 41, raw or a ugen_pass.py path (drafts/T-25608/ido41/ug41_shim.py). The front half is the
build's (tools/cc.py IDO_CFLAGS, tools/cvt_pass.py around uopt). With 4.1's ugen the frame pass is
off (4.1's ugen makes the +16 frames itself); otherwise tools/frame_pass.py runs before as1.
--asmproc compiles through asm-processor (a unit with INCLUDE_ASM) and adds the padding of tools/cc.py
(looked up for the source C, default SRC: a copy of a unit names its src/ path). For every mode the script
prints, per function, the state of each lwl/lwr block-copy loop (copystate.py) and, with --keep,
writes DIR/<mode tag>.o and DIR/<mode tag>.s (objdump -dr) and the ugen input as DIR/ucode/<n>.O.
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

ROOT = "/work"
HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.join(ROOT, "tools"))
sys.path.insert(0, HERE)
import cc  # noqa: E402
import copystate  # noqa: E402

SHIM = os.path.join(HERE, "ug41_shim.py")


def tag(mode):
    return mode if mode in ("41", "raw") else re.sub(r"[^A-Za-z0-9]+", "_", mode).strip("_")


def compile_one(src, mode, out, asmproc, save=None, pad_src=None):
    shims = {"ugen": SHIM}
    if mode == "41":
        shims["as1"] = None
    tmp = tempfile.mkdtemp(prefix="run41")
    try:
        env = cc.ido_frame_env("5.3", tmp, shims)
        env["UG41_MODE"] = mode
        if save:
            env["UG41_SAVE"] = save
        if asmproc:
            cmd = (["python3", cc.ASM_PROCESSOR, "--no-dep-file", "--drop-mdebug-gptab",
                    "--convert-statics", "no", "--asm-prelude", cc.ASM_PRELUDE,
                    "/opt/ido/5.3/cc", "--"] + cc.AS + ["--"] + cc.IDO_CFLAGS + ["-o", out, src])
        else:
            cmd = ["/opt/ido/5.3/cc"] + cc.IDO_CFLAGS + ["-o", out, src]
        proc = subprocess.run(cmd, env=env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        if proc.returncode:
            sys.stderr.write(proc.stdout.decode("latin-1")[-3000:])
            sys.exit("run41.py: compile failed (%s, %s)" % (src, mode))
        if asmproc:
            # the padding tools/cc.py adds, so funcdiff can compare linked addresses
            with open(src) as f:
                includes = cc.INCLUDE_ASM_RE.findall(f.read())
            cc.trailing_pad.pad_functions(out, pad_src or src, {name for _, name in includes})
            cc.pad_text(out)
    finally:
        shutil.rmtree(tmp)


def main(argv):
    asmproc, keep, pad_src = False, None, None
    args = argv[1:]
    while args and args[0].startswith("--"):
        if args[0] == "--asmproc":
            asmproc, args = True, args[1:]
        elif args[0] == "--pad-src":
            pad_src, args = args[1], args[2:]
        elif args[0] == "--keep":
            keep, args = args[1], args[2:]
        else:
            sys.exit(__doc__)
    if len(args) < 2:
        sys.exit(__doc__)
    src, modes = args[0], args[1:]
    for mode in modes:
        work = keep or tempfile.mkdtemp(prefix="run41o")
        os.makedirs(work, exist_ok=True)
        obj = os.path.join(work, tag(mode) + ".o")
        save = os.path.join(work, "ucode") if keep and mode == modes[0] else None
        if save and os.path.isdir(save):
            shutil.rmtree(save)
        compile_one(src, mode, obj, asmproc, save, pad_src)
        dis = subprocess.run(["mips-linux-gnu-objdump", "-dr", "-z", obj], check=True,
                             stdout=subprocess.PIPE, text=True).stdout
        if keep:
            with open(os.path.join(work, tag(mode) + ".s"), "w") as f:
                f.write(dis)
        for func, states in copystate.states(dis):
            print("%-6s %-24s %s" % (tag(mode)[-6:], func, " ".join(states) or "-"))
        if not keep:
            shutil.rmtree(work)


if __name__ == "__main__":
    main(sys.argv)
