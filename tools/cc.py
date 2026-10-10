#!/usr/bin/env python3
"""Compile one C file with the toolchain chosen for its segment, checking every stage.

Usage (inside Docker, called by ninja):
    python3 tools/cc.py <src.c> <out.o> ido <ido_version>
    python3 tools/cc.py <src.c> <out.o> gcc <gcc_version> <aspsx_version>

ido: SGI IDO (decompals/ido-static-recomp) run through asm-processor, which
     splices the INCLUDE_ASM functions in and assembles them with GNU as.
     Used for the game code (T-0013, wiki/matching-notes.md). Every IDO
     compile also runs the frame-layout emulation pass (tools/frame_pass.py,
     T-0016): the original's frames are 16 bytes larger than IDO's, and the
     unsigned-load conversion pass around uopt (tools/cvt_pass.py, T-1321,
     T-5010): the original keeps unsigned byte and halfword globals in a register.
gcc: the PsyQ way, cpp | cc1 | maspsx | as.

Every object's .text is zero-padded to a multiple of 16 bytes (T-0012): the original link
aligned the .text of each object to 16, and the build's linker script uses SUBALIGN(2), so the
padding that the original linker added after the last function of a source file is made part of
the object. Without it, a file whose last function is C (not INCLUDE_ASM, which carries the
padding nops itself) would shift every later file. See wiki/source-files.md.

After the compile, tools/trailing_pad.py (T-1310) puts the original's trailing zero words back
after every function the C source defines, read from the function's splat disassembly under
asm/. That replaces the old per-site INCLUDE_ASM("src/ovl/pad", ...) stubs, which asm-processor
could not do for a single nop. Rule and limits: wiki/toolchain.md, tools/trailing_pad.py.

C sources are UTF-8. Before IDO, string and character literals with non-ASCII characters are
re-encoded to Shift-JIS octal escapes (sjis_literals, T-0500; the rule of tools/asm.py), and the
copy that is compiled is <out.o>.sjis.c. Non-ASCII text outside literals stops the compile.

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

import trailing_pad

AS = ["mips-linux-gnu-as", "-EL", "-march=r3000", "-mabi=32", "-G0",
      "-Iinclude", "-I."]
INCLUDE_ASM_RE = re.compile(r'^INCLUDE_(?:ASM|RODATA)\("([^"]*)",\s*(\w+)\)', re.M)

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
CVT_PASS = os.path.join(os.path.dirname(os.path.abspath(__file__)), "cvt_pass.py")
# IDO passes replaced by a shim that runs a toolchain emulation pass and then the real pass.
# tools/cvt_pass.py (T-1321, entry and compare rules T-5010) is in the build since T-5010:
# with those rules it leaves every matched function unchanged except the ones on unit-private
# data, which switch on a local copy (wiki/matching-notes.md, "Selector register rule").
SHIMS = {"as1": FRAME_PASS, "uopt": CVT_PASS}


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


def ido_frame_env(ido_ver, tmp, extra_shims=None):
    """Environment that runs IDO with the toolchain emulation passes.

    IDO's cc finds its passes in USR_LIB. Point it at a directory holding
    symlinks to every IDO file except the shimmed passes (SHIMS plus
    extra_shims): `as1` applies the frame-layout emulation pass
    (tools/frame_pass.py, T-0016) to ugen's output before the real as1, for
    every function; `uopt` runs the unsigned-load conversion pass
    (tools/cvt_pass.py, T-1321, T-5010) around the real uopt. extra_shims
    adds or replaces shims for experiments; a None value removes one.
    """
    ido = "/opt/ido/%s" % ido_ver
    shims = {k: v for k, v in dict(SHIMS, **(extra_shims or {})).items() if v}
    for name in sorted(os.listdir(ido)):
        if name not in shims:
            os.symlink(os.path.join(ido, name), os.path.join(tmp, name))
    for name, script in sorted(shims.items()):
        shim = os.path.join(tmp, name)
        with open(shim, "w") as f:
            f.write('#!/bin/sh\nexec python3 "%s" --%s "%s/%s" "$@"\n'
                    % (script, name, ido, name))
        os.chmod(shim, 0o755)
    env = dict(os.environ)
    env["USR_LIB"] = tmp
    return env


def sjis_literals(text):
    """C source text with every non-ASCII character inside a string or character literal
    replaced by its Shift-JIS bytes as octal escapes (the game's text encoding; T-0500). The
    sources are UTF-8 so the strings stay readable; IDO reads bytes, and a Shift-JIS second byte
    0x5C would end a literal early, so escapes are the only safe form. Comments and code are
    left alone."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(text[i:j])
            i = j
        elif text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(text[i:j])
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c:
                j += 2 if text[j] == "\\" else 1
            body = text[i + 1:j]
            out.append(c + "".join(ch if ord(ch) < 0x80 else
                                   "".join("\\%03o" % b for b in ch.encode("shift_jis"))
                                   for ch in body) + c)
            i = j + 1
        else:
            out.append(c)
            i += 1
    return "".join(out)


def compile_ido(src, out, ido_ver):
    with open(src, encoding="utf-8") as f:
        text = f.read()
    if any(ord(c) >= 0x80 for c in text):
        # compile a re-encoded copy next to the object (asm-processor and cfe only see ASCII)
        text = sjis_literals(text)
        bad = [c for c in text if ord(c) >= 0x80]
        if bad:
            sys.exit("cc.py: %s: non-ASCII character %r outside a string literal" % (src, bad[0]))
        src = out + ".sjis.c"
        with open(src, "w", encoding="ascii") as f:
            f.write(text)
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
    """Zero-pad the .text section of `obj` to a multiple of 16 bytes (original per-object alignment).

    Edited in Python (trailing_pad.pad_text_to): objcopy --update-section drops the relocations
    when the section grows (T-1310).
    """
    trailing_pad.pad_text_to(obj, 16)


def main(argv):
    if len(argv) < 5:
        sys.exit(__doc__)
    src, out, kind = argv[1:4]
    with open(src) as f:
        text = f.read()
    includes = INCLUDE_ASM_RE.findall(text)
    deps = ["%s/%s.s" % m for m in includes]
    if kind == "ido" and len(argv) == 5:
        compile_ido(src, out, argv[4])
    elif kind == "gcc" and len(argv) == 6:
        compile_gcc(src, out, argv[4], argv[5])
    else:
        sys.exit(__doc__)
    deps += trailing_pad.pad_functions(out, src, {name for _, name in includes})
    pad_text(out)
    with open(out + ".d", "w") as f:
        f.write("%s: %s\n" % (out, " ".join(deps)))


if __name__ == "__main__":
    main(sys.argv)
