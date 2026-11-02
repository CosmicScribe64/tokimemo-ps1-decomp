#!/usr/bin/env python3
"""Generate build.ninja for the SLPM_86.053 matching build.

Usage (inside Docker): tools/docker.sh python3 configure.py   (put the game in game/ first)
Then:                  tools/docker.sh ninja            (build + sha1 check)

Pipeline: splat split -> assemble asm (GNU as) -> compile C (per-file
toolchain, see C_FILES and tools/cc.py) -> link with the splat linker
script -> objcopy to the PS-X EXE -> sha1sum -c config/SLPM_86.053.sha1.
Each overlay in config/overlays.txt (T-0008) is its own target: splat split
config/overlays/<NAME>.yaml -> IDO C + asm -> ld -> objcopy -> sha1 check
(build/ovl/<NAME>.ok, or `ninja overlays` for all). Also writes objdiff.json.
`ninja english` (not a default target) builds the English variant into build/en/ (T-39502,
wiki/english-text-runtime.md); `ninja relink` builds the relink check into build/jr/ (T-39505,
wiki/relink.md).
"""
import glob
import json
import os
import sys

import ninja_syntax
import yaml

sys.path.insert(0, "tools")
import prepare_disc  # noqa: E402  (tools/prepare_disc.py: finds the user's game image)
import srcscan  # noqa: E402  (tools/srcscan.py: the C files of every unit, from the yaml files)
import symtab  # noqa: E402  (tools/symtab.py: the symbol files of every unit, T-27002)

EXE = "SLPM_86.053"
DISC_STAMP = "build/disc.stamp"  # tools/prepare_disc.py: disc/files is unpacked from game/ (T-3300)
INCLUDE_FILES = "build/include-files.txt"  # membership changes can alter include lookup (T-13500)
HEADERS_OK = "build/headers.ok"  # tools/check_headers.py passed (T-1200)
SYMBOLS_OK = "build/symbols.ok"  # no overlay symbols in shared main config (T-9400)
GLOBALS_OK = "build/globals.ok"  # tools/migrate_globals.py --check passed (T-5100)
RENAMES_OK = "build/renames.ok"  # tools/rename.py check passed: no renamed name came back (T-27002)
LEDGER_DIR = "config/renames"    # rename ledger of tools/rename.py
HEADER_FILES = sorted(glob.glob("include/**/*.h", recursive=True))

# splat output layout (config/SLPM_86.053.yaml): C sources and asm objects.
# Every `c` subsegment of the `main` segment is a C file src/<name>.c (T-0012: one per original
# object, named by start address) and is picked up automatically. They are IDO-compiled (T-0013,
# wiki/toolchain.md) unless config/toolchains.txt names another toolchain for the object, e.g.
# `TACO/8015D270 gcc 2.7.2-psx 2.79` for the PsyQ-gcc-built SDK code inside an overlay (T-9200).
C_TOOLCHAIN = ["ido", "5.3"]
TOOLCHAINS_FILE = "config/toolchains.txt"


def read_toolchains(known, path=TOOLCHAINS_FILE):
    """{object: toolchain words} from config/toolchains.txt (T-9200).

    One line per object that is not built by the default IDO: `<object> <ido|gcc> <args...>`, where
    an `ido` line may add per-object pass options after the version (`ido 5.3 -Wb,-noxbb`, T-24800),
    <object> is the splat subsegment name of its `c` subsegment (`main/80041000`,
    `TACO/8015D270`; srcscan CFile.name) and the args are those of tools/cc.py. `known` is the set
    of valid names; an unknown object or toolchain stops the configure step."""
    out = {}
    if not os.path.exists(path):
        return out
    for lineno, line in enumerate(open(path), 1):
        line = line.split("#", 1)[0].split()
        if not line:
            continue
        where = "%s:%d" % (path, lineno)
        name, tc = line[0], line[1:]
        if name not in known:
            sys.exit("configure.py: %s: %s is not a `c` subsegment of any unit" % (where, name))
        if ((tc[:1] == ["ido"] and len(tc) >= 2 and all(a.startswith("-W") for a in tc[2:]))
                or (tc[:1] == ["gcc"] and len(tc) == 3)):
            pass
        else:
            sys.exit("configure.py: %s: toolchain must be `ido <ver> [-W<pass>,<option> ...]` or "
                     "`gcc <ver> <aspsx>`, got %r"
                     % (where, " ".join(tc)))
        if name in out:
            sys.exit("configure.py: %s: %s listed twice" % (where, name))
        out[name] = tc
    return out


