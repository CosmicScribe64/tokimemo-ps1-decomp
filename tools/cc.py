#!/usr/bin/env python3
"""Compile one C file with the toolchain chosen for its segment, checking every stage.

Usage (inside Docker, called by ninja):
    python3 tools/cc.py <src.c> <out.o> ido <ido_version>
    python3 tools/cc.py <src.c> <out.o> gcc <gcc_version> <aspsx_version>

ido: SGI IDO (decompals/ido-static-recomp) run through asm-processor, which
     splices the INCLUDE_ASM functions in and assembles them with GNU as.
     Used for the game code (T-0013, wiki/matching-notes.md). Every IDO
     compile also runs the frame-layout emulation pass (tools/frame_pass.py,
     T-0016): the original's frames are 16 bytes larger than IDO's.
gcc: the PsyQ way, cpp | cc1 | maspsx | as.

Every object's .text is zero-padded to a multiple of 16 bytes (T-0012): the original link
aligned the .text of each object to 16, and the build's linker script uses SUBALIGN(2), so the
padding that the original linker added after the last function of a source file is made part of
the object. Without it, a file whose last function is C (not INCLUDE_ASM, which carries the
padding nops itself) would shift every later file. See wiki/source-files.md.

Also writes <out.o>.d (a make-style depfile) listing the asm/nonmatchings
files named by INCLUDE_ASM, because the compiler cannot see them.
Exits non-zero if any stage fails.
"""
import os
import re
import shutil
import subprocess
import sys
import tempfile

AS = ["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-G0",
      "-Iinclude", "-I."]
INCLUDE_ASM_RE = re.compile(r'^INCLUDE_ASM\("([^"]*)",\s*(\w+)\)', re.M)

GCC_CFLAGS = ["-O2", "-G0", "-mcpu=3000", "-quiet"]
MASPSX = "/opt/maspsx/maspsx.py"

# -Wo,-nokpicopt: uopt must not keep the address of a directly accessed global
# in a register; the original reloads %hi/%lo per access (func_80042400). It
# still keeps integer constants and array base addresses in registers, as the
# original does (loop bounds hoisted, `li t1,0x44; multu`). Replaces
# -Wo,-no_const_in_reg, which also stopped those (T-0014, T-0017).
IDO_CFLAGS = ["-c", "-EL", "-O2", "-mips1", "-G", "0", "-non_shared",
              "-Wo,-nokpicopt", "-Xcpluscomm", "-Iinclude"]
ASM_PROCESSOR = "/opt/asm-processor/build.py"
ASM_PRELUDE = "include/asmproc_prelude.inc"
FRAME_PASS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "frame_pass.py")


def run_stage(cmd, data, env=None):
    proc = subprocess.run(cmd, input=data, stdout=subprocess.PIPE, env=env)
    if proc.returncode != 0:
        sys.exit("cc.py: stage failed (%d): %s" % (proc.returncode, " ".join(cmd)))
    return proc.stdout


def compile_gcc(src, out, gcc_ver, aspsx_ver):
    gcc = "/opt/gcc/" + gcc_ver
    data = run_stage([gcc + "/cpp", "-Iinclude", "-undef", "-lang-c", src], None)
    data = run_stage([gcc + "/cc1"] + GCC_CFLAGS, data)
    data = run_stage(["python3", MASPSX, "--aspsx-version=" + aspsx_ver], data)
    run_stage(AS + ["-o", out], data)


def ido_frame_env(ido_ver, tmp):
    """Environment that runs IDO with tools/frame_pass.py in front of as1.

    IDO's cc finds its passes in USR_LIB. Point it at a directory holding
    symlinks to every IDO file except as1, which is a shim that applies the
    frame-layout emulation pass (T-0016) to ugen's output and runs the real
    as1 on the result. The pass applies to every function; see frame_pass.py.
    """
    ido = "/opt/ido/%s" % ido_ver
    for name in sorted(os.listdir(ido)):
        if name != "as1":
            os.symlink(os.path.join(ido, name), os.path.join(tmp, name))
    shim = os.path.join(tmp, "as1")
    with open(shim, "w") as f:
        f.write('#!/bin/sh\nexec python3 "%s" --as1 "%s/as1" "$@"\n'
                % (FRAME_PASS, ido))
    os.chmod(shim, 0o755)
    env = dict(os.environ)
    env["USR_LIB"] = tmp
    return env


def compile_ido(src, out, ido_ver):
    cmd = (["python3", ASM_PROCESSOR, "--no-dep-file", "--drop-mdebug-gptab",
            "--convert-statics", "no",
            "--asm-prelude", ASM_PRELUDE, "/opt/ido/%s/cc" % ido_ver, "--"]
           + AS + ["--"] + IDO_CFLAGS + ["-o", out, src])
    tmp = tempfile.mkdtemp(prefix="idolib")
    try:
        run_stage(cmd, None, ido_frame_env(ido_ver, tmp))
    finally:
        shutil.rmtree(tmp)


def pad_text(obj):
    """Zero-pad the .text section of `obj` to a multiple of 16 bytes (original per-object alignment)."""
    tmp = tempfile.mkdtemp(prefix="padtext")
    try:
        raw = os.path.join(tmp, "text.bin")
        subprocess.run(["mips-linux-gnu-objcopy", "--dump-section", ".text=" + raw, obj, os.devnull],
                       check=True)
        data = open(raw, "rb").read()
        if len(data) % 16:
            with open(raw, "wb") as f:
                f.write(data + b"\0" * (-len(data) % 16))
            subprocess.run(["mips-linux-gnu-objcopy", "--update-section", ".text=" + raw, obj], check=True)
    finally:
        shutil.rmtree(tmp)


def main(argv):
    if len(argv) < 5:
        sys.exit(__doc__)
    src, out, kind = argv[1:4]
    with open(src) as f:
        text = f.read()
    deps = ["%s/%s.s" % m for m in INCLUDE_ASM_RE.findall(text)]
    with open(out + ".d", "w") as f:
        f.write("%s: %s\n" % (out, " ".join(deps)))
    if kind == "ido" and len(argv) == 5:
        compile_ido(src, out, argv[4])
    elif kind == "gcc" and len(argv) == 6:
        compile_gcc(src, out, argv[4], argv[5])
    else:
        sys.exit(__doc__)
    pad_text(out)


if __name__ == "__main__":
    main(sys.argv)
