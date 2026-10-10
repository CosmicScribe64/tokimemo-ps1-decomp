#!/usr/bin/env python3
"""Run m2c on one function with project context and print a C draft (T-1330).

Usage (in Docker): python3 tools/m2c.py [--no-context] [--no-rodata] [--target T] func_80045414 [-- m2c options]

What it adds over a bare `m2c <file>.s`:
- finds the function's .s under asm/{nonmatchings,matchings}/ (main exe) or asm/ovl/<NAME>/ (overlay);
- builds a context file by preprocessing include/game.h (+ include/ovl/<NAME>.h for an overlay) with
  `gcc -E -P -D__sgi`, so m2c knows the real globals, struct layouts and callee prototypes;
- extracts only the rodata blocks the function references (jump tables, strings) from the
  segment's *.rodata.s, so `switch` statements come out as `switch`, not as computed jumps;
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


def locate(root, name):
    """Return (asm_path, overlay_name or None) for function `name`, or raise LookupError."""
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
    path = sorted(hits)[0]
    m = re.search(r"asm/ovl/([^/]+)/", path)
    return path, (m.group(1) if m else None)


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


def run_m2c(root, name, target=DEFAULT_TARGET, context=True, rodata=True, extra=(), blind=False):
    """Return m2c's raw output for function `name`."""
    asm, overlay = locate(root, name)
    with tempfile.TemporaryDirectory(prefix="m2c") as tmp:
        cmd = ["m2c", "-t", target]
        if context:
            ctx = os.path.join(tmp, "ctx.c")
            build_context(root, context_sources(root, overlay), ctx, name if blind else None)
            cmd += ["--context", ctx]
        cmd += list(extra) + [asm]
        if rodata:
            func_text = open(asm).read()
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
    ap.add_argument("func")
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
                       not args.no_rodata, extra, args.blind)
    except (LookupError, RuntimeError) as e:
        sys.exit("m2c.py: %s" % e)
    if args.raw:
        sys.stdout.write(text)
        return
    decls, body = split_draft(text, args.func)
    if decls:
        sys.stderr.write("m2c.py: symbols not in the headers:\n%s\n" % decls)
    sys.stdout.write(body)


if __name__ == "__main__":
    main(sys.argv[1:])
