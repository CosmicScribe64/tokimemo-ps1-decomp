#!/usr/bin/env python3
"""Per-function diff of the built object against the original object.

Compares `objdump -dr` of the built object of the source file that holds each function
(build/src/main/<addr>.o or an overlay object; the file is the one whose INCLUDE_ASM or C definition
names the function, tools/funcloc.py, T-5030) with the original object and
prints MATCH or a short diff for each named function.

The original object is made here (T-7030), never copied by hand: expected/<built path> is the
all-INCLUDE_ASM object of the same C file, built from the original splat disassembly
(asm/**/{matchings,nonmatchings}/<file>/*.s, whatever state the C is in) by tools/cc.py, the same
pipeline as ninja. It is rebuilt whenever one of those .s files, or the list of them, changed
(a sha1 of their contents is kept in expected/<path>.key), so an edited or stale reference cannot
occur; `--refresh-expected` forces a rebuild, and `--expected OBJ` names an object yourself. Instruction
addresses and absolute branch targets are stripped, so only instructions,
relocations and symbol-relative targets are compared.

Usage (in Docker): python3 tools/funcdiff.py [--unit UNIT|PATH] [--built OBJ] [--expected OBJ] [--refresh-expected] [--resolve] [--no-build] [UNIT:]func_80042400 [func_...]
The function is looked up by definition, not by mention: the C file with its INCLUDE_ASM or its
body. Overlays all load at 0x80132000, so a name such as func_8013xxxx exists in several of them;
`--unit` (`main`, an overlay name, or a C/asm path such as src/ovl/TT/80147380.c; also a `UNIT:`
prefix on the name) picks one. A name that is held by several files and not scoped is refused with
the list of candidates.
--built compares another object (e.g. a scratch compile or an overlay object)
instead of the per-file object (the original side is then taken from the C file that holds the
function); --expected names the original-side object yourself.
--resolve compares the instruction words with the relocations applied instead of the text: a
`lui/lh %hi/%lo(D_801D63C8)` against the original's symbol and `*(s16 *)0x801D63C8` in C give the
same words, so the DIFF that only comes from that idiom (T-3300) disappears. Symbols that carry
their address in the name (`<prefix>_<8 hex digits>`), and renamed ones (config/obin_renames.txt,
config/symbol_addrs*.txt) are resolved to it. A relocation into the object's own .rodata (a string
literal: `.rodata` + offset in the built object, `D_8013xxxx` in the original) is compared by the
bytes it points at, up to the NUL. Any other relocation is still compared by type and symbol.
The built object is checked first: `ninja <object>` is run (skipped with --no-build or without
build.ninja), and an object that is missing, older than its C source or than any header it
includes, or whose build fails, is an error, never a MATCH. The check also applies to `--built`
when that object is a build/ object of the C files.
Exit code 1 if any function differs or cannot be compared.
"""
import argparse
import difflib
import hashlib
import os
import re
import shutil
import subprocess
import sys

import funcloc
import srcscan

# internal labels: stay inside the current function
LABEL_RE = re.compile(r"^(L[0-9A-Fa-f]{8}|\.L\w+|jtbl_\w+)$")
SYM_ADDR_RE = re.compile(r"_([0-9A-Fa-f]{8})$")
INSN_RE = re.compile(r"^\s*[0-9a-f]+:\s+([0-9a-f]{8})\s+(.*)$")
RELOC_RE = re.compile(r"^\s*[0-9a-f]+:\s+(R_MIPS_\w+)\s+([^\s+]+)(?:\+0x([0-9a-f]+))?\s*$")
NO_DOCKER = ("funcdiff.py: mips-linux-gnu-objdump not found. The tools only run in Docker: "
             "tools/docker.sh python3 tools/funcdiff.py ...")


def locate(name, unit=None):
    """srcscan.CFile of the C file that holds `name` (INCLUDE_ASM or definition) in `unit`.
    Raises funcloc.LocateError / AmbiguousError (T-5030: no first-mention guess)."""
    return funcloc.locate(name, unit)


def object_of(name, unit=None):
    """Built object path of the C file that holds `name`."""
    return locate(name, unit).obj


def cfile_of_object(obj):
    """srcscan.CFile whose built object is `obj`, or None (a scratch object)."""
    want = os.path.normpath(obj)
    for c in srcscan.c_files():
        if os.path.normpath(c.obj) == want:
            return c
    return None


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


_names = {}


def renamed_addresses(root="."):
    """{name: address} of the symbols whose name carries no address: the new names of
    config/obin_renames.txt (`old new`, old = func_/D_<addr>) and config/symbol_addrs*.txt
    (`name = 0xADDR;`). Cached per root."""
    key = os.path.abspath(root)
    if key not in _names:
        out = {}
        cfg = os.path.join(root, "config")
        try:
            with open(os.path.join(cfg, "obin_renames.txt"), errors="replace") as f:
                for line in f:
                    parts = line.split()
                    if len(parts) == 2 and not line.lstrip().startswith("#"):
                        m = SYM_ADDR_RE.search(parts[0])
                        if m:
                            out[parts[1]] = int(m.group(1), 16)
        except OSError:
            pass
        try:
            names = sorted(f for f in os.listdir(cfg) if f.startswith("symbol_addrs") and f.endswith(".txt"))
        except OSError:
            names = []
        for fn in names:
            with open(os.path.join(cfg, fn), errors="replace") as f:
                for line in f:
                    m = re.match(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)", line)
                    if m:
                        out.setdefault(m.group(1), int(m.group(2), 16))
        _names[key] = out
    return _names[key]


