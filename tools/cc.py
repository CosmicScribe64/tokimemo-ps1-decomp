#!/usr/bin/env python3
"""Compile one C file with the toolchain chosen for its segment, checking every stage.

Usage (inside Docker, called by ninja):
    python3 tools/cc.py <src.c> <out.o> ido <ido_version>
    python3 tools/cc.py <src.c> <out.o> gcc <gcc_version> <aspsx_version>

ido: SGI IDO (decompals/ido-static-recomp) run through asm-processor, which
     splices the INCLUDE_ASM functions in and assembles them with GNU as.
     Used for the game code (T-0013, wiki/matching-notes.md).
gcc: the PsyQ way, cpp | cc1 | maspsx | as.

Also writes <out.o>.d (a make-style depfile) listing the asm/nonmatchings
files named by INCLUDE_ASM, because the compiler cannot see them.
Exits non-zero if any stage fails.
"""
import re
import subprocess
import sys

AS = ["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-G0",
      "-Iinclude", "-I."]
INCLUDE_ASM_RE = re.compile(r'^INCLUDE_ASM\("([^"]*)",\s*(\w+)\)', re.M)

GCC_CFLAGS = ["-O2", "-G0", "-mcpu=3000", "-quiet"]
MASPSX = "/opt/maspsx/maspsx.py"

IDO_CFLAGS = ["-c", "-EL", "-O2", "-mips1", "-G", "0", "-non_shared",
              "-Xcpluscomm", "-Iinclude"]
ASM_PROCESSOR = "/opt/asm-processor/build.py"
ASM_PRELUDE = "include/asmproc_prelude.inc"


def run_stage(cmd, data):
    proc = subprocess.run(cmd, input=data, stdout=subprocess.PIPE)
    if proc.returncode != 0:
        sys.exit("cc.py: stage failed (%d): %s" % (proc.returncode, " ".join(cmd)))
    return proc.stdout


def compile_gcc(src, out, gcc_ver, aspsx_ver):
    gcc = "/opt/gcc/" + gcc_ver
    data = run_stage([gcc + "/cpp", "-Iinclude", "-undef", "-lang-c", src], None)
    data = run_stage([gcc + "/cc1"] + GCC_CFLAGS, data)
    data = run_stage(["python3", MASPSX, "--aspsx-version=" + aspsx_ver], data)
    run_stage(AS + ["-o", out], data)


def compile_ido(src, out, ido_ver):
    cmd = (["python3", ASM_PROCESSOR, "--no-dep-file", "--drop-mdebug-gptab",
            "--convert-statics", "no",
            "--asm-prelude", ASM_PRELUDE, "/opt/ido/%s/cc" % ido_ver, "--"]
           + AS + ["--"] + IDO_CFLAGS + ["-o", out, src])
    run_stage(cmd, None)


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


if __name__ == "__main__":
    main(sys.argv)
