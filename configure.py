#!/usr/bin/env python3
"""Generate build.ninja for the SLPM_86.053 matching build.

Usage (inside Docker): tools/docker.sh python3 configure.py
Then:                  tools/docker.sh ninja            (build + sha1 check)

Pipeline: splat split -> assemble asm (GNU as) -> compile C (per-file
toolchain, see C_FILES and tools/cc.py) -> link with the splat linker
script -> objcopy to the PS-X EXE -> sha1sum -c config/SLPM_86.053.sha1.
Each overlay in config/overlays.txt (T-0008) is its own target: splat split
config/overlays/<NAME>.yaml -> IDO C + asm -> ld -> objcopy -> sha1 check
(build/ovl/<NAME>.ok, or `ninja overlays` for all). Also writes objdiff.json.
"""
import glob
import json
import sys

import ninja_syntax
import yaml

sys.path.insert(0, "tools")
import srcscan  # noqa: E402  (tools/srcscan.py: the C files of every unit, from the yaml files)

EXE = "SLPM_86.053"
HEADERS_OK = "build/headers.ok"  # tools/check_headers.py passed (T-1200)
HEADER_FILES = sorted(glob.glob("include/**/*.h", recursive=True))

# splat output layout (config/SLPM_86.053.yaml): C sources and asm objects.
# Every `c` subsegment of the `main` segment is a C file src/<name>.c (T-0012: one per original
# object, named by start address) and is picked up automatically. All are IDO-compiled (T-0013,
# wiki/toolchain.md); SDK C, once split, would need a per-file toolchain here (e.g.
# ["gcc", "2.7.2-psx", "2.79"]).
C_TOOLCHAIN = ["ido", "5.3"]


def main_subsegments():
    """(type, name) of every subsegment of the `main` segment of the splat config."""
    with open("config/%s.yaml" % EXE) as f:
        cfg = yaml.safe_load(f)
    main_seg = [s for s in cfg["segments"] if isinstance(s, dict) and s.get("name") == "main"]
    if not main_seg:
        sys.exit("configure.py: no `main` segment in config/%s.yaml" % EXE)
    out = []
    for sub in main_seg[0]["subsegments"]:
        if isinstance(sub, dict):
            out.append((sub["type"], sub["name"]))
        else:
            out.append((sub[1], sub[2]))
    return out


def c_files():
    """{src/<name>.c: toolchain} for the `c` subsegments of the main segment."""
    return {"src/%s.c" % name: C_TOOLCHAIN
            for typ, name in main_subsegments() if typ == "c"}


def asm_files():
    """asm objects splat writes for the `main` segment, from the splat config."""
    out = ["asm/header.s"]
    for typ, name in main_subsegments():
        if typ == "asm":
            out.append("asm/%s.s" % name)
        elif typ in ("rodata", "data", "bss"):
            out.append("asm/data/%s.%s.s" % (name, typ))
    return out


C_FILES = c_files()
ASM_FILES = asm_files()
OVL_C_TOOLCHAIN = ["ido", "5.3"]


def read_overlays():
    """Return [(name, load address, text size)] from config/overlays.txt."""
    rows = []
    for line in open("config/overlays.txt"):
        if line.strip() and not line.startswith("#"):
            name, base, text = line.split()
            rows.append((name, base, text))
    return rows


def overlay_data_files(name):
    """Asm objects splat writes for the data subsegments (`rodata`/`data`/`bss`, not the
    dot-prefixed siblings that the C object provides) of overlay `name` (T-1340)."""
    with open("config/overlays/%s.yaml" % name) as f:
        cfg = yaml.safe_load(f)
    out = []
    for seg in cfg["segments"]:
        if not isinstance(seg, dict):
            continue
        for sub in seg.get("subsegments", []):
            typ, sname = (sub["type"], sub["name"]) if isinstance(sub, dict) else (sub[1], sub[2])
            if typ in ("rodata", "data", "bss"):
                out.append("asm/ovl/%s/data/%s.%s.s" % (name, sname, typ))
    return out


def symbol_files(path):
    """The symbol_addrs_path files of a splat config (inputs of its split step)."""
    with open(path) as f:
        cfg = yaml.safe_load(f)
    return list(cfg["options"].get("symbol_addrs_path", []))