def sym_address(sym, addend=0, names=None):
    """Address a symbol name carries (`D_801D63C8`, `.L80133A00`) or a renamed symbol stands for
    (`bg_read_sub2` -> func_8007ED84, `names`: renamed_addresses()), or None."""
    m = SYM_ADDR_RE.search(sym)
    if m:
        addr = int(m.group(1), 16)
    else:
        addr = (renamed_addresses() if names is None else names).get(sym)
    return addr + addend if addr is not None and addr >= 0x80000000 else None


class ObjInfo:
    """What `resolve` needs to know about one object's .rodata: its bytes and the offsets of the
    symbols defined there."""

    def __init__(self, rodata=b"", symbols=None):
        self.rodata = rodata
        self.symbols = symbols or {}   # name -> offset in .rodata (the section symbol is ".rodata", 0)

    def string_at(self, name, add):
        """Bytes up to the NUL at .rodata `name` + add, or None if name is no .rodata symbol or the
        place holds no string."""
        if name == ".rodata":
            base = 0
        elif name in self.symbols:
            base = self.symbols[name]
        else:
            return None
        off = base + add
        if not 0 <= off < len(self.rodata):
            return None
        end = self.rodata.find(b"\0", off)
        text = self.rodata[off:end if end >= 0 else len(self.rodata)]
        return text or None


def parse_object_info(symtab, contents):
    """ObjInfo from `objdump -t` and `objdump -s -j .rodata` texts."""
    syms = {}
    for line in symtab.splitlines():
        if "\t" not in line:
            continue
        left, right = line.split("\t", 1)
        lt, rt = left.split(), right.split()
        if len(lt) >= 3 and lt[-1] == ".rodata" and len(rt) >= 2:
            syms[rt[1]] = int(lt[0], 16)
    data = {}
    for line in contents.splitlines():
        m = re.match(r"^ ([0-9a-f]+) ([0-9a-f ]{35})", line)
        if m:
            data[int(m.group(1), 16)] = bytes.fromhex(m.group(2).replace(" ", ""))
    buf = bytearray()
    for off in sorted(data):
        buf.extend(b"\0" * (off - len(buf)))
        buf[off:off + len(data[off])] = data[off]
    return ObjInfo(bytes(buf), syms)


_info_cache = {}


def object_info(obj):
    """ObjInfo of an object file (cached by path and mtime)."""
    key = (obj, os.path.getmtime(obj) if os.path.exists(obj) else 0)
    if key not in _info_cache:
        def run(*args):
            r = subprocess.run(["mips-linux-gnu-objdump"] + list(args) + [obj], stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE, text=True)
            return r.stdout
        _info_cache[key] = parse_object_info(run("-t"), run("-s", "-j", ".rodata"))
    return _info_cache[key]


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


def resolve(insns, info=None, names=None):
    """[(key, display)] with the relocations of the instructions applied where the target is known.
    key is what gets compared. Known: a symbol with its address in the name or renamed (`names`),
    and, with `info` (ObjInfo of the object), a string in the object's own .rodata (compared by
    its bytes, so `.rodata`+off and `D_8013xxxx` of the original agree)."""
    out = []
    for i, (word, text, relocs) in enumerate(insns):
        if word is None:
            out.append((text, text))
            continue
        left = []
        for typ, sym, addend in relocs:
            if info is not None and typ in ("R_MIPS_HI16", "R_MIPS_LO16"):
                if typ == "R_MIPS_LO16":
                    imm = sext16(word)
                else:
                    imm = 0
                    for w2, _t2, r2 in insns[i + 1:]:
                        if w2 is not None and any(t == "R_MIPS_LO16" and s == sym for t, s, _a in r2):
                            imm = sext16(w2)
                            break
                lit = info.string_at(sym, addend + imm)
                if lit is not None:
                    word &= 0xFFFF0000
                    left.append("%s string %r" % (typ, lit))
                    continue
            base = sym_address(sym, addend, names)
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
    info = object_info(obj) if any(k in funcs for k in wanted) else None
    return {k: (resolve(parse_insns(funcs[k]), info) if k in funcs else None) for k in wanted}


_checked = {}


