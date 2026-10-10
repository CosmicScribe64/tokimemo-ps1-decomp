#!/usr/bin/env python3
"""Run m2c on one function with project context and print a C draft (T-1330).

Usage (in Docker): python3 tools/m2c.py [--unit UNIT|PATH] [--no-context] [--no-rodata] [--target T] [UNIT:]func_80045414 [-- m2c options]
UNIT is `main` or an overlay name. All overlays load at 0x80132000, so a name such as func_8013xxxx
exists in several of them (T-3300). The unit is taken, in this order, from `--unit`, from a `UNIT:`
prefix, from a path given as the function (an asm .s file) or as --unit (a C or asm file under
src/ovl/<NAME>.c, src/ovl/<NAME>/<addr>.c, src/main/<addr>.c, asm/ovl/<NAME>/...), or inferred
from the C file that still holds `INCLUDE_ASM(..., func)`. A name that stays ambiguous is refused
with the list of candidate units.

What it adds over a bare `m2c <file>.s`:
- finds the function's .s under asm/{nonmatchings,matchings}/ (main exe) or asm/ovl/<NAME>/ (overlay);
- builds a context file by preprocessing include/game.h (+ include/ovl/<NAME>.h for an overlay) with
  `gcc -E -P -D__sgi`, so m2c knows the real globals, struct layouts and callee prototypes;
- extracts only the rodata blocks the function references (jump tables, strings) from the
  segment's *.rodata.s, so `switch` statements come out as `switch`, not as computed jumps;
- rewrites `%lo(D_<addr>)` after a `lui` with a literal high half (splat's form for an address that
  has no symbol of its own) to the signed low half; m2c drops such a %lo (T-3300, fix_lo_literals);
- uses the IDO little-endian target (`mipsel-ido-c`), the compiler family of the game code.
The draft goes to stdout (body only); declarations m2c invented for symbols missing from the
headers go to stderr, so the C can be pasted into src/ without the noise. Output is a starting
point: clean it to C89 and our types (CODING_STANDARDS), and verify with funcdiff.py.
"""
import argparse
import glob
import os
import re
import subprocess
import sys
import tempfile

DEFAULT_TARGET = "mipsel-ido-c"
CPP = ["gcc", "-E", "-P", "-Iinclude", "-D__sgi"]
LABEL_RE = re.compile(r"^(?:dlabel|glabel|jlabel)\s+(\w+)\s*$")
END_RE = re.compile(r"^enddlabel\s+(\w+)\s*$")
WORD_RE = re.compile(r"[A-Za-z_]\w*")


class AmbiguousError(LookupError):
    """The function name exists in several units and nothing says which one is meant."""


def unit_of_path(path):
    """Unit (`main` or an overlay name) a source or asm path belongs to, or None.
    src/ovl/<NAME>.c, src/ovl/<NAME>/<addr>.c and asm/ovl/<NAME>/... give <NAME>; src/main/*.c
    and asm/{nonmatchings,matchings}/main/... give main."""
    p = str(path).replace(os.sep, "/")
    m = re.search(r"(?:^|/)(?:src|asm)/ovl/([^/.]+)", p)
    if m:
        return m.group(1)
    if re.search(r"(?:^|/)src/main/|(?:^|/)asm/(?:non)?matchings/main/|(?:^|/)asm/data/", p):
        return "main"
    return None


def _unit_arg(unit):
    """`--unit` value: a unit name, or a path from which the unit is taken."""
    if unit and ("/" in unit or unit.endswith((".c", ".s"))):
        found = unit_of_path(unit)
        if found is None:
            raise LookupError("cannot tell the unit from the path %s (expected src/ovl/<NAME>.c, "
                              "src/main/<addr>.c or an asm/ovl/<NAME>/... path)" % unit)
        return found
    return unit


def _hit_unit(root, path):
    return unit_of_path(os.path.relpath(path, root)) or "main"


def units_with_include_asm(root, name):
    """Units whose C sources still hold `INCLUDE_ASM(..., name)`."""
    pat = re.compile(r'^\s*INCLUDE_ASM\(\s*"[^"]+"\s*,\s*%s\s*\)' % re.escape(name), re.M)
    out = set()
    for pattern in ("src/main/*.c", "src/ovl/*.c", "src/ovl/*/*.c"):
        for c in glob.glob(os.path.join(root, pattern)):
            with open(c, errors="replace") as f:
                if pat.search(f.read()):
                    out.add(_hit_unit(root, c))
    return out


