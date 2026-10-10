#!/usr/bin/env python3
"""Compare IDO 5.2 and 4.1 with the build's IDO 5.3 on the game code (T-3110).

Evaluation tool, not part of the build. IDO 5.2 and 4.1 are IRIX binaries from
decomp.me's compiler distribution (wiki/ido-52-evaluation.md has the source URLs,
checksums and licensing notes). They are never committed: they live in the gitignored
tools/local-compilers/ido5.2 and tools/local-compilers/ido4.1, and run under the
qemu-irix that ships inside each archive, as decomp.me's cromper/compilers.py runs
them. The image needs libglib2.0-0 (5.2's qemu-irix) and a glibc >= 2.38 in
/opt/u2404/lib (4.1's qemu-irix-4.0); the evaluation page lists the Dockerfile lines.

Each compile runs the IDO driver up to ugen (`-Hc -K`), optionally applies
tools/frame_pass.py to ugen's binasm output, and then runs that version's own as1.
IDO 4.1 writes ECOFF; the archive's ecoff_tool.py converts it to ELF.

Usage (inside Docker, repo root):
  python3 tools/ido_eval.py cc VER FRAME <cc flags...> -o OUT SRC
      compiler entry for asm-processor; VER is 5.3, 5.2 or 4.1, or FE/UOPT/UGEN/AS1 (one
      version per pass, e.g. 5.2/5.2/4.1/4.1); FRAME is frame or noframe
  python3 tools/ido_eval.py obj VER FRAME SRC OUT [-- extra cc flags]
      one C file through asm-processor and the trailing/object padding of tools/cc.py
  python3 tools/ido_eval.py regress VER FRAME OUTDIR [stem ...] [-- extra cc flags]
      every src/main/*.c and src/ovl/*.c (or the given stems), each C-defined function
      compared with the ninja-built object (which is sha1-checked against the original);
      prints one line per file and a summary, and writes OUTDIR/<config>.txt
  python3 tools/ido_eval.py cases CASEDIR OUTDIR [-- extra cc flags]
      each CASEDIR/<SEG>__<func>.c (SEG = main or an overlay name) compiled with every
      configuration in CONFIGS; <func> compared with its original .s (asm/);
      prints MATCH or the number of differing lines, and writes OUTDIR/cases.md
  python3 tools/ido_eval.py frames OUTDIR CONFIG
      after `regress`: for the functions OUTDIR/CONFIG.txt lists as different, report those
      whose frame size or set of $sp offsets differs (CONFIG as in the file name, e.g.
      5.2_5.2_4.1_5.2-noframe)
  python3 tools/ido_eval.py stamp VER
      compile an empty function and print the ELF/ECOFF version stamp fields

Exits non-zero on a failed compile (cc, obj) or on bad arguments.
"""
import datetime
import difflib
import glob
import os
import re
import runpy
import shutil
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import cc  # noqa: E402
import frame_pass  # noqa: E402
import funcdiff  # noqa: E402
import trailing_pad  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LOCAL = os.path.join(ROOT, "tools", "local-compilers")
U2404 = "/opt/u2404/lib"
HERE = os.path.abspath(__file__)

# as1 arguments the 5.2 and 4.1 drivers pass for `-O2 -G 0 -EL` (read from `cc -v`).
AS1_ARGS = {"5.3": ["-G", "0", "-p0", "-EL", "-g0", "-O2", "-elf"],
            "5.2": ["-G", "0", "-p0", "-EL", "-g0", "-O2", "-elf"],
            "4.1": ["-G", "0", "-p0", "-EL", "-g0", "-O2"]}


def qemu(ver, tmp):
    """Command prefix that runs an IRIX binary of IDO `ver` under its qemu-irix."""
    root = os.path.join(LOCAL, "ido" + ver)
    if ver == "5.2":
        return [os.path.join(root, "usr/bin/qemu-irix"), "-silent", "-L", root], root
    # qemu-irix-4.0 needs glibc >= 2.38 and re-executes itself for every pass,
    # so it is started through a wrapper that names the newer loader.
    wrap = os.path.join(tmp, "qemu-irix-4.0")
    with open(wrap, "w") as f:
        f.write('#!/bin/sh\nexec %s/ld-linux-x86-64.so.2 --library-path %s --argv0 "$0" '
                '%s "$@"\n' % (U2404, U2404, os.path.join(root, "usr/bin/qemu-irix-4.0")))
    os.chmod(wrap, 0o755)
    return [wrap, "-silent", "-L", root], root