def all_c_names():
    """Names of every `c` subsegment of the main exe and the overlays."""
    names = {n for typ, n in main_subsegments() if typ == "c"}
    for ovl in srcscan.overlay_names():
        names.update(cf.name for cf in srcscan.unit_c_files(ovl))
    return names


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


def c_files(toolchains=None):
    """{src/<name>.c: toolchain} for the `c` subsegments of the main segment."""
    toolchains = toolchains or {}
    return {"src/%s.c" % name: toolchains.get(name, C_TOOLCHAIN)
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


OBJECT_TOOLCHAINS = read_toolchains(all_c_names())
C_FILES = c_files(OBJECT_TOOLCHAINS)
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
    """Asm objects splat writes for the asm and data subsegments (`asm` and `rodata`/`data`/`bss`,
    not the dot-prefixed siblings that the C object provides) of overlay `name` (T-1340, T-24093)."""
    with open("config/overlays/%s.yaml" % name) as f:
        cfg = yaml.safe_load(f)
    out = []
    for seg in cfg["segments"]:
        if not isinstance(seg, dict):
            continue
        for sub in seg.get("subsegments", []):
            typ, sname = (sub["type"], sub["name"]) if isinstance(sub, dict) else (sub[1], sub[2])
            if typ == "asm":
                out.append("asm/ovl/%s/%s.s" % (name, sname))
            elif typ in ("rodata", "data", "bss"):
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
                    "python3 tools/rodata_pieces.py config/overlays/$name.yaml && "
                    "python3 tools/data_pieces.py config/overlays/$name.yaml && touch $out"),
           description="splat split $name")
    n.rule("old",
           command=("mips-linux-gnu-ld -EL -T build/ovl/$name.ld "
                    "-T build/ovl/${name}_undefined_funcs_auto.txt "
                    "-T build/ovl/${name}_undefined_syms_auto.txt "
                    "-T build/ovl/${name}_data_syms.ld "
                    "-T build/main_names.ld "
                    "-Map build/ovl/$name.map -o $out"),
           description="LD $out")
    n.rule("osha1",
           command=("h=$$(cut -d' ' -f1 config/overlays/$name.sha1) && "
                    "echo \"$$h  $in\" | sha1sum -c && touch $out"),
           description="SHA1 CHECK $in")
    # main-exe names that the overlay yamls do not load (main-only sizes, EVENT-range names, aggregate bases of T-5000)
    # and overlay C uses by name; the overlay yamls load the O.BIN and SDK names themselves (T-27003); see tools/syms_to_ld.py
    n.rule("symsld", command="python3 tools/syms_to_ld.py $out $in", description="SYMS $out")
    # inputs: the main yaml's symbol files that the overlay yamls do not load, and the aggregate bases
    # (symtab.SymbolTable.main_link_files, T-27002)
    n.build("build/main_names.ld", "symsld", symtab.SymbolTable().main_link_files(),
            implicit=["tools/syms_to_ld.py"])
    oks = []
    for name, _base, _text in overlays:
        v = {"name": name}
        stamp = "build/ovl/%s.stamp" % name
        ld = "build/ovl/%s.ld" % name
        data_ss = overlay_data_files(name)
        # build/ovl/<NAME>_data_syms.ld: labels inside C-defined data (tools/data_pieces.py, T-9010)
        n.build([stamp, ld, "build/ovl/%s_data_syms.ld" % name] + data_ss, "osplit",
                ["config/overlays/%s.yaml" % name, "config/reloc_addrs.txt"]
                + symbol_files("config/overlays/%s.yaml" % name),
                implicit=[DISC_STAMP, "tools/data_pieces.py"],
                variables=v)
        # one C file per `c` subsegment: src/ovl/<NAME>.c, or src/ovl/<NAME>/<addr>.c per
        # original object once tools/split_objects.py has run (T-0500)
        c_os = []
        for cf in srcscan.unit_c_files(name):
            n.build(cf.obj, "cc", str(cf.src),
                    variables={"toolchain": " ".join(OBJECT_TOOLCHAINS.get(cf.name, OVL_C_TOOLCHAIN))},
                    implicit=[stamp, SYMBOLS_OK, INCLUDE_FILES, "include/macro.inc", "include/labels.inc",
                              "include/asmproc_prelude.inc",
                              "include/gte_macros.inc", "tools/cc.py", "tools/frame_pass.py", "tools/cvt_pass.py", "tools/ugen_pass.py",
                              "tools/object_layout.py"], order_only=[HEADERS_OK])
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