def locate(root, name, unit=None):
    """Return (asm_path, overlay_name or None) for function `name`, or raise LookupError.

    `name` is a function name, `UNIT:func_X` or the path of an asm file. `unit` (`main`, an overlay
    name or a source/asm path) picks the unit when several overlays have a function at the same
    address. Without it the unit is inferred from the C file holding the function's INCLUDE_ASM; a
    name that is still ambiguous raises AmbiguousError (no silent first match)."""
    if name.endswith(".s") or "/" in name:
        path = name if os.path.isabs(name) else os.path.join(root, name)
        if not os.path.isfile(path):
            raise LookupError("no such asm file: %s" % name)
        m = re.search(r"asm/ovl/([^/]+)/", path.replace(os.sep, "/"))
        return path, (m.group(1) if m else None)
    if ":" in name:
        prefix, name = name.split(":", 1)
        if unit and _unit_arg(unit) != prefix:
            raise LookupError("conflicting units: %s: prefix and --unit %s" % (prefix, unit))
        unit = prefix
    unit = _unit_arg(unit)
    pats = [
        "asm/nonmatchings/main/*/%s.s",
        "asm/matchings/main/*/%s.s",
        "asm/ovl/*/nonmatchings/**/%s.s",
        "asm/ovl/*/matchings/**/%s.s",
    ]
    hits = []
    for pat in pats:
        hits += glob.glob(os.path.join(root, pat % name), recursive=True)
    if not hits:
        raise LookupError("no asm file for %s under %s/asm" % (name, root))
    by_unit = {}
    for h in sorted(hits):
        by_unit.setdefault(_hit_unit(root, h), []).append(h)
    if unit:
        if unit not in by_unit:
            raise LookupError("%s is not in unit %s (it exists in: %s)"
                              % (name, unit, ", ".join(sorted(by_unit))))
        chosen = unit
    elif len(by_unit) == 1:
        chosen = next(iter(by_unit))
    else:
        inferred = units_with_include_asm(root, name) & set(by_unit)
        if len(inferred) != 1:
            raise AmbiguousError(
                "%s exists in %d units (%s) and no single INCLUDE_ASM names it; pick one with "
                "--unit NAME, a UNIT:%s prefix or a source path as --unit"
                % (name, len(by_unit), ", ".join(sorted(by_unit)), name))
        chosen = next(iter(inferred))
    if len(by_unit[chosen]) > 1:
        raise AmbiguousError("%s has %d asm files in unit %s: %s; pass the .s path"
                             % (name, len(by_unit[chosen]), chosen, ", ".join(by_unit[chosen])))
    path = by_unit[chosen][0]
    m = re.search(r"asm/ovl/([^/]+)/", path.replace(os.sep, "/"))
    return path, (m.group(1) if m else None)


LUI_RE = re.compile(r"\blui\s+(\$\w+)\s*,\s*(\S.*?)\s*$")
LO_RE = re.compile(r"%lo\(\s*([A-Za-z_]\w*)\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*)?\)")
HI_RE = re.compile(r"%hi\(\s*([A-Za-z_]\w*)")
ADDR_NAME_RE = re.compile(r"_([0-9A-Fa-f]{8})$")


