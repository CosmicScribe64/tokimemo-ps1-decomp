#!/usr/bin/env python3
"""Generate build.ninja for the SLPM_86.053 matching build.

Usage (inside Docker): tools/docker.sh python3 configure.py
Then:                  tools/docker.sh ninja            (build + sha1 check)

Pipeline: splat split -> assemble asm (GNU as) -> compile C (per-file
toolchain, see C_FILES and tools/cc.py) -> link with the splat linker script -> objcopy to
the PS-X EXE -> sha1sum -c config/SLPM_86.053.sha1. Also writes objdiff.json.
"""
import json
import sys

import ninja_syntax

EXE = "SLPM_86.053"
AS = "mips-linux-gnu-as -EL -march=r3000 -mabi=32 -G0 -Iinclude -I."

# splat output layout (config/SLPM_86.053.yaml): C sources and asm objects.
# Each C file maps to its toolchain arguments for tools/cc.py. The game code
# is IDO-compiled (T-0013, wiki/toolchain.md); SDK C, once split, would use
# e.g. ["gcc", "2.7.2-psx", "2.79"].
C_FILES = {"src/game.c": ["ido", "5.3"]}
ASM_FILES = [
    "asm/header.s",
    "asm/sdk_libs.s",
    "asm/libapi_stubs.s",
    "asm/data/rodata.rodata.s",
    "asm/data/data.data.s",
    "asm/data/bss.bss.s",
]


def main():
    n = ninja_syntax.Writer(open("build.ninja", "w"))
    n.variable("ninja_required_version", "1.10")
    n.rule("configure", command="python3 configure.py", generator=True,
           description="configure")
    n.build("build.ninja", "configure", "configure.py")
    n.rule("split",
           command="python3 -m splat split config/%s.yaml && touch %s" % (EXE, "build/split.stamp"),
           description="splat split")
    n.rule("as", command=AS + " -o $out $in", description="AS $in")
    n.rule("cc",
           command="python3 tools/cc.py $in $out $toolchain",
           depfile="$out.d", deps="gcc", description="CC $in")
    n.rule("ld",
           command=("mips-linux-gnu-ld -EL -T build/%s.ld "
                    "-T build/undefined_funcs_auto.txt "
                    "-T build/undefined_syms_auto.txt -Map build/%s.map "
                    "-o $out") % (EXE, EXE),
           description="LD $out")
    n.rule("objcopy", command="mips-linux-gnu-objcopy -O binary $in $out",
           description="OBJCOPY $out")
    n.rule("sha1",
           command=("h=$$(cut -d' ' -f1 config/%s.sha1) && "
                    "echo \"$$h  $in\" | sha1sum -c && touch $out") % EXE,
           description="SHA1 CHECK $in")

    stamp = "build/split.stamp"
    split_in = ["config/%s.yaml" % EXE, "config/symbol_addrs.txt",
                "config/reloc_addrs.txt"]
    n.build([stamp, "build/%s.ld" % EXE] + ASM_FILES, "split", split_in, implicit=["disc/files/" + EXE])

    objs = []
    for c, toolchain in sorted(C_FILES.items()):
        o = "build/" + c[:-2] + ".o"
        n.build(o, "cc", c, variables={"toolchain": " ".join(toolchain)},
                implicit=[stamp, "include/common.h", "include/include_asm.h",
                          "include/game.h", "include/asmproc_prelude.inc",
                          "include/gte_macros.inc", "tools/cc.py"])
        objs.append(o)
    for s in ASM_FILES:
        o = "build/" + s[:-2] + ".o"
        n.build(o, "as", s, implicit=[stamp, "include/macro.inc"])
        objs.append(o)

    elf = "build/%s.elf" % EXE
    n.build(elf, "ld", objs, implicit=[stamp])
    n.build("build/%s.bin" % EXE, "objcopy", elf)
    n.build("build/%s.ok" % EXE, "sha1", "build/%s.bin" % EXE)
    n.rule("progress", command="python3 tools/progress.py", description="PROGRESS", pool="console")
    n.build("progress", "progress")
    n.default("build/%s.ok" % EXE)
    n.close()

    units = [{
        "name": c[:-2],
        "target_path": "expected/build/" + c[:-2] + ".o",
        "base_path": "build/" + c[:-2] + ".o",
        "metadata": {"source_path": c},
    } for c in sorted(C_FILES)]
    with open("objdiff.json", "w") as f:
        json.dump({"$schema": "https://raw.githubusercontent.com/encounter/"
                   "objdiff/main/config.schema.json",
                   "custom_make": "ninja", "build_target": False,
                   "build_base": True, "units": units}, f, indent=2)
        f.write("\n")


if __name__ == "__main__":
    sys.exit(main())