def overlay_targets(n, overlays):
    """Write the split/compile/link/check rules for every overlay."""
    n.rule("osplit",
           command=("python3 -m splat split config/overlays/$name.yaml && "
                    "python3 tools/rodata_pieces.py config/overlays/$name.yaml && touch $out"),
           description="splat split $name")
    n.rule("old",
           command=("mips-linux-gnu-ld -EL -T build/ovl/$name.ld "
                    "-T build/ovl/${name}_undefined_funcs_auto.txt "
                    "-T build/ovl/${name}_undefined_syms_auto.txt "
                    "-T build/main_names.ld "
                    "-Map build/ovl/$name.map -o $out"),
           description="LD $out")
    n.rule("osha1",
           command=("h=$$(cut -d' ' -f1 config/overlays/$name.sha1) && "
                    "echo \"$$h  $in\" | sha1sum -c && touch $out"),
           description="SHA1 CHECK $in")
    # main-exe names (O.BIN, SDK) that overlays call by name; see tools/syms_to_ld.py
    n.rule("symsld", command="python3 tools/syms_to_ld.py $out $in", description="SYMS $out")
    n.build("build/main_names.ld", "symsld",
            ["config/symbol_addrs_obin.txt", "config/symbol_addrs_sdk.txt"],
            implicit=["tools/syms_to_ld.py"])
    oks = []
    for name, _base, _text in overlays:
        v = {"name": name}
        stamp = "build/ovl/%s.stamp" % name
        ld = "build/ovl/%s.ld" % name
        data_ss = overlay_data_files(name)
        n.build([stamp, ld] + data_ss, "osplit",
                ["config/overlays/%s.yaml" % name, "config/reloc_addrs.txt"]
                + symbol_files("config/overlays/%s.yaml" % name),
                implicit=["disc/files/CDROM/EXEDIR/%s.EXN" % name],
                variables=v)
        # one C file per `c` subsegment: src/ovl/<NAME>.c, or src/ovl/<NAME>/<addr>.c per
        # original object once tools/split_objects.py has run (T-0500)
        c_os = []
        for cf in srcscan.unit_c_files(name):
            n.build(cf.obj, "cc", str(cf.src),
                    variables={"toolchain": " ".join(OVL_C_TOOLCHAIN)},
                    implicit=[stamp, HEADERS_OK, "include/common.h", "include/include_asm.h",
                              "include/asmproc_prelude.inc",
                              "include/gte_macros.inc", "tools/cc.py", "tools/frame_pass.py"])
            c_os.append(cf.obj)
        data_os = []
        for data_s in data_ss:
            data_o = "build/ovl/%s/%s.o" % (name, data_s[:-2])
            n.build(data_o, "as", data_s, implicit=[stamp, "include/macro.inc"])
            data_os.append(data_o)
        elf = "build/ovl/%s.elf" % name
        n.build(elf, "old", c_os + data_os, implicit=[stamp, ld, "build/main_names.ld"], variables=v)
        n.build("build/ovl/%s.bin" % name, "objcopy", elf)
        ok = "build/ovl/%s.ok" % name
        n.build(ok, "osha1", "build/ovl/%s.bin" % name, variables=v)
        oks.append(ok)
    n.build("overlays", "phony", oks)
    return oks


def main():
    n = ninja_syntax.Writer(open("build.ninja", "w"))
    n.variable("ninja_required_version", "1.10")
    n.rule("configure", command="python3 configure.py", generator=True,
           description="configure")
    # the asm objects to build depend on the subsegments of the yaml files (T-1340)
    n.build("build.ninja", "configure", "configure.py",
            implicit=["config/%s.yaml" % EXE, "config/overlays.txt"]
            + ["config/overlays/%s.yaml" % o[0] for o in read_overlays()])
    n.rule("split",
           command=("python3 -m splat split config/%s.yaml && python3 tools/rodata_pieces.py "
                    "config/%s.yaml && touch %s") % (EXE, EXE, "build/split.stamp"),
           description="splat split")
    n.rule("headers", command="python3 tools/check_headers.py include src && touch $out",
           description="CHECK HEADERS")
    n.build(HEADERS_OK, "headers", implicit=["tools/check_headers.py"] + HEADER_FILES)
    n.build("headers", "phony", HEADERS_OK)
    n.rule("as", command="python3 tools/asm.py $in $out", description="AS $in")
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
    split_in = ["config/%s.yaml" % EXE, "config/reloc_addrs.txt"] + symbol_files("config/%s.yaml" % EXE)
    n.build([stamp, "build/%s.ld" % EXE] + ASM_FILES, "split", split_in, implicit=["disc/files/" + EXE])

    objs = []
    headers = sorted(glob.glob("include/**/*.h", recursive=True) + glob.glob("include/**/*.inc", recursive=True))
    for c, toolchain in sorted(C_FILES.items()):
        o = "build/" + c[:-2] + ".o"
        n.build(o, "cc", c, variables={"toolchain": " ".join(toolchain)},
                implicit=[stamp, HEADERS_OK, "tools/cc.py", "tools/frame_pass.py"] + headers)
        objs.append(o)
    for s in ASM_FILES:
        o = "build/" + s[:-2] + ".o"
        n.build(o, "as", s, implicit=[stamp, "include/macro.inc", "tools/asm.py"])
        objs.append(o)

    elf = "build/%s.elf" % EXE
    n.build(elf, "ld", objs, implicit=[stamp])
    n.build("build/%s.bin" % EXE, "objcopy", elf)
    n.build("build/%s.ok" % EXE, "sha1", "build/%s.bin" % EXE)
    n.rule("progress", command="python3 tools/progress.py", description="PROGRESS", pool="console")
    n.build("progress", "progress")
    oks = overlay_targets(n, read_overlays())
    n.default(["build/%s.ok" % EXE] + oks)
    n.close()

    units = [{
        "name": c[:-2],
        "target_path": "expected/build/" + c[:-2] + ".o",
        "base_path": "build/" + c[:-2] + ".o",
        "metadata": {"source_path": c, "progress_categories": ["main"]},
    } for c in sorted(C_FILES)]
    # Overlays (T-0902): the object `cc` writes for each overlay C file (src/ovl/<NAME>.c or
    # src/ovl/<NAME>/<addr>.c, T-0500); the target is the all-INCLUDE_ASM copy in expected/ovl/
    # (see wiki/decompile-workflow.md).
    units += [{
        "name": str(cf.src)[:-2],
        "target_path": "expected/ovl/%s.o" % cf.name,
        "base_path": cf.obj,
        "metadata": {"source_path": str(cf.src), "progress_categories": ["overlays"]},
    } for name, _base, _text in read_overlays() for cf in srcscan.unit_c_files(name)]
    with open("objdiff.json", "w") as f:
        json.dump({"$schema": "https://raw.githubusercontent.com/encounter/"
                   "objdiff/main/config.schema.json",
                   "custom_make": "ninja", "build_target": False,
                   "build_base": True,
                   "progress_categories": [{"id": "main", "name": "Main executable"},
                                           {"id": "overlays", "name": "Overlays"}],
                   "units": units}, f, indent=2)
        f.write("\n")


if __name__ == "__main__":
    sys.exit(main())