# Relinked variants (T-39502, T-39505; wiki/english-text-runtime.md, wiki/relink.md), never part of the
# default targets. tools/english_link.py links every unit freely (no address is pinned):
#   `ninja english`  build/en/: objects whose C source mentions the flag are rebuilt with -DTOKIMEMO_ENGLISH,
#                    the English runtime (src/english/, the font) is added to the main executable, and the
#                    entry hooks of config/english_hooks.txt are written;
#   `ninja relink`   build/j0/: the default objects relinked as they are; every binary must equal the default
#                    one (build/j0/identity.ok). build/jr/: the default objects with padding inserted in the
#                    code and the data of every unit, so that almost every address moves; its disc
#                    (tools/patch/variant_disc.py) must play like the original.
EN_FLAG = "TOKIMEMO_ENGLISH"
EN_DIR = "build/en"
J0_DIR = "build/j0"
JR_DIR = "build/jr"
# bytes inserted before the second .text input and before the first .data input: the main executable has
# the 27 KB below the overlay slot, TACO only 291 bytes of free tail (wiki/ram-map.md)
JR_SHIFT_MAIN = "0x1110:0x2220"
JR_SHIFT_OVERLAY = "0x50:0x70"
EN_HOOKS = "config/english_hooks.txt"
EN_FONT = "translations/en/font/latin12.txt"
EN_TOOLS = ["tools/english_link.py", "tools/raw_addrs.py", "tools/elf32.py",
            "config/english_overlay_entries.txt", "config/english_raw_addrs.txt"]
UNLOADED_OVERLAYS = ("EVENT", "GYOZI")  # the game never loads them (wiki/overlays.md): copied unchanged


def uses_english(path):
    with open(path, encoding="utf-8") as f:
        return EN_FLAG in f.read()


def variant_obj(obj, out_dir):
    """build/<path>.o of the default build -> <out_dir>/<path>.o."""
    return out_dir + "/" + obj[len("build/"):]


def check_english_headers():
    """The flag may change code only in .c files: a header outside include/english/ that tests it would
    make every object that includes it differ, and only flagged .c files are rebuilt."""
    bad = [h for h in HEADER_FILES if not h.startswith("include/english/") and uses_english(h)]
    if bad:
        sys.exit("configure.py: %s used in %s; keep English code in .c files or include/english/"
                 % (EN_FLAG, ", ".join(bad)))


def english_cc(n, src, obj, toolchain, implicit, rule="cc"):
    """Compile with the flag: `cc` for an original object (same steps as the default build), `encc`
    for new code, which has no original span for tools/object_layout.py."""
    if toolchain[0] != "ido":
        sys.exit("configure.py: %s uses %s but is built with %s; only IDO objects take the flag"
                 % (src, EN_FLAG, " ".join(toolchain)))
    n.build(obj, rule, src, variables={"toolchain": " ".join(toolchain + ["-D" + EN_FLAG])},
            implicit=implicit, order_only=[HEADERS_OK])


def side_scripts(unit):
    """The default link's scripts of absolute definitions (tools/english_link.py side_scripts)."""
    if unit == "main":
        return ["build/undefined_funcs_auto.txt", "build/undefined_syms_auto.txt", "build/%s_data_syms.ld" % EXE]
    return ["build/ovl/%s_undefined_funcs_auto.txt" % unit, "build/ovl/%s_undefined_syms_auto.txt" % unit,
            "build/ovl/%s_data_syms.ld" % unit, "build/main_names.ld"]