def ecoff_to_elf(root, src, dst):
    """Convert an IDO 4.1 ECOFF object with the archive's ecoff_tool.py (needs datetime.UTC)."""
    if not hasattr(datetime, "UTC"):
        datetime.UTC = datetime.timezone.utc
    argv = sys.argv
    sys.argv = ["ecoff_tool.py", "--convert-elf", src, "-o", dst]
    try:
        runpy.run_path(os.path.join(root, "usr/bin/ecoff_tool.py"), run_name="__main__")
    except SystemExit as e:
        if e.code not in (None, 0):
            raise
    finally:
        sys.argv = argv


def set_function_sizes(path):
    """Fix up an object converted by ecoff_tool.py.

    Every size-0 FUNC symbol of .text gets the distance to the next function (ECOFF has no
    sizes), and REFWORD relocations get the right ELF type.
    """
    with open(path, "rb") as f:
        data = bytearray(f.read())
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum = struct.unpack_from("<HH", data, 0x2E)
    secs = [struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize) for i in range(shnum)]
    symtab = [s for s in secs if s[1] == 2][0]
    text_idx = text_size = None
    shstr = secs[struct.unpack_from("<H", data, 0x32)[0]]
    for i, s in enumerate(secs):
        name = data[shstr[4] + s[0]:data.index(b"\0", shstr[4] + s[0])]
        if name == b".text":
            text_idx, text_size = i, s[5]
    syms = []
    for off in range(symtab[4], symtab[4] + symtab[5], 16):
        _, value, size, info, _, shndx = struct.unpack_from("<IIIBBH", data, off)
        if shndx == text_idx and info & 0xF == 2:
            syms.append((off, value, size))
    # ecoff_tool.py maps ECOFF R_REFWORD (2) to ELF type 3 (R_MIPS_REL32); it is R_MIPS_32 (2).
    for s in secs:
        if s[1] == 9:  # SHT_REL
            for off in range(s[4], s[4] + s[5], 8):
                info, = struct.unpack_from("<I", data, off + 4)
                if info & 0xFF == 3:
                    struct.pack_into("<I", data, off + 4, (info & ~0xFF) | 2)
    starts = sorted({v for _, v, _ in syms}) + [text_size]
    for off, value, size in syms:
        if size == 0:
            nxt = [s for s in starts if s > value][0]
            struct.pack_into("<I", data, off + 8, nxt - value)
    with open(path, "wb") as f:
        f.write(data)


def run(cmd, cwd=None, env=None):
    proc = subprocess.run(cmd, cwd=cwd, env=env, stdout=subprocess.PIPE,
                          stderr=subprocess.STDOUT)
    if proc.returncode != 0 or proc.stdout:
        sys.stderr.write(proc.stdout.decode("latin-1"))
    if proc.returncode != 0:
        sys.exit("ido_eval.py: failed (%d): %s" % (proc.returncode, " ".join(cmd)))


def absolute_includes(flags):
    out, flags = [], list(flags)
    while "-I" in flags:  # asm-processor passes "-I dir" as two arguments
        i = flags.index("-I")
        flags[i:i + 2] = ["-I" + flags[i + 1]]
    for f in flags:
        if f.startswith("-I") and len(f) > 2 and not f[2:].startswith("/"):
            f = "-I" + os.path.abspath(f[2:])
        out.append(f)
    return out


def split_spec(ver):
    """'5.2' -> all four passes from 5.2; 'A/B/C/D' -> front end, uopt, ugen, as1 versions."""
    parts = ver.split("/")
    if len(parts) == 1:
        parts *= 4
    if len(parts) != 4 or any(p not in ("5.3", "5.2", "4.1") for p in parts):
        sys.exit("ido_eval.py: bad compiler spec %r" % ver)
    return parts


