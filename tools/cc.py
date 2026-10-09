#!/usr/bin/env python3
"""Compile one C file the PsyQ way: cpp | cc1 | maspsx | as, checking every stage.

Usage (inside Docker, called by ninja):
    python3 tools/cc.py <src.c> <out.o> <gcc_version> <aspsx_version>

Also writes <out.o>.d (a make-style depfile) listing the asm/nonmatchings
files named by INCLUDE_ASM, because gcc cannot see assembler .include lines.
Exits non-zero if any stage fails.
"""
import re
import subprocess
import sys

CFLAGS = ["-O2", "-G0", "-mcpu=3000", "-quiet"]
AS = ["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-G0",
      "-Iinclude", "-I."]
MASPSX = "/opt/maspsx/maspsx.py"
INCLUDE_ASM_RE = re.compile(r'^INCLUDE_ASM\("([^"]*)",\s*(\w+)\)', re.M)


def run_stage(cmd, data):
    proc = subprocess.run(cmd, input=data, stdout=subprocess.PIPE)
    if proc.returncode != 0:
        sys.exit("cc.py: stage failed (%d): %s" % (proc.returncode, " ".join(cmd)))
    return proc.stdout


def main(argv):
    if len(argv) != 5:
        sys.exit(__doc__)
    src, out, gcc_ver, aspsx_ver = argv[1:]
    gcc = "/opt/gcc/" + gcc_ver
    with open(src) as f:
        text = f.read()
    deps = ["%s/%s.s" % m for m in INCLUDE_ASM_RE.findall(text)]
    with open(out + ".d", "w") as f:
        f.write("%s: %s\n" % (out, " ".join(deps)))
    data = run_stage([gcc + "/cpp", "-Iinclude", "-undef", "-lang-c", src], None)
    data = run_stage([gcc + "/cc1"] + CFLAGS, data)
    data = run_stage(["python3", MASPSX, "--aspsx-version=" + aspsx_ver], data)
    run_stage(AS + ["-o", out], data)


if __name__ == "__main__":
    main(sys.argv)
