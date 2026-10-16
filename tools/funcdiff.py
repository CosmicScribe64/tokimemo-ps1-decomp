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

Usage (in Docker): python3 tools/funcdiff.py [--unit UNIT|PATH] [--built OBJ] [--expected OBJ] [--refresh-expected] [--resolve] [--no-build] [--proto DECL ...] [UNIT:]func_80042400 [func_...]
The function is looked up by definition, not by mention: the C file with its INCLUDE_ASM or its
body. Overlays all load at 0x80132000, so a name such as func_8013xxxx exists in several of them;
`--unit` (`main`, an overlay name, or a C/asm path such as src/ovl/TT/80147380.c; also a `UNIT:`
prefix on the name) picks one. A name that is held by several files and not scoped is refused with
the list of candidates.
--built compares another object (e.g. a scratch compile or an overlay object)
instead of the per-file object (the original side is then taken from the C file that holds the
function); --expected names the original-side object yourself.
--version auto (default) compiles the full NON_MATCHING translation unit when the requested
function has guarded C and an asm fallback; --version default compares the production build.
Every verdict identifies the compared version. --built always compares the supplied object.
--resolve compares the instruction words with the relocations applied instead of the text: a
`lui/lh %hi/%lo(D_801D63C8)` against the original's symbol and `*(s16 *)0x801D63C8` in C give the
same words, so the DIFF that only comes from that idiom (T-3300) disappears. Symbols that carry
their address in the name (`<prefix>_<8 hex digits>`), and renamed ones (config/obin_renames.txt,
config/symbol_addrs*.txt) are resolved to it. A relocation into the object's own .rodata (a string
literal: `.rodata` + offset in the built object, `D_8013xxxx` in the original) is compared by the
bytes it points at, up to the NUL. Scheduled string HI16s compare as string-base references;
the LO16s independently compare the actual contents. PC16 branches use section symbol offsets
or the named function VRAM, including the encoded -4 addend. Short D_ hexadecimal symbols
such as D_7FFFFF are absolute constants. Any other relocation is still compared by type and symbol.
Normalized strings and table references are diagnostic equality only. MATCH for a repository
function additionally requires a private link with that exact object: the function's address,
size and literal bytes (including original padding) must equal the immutable disc target.
Scratch objects without a unit layout can MATCH only exact, fully resolved instruction words;
unknown section/string/table addresses require unit layout and fail closed.
--proto 'void func_80012345(s32 a0);' (repeatable, T-16530; tools/protoverride.py) replaces the declaration of a
function the file calls for this comparison only: the file's complete translation unit is compiled like the
build, with that declaration in place of the header's, and nothing under src/ or include/ is edited. The verdict
names the override; on a MATCH it says which `*_OVERRIDE_<symbol>` guard keeps the view in the tree. A function
that the file defines is refused, and --proto cannot be combined with --built.
The built object is checked first: `ninja <object>` is run (skipped with --no-build or without
build.ninja), and an object that is missing, older than its C source or than any header it
includes, or whose build fails, is an error, never a MATCH. The check also applies to `--built`
when that object is a build/ object of the C files.
Exit code 1 if any function differs or cannot be compared.
"""
import argparse
import fcntl
import difflib
import hashlib
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import funcloc
import protoverride
import srcscan

# internal labels: stay inside the current function
LABEL_RE = re.compile(r"^(L[0-9A-Fa-f]{8}|\.L\w+|jtbl_\w+)$")
SYM_ADDR_RE = re.compile(r"(?:_|^\.?L)([0-9A-Fa-f]{8})$|^D_([0-9A-Fa-f]{1,7})$")
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
        return subprocess.run(["mips-linux-gnu-objdump", "-drz", obj], check=True,
                              stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True).stdout
    except FileNotFoundError:
        sys.exit(NO_DOCKER)
    except subprocess.CalledProcessError as e:
        sys.exit("funcdiff.py: objdump failed on %s: %s" % (obj, e.stderr.strip()))


def split_functions(out):
    """{function: [objdump lines]} of an `objdump -dr` text; internal labels stay in their function."""
    funcs, cur = {}, None
    for line in out.splitlines():
        m = re.match(r"^[0-9a-f]+ <([^>]+)>:", line)
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
                            out[parts[1]] = int(m.group(1) or m.group(2), 16)
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
        addr = int(m.group(1) or m.group(2), 16)
    else:
        addr = (renamed_addresses() if names is None else names).get(sym)
    return addr + addend if addr is not None else None


class ObjInfo:
    """What `resolve` needs to know about one object's .rodata: its bytes and the offsets of the
    symbols defined there."""

    def __init__(self, rodata=b"", symbols=None, tables=None, original=(), text_symbols=None):
        self.rodata = rodata
        self.symbols = symbols or {}   # name -> offset in .rodata (the section symbol is ".rodata", 0)
        self.tables = tables or set()  # .rodata offsets that hold a code address (R_MIPS_32): jump tables
        self.text_symbols = text_symbols or {}  # name -> offset in .text
        self.original = original      # (original VRAM, bytes) ranges, expected side only

    def is_table(self, name, add):
        """True when .rodata `name` + add is a jump table: the original's `jtbl_<addr>` symbols, or a place
        of .rodata that holds relocated code addresses (IDO's own table, `.rodata`+off)."""
        if name.startswith("jtbl_"):
            return True
        base = 0 if name == ".rodata" else self.symbols.get(name)
        return base is not None and (base + add) in self.tables

    def string_at(self, name, add):
        """Bytes up to the NUL at .rodata `name` + add, or None if name is no .rodata symbol or the
        place holds no string."""
        if name == ".rodata":
            base = 0
        elif name in self.symbols:
            base = self.symbols[name]
        else:
            # Matched .s files contain only text: their strings are external to the
            # all-INCLUDE_ASM expected object. Read those from this unit's original
            # rodata ranges, never from a different overlay sharing the address.
            addr = sym_address(name, add)
            if addr is not None:
                for start, data in self.original:
                    if start <= addr < start + len(data):
                        off = addr - start
                        end = data.find(b"\0", off)
                        return data[off:end] if end >= 0 else None
            return None
        off = base + add
        if not 0 <= off < len(self.rodata):
            return None
        end = self.rodata.find(b"\0", off)
        text = self.rodata[off:end if end >= 0 else len(self.rodata)]
        return text if end >= 0 else None


def parse_object_info(symtab, contents, relocs=""):
    """ObjInfo from `objdump -t`, `objdump -s -j .rodata` and `objdump -r -j .rodata` texts."""
    syms, text_syms = {}, {}
    for line in symtab.splitlines():
        if "\t" not in line:
            continue
        left, right = line.split("\t", 1)
        lt, rt = left.split(), right.split()
        if len(lt) >= 3 and len(rt) >= 2:
            if lt[-1] == ".rodata":
                syms[rt[1]] = int(lt[0], 16)
            elif lt[-1] == ".text":
                text_syms[rt[1]] = int(lt[0], 16)
    data = {}
    for line in contents.splitlines():
        m = re.match(r"^ ([0-9a-f]+) ([0-9a-f ]{35})", line)
        if m:
            data[int(m.group(1), 16)] = bytes.fromhex(m.group(2).replace(" ", ""))
    buf = bytearray()
    for off in sorted(data):
        buf.extend(b"\0" * (off - len(buf)))
        buf[off:off + len(data[off])] = data[off]
    tables = set()
    for line in relocs.splitlines():
        m = re.match(r"^([0-9a-f]+)\s+R_MIPS_32\s", line)
        if m:
            tables.add(int(m.group(1), 16))
    return ObjInfo(bytes(buf), syms, tables, text_symbols=text_syms)


_info_cache = {}


def object_info(obj):
    """ObjInfo of an object file (cached by path and mtime)."""
    key = (obj, os.path.getmtime(obj) if os.path.exists(obj) else 0)
    if key not in _info_cache:
        def run(*args):
            r = subprocess.run(["mips-linux-gnu-objdump"] + list(args) + [obj], stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE, text=True)
            return r.stdout
        _info_cache[key] = parse_object_info(run("-t"), run("-s", "-j", ".rodata"), run("-r", "-j", ".rodata"))
    return _info_cache[key]


class Insn(list):
    """Three-item instruction record with its section offset for PC-relative relocs."""
    def __init__(self, word, text, offset):
        super().__init__([word, text, []])
        self.offset = offset


def parse_insns(lines):
    """[[word, text, [(reloc type, symbol, addend)]]] of one function's objdump lines.
    A line that is no instruction (`...` for folded zero words) has word None."""
    insns = []
    for line in lines:
        m = INSN_RE.match(line)
        if m:
            insns.append(Insn(int(m.group(1), 16), re.sub(r"\s+", " ", m.group(2).strip()),
                              int(line.split(":", 1)[0].strip(), 16)))
            continue
        r = RELOC_RE.match(line)
        if r and insns:
            insns[-1][2].append((r.group(1), r.group(2), int(r.group(3) or "0", 16)))
        else:
            insns.append([None, re.sub(r"\s+", " ", line.strip()), []])
    return insns


TABLE_REF = "jump-table"


def _paired_low(insns, i, sym, addend):
    """Immediate of the %lo that goes with the %hi of insns[i]: the first following LO16 of the same
    symbol and addend, else of the same symbol; 0 when there is none."""
    same_sym = None
    for w2, _t2, r2 in insns[i + 1:]:
        if w2 is None:
            continue
        for t, s2, a2 in r2:
            if t == "R_MIPS_LO16" and s2 == sym:
                if a2 == addend:
                    return sext16(w2)
                if same_sym is None:
                    same_sym = sext16(w2)
    return same_sym or 0


def resolve(insns, info=None, names=None, text_base=None):
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
            if typ in ("R_MIPS_HI16", "R_MIPS_LO16"):
                imm = sext16(word) if typ == "R_MIPS_LO16" else _paired_low(insns, i, sym, addend)
                if sym.startswith("jtbl_") or (info is not None and info.is_table(sym, addend + imm)):
                    # a jump-table address: the built table is IDO's own (`.rodata`+off), the original's is
                    # `jtbl_<addr>`; they never agree by name or address, so the reference is compared as
                    # "a table" and the table contents are left to the unit sha1 (T-9220)
                    word &= 0xFFFF0000
                    left.append("%s %s" % (typ, TABLE_REF))
                    continue
            if info is not None and typ in ("R_MIPS_HI16", "R_MIPS_LO16"):
                if typ == "R_MIPS_LO16":
                    imm = sext16(word)
                else:
                    imm = ((word & 0xFFFF) << 16) + _paired_low(insns, i, sym, addend)
                lit = info.string_at(sym, addend + imm)
                if lit is not None:
                    word &= 0xFFFF0000
                    # A scheduled HI16 can feed different LO16s on different branches.
                    # Its first linker-paired LO is not a unique string identity; compare
                    # the high reference as a string base, and every LO by its bytes.
                    left.append("%s string %r" % (typ, lit) if typ == "R_MIPS_LO16"
                                else "R_MIPS_HI16 string-base")
                    continue
            if typ == "R_MIPS_PC16":
                offset = getattr(insns[i], "offset", None)
                target = info.text_symbols.get(sym) if info is not None else None
                base = sym_address(sym, addend, names)
                if offset is not None and target is not None:
                    delta = target + addend + (sext16(word) << 2) - offset
                elif offset is not None and base is not None and text_base is not None:
                    delta = base + (sext16(word) << 2) - (text_base + offset)
                else:
                    delta = None
                if delta is not None and delta % 4 == 0 and -0x8000 <= delta // 4 < 0x8000:
                    word = (word & 0xFFFF0000) | ((delta // 4) & 0xFFFF)
                    continue
            base = sym_address(sym, addend, names)
            if base is None:
                left.append("%s %s%s" % (typ, sym, "+0x%x" % addend if addend else ""))
            elif typ == "R_MIPS_LO16":
                word = (word & 0xFFFF0000) | ((base + sext16(word)) & 0xFFFF)
            elif typ == "R_MIPS_HI16":
                low = _paired_low(insns, i, sym, addend)
                word = (word & 0xFFFF0000) | (((base + low + 0x8000) >> 16) & 0xFFFF)
            elif typ == "R_MIPS_26":
                word = (word & 0xFC000000) | (((base + ((word & 0x3FFFFFF) << 2)) >> 2) & 0x3FFFFFF)
            else:
                left.append("%s %s%s" % (typ, sym, "+0x%x" % addend if addend else ""))   # addend is a real difference
        key = "%08x" % word + "".join(" " + x for x in left)
        out.append((key, "%s  ; %s" % (key, text)))
    return out


def original_rodata(c):
    """Original (VRAM, bytes) ranges for the expected side of this specific unit.
    Converted functions' matched asm lacks the strings emitted by C; the original
    binary and yaml ranges provide them without guessing a relocation's address."""
    if c is None:
        return []
    path = "config/SLPM_86.053.yaml" if c.unit == "main" else "config/overlays/%s.yaml" % c.unit
    cfg = srcscan._load_yaml(path)
    rows = srcscan.subsegments(cfg)
    with open(cfg["options"]["target_path"], "rb") as f:
        blob = f.read()
    return [(off + delta, blob[off:rows[i + 1][0]])
            for i, (off, typ, _name, delta) in enumerate(rows[:-1])
            if typ in ("rodata", ".rodata")]


def resolved_functions(obj, wanted, original_file=None):
    """{name: [(key, display)] or None}: like functions(), with relocations resolved."""
    funcs = {k: object_instructions(obj, k) for k in wanted}
    info = object_info(obj) if any(v is not None for v in funcs.values()) else None
    if info is not None and original_file is not None:
        info = ObjInfo(info.rodata, info.symbols, info.tables, original_rodata(original_file), info.text_symbols)
    result = {}
    for k in wanted:
        addr = sym_address(k)
        start = info.text_symbols.get(k) if info is not None else None
        base = addr - start if addr is not None and start is not None else None
        result[k] = resolve(funcs[k], info, text_base=base) if funcs[k] is not None else None
    return result


def object_instructions(obj, name):
    """Bound by ELF global text symbols; local objdump headers cannot truncate a function.

    Include original inter-function padding, and disassemble zero words explicitly.
    ELF bounds also make a missing/truncated disassembly fail closed.
    """
    import trailing_pad
    elf = trailing_pad.Elf(Path(obj).read_bytes())
    text = elf.find(".text")
    syms = trailing_pad.read_symbols(elf, elf.find(".symtab"))
    symbols = {s[0]: s[1] for s in syms if s[5] == text and (s[3] >> 4 or s[3] & 15 == 2)
               and not LABEL_RE.match(s[0])}
    if name not in symbols:
        return None
    start = symbols[name]
    end = min([v for v in symbols.values() if v > start] or [elf.sections[text]["size"]])
    dump = subprocess.run(["mips-linux-gnu-objdump", "-drz", "-j", ".text", obj],
                          check=True, capture_output=True, text=True).stdout
    insns = [i for i in parse_insns(dump.splitlines()) if i[0] is not None and start <= i.offset < end]
    if end <= start or end % 4 or [i.offset for i in insns] != list(range(start, end, 4)):
        raise ValueError("incomplete function disassembly for " + name)
    return insns


def absolute_object_rows(obj, name):
    """No layout proof is possible for relocations to a section-owned symbol.

    An address-looking D_/func_ name still belongs to its actual ELF section;
    only undefined external address conventions or SHN_ABS can resolve here.
    """
    import trailing_pad
    elf = trailing_pad.Elf(Path(obj).read_bytes())
    symbols = {s[0]: (s[5], s[1]) for s in trailing_pad.read_symbols(elf, elf.find(".symtab"))}
    insns = object_instructions(obj, name) or []
    for insn in insns:
        for _kind, symbol, _addend in insn[2]:
            section, value = symbols.get(symbol, (0, 0))
            if section not in (0, 0xFFF1):
                raise ValueError("section-defined relocation %s requires unit layout" % symbol)
            if section == 0xFFF1 and sym_address(symbol) != value:
                raise ValueError("absolute relocation %s has an ELF value differing from its address convention" % symbol)
    return resolve(insns)


_checked = {}


def check_fresh(obj, src, build=True, deps=()):
    """None if the built object `obj` is up to date with `src` and the headers `deps`, else the
    reason it is not. Runs `ninja obj` (when build.ninja and ninja exist), so a failed build is
    reported instead of leaving the previous object to produce a stale MATCH (`ninja obj` also fails
    while tools/check_headers.py does); then requires the object to exist and to be at least as new
    as its source and every header the source includes."""
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
    return problem


HEADERS_OK = os.path.join("build", "headers.ok")


_headers = {}


def header_problem(build=True):
    """None when the header rules hold, else the check_headers output (T-9030): an object that was built
    earlier can be up to date while build/headers.ok fails (an unguarded MAIN_API_OVERRIDE_, a
    duplicate in main_api.h), and the merge then breaks the build. Runs `ninja build/headers.ok` when
    the build exists, else tools/check_headers.py on include/ and src/. Cached per process."""
    if build not in _headers:
        _headers[build] = _header_problem(build)
    return _headers[build]


def _header_problem(build):
    if build and os.path.exists("build.ninja") and shutil.which("ninja"):
        r = subprocess.run(["ninja", HEADERS_OK], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
        if r.returncode == 0:
            return None
        return "\n".join(r.stdout.strip().splitlines()[-15:])
    if not os.path.isdir("include"):
        return None
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import check_headers
    problems = check_headers.check("include", "src" if os.path.isdir("src") else None)
    return "\n".join(problems[:15]) if problems else None


def asm_functions(c, stale=None):
    """[(vram, folder, name)] of every function of the C file `c` in the original disassembly
    (matched or not), in address order: the .s files of its matchings and nonmatchings folders. The
    address is the one of the first instruction (names such as bg_read_sub2 carry none).

    A function that flipped between C and INCLUDE_ASM leaves its .s in the folder of the old state
    next to the new one (splat never deletes), and both in one stub is "symbol already defined"
    (T-9220). Only the copy of the folder that matches the C file's state is used: matchings when the C
    file defines the function, else nonmatchings (when the C file cannot be read: the newer file). The
    ignored copies are appended to `stale` (a list) when given."""
    found = {}
    for folder in (c.matchings, c.nonmatchings):
        if not os.path.isdir(folder):
            continue
        for fn in sorted(os.listdir(folder)):
            if not fn.endswith(".s"):
                continue
            path = os.path.join(folder, fn)
            with open(path, errors="replace") as fh:
                m = re.search(r"^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/", fh.read(), re.M)
            found.setdefault(fn[:-2], []).append(
                (int(m.group(1), 16) if m else 0xFFFFFFFF, str(folder).replace(os.sep, "/"), fn[:-2], path))
    try:
        defined = srcscan.defined_functions(c.src)
    except OSError:
        defined = None
    out = []
    for name, copies in found.items():
        if len(copies) > 1:
            if defined is not None:
                want = str(c.matchings if name in defined else c.nonmatchings).replace(os.sep, "/")
                keep = next((x for x in copies if x[1] == want), None)
            else:
                keep = None
            keep = keep or max(copies, key=lambda x: os.path.getmtime(x[3]))
            if stale is not None:
                stale += [x[3] for x in copies if x is not keep]
            copies = [keep]
        out.append(copies[0][:3])
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


def expected_object(c, refresh=False, compile_fn=compile_expected, stale=None):
    """(path, problem): the original-side object of the srcscan.CFile `c` under expected/, created or
    refreshed from the original .s files when it is missing, forced, or its key changed."""
    obj = os.path.join("expected", c.obj)
    os.makedirs(os.path.dirname(obj), exist_ok=True)
    with open(obj + ".lock", "a") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        return _expected_object(c, refresh, compile_fn, stale)


def _expected_object(c, refresh, compile_fn, stale):
    funcs = asm_functions(c, stale)
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
    for stale in (obj + ".key",):
        if os.path.exists(stale):
            os.remove(stale)
    with tempfile.TemporaryDirectory(dir=os.path.dirname(obj), prefix="expected-") as tmp:
        pending = os.path.join(tmp, "original.o")
        problem = compile_fn(stub_path, pending)
        if problem is None:
            os.replace(pending, obj)
    if problem:
        if os.path.exists(obj):
            os.remove(obj)
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
    if len(want) == len(got):
        # SequenceMatcher describes a scheduling swap as one moved instruction,
        # hiding the other half. Equal-length functions need positional evidence.
        for i, (a, b) in enumerate(zip(want, got)):
            if keys([a]) != keys([b]):
                rows += ["-@row %d %s" % (i, disp(a)), "+@row %d %s" % (i, disp(b))]
        return rows
    sm = difflib.SequenceMatcher(None, keys(want), keys(got), autojunk=False)
    for op, i1, i2, j1, j2 in sm.get_opcodes():
        if op != "equal":
            rows += ["-" + disp(r) for r in want[i1:i2]] + ["+" + disp(r) for r in got[j1:j2]]
    return rows


def resolved_verdict(built, expected, name, original_file=None):
    """How the text DIFF of `name` looks once relocations are applied (T-9220): ("table", None) when the
    resolved words agree and a jump table is involved, ("same", None) when they agree (naming noise only),
    ("real", diff rows) when they differ (a wrong symbol+offset, for example a GameState field), or None
    when the resolved comparison cannot be made."""
    got = resolved_functions(built, [name])[name]
    want = resolved_functions(expected, [name], original_file)[name]
    if got is None or want is None:
        return None
    if keys(got) == keys(want):
        return ("table" if any(TABLE_REF in k for k in keys(want)) else "same"), None
    return "real", diff_lines(want, got)


def main(argv):
    with tempfile.TemporaryDirectory(prefix="funcdiff-") as tmp:
        return _main(argv, Path(tmp))


def _main(argv, directory):
    import funcdiff_verify as proof
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
    ap.add_argument("--version", choices=("auto", "c", "default"), default="auto",
                    help="auto compiles NON_MATCHING C when guarded; c forces it; default compares the normal build")
    protoverride.add_argument(ap)
    args = ap.parse_args(argv)
    if shutil.which("mips-linux-gnu-objdump") is None:
        sys.exit(NO_DOCKER)
    if not args.built and not os.path.isdir("config"):
        sys.exit("funcdiff.py: run it from the repository root")
    try:
        protos = protoverride.parse(args.proto)
    except protoverride.ProtoError as err:
        sys.exit("funcdiff.py: %s" % err)
    if protos and args.built:
        sys.exit("funcdiff.py: --proto changes how the C file is compiled; it cannot be combined with --built")
    get = resolved_functions if args.resolve else functions
    bad = 0
    refreshed, noted = set(), set()
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
        inputs = [c.src, *sorted(funcloc.includes_of(c.src))] if c is not None else []
        before = {str(p): hashlib.sha256(Path(p).read_bytes()).digest() for p in inputs}
        if c is not None and not protos:   # an object of the build: its source and headers must not be newer
            problem = check_fresh(b, c.src, not args.no_build, sorted(funcloc.includes_of(c.src)))
            if problem:
                print("%s: ERROR %s" % (n, problem))
                bad += 1
                continue
        # Each comparison owns its snapshots, even when several names share an object.
        job = Path(tempfile.mkdtemp(dir=directory))
        version = "explicit object" if args.built else "default build"
        notes = []
        try:
            use_c = c is not None and not args.built and (args.version == "c" or
                                                          (args.version == "auto" and proof.guarded(c.src, name)))
            if protos and not args.built and c is not None:
                b, notes = proof.compile_unit(c, job, protos, non_matching=use_c)
                version = "%s with --proto %s" % (
                    "NON_MATCHING C (full translation unit)" if use_c else "default build (full translation unit)",
                    protoverride.describe(protos))
            elif use_c:
                b = proof.compile_guarded(c, job)
                version = "NON_MATCHING C (full translation unit)"
            else:
                snapshot = job / "built.o"
                snapshot.write_bytes(Path(b).read_bytes())
                b = str(snapshot)
        except (OSError, ValueError, SystemExit) as err:
            print("%s: ERROR %s" % (n, err))
            bad += 1
            continue
        print("%s: compared %s" % (n, version))
        for note in notes:
            print("  " + note)
        e = args.expected
        if e is None:
            if c is None:       # a scratch object: the original side is that of the file holding the function
                try:
                    c = locate(name, scope)
                except funcloc.LocateError as err:
                    print("%s: ERROR %s" % (n, err))
                    bad += 1
                    continue
            stale = []
            e, problem = expected_object(c, args.refresh_expected and c.obj not in refreshed, stale=stale)
            refreshed.add(c.obj)
            if stale and c.obj not in noted:
                noted.add(c.obj)
                print("note: ignored %d stale asm file(s) of functions that changed between C and INCLUDE_ASM "
                      "(e.g. %s); `rm` them or rebuild asm/ to clean up" % (len(stale), stale[0]))
            if problem:
                print("%s: ERROR %s" % (n, problem))
                bad += 1
                continue
        problem = header_problem(not args.no_build)
        if problem:     # never MATCH on a tree whose headers fail the build
            print("%s: ERROR the header check (build/headers.ok) FAILED, a MATCH would not survive the build:\n%s"
                  % (n, problem))
            bad += 1
            continue
        reference = job / "expected.o"
        reference.write_bytes(Path(e).read_bytes())
        e = str(reference)
        try:
            got = get(b, [name])[name]
            want = (resolved_functions(e, [name], c) if args.resolve else get(e, [name]))[name]
        except (OSError, ValueError, subprocess.CalledProcessError) as err:
            print("%s: ERROR %s" % (n, err))
            bad += 1
            continue
        if got is None or want is None:
            print("%s: missing in %s" % (n, "built" if got is None else "expected"))
            bad += 1
        elif keys(got) == keys(want):
            try:
                if c is not None:
                    equal, reason = proof.exact_match(b, c, name, job, not args.no_build,
                                                      None if args.built else version.startswith("NON_MATCHING") or
                                                      (not proof.guarded(c.src, name) and name in srcscan.defined_functions(Path(c.src))))
                else:
                    # Without unit layout, prove only relocations with known absolute addresses.
                    strict = [absolute_object_rows(o, name) for o in (b, e)]
                    equal = (keys(strict[0]) == keys(strict[1]) and bool(strict[0]) and
                             all(re.fullmatch(r"[0-9a-f]{8}", k) for rows in strict for k in keys(rows)))
                    reason = "exact resolved instruction bytes" if equal else "unit layout required for literal-byte proof"
                if any(hashlib.sha256(Path(p).read_bytes()).digest() != digest for p, digest in before.items()):
                    equal, reason = False, "source or header changed during comparison; rerun"
            except (OSError, ValueError, subprocess.CalledProcessError, SystemExit) as err:
                equal, reason = False, "literal-byte proof unavailable: %s" % err
            print("%s: %s (%s)" % (n, "MATCH" if equal else "DIFF", reason))
            if equal and protos:
                print("  (with the prototype override above; nothing on disk changed)")
                for line in protoverride.commit_hint(protos):
                    print("  " + line)
            bad += not equal
        else:
            verdict = ("real", diff_lines(want, got)) if args.resolve else resolved_verdict(b, e, name, c)
            if verdict and verdict[0] == "table":
                print("%s: normalized table references agree; literal-byte proof requires --resolve" % n)
                verdict = None
            bad += 1
            print("%s: DIFF (- expected, + built)" % n)
            for d in diff_lines(want, got):
                print("  " + d)
            if verdict and verdict[0] == "same":
                print("  resolved: the words are identical once relocations are applied; the rows above differ "
                      "only in symbol naming (`D_800E6448` against `D_800E6280+0x1C8`). --resolve verifies literal bytes.")
            elif verdict:
                if not args.resolve:
                    print("  after relocation (- expected, + built):")
                    for d in verdict[1] or []:
                        print("    " + d)
                print("  resolved: REAL DIFFERENCE, not naming. Symbol+offset or instruction words differ "
                      "after relocation.")
            else:
                print("  resolved: UNAVAILABLE; the function is missing from the resolved comparison.")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