def pass_cmd(ver, name, tmp):
    """Command prefix that runs pass `name` (uopt, ugen, as1, or the driver 'cc') of IDO `ver`."""
    if ver == "5.3":
        return ["/opt/ido/5.3/" + name]
    pre, root = qemu(ver, tmp)
    if name == "cc":
        name = "usr/lib/driver" if ver == "5.2" else "usr/bin/cc"
    else:
        name = "usr/lib/" + name
    return pre + [os.path.join(root, name)]


def compile_c(ver, frame, flags, out, src):
    """Compile `src` to the ELF object `out` with the passes of `ver`; frame pass if `frame`.

    The front end's driver runs up to ugen with -Hc -K, which leaves cfe's ucode (u.B), the
    symbol table (u.T), uopt's output (u.O) and ugen's binasm (u.G). Passes taken from
    another version are then rerun on those files with the arguments the drivers use.
    """
    fe, uo, ug, a1 = split_spec(ver)
    src, out = os.path.abspath(src), os.path.abspath(out)
    tmp = tempfile.mkdtemp(prefix="idoeval")
    env = dict(os.environ, USR_LIB="/opt/ido/5.3")
    try:
        cflags = [f for f in absolute_includes(flags) if f != "-c"]
        if fe == "4.1":
            # 4.1's acpp (GNU cpp 1.35) hangs under qemu-irix-4.0 on any #include, so
            # the file is preprocessed by the host cpp with the macros the headers test
            # (__sgi) and the driver gets the .i (it then skips acpp).
            incs = [f for f in cflags if f.startswith("-I")]
            cflags = [f for f in cflags if not f.startswith("-I")]
            with open(os.path.join(tmp, "u.i"), "wb") as f:
                f.write(subprocess.run(
                    ["cpp", "-P", "-undef", "-D__sgi", "-Dsgi", "-DLANGUAGE_C", "-D_LANGUAGE_C",
                     "-Dmips", "-D__mips=1", "-D_MIPSEL", "-DMIPSEL"] + incs + [src],
                    check=True, stdout=subprocess.PIPE).stdout)
            inp = "u.i"
        else:
            shutil.copy(src, os.path.join(tmp, "u.c"))
            inp = "u.c"
        run(pass_cmd(fe, "cc", tmp) + ["-c", "-Hc", "-K"] + cflags + [inp], cwd=tmp, env=env)
        uopt_flags = [f[4:] for f in cflags if f.startswith("-Wo,")]
        opt = ["-G", "0", "-EL", "-g0", "-O2"]
        if uo != fe:
            run(pass_cmd(uo, "uopt", tmp) + opt[:2] + [x for f in uopt_flags for x in f.split(",")]
                + opt[2:] + ["u.B", "u.O", "-t", "u.T", "u.S"], cwd=tmp, env=env)
        if ug != fe or uo != fe:
            run(pass_cmd(ug, "ugen", tmp) + opt + ["u.O", "-o", "u.G", "-t", "u.T",
                                                    "-temp", "u.tmp"], cwd=tmp, env=env)
        g = os.path.join(tmp, "u.G")
        if frame:
            with open(g, "rb") as f:
                data = frame_pass.transform(f.read())
            with open(g, "wb") as f:
                f.write(data)
        obj = os.path.join(tmp, "u.o")
        run(pass_cmd(a1, "as1", tmp) + AS1_ARGS[a1] + ["u.G", "-o", obj, "-t", "u.T"],
            cwd=tmp, env=env)
        if a1 == "4.1":
            root = os.path.join(LOCAL, "ido4.1")
            ecoff_to_elf(root, obj, out)
            set_function_sizes(out)
            # ECOFF read-only data is .rdata; asm-processor expects IDO's ELF name .rodata
            names = subprocess.run(["mips-linux-gnu-readelf", "-SW", out], check=True,
                                   stdout=subprocess.PIPE, text=True).stdout
            if " .rdata " in names:
                run(["mips-linux-gnu-objcopy", "--rename-section", ".rdata=.rodata", out])
        else:
            shutil.copy(obj, out)
    finally:
        shutil.rmtree(tmp)


