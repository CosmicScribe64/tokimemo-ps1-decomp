#!/usr/bin/env python3
"""Per-function diff of the built object against the original object.

Compares `objdump -dr` of the built object of the source file that holds each function
(build/src/main/<addr>.o or an overlay object, found by searching the C files; T-0012, T-0500) with
the same object under
expected/ (a copy of the all-INCLUDE_ASM build; see wiki/decompile-workflow.md) and
prints MATCH or a short diff for each named function. Instruction
addresses and absolute branch targets are stripped, so only instructions,
relocations and symbol-relative targets are compared.

Usage (in Docker): python3 tools/funcdiff.py [--built OBJ] [--expected OBJ] [--resolve] [--no-build] func_80042400 [func_...]
--built compares another object (e.g. a scratch compile or an overlay object)
instead of the per-file object; --expected names the original-side object (default
expected/<same path as built>, e.g. a copy of build/ovl/<NAME>/<NAME>.o).
--resolve compares the instruction words with the relocations applied instead of the text: a
`lui/lh %hi/%lo(D_801D63C8)` against the original's symbol and `*(s16 *)0x801D63C8` in C give the
same words, so the DIFF that only comes from that idiom (T-3300) disappears. Only symbols that
carry their address in the name (`<prefix>_<8 hex digits>`) are resolved; any other relocation is
still compared by type and symbol.
The built object is checked first: `ninja <object>` is run (skipped with --no-build or without
build.ninja), and an object that is missing or older than its C source, or whose build fails,
is an error, never a MATCH.
Exit code 1 if any function differs or cannot be compared.
"""
import argparse
import difflib
import os
import re
import shutil
import subprocess
import sys

import srcscan

# internal labels: stay inside the current function
LABEL_RE = re.compile(r"^(L[0-9A-Fa-f]{8}|\.L\w+|jtbl_\w+)$")
SYM_ADDR_RE = re.compile(r"_([0-9A-Fa-f]{8})$")
INSN_RE = re.compile(r"^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+(.*)$")
RELOC_RE = re.compile(r"^\s*[0-9a-f]+:\s+(R_MIPS_\w+)\s+([^\s+]+)(?:\+0x([0-9a-f]+))?\s*$")
NO_DOCKER = ("funcdiff.py: mips-linux-gnu-objdump not found. The tools only run in Docker: "
             "tools/docker.sh python3 tools/funcdiff.py ...")


def locate(name):
    """srcscan.CFile of the first C file (main exe or overlay) that mentions `name`, or None."""
    pat = re.compile(r"\b%s\b" % re.escape(name))
    for c in srcscan.c_files():
        if c.src.exists() and pat.search(c.src.read_text()):
            return c
    return None


def object_of(name):
    """Built object path of the C file that mentions `name`, or None."""
    c = locate(name)
    return c.obj if c else None


def disassemble(obj):
    """`objdump -dr` text of `obj`."""
    try:
        return subprocess.run(["mips-linux-gnu-objdump", "-dr", obj], check=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True).stdout
    except FileNotFoundError:
        sys.exit(NO_DOCKER)
    except subprocess.CalledProcessError as e:
        sys.exit("funcdiff.py: objdump failed on %s: %s" % (obj, e.stderr.strip()))


def split_functions(out):
    """{function: [objdump lines]} of an `objdump -dr` text; internal labels stay in their function."""
    funcs, cur = {}, None
    for line in out.splitlines():
        m = re.match(r"^[0-9a-f]+ <(\w+)>:", line)
        if m and LABEL_RE.match(m.group(1)):
            continue
        if m:
            cur = m.group(1)
            funcs[cur] = []
        elif cur and line.strip() and not line.startswith("Disassembly"):
            funcs[cur].append(line)
    return funcs


def functions(obj, wanted):
    """{name: normalised instruction/relocation lines or None} for the wanted functions."""
    funcs = split_functions(disassemble(obj))
    out = {}
    for k in wanted:
        if k not in funcs:
            out[k] = None
            continue
        lines = []
        for line in funcs[k]:
            line = re.sub(r"^\s*[0-9a-f]+:\s*", "", line)
            # branch/jump targets: drop the absolute address, keep <sym+off>
            line = re.sub(r"\b[0-9a-f]+ (<[^>]+>)", r"\1", line)
            lines.append(re.sub(r"\s+", " ", line.strip()))
        out[k] = lines
    return out


def sext16(v):
    v &= 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


def sym_address(sym, addend=0):
    """Address a splat-style symbol name carries (`D_801D63C8`, `.L80133A00`), or None."""
    m = SYM_ADDR_RE.search(sym)
    if not m:
        return None
    addr = int(m.group(1), 16)
    return addr + addend if addr >= 0x80000000 else None


def parse_insns(lines):
    """[[word, text, [(reloc type, symbol, addend)]]] of one function's objdump lines.
    A line that is no instruction (`...` for folded zero words) has word None."""
    insns = []
    for line in lines:
        m = INSN_RE.match(line)
        if m:
            insns.append([int(m.group(1), 16), re.sub(r"\s+", " ", m.group(2).strip()), []])
            continue
        r = RELOC_RE.match(line)
        if r and insns:
            insns[-1][2].append((r.group(1), r.group(2), int(r.group(3) or "0", 16)))
        else:
            insns.append([None, re.sub(r"\s+", " ", line.strip()), []])
    return insns