def relink_rules(n):
    check_english_headers()
    n.rule("rlink", command="python3 tools/english_link.py $args", description="RELINK $out")
    n.rule("enfont", command="python3 tools/english_font.py $in $out", description="FONT $in")
    n.rule("encc", command="python3 tools/cc.py $in $out $toolchain", depfile="$out.d", deps="gcc",
           description="CC $in")
    n.rule("enas", command="mips-linux-gnu-as -EL -march=r3000 -mabi=32 -G0 $in -o $out",
           description="AS $in")
    n.rule("rld", command="mips-linux-gnu-ld -EL -T $script -T $aux -Map $map -o $out", description="LD $out")


def relink_targets(n, overlays, objs, cc_implicit, out_dir, english, shifts=None):
    """Rules for one relinked variant: the main executable (a first link for the overlays, then a second
    with the overlay entry points), and every overlay. `shifts` (main, overlay) pads the code and data.
    Returns the outputs."""
    tools = EN_TOOLS
    hooks = EN_HOOKS if english else "-"
    hook_deps = [EN_HOOKS] if english else []
    shift = ["--shift", shifts[0]] if shifts else []
    ovl_shift = ["--shift", shifts[1]] if shifts else []
    outputs = []

    # main executable
    redirect = [o for c, o in objs if english and c.endswith(".c") and uses_english(c)]
    main_objs = []
    for c, o in objs:
        if o in redirect:
            english_cc(n, c, variant_obj(o, out_dir), C_FILES[c], cc_implicit)
            main_objs.append(variant_obj(o, out_dir))
        else:
            main_objs.append(o)
    extra = []
    if english:
        font_s = out_dir + "/gen/english_font.s"
        font_json = out_dir + "/font_metrics.json"
        n.build([font_s, font_json], "enfont", EN_FONT, implicit=["tools/english_font.py"])
        for c in sorted(glob.glob("src/english/*.c")):
            o = variant_obj("build/" + c[:-2] + ".o", out_dir)
            english_cc(n, c, o, C_TOOLCHAIN, cc_implicit, rule="encc")
            extra.append(o)
        font_o = font_s[:-2] + ".o"
        n.build(font_o, "enas", font_s)
        extra.append(font_o)
        outputs.append(font_json)
    default = ["build/%s.elf" % EXE, "build/%s.map" % EXE]
    script = out_dir + "/%s.ld" % EXE
    n.build(script, "rlink", ["build/%s.ld" % EXE], implicit=default + tools,
            variables={"args": " ".join(["ld main", out_dir, "build/%s.ld" % EXE, "build/%s.map" % EXE, script]
                                        + (["--redirect"] + redirect if redirect else [])
                                        + (["--extra"] + extra if extra else []) + shift)})
    aux_first = out_dir + "/main_aux_first.ld"
    n.build(aux_first, "rlink", [script], implicit=default + side_scripts("main") + tools,
            variables={"args": "aux main %s %s" % (out_dir, aux_first)})
    first = out_dir + "/main_first.elf"
    n.build(first, "rld", main_objs + extra, implicit=[script, aux_first],
            variables={"script": script, "aux": aux_first, "map": out_dir + "/main_first.map"})

    # overlays, linked against the first main link
    ovl_elfs = []
    for name, _base, _text in overlays:
        out = out_dir + "/ovl/%s.bin" % name
        flagged = [cf for cf in srcscan.unit_c_files(name) if english and uses_english(str(cf.src))]
        if name in UNLOADED_OVERLAYS:
            if flagged:
                sys.exit("configure.py: %s is never loaded; do not flag its files" % name)
            n.build(out, "rlink", ["build/ovl/%s.bin" % name], implicit=tools,
                    variables={"args": "copy %s build/ovl/%s.bin %s" % (name, name, out)})
            outputs.append(out)
            continue
        default = ["build/ovl/%s.elf" % name, "build/ovl/%s.map" % name, "build/ovl/%s.bin" % name]
        redirect = []
        for cf in flagged:
            tc = OBJECT_TOOLCHAINS.get(cf.name, OVL_C_TOOLCHAIN)
            english_cc(n, str(cf.src), variant_obj(cf.obj, out_dir), tc, cc_implicit + ["build/ovl/%s.stamp" % name])
            redirect.append(cf.obj)
        ovl_objs = [variant_obj(o, out_dir) if o in redirect else o for o in n_overlay_objects(name)]
        script = out_dir + "/ovl/%s.ld" % name
        n.build(script, "rlink", ["build/ovl/%s.ld" % name], implicit=default + tools,
                variables={"args": " ".join(["ld", name, out_dir, "build/ovl/%s.ld" % name,
                                             "build/ovl/%s.map" % name, script]
                                            + (["--redirect"] + redirect if redirect else []) + ovl_shift)})
        aux = out_dir + "/ovl/%s_aux.ld" % name
        n.build(aux, "rlink", [script], implicit=default + side_scripts(name) + [first, out_dir + "/%s.ld" % EXE]
                + tools, variables={"args": "aux %s %s %s --main-elf %s" % (name, out_dir, aux, first)})
        elf = out_dir + "/ovl/%s.elf" % name
        n.build(elf, "rld", ovl_objs, implicit=[script, aux],
                variables={"script": script, "aux": aux, "map": out_dir + "/ovl/%s.map" % name})
        raw = out_dir + "/ovl/%s.raw" % name
        n.build(raw, "objcopy", elf)
        n.build(out, "rlink", [elf, raw], implicit=default + [first] + hook_deps + tools,
                variables={"args": "ovl %s %s %s %s %s %s %s" % (name, out_dir, elf, raw, first, hooks, out)})
        ovl_elfs.append(elf)
        outputs.append(out)

    # second main link, with the overlay entry points
    entries = out_dir + "/ovl_entries.ld"
    n.build(entries, "rlink", ovl_elfs, implicit=tools + ["build/undefined_syms_auto.txt"],
            variables={"args": "entries %s %s" % (out_dir, entries)})
    aux = out_dir + "/main_aux.ld"
    n.build(aux, "rlink", [script, entries], implicit=default + side_scripts("main") + tools,
            variables={"args": "aux main %s %s --entries %s" % (out_dir, aux, entries)})
    elf = out_dir + "/%s.elf" % EXE
    n.build(elf, "rld", main_objs + extra, implicit=[script, aux],
            variables={"script": out_dir + "/%s.ld" % EXE, "aux": aux, "map": out_dir + "/%s.map" % EXE})
    raw = out_dir + "/%s.raw" % EXE
    n.build(raw, "objcopy", elf)
    exe = out_dir + "/%s.bin" % EXE
    n.build(exe, "rlink", [elf, raw], implicit=default + [first] + hook_deps + tools,
            variables={"args": "exe %s %s %s %s %s %s" % (out_dir, elf, raw, first, hooks, exe)})
    outputs.append(exe)
    return outputs