def cmd_cc(argv):
    ver, frame, rest = argv[0], argv[1] == "frame", argv[2:]
    i = rest.index("-o")
    out, src = rest[i + 1], rest[-1]
    flags = rest[:i] + rest[i + 2:-1]
    compile_c(ver, frame, flags, out, src)


def obj(ver, frame, src, out, extra=()):
    """One C file through asm-processor with IDO `ver`, then the padding passes of tools/cc.py."""
    flags = list(cc.IDO_CFLAGS) + list(extra)
    cmd = (["python3", cc.ASM_PROCESSOR, "--no-dep-file", "--drop-mdebug-gptab",
            "--convert-statics", "no", "--asm-prelude", cc.ASM_PRELUDE,
            "python3", HERE, "cc", ver, "frame" if frame else "noframe", "--"]
           + cc.AS + ["--"] + flags + ["-o", out, src])
    proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    if proc.returncode != 0:
        return proc.stdout.decode("latin-1")[-2000:]
    with open(src) as f:
        includes = {n for _, n in cc.INCLUDE_ASM_RE.findall(f.read())}
    trailing_pad.pad_functions(out, src, includes)
    cc.pad_text(out)
    return None


def built_object(src):
    stem = os.path.splitext(os.path.basename(src))[0]
    if "/main/" in src:
        return "build/src/main/%s.o" % stem
    return "build/ovl/%s/src/ovl/%s.o" % (stem, stem)


def c_functions(src, objfile):
    """Function symbols of `objfile` that `src` defines in C (not INCLUDE_ASM)."""
    with open(src) as f:
        includes = {n for _, n in cc.INCLUDE_ASM_RE.findall(f.read())}
    out = subprocess.run(["mips-linux-gnu-nm", objfile], check=True,
                         stdout=subprocess.PIPE, text=True).stdout
    names = set()
    for line in out.splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[1] in "Tt" and parts[2] not in includes \
                and not parts[2].endswith(".NON_MATCHING") \
                and not re.match(r"^(L[0-9A-Fa-f]{8}|\.?L\w+|jtbl_\w+)$", parts[2]):
            names.add(parts[2])
    return sorted(names)


def cmd_regress(argv, extra):
    ver, frame, outdir = argv[0], argv[1] == "frame", argv[2]
    stems = argv[3:]
    srcs = sorted(glob.glob("src/main/*.c")) + sorted(glob.glob("src/ovl/*.c"))
    if stems:
        srcs = [s for s in srcs if os.path.splitext(os.path.basename(s))[0] in stems]
    os.makedirs(outdir, exist_ok=True)
    tag = "%s-%s%s" % (ver.replace("/", "_"), argv[1], "".join(extra).replace(",", "_") if extra else "")
    total = same = 0
    lines, failed = [], []
    for src in srcs:
        ref = built_object(src)
        names = c_functions(src, ref)
        if not names:
            continue
        out = os.path.join(outdir, tag + "-" + os.path.basename(src)[:-2] + ".o")
        err = obj(ver, frame, src, out, extra)
        if err:
            failed.append(src)
            lines.append("%s: COMPILE FAILED (%d C functions)\n%s" % (src, len(names), err))
            total += len(names)
            print("%s: compile failed" % src, flush=True)
            continue
        want = funcdiff.functions(ref, names)
        got = funcdiff.functions(out, names)
        diff = [n for n in names if got[n] != want[n]]
        total += len(names)
        same += len(names) - len(diff)
        lines.append("%s: %d/%d identical; differ: %s" % (src, len(names) - len(diff),
                                                          len(names), " ".join(diff)))
        print("%s: %d/%d" % (src, len(names) - len(diff), len(names)), flush=True)
    summary = "%s: %d of %d C functions identical; %d files failed to compile" % (
        tag, same, total, len(failed))
    with open(os.path.join(outdir, tag + ".txt"), "w") as f:
        f.write("\n".join(lines + [summary]) + "\n")
    print(summary)