def check_fresh(obj, src, build=True, deps=()):
    """None if the built object `obj` is up to date with `src` and the headers `deps`, else the
    reason it is not. Runs `ninja obj` (when build.ninja and ninja exist), so a failed build is
    reported instead of leaving the previous object to produce a stale MATCH (`ninja obj` also fails
    while tools/check_headers.py does); then requires the object to exist and to be at least as new
    as its source and every header the source includes."""
    key = (obj, str(src), tuple(str(d) for d in deps), build)
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
        else:
            newest = [(os.path.getmtime(f), f) for f in [src, *deps] if f is not None and os.path.exists(f)]
            newer = [f for t, f in newest if t > os.path.getmtime(obj)]
            if newer:
                problem = "%s is older than %s; rebuild it (ninja %s), or the build failed" % (
                    obj, ", ".join(str(f) for f in newer[:3]), obj)
    _checked[key] = problem
    return problem


def asm_functions(c):
    """[(vram, folder, name)] of every function of the C file `c` in the original disassembly
    (matched or not), in address order: the .s files of its matchings and nonmatchings folders. The
    address is the one of the first instruction (names such as bg_read_sub2 carry none)."""
    out = []
    for folder in (c.matchings, c.nonmatchings):
        if not os.path.isdir(folder):
            continue
        for fn in sorted(os.listdir(folder)):
            if not fn.endswith(".s"):
                continue
            with open(os.path.join(folder, fn), errors="replace") as fh:
                m = re.search(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/", fh.read(), re.M)
            out.append((int(m.group(1), 16) if m else 0xFFFFFFFF, str(folder).replace(os.sep, "/"), fn[:-2]))
    return sorted(out)


def expected_stub(funcs):
    """C text of the all-INCLUDE_ASM version of a file: one INCLUDE_ASM per original function."""
    lines = ['#include "common.h"', ""]
    lines += ['INCLUDE_ASM("%s", %s);' % (folder, name) for _v, folder, name in funcs]
    return "\n".join(lines) + "\n"


def expected_key(stub, funcs):
    """sha1 of the stub and of the .s contents it includes: changes when the original changes."""
    h = hashlib.sha1(stub.encode())
    for _v, folder, name in funcs:
        with open(os.path.join(folder, name + ".s"), "rb") as fh:
            h.update(fh.read())
    return h.hexdigest()


def compile_expected(stub_path, obj):
    """Compile the stub exactly like the build does (tools/cc.py, ido 5.3)."""
    r = subprocess.run([sys.executable, os.path.join(os.path.dirname(os.path.abspath(__file__)), "cc.py"),
                        stub_path, obj, "ido", "5.3"], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    if r.returncode != 0:
        return "compiling the all-INCLUDE_ASM object %s failed:\n%s" % (obj, "\n".join(r.stdout.strip().splitlines()[-15:]))
    return None


def expected_object(c, refresh=False, compile_fn=compile_expected):
    """(path, problem): the original-side object of the srcscan.CFile `c` under expected/, created or
    refreshed from the original .s files when it is missing, forced, or its key changed."""
    funcs = asm_functions(c)
    if not funcs:
        return None, "no original disassembly found for %s (asm/ is missing: run configure.py and ninja first)" % c.src
    obj = os.path.join("expected", c.obj)
    stub = expected_stub(funcs)
    key = expected_key(stub, funcs)
    try:
        with open(obj + ".key") as fh:
            have = fh.read().strip()
    except OSError:
        have = None
    if not refresh and have == key and os.path.exists(obj):
        return obj, None
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    stub_path = obj[:-2] + ".c" if obj.endswith(".o") else obj + ".c"
    with open(stub_path, "w") as fh:
        fh.write(stub)
    for stale in (obj, obj + ".key"):
        if os.path.exists(stale):
            os.remove(stale)
    problem = compile_fn(stub_path, obj)
    if problem:
        return None, problem
    with open(obj + ".key", "w") as fh:
        fh.write(key + "\n")
    return obj, None


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
    ap.add_argument("--refresh-expected", action="store_true",
                    help="rebuild the original-side object (expected/...) even if it looks current")
    ap.add_argument("--unit", help="unit (main or an overlay name) or C/asm path that holds the function")
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
    refreshed = set()
    for n in args.names:
        b, c = args.built, None
        try:
            name, scope = funcloc.split_scope(n, args.unit)
            if b is None:
                c = locate(name, scope)
                b = c.obj
            elif os.path.isdir("config"):
                c = cfile_of_object(b)
        except funcloc.LocateError as e:
            print("%s: ERROR %s" % (n, e))
            bad += 1
            continue
        if c is not None:   # an object of the build: its source and headers must not be newer
            problem = check_fresh(b, c.src, not args.no_build, sorted(funcloc.includes_of(c.src)))
            if problem:
                print("%s: ERROR %s" % (n, problem))
                bad += 1
                continue
        e = args.expected
        if e is None:
            if c is None:       # a scratch object: the original side is that of the file holding the function
                try:
                    c = locate(name, scope)
                except funcloc.LocateError as err:
                    print("%s: ERROR %s" % (n, err))
                    bad += 1
                    continue
            e, problem = expected_object(c, args.refresh_expected and c.obj not in refreshed)
            refreshed.add(c.obj)
            if problem:
                print("%s: ERROR %s" % (n, problem))
                bad += 1
                continue
        got, want = get(b, [name])[name], get(e, [name])[name]
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