def english_targets(n, overlays, objs, cc_implicit):
    """`ninja english` and `ninja relink`."""
    relink_rules(n)
    n.build("english", "phony", relink_targets(n, overlays, objs, cc_implicit, EN_DIR, True))
    same = relink_targets(n, overlays, objs, cc_implicit, J0_DIR, False)
    n.rule("rsame", command="cmp $in && touch $out", description="SAME $in")
    oks = []
    for out in same:
        default = "build/" + out[len(J0_DIR) + 1:]
        oks.append(out[:-4] + ".same")
        n.build(oks[-1], "rsame", [out, default])
    n.build(J0_DIR + "/identity.ok", "phony", oks)
    n.build("relink", "phony", [J0_DIR + "/identity.ok"]
            + relink_targets(n, overlays, objs, cc_implicit, JR_DIR, False, (JR_SHIFT_MAIN, JR_SHIFT_OVERLAY)))


def n_overlay_objects(name):
    """Default-build objects of overlay `name`, as overlay_targets links them."""
    return ([cf.obj for cf in srcscan.unit_c_files(name)]
            + ["build/ovl/%s/%s.o" % (name, s[:-2]) for s in overlay_data_files(name)])


def main():
    # -M records files read, not failed higher-priority include lookups. A new
    # or removed file can change resolution without touching an old dependency.
    # Conservatively invalidate C objects on membership changes, not file edits.
    paths = sorted(p for root in ("include", "src")
                   for p in glob.glob(root + "/**/*", recursive=True) if os.path.isfile(p))
    contents = "\n".join(paths) + "\n"
    os.makedirs("build", exist_ok=True)
    previous = None
    if os.path.exists(INCLUDE_FILES):
        with open(INCLUDE_FILES) as f:
            previous = f.read()
    if previous != contents:
        with open(INCLUDE_FILES, "w") as f:
            f.write(contents)
    # config/ and the ledger folder too: a new ledger or per-overlay symbol file changes the inputs (T-27002)
    directories = sorted(glob.glob("include/**/", recursive=True)
                         + glob.glob("src/**/", recursive=True)
                         + ["config/"] + ([LEDGER_DIR + "/"] if os.path.isdir(LEDGER_DIR) else []))
    n = ninja_syntax.Writer(open("build.ninja", "w"))
    n.variable("ninja_required_version", "1.10")
    n.rule("configure", command="python3 configure.py", generator=True, restat=True,
           description="configure")
    # the asm objects to build depend on the subsegments of the yaml files (T-1340)
    images, image_dirs = prepare_disc.find_candidates()
    n.build(["build.ninja", INCLUDE_FILES], "configure", "configure.py",
            implicit=["config/%s.yaml" % EXE, "config/overlays.txt"]
            + ([TOOLCHAINS_FILE] if os.path.exists(TOOLCHAINS_FILE) else []) + image_dirs
            + directories
            + ["config/overlays/%s.yaml" % o[0] for o in read_overlays()])
    # Missing watched directories must allow regeneration after directory removal.
    for directory in directories:
        n.build(directory, "phony")
    # game image -> disc/files (T-3300); reruns only when an image or the tools change, and a new
    # file in game/ changes a directory time, which regenerates build.ninja and so re-finds the images
    n.rule("disc", command="python3 tools/prepare_disc.py --stamp $out", description="DISC game/ -> disc/files")
    n.build(DISC_STAMP, "disc", implicit=images + ["tools/prepare_disc.py", "tools/identify_version.py",
                                                  "tools/extract_disc.py", "config/versions.txt",
                                                  "config/%s.sha1" % EXE])
    n.rule("split",
           command=("python3 -m splat split config/%s.yaml && python3 tools/rodata_pieces.py "
                    "config/%s.yaml && python3 tools/data_pieces.py config/%s.yaml && touch $out")
           % (EXE, EXE, EXE),
           description="splat split")
    # every symbol file of every yaml (T-27002); restat: a symbol file edit that keeps the check
    # passing leaves the stamp alone, so it does not rebuild every C object
    n.rule("symbols", command="python3 tools/check_symbols.py --stamp $out",
           description="CHECK SYMBOLS", restat=True)
    table = symtab.SymbolTable()
    n.build(SYMBOLS_OK, "symbols", implicit=["tools/check_symbols.py", "tools/symtab.py", "config/overlays.txt"]
            + sorted({u.yaml for u in table.units.values()} | set(table.lines)))
    n.build("symbols", "phony", SYMBOLS_OK)
    n.rule("headers", command="python3 tools/check_headers.py include src && touch $out",
           description="CHECK HEADERS")
    n.build(HEADERS_OK, "headers",
            implicit=["tools/check_headers.py", "tools/sync_protos.py", "tools/srcscan.py"]
            + HEADER_FILES + sorted(glob.glob("src/**/*.c", recursive=True)))
    n.build("headers", "phony", HEADERS_OK)
    # old D_ names of aggregate fields (T-5100): tools/migrate_globals.py --check over every C file
    n.rule("globals", command="python3 tools/migrate_globals.py --check && touch $out",
           description="CHECK GLOBALS")
    # a `keep` line is checked against the asm of its object (T-7010), so the split runs first
    n.build(GLOBALS_OK, "globals",
            implicit=["tools/migrate_globals.py", "tools/aggregate_audit.py", "config/migrate_globals.txt"]
            + HEADER_FILES + sorted(glob.glob("src/**/*.c", recursive=True)),
            order_only=["build/split.stamp"] + ["build/ovl/%s.stamp" % o[0] for o in read_overlays()])
    n.build("globals", "phony", GLOBALS_OK)
    # renamed names stay renamed (T-27002): tools/rename.py check over src/, include/, config/
    n.rule("renames", command="python3 tools/rename.py check && touch $out", description="CHECK RENAMES")
    n.build(RENAMES_OK, "renames",
            implicit=["tools/rename.py", "tools/symtab.py"] + sorted(glob.glob(LEDGER_DIR + "/*.txt"))
            + sorted({u.yaml for u in table.units.values()} | set(table.lines))
            + [p for p in ("config/proto_known.txt", "config/migrate_globals.txt", "config/toolchains.txt")
               if os.path.exists(p)]
            + HEADER_FILES + sorted(glob.glob("src/**/*.c", recursive=True)))
    n.build("renames", "phony", RENAMES_OK)
    n.rule("as", command="python3 tools/asm.py $in $out", description="AS $in")
    n.rule("cc",
           command="python3 tools/cc.py $in $out $toolchain && python3 tools/object_layout.py $in $out",
           depfile="$out.d", deps="gcc", description="CC $in")
    n.rule("ld",
           command=("mips-linux-gnu-ld -EL -T build/%s.ld "
                    "-T build/undefined_funcs_auto.txt "
                    "-T build/undefined_syms_auto.txt -T build/%s_data_syms.ld "
                    "-Map build/%s.map -o $out") % (EXE, EXE, EXE),
           description="LD $out")
    n.rule("objcopy", command="mips-linux-gnu-objcopy -O binary $in $out",
           description="OBJCOPY $out")
    n.rule("sha1",
           command=("h=$$(cut -d' ' -f1 config/%s.sha1) && "
                    "echo \"$$h  $in\" | sha1sum -c && touch $out") % EXE,
           description="SHA1 CHECK $in")

    stamp = "build/split.stamp"
    split_in = ["config/%s.yaml" % EXE, "config/reloc_addrs.txt"] + symbol_files("config/%s.yaml" % EXE)
    n.build([stamp, "build/%s.ld" % EXE, "build/%s_data_syms.ld" % EXE] + ASM_FILES, "split", split_in,
            implicit=[DISC_STAMP, "tools/data_pieces.py"])

    objs = []
    sources = []
    cc_implicit = [stamp, SYMBOLS_OK, INCLUDE_FILES, "tools/cc.py", "tools/frame_pass.py", "tools/cvt_pass.py",
                   "tools/ugen_pass.py", "tools/object_layout.py", "include/asmproc_prelude.inc",
                   "include/gte_macros.inc", "include/macro.inc", "include/labels.inc"]
    for c, toolchain in sorted(C_FILES.items()):
        o = "build/" + c[:-2] + ".o"
        n.build(o, "cc", c, variables={"toolchain": " ".join(toolchain)},
                implicit=cc_implicit, order_only=[HEADERS_OK])
        objs.append(o)
        sources.append((c, o))
    for s in ASM_FILES:
        o = "build/" + s[:-2] + ".o"
        n.build(o, "as", s, implicit=[stamp, "include/macro.inc", "tools/asm.py"])
        objs.append(o)
        sources.append((s, o))

    elf = "build/%s.elf" % EXE
    n.build(elf, "ld", objs, implicit=[stamp])
    n.build("build/%s.bin" % EXE, "objcopy", elf)
    n.build("build/%s.ok" % EXE, "sha1", "build/%s.bin" % EXE)
    n.rule("progress", command="python3 tools/progress.py", description="PROGRESS", pool="console")
    n.build("progress", "progress")
    oks = overlay_targets(n, read_overlays())
    english_targets(n, read_overlays(), sources, cc_implicit)
    n.default(["build/%s.ok" % EXE] + oks + [GLOBALS_OK, SYMBOLS_OK, RENAMES_OK])
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