def cmd_stamp(ver):
    tmp = tempfile.mkdtemp(prefix="idostamp")
    try:
        src = os.path.join(tmp, "s.c")
        with open(src, "w") as f:
            f.write("void f(void) {}\n")
        if ver == "5.3":
            run(["/opt/ido/5.3/cc", "-c", "-EL", "-O2", "-G", "0", "-non_shared", "-o",
                 os.path.join(tmp, "s.o"), src], env=dict(os.environ, USR_LIB="/opt/ido/5.3"))
        else:
            pre, root = qemu(ver, tmp)
            driver = os.path.join(root, "usr/lib/driver" if ver == "5.2" else "usr/bin/cc")
            run(pre + [driver, "-c", "-EL", "-O2", "-G", "0", "-non_shared", "s.c"], cwd=tmp)
        with open(os.path.join(tmp, "s.o"), "rb") as f:
            data = f.read()
        print("first 8 bytes: %s, size %d" % (data[:8].hex(), len(data)))
        if data[:4] == b"\x7fELF":
            # .mdebug starts with the symbolic header (HDRR): magic 0x7009, vstamp
            shoff, = struct.unpack_from("<I", data, 0x20)
            shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
            secs = [struct.unpack_from("<IIIIII", data, shoff + i * shentsize)
                    for i in range(shnum)]
            for sec in secs:
                if sec[1] == 0x70000005:  # SHT_MIPS_DEBUG
                    end = ">" if data[sec[4]:sec[4] + 2] == b"\x70\x09" else "<"
                    magic, vst = struct.unpack_from(end + "HH", data, sec[4])
                    print("ELF; .mdebug HDRR (%s) magic 0x%04x vstamp 0x%04x (%d.%d)"
                          % ("big-endian" if end == ">" else "little-endian",
                             magic, vst, vst >> 8, vst & 0xFF))
        else:
            # ECOFF object: the file header may be stored in either byte order
            end = ">" if data[:2] == b"\x01\x62" else "<"
            magic, nscns, timdat, symptr, nsyms, opthdr, flags = struct.unpack_from(
                end + "HHIIIHH", data, 0)
            hmagic, hv = struct.unpack_from(end + "HH", data, symptr)
            print("ECOFF, header byte order %s: f_magic 0x%04x f_flags 0x%04x opthdr %d; "
                  "HDRR magic 0x%04x vstamp 0x%04x (%d.%d)"
                  % ("big-endian" if end == ">" else "little-endian", magic, flags, opthdr,
                     hmagic, hv, hv >> 8, hv & 0xFF))
    finally:
        shutil.rmtree(tmp)


# (compiler spec, frame pass). A spec is one version or front end/uopt/ugen/as1 versions.
CONFIGS = [("5.3", True), ("5.2", True), ("5.2", False), ("4.1", False),
           ("5.2/5.2/4.1/4.1", False), ("5.2/5.2/4.1/5.2", False),
           ("5.2/4.1/5.2/5.2", True), ("5.2/4.1/4.1/4.1", False)]


def find_asm(seg, func):
    pats = (["asm/nonmatchings/main/*/%s.s", "asm/matchings/main/*/%s.s"] if seg == "main" else
            ["asm/ovl/%s/nonmatchings/%s/%%s.s" % (seg, seg), "asm/ovl/%s/matchings/%s/%%s.s" % (seg, seg)])
    for p in pats:
        hits = glob.glob(p % func)
        if hits:
            return hits[0]
    return None


def normalized(lines):
    """Drop trailing nops (alignment padding) and name rodata relocations alike."""
    out = [re.sub(r"(R_MIPS_\w+\s+)(\.rodata\S*|jtbl_\w+|D_\w+\.rodata)", r"\1RODATA", ln)
           for ln in lines or []]
    while out and out[-1].endswith("nop"):
        out.pop()
    return out