def fix_lo_literals(text):
    """Replace `%lo(D_801D63C8)` by its signed low half (`0x63C8`) where the base register was
    loaded by a `lui` with a literal high half, as in
        lui $t9, (0x801D2000 >> 16) ; lh $t0, %lo(D_801D63C8)($t9)
    m2c (upstream, also at its newest commit) pairs a %lo only with a %hi of the same symbol;
    an unpaired %lo becomes 0, so the draft read `*(s16 *)0x801D0000` instead of 0x801D63C8.
    Only symbols that carry their address in the name (`<prefix>_<8 hex digits>`, splat's
    naming) can be resolved; %hi/%lo pairs of one symbol are left alone."""
    hi_syms = set(HI_RE.findall(text))
    kinds = {}   # register -> "hi" (lui %hi(sym)) or "lit" (lui with a literal)
    out = []
    for line in text.splitlines():
        m = LUI_RE.search(line)
        if m:
            kinds[m.group(1)] = "hi" if m.group(2).startswith("%hi") else "lit"
            out.append(line)
            continue
        base = re.search(r"%lo\([^)]*\)\s*\((\$\w+)\)", line)
        if base is None:
            base = re.search(r"\baddiu?\s+\$\w+\s*,\s*(\$\w+)\s*,\s*%lo\(", line)
        reg = base.group(1) if base else None

        def sub(mm, reg=reg):
            sym = mm.group(1)
            a = ADDR_NAME_RE.search(sym)
            kind = kinds.get(reg)
            if not a or kind == "hi" or (kind is None and sym in hi_syms):
                return mm.group(0)
            addr = int(a.group(1), 16)
            if mm.group(2):
                delta = int(mm.group(3), 0)
                addr += -delta if mm.group(2) == "-" else delta
            lo = addr & 0xFFFF
            if lo >= 0x8000:
                lo -= 0x10000
            return "-0x%X" % -lo if lo < 0 else "0x%X" % lo
        out.append(LO_RE.sub(sub, line))
    return "\n".join(out) + ("\n" if text.endswith("\n") else "")


def context_sources(root, overlay):
    """Headers whose declarations make up the context, in include order."""
    heads = ["include/game.h"]
    if overlay:
        ovl = "include/ovl/%s.h" % overlay
        if os.path.exists(os.path.join(root, ovl)):
            heads.append(ovl)
    return heads


def context_text(heads):
    """C source that includes `heads`; fed to the preprocessor."""
    return "".join('#include "%s"\n' % h.replace("include/", "", 1) for h in heads)


STRING_RE = re.compile(r"\.(?:asciz|ascii|string)\b")


def rodata_blocks(rodata_text, referenced_text):
    """The jump-table and string blocks of a splat rodata .s that `referenced_text` mentions.

    Returns assembly text starting with `.section .rodata`; splat's `dlabel` becomes `glabel`
    (the form m2c reads). Other blocks are dropped: they are mostly data variables that splat
    placed in rodata, and with their initial values m2c prints them as constants and folds them
    into the code (`D_x = (s32) D_y;`), which is not what the C looked like. The segment's
    rodata can also be megabytes and m2c would parse all of it.
    """
    used = set(WORD_RE.findall(referenced_text))
    out, cur, buf = [".section .rodata"], None, []
    for line in rodata_text.splitlines():
        m = LABEL_RE.match(line)
        if m and cur is None:
            cur, buf = m.group(1), []
            continue
        if END_RE.match(line):
            if cur in used and (cur.startswith("jtbl_") or any(STRING_RE.search(x) for x in buf)):
                out += ["glabel " + cur] + buf
            cur = None
            continue
        if cur is not None:
            buf.append(line)
    return "\n".join(out) + "\n"


def rodata_files(root, overlay):
    """The splat rodata files of the main exe or an overlay, islands of per-object files
    (data/<NAME>/<addr>.rodata.s, T-0500) included."""
    base = "asm/ovl/%s/data" % overlay if overlay else "asm/data"
    return sorted(glob.glob(os.path.join(root, base, "**/*.rodata.s"), recursive=True))


def unique_blocks(text, seen):
    """`text` without the glabel blocks whose label is in `seen` (stale copies of a block in an
    old layout's rodata file); adds the kept labels to `seen`."""
    out, keep = [], True
    for line in text.splitlines():
        m = re.match(r"^glabel\s+(\w+)", line)
        if m:
            keep = m.group(1) not in seen
            seen.add(m.group(1))
        if keep:
            out.append(line)
    return "\n".join(out) + ("\n" if out else "")


def drop_declaration(text, name):
    """Remove one-line prototypes/externs of `name` (to see what m2c does without its own signature)."""
    pat = re.compile(r"^[^;{}()]*\b%s\b[^;{}]*;[ \t]*$" % re.escape(name), re.M)
    return pat.sub("", text)