def resolve(insns):
    """[(key, display)] with the relocations of the instructions applied where the symbol's
    address is known. key is what gets compared."""
    out = []
    for i, (word, text, relocs) in enumerate(insns):
        if word is None:
            out.append((text, text))
            continue
        left = []
        for typ, sym, addend in relocs:
            base = sym_address(sym, addend)
            if base is None:
                left.append("%s %s%s" % (typ, sym, "+0x%x" % addend if addend else ""))
            elif typ == "R_MIPS_LO16":
                word = (word & 0xFFFF0000) | ((base + sext16(word)) & 0xFFFF)
            elif typ == "R_MIPS_HI16":
                low = 0
                for w2, _t2, r2 in insns[i + 1:]:
                    if w2 is not None and any(t == "R_MIPS_LO16" and s == sym for t, s, _a in r2):
                        low = sext16(w2)
                        break
                word = (word & 0xFFFF0000) | (((base + low + 0x8000) >> 16) & 0xFFFF)
            elif typ == "R_MIPS_26":
                word = (word & 0xFC000000) | (((base + ((word & 0x3FFFFFF) << 2)) >> 2) & 0x3FFFFFF)
            else:
                left.append("%s %s" % (typ, sym))
        key = "%08x" % word + "".join(" " + x for x in left)
        out.append((key, "%s  ; %s" % (key, text)))
    return out


def resolved_functions(obj, wanted):
    """{name: [(key, display)] or None}: like functions(), with relocations resolved."""
    funcs = split_functions(disassemble(obj))
    return {k: (resolve(parse_insns(funcs[k])) if k in funcs else None) for k in wanted}


_checked = {}


def check_fresh(obj, src, build=True):
    """None if the built object `obj` is up to date with `src`, else the reason it is not.
    Runs `ninja obj` (when build.ninja and ninja exist), so a failed build is reported instead of
    leaving the previous object to produce a stale MATCH; then requires the object to exist and
    not be older than its source."""
    key = (obj, str(src))
    if key in _checked:
        return _checked[key]
    problem = None
    if build and os.path.exists("build.ninja") and shutil.which("ninja"):
        r = subprocess.run(["ninja", obj], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if r.returncode != 0:
            tail = "\n".join(r.stdout.strip().splitlines()[-15:])
            problem = "the build of %s FAILED, the object is stale:\n%s" % (obj, tail)
    if problem is None:
        if not os.path.exists(obj):
            problem = "%s does not exist; build it first (ninja %s)" % (obj, obj)
        elif src is not None and os.path.exists(src) and os.path.getmtime(obj) < os.path.getmtime(src):
            problem = "%s is older than %s; rebuild it (ninja %s), or the build failed" % (obj, src, obj)
    _checked[key] = problem
    return problem


def keys(rows):
    return [r[0] if isinstance(r, tuple) else r for r in rows]


def diff_lines(want, got):
    """Printable diff rows (`-` expected, `+` built) of two lists of strings or (key, display)."""
    def disp(r):
        return r[1] if isinstance(r, tuple) else r
    rows = []
    sm = difflib.SequenceMatcher(None, keys(want), keys(got), autojunk=False)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op != "equal":
            rows += ["-" + disp(r) for r in want[i1:i2]] + ["+" + disp(r) for r in got[j1:j2]]
    return rows


def main(argv):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("names", nargs="+")
    ap.add_argument("--built")
    ap.add_argument("--expected")
    ap.add_argument("--resolve", action="store_true",
                    help="compare instruction words with the relocations applied")
    ap.add_argument("--no-build", action="store_true",
                    help="do not run ninja on the built object (it still fails if it is stale)")
    args = ap.parse_args(argv)
    if shutil.which("mips-linux-gnu-objdump") is None:
        sys.exit(NO_DOCKER)
    if not args.built and not os.path.isdir("config"):
        sys.exit("funcdiff.py: run it from the repository root")
    get = resolved_functions if args.resolve else functions
    bad = 0
    for n in args.names:
        b = args.built
        if b is None:
            c = locate(n)
            if c is None:
                print("%s: ERROR not found in any C source of the splat configs (srcscan.c_files)" % n)
                bad += 1
                continue
            b = c.obj
            problem = check_fresh(b, c.src, not args.no_build)
            if problem:
                print("%s: ERROR %s" % (n, problem))
                bad += 1
                continue
        e = args.expected or "expected/" + b
        got, want = get(b, [n])[n], get(e, [n])[n]
        if got is None or want is None:
            print("%s: missing in %s" % (n, "built" if got is None else "expected"))
            bad += 1
        elif keys(got) == keys(want):
            print("%s: MATCH%s" % (n, " (relocations resolved)" if args.resolve else ""))
        else:
            bad += 1
            print("%s: DIFF (- expected, + built)" % n)
            for d in diff_lines(want, got):
                print("  " + d)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