def cmd_cases(casedir, outdir, configs=CONFIGS, extra=()):
    """Compile each case file (SEG__func.c) and compare `func` with its original .s."""
    os.makedirs(outdir, exist_ok=True)
    rows = []
    for case in sorted(glob.glob(os.path.join(casedir, "*.c"))):
        seg, func = os.path.basename(case)[:-2].split("__")
        asm = find_asm(seg, func)
        if asm is None:
            rows.append((func, ["no asm"] * len(configs)))
            continue
        target = os.path.join(outdir, "%s__%s.target.o" % (seg, func))
        with open(asm) as f:
            text = '.include "asmproc_prelude.inc"\n' + f.read()
        tmp_s = target + ".s"
        with open(tmp_s, "w") as f:
            f.write(text)
        subprocess.run(cc.AS + ["-o", target, tmp_s], check=True)
        want = normalized(funcdiff.functions(target, [func])[func])
        cells = []
        for ver, frame in configs:
            out = os.path.join(outdir, "%s__%s.%s-%s.o" % (seg, func, ver.replace("/", "_"),
                                                             "f" if frame else "n"))
            proc = subprocess.run(["python3", HERE, "cc", ver, "frame" if frame else "noframe"]
                                  + list(cc.IDO_CFLAGS) + list(extra) + ["-o", out, case],
                                  stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
            if proc.returncode != 0:
                cells.append("ERR")
                continue
            got = normalized(funcdiff.functions(out, [func])[func])
            if got == want:
                cells.append("MATCH")
            else:
                n = sum(1 for d in difflib.ndiff(want, got) if d[:1] in "+-")
                cells.append("%d" % n)
        rows.append((func, cells))
        print("%-16s %s" % (func, " ".join("%6s" % c for c in cells)), flush=True)
    head = "| case | " + " | ".join("%s%s" % (v, " +pass" if f else "") for v, f in configs) + " |"
    lines = [head, "|" + "---|" * (len(configs) + 1)]
    lines += ["| `%s` | %s |" % (fn, " | ".join(c)) for fn, c in rows]
    with open(os.path.join(outdir, "cases.md"), "w") as f:
        f.write("\n".join(lines) + "\n")
    print("\n".join(lines))


def cmd_frames(outdir, tag):
    """For the functions a `regress` run reported as different, compare frame sizes and $sp offsets."""
    with open(os.path.join(outdir, tag + ".txt")) as f:
        txt = f.read()
    sp = re.compile(r"addiu sp,sp,-?\d+|-?\d+\(sp\)")
    total, bad = 0, []
    for line in txt.splitlines():
        m = re.match(r"(src/(?:main|ovl)/\w+\.c): .*differ: (.*)", line)
        if not m or not m.group(2).strip():
            continue
        src, names = m.group(1), m.group(2).split()
        out = os.path.join(outdir, tag + "-" + os.path.basename(src)[:-2] + ".o")
        want, got = funcdiff.functions(built_object(src), names), funcdiff.functions(out, names)
        for n in names:
            total += 1
            if sorted(sp.findall(" ".join(want[n]))) != sorted(sp.findall(" ".join(got[n]))):
                bad.append(n)
    print("%d differing functions; frame size or $sp offsets differ in %d: %s"
          % (total, len(bad), " ".join(bad)))


def main(argv):
    extra = []
    if "--" in argv:
        i = argv.index("--")
        argv, extra = argv[:i], argv[i + 1:]
    if len(argv) < 2:
        sys.exit(__doc__)
    if argv[1] == "cc" and len(argv) >= 6:
        cmd_cc(argv[2:])
    elif argv[1] == "obj" and len(argv) == 6:
        err = obj(argv[2], argv[3] == "frame", argv[4], argv[5], extra)
        if err:
            sys.exit(err)
    elif argv[1] == "regress" and len(argv) >= 5:
        cmd_regress(argv[2:], extra)
    elif argv[1] == "cases" and len(argv) == 4:
        cmd_cases(argv[2], argv[3], extra=extra)
    elif argv[1] == "frames" and len(argv) == 4:
        cmd_frames(argv[2], argv[3])
    elif argv[1] == "stamp" and len(argv) == 3:
        cmd_stamp(argv[2])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main(sys.argv)