def build_context(root, heads, out_path, drop=None):
    """Preprocess the headers into a pycparser-readable context file. Raises on cpp errors."""
    src = context_text(heads)
    proc = subprocess.run(CPP + ["-x", "c", "-"], input=src, text=True, cwd=root,
                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    if proc.returncode != 0:
        raise RuntimeError("preprocessing the context failed:\n" + proc.stderr)
    out = drop_declaration(proc.stdout, drop) if drop else proc.stdout
    with open(out_path, "w") as f:
        f.write(out)


def split_draft(text, name):
    """Split m2c output into (declarations before the function, function text).

    m2c prints prototypes, externs and (for rodata it was given) initialised constants before
    the function; the function starts at the first line that defines `name`.
    """
    lines = text.splitlines()
    start = re.compile(r"^[A-Za-z_][^;{}]*\b%s\(.*\)\s*\{" % re.escape(name))
    for i, line in enumerate(lines):
        if start.match(line):
            return "\n".join(lines[:i]).strip(), "\n".join(lines[i:]).strip() + "\n"
    return "", text


def func_name(arg):
    """Function name of a command-line `func` argument (name, UNIT:name or an asm path)."""
    return re.sub(r"\.s$", "", re.sub(r"^.*[:/]", "", arg))


def run_m2c(root, name, target=DEFAULT_TARGET, context=True, rodata=True, extra=(), blind=False,
            unit=None):
    """Return m2c's raw output for function `name` (`unit`: see locate())."""
    asm, overlay = locate(root, name, unit)
    sys.stderr.write("m2c.py: %s (%s)\n" % (os.path.relpath(asm, root), overlay or "main"))
    name = func_name(name)
    with tempfile.TemporaryDirectory(prefix="m2c") as tmp:
        cmd = ["m2c", "-t", target]
        if context:
            ctx = os.path.join(tmp, "ctx.c")
            build_context(root, context_sources(root, overlay), ctx, name if blind else None)
            cmd += ["--context", ctx]
        func_text = open(asm).read()
        asm_in = os.path.join(tmp, os.path.basename(asm))
        with open(asm_in, "w") as f:
            f.write(fix_lo_literals(func_text))
        cmd += list(extra) + [asm_in]
        if rodata:
            blocks, seen = "", set()
            for rf in rodata_files(root, overlay):
                blocks += unique_blocks(rodata_blocks(open(rf).read(), func_text).split("\n", 1)[1], seen)
            if blocks.strip():
                ro = os.path.join(tmp, "rodata.s")
                with open(ro, "w") as f:
                    f.write(".section .rodata\n" + blocks)
                cmd.append(ro)
        proc = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        if proc.returncode != 0:
            raise RuntimeError("m2c failed:\n" + proc.stderr)
        return proc.stdout


def main(argv):
    if "--" in argv:
        k = argv.index("--")
        argv, extra = argv[:k], argv[k + 1:]
    else:
        extra = []
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("func", help="function name, UNIT:name, or the path of its asm file")
    ap.add_argument("--unit", help="main, an overlay name, or a C/asm path of the unit "
                    "(needed when the name exists in several overlays and no INCLUDE_ASM decides)")
    ap.add_argument("--root", default=".")
    ap.add_argument("--target", default=DEFAULT_TARGET)
    ap.add_argument("--no-context", action="store_true", help="skip the header context")
    ap.add_argument("--no-rodata", action="store_true", help="skip the jump table/rodata blocks")
    ap.add_argument("--blind", action="store_true",
                    help="hide the function's own prototype from the context (for evaluation)")
    ap.add_argument("--raw", action="store_true", help="print m2c output unsplit")
    args = ap.parse_args(argv)
    try:
        text = run_m2c(args.root, args.func, args.target, not args.no_context,
                       not args.no_rodata, extra, args.blind, args.unit)
    except (LookupError, RuntimeError) as e:
        sys.exit("m2c.py: %s" % e)
    if args.raw:
        sys.stdout.write(text)
        return
    decls, body = split_draft(text, func_name(args.func))
    if decls:
        sys.stderr.write("m2c.py: symbols not in the headers:\n%s\n" % decls)
    sys.stdout.write(body)


if __name__ == "__main__":
    main(sys.argv[1:])
