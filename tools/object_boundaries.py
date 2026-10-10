#!/usr/bin/env python3
"""Find the original objects (translation units) of the main game code and of every overlay (T-0500).

usage: object_boundaries.py [--write] [--verbose] [UNIT...]
       (UNIT = main or an overlay name; default all. Needs disc/ and a split asm/: run ninja once.)

The original link put the objects in the same order in every section: [.text of object 1][.text of
object 2]... then [.rodata of object 1][.rodata of object 2]... then .data. Each object's section
starts 16-aligned. IDO writes one object's .rodata as [strings and constants][jump tables], padded
with zeros to a multiple of 16 (wiki/build-system.md, T-1340). The script reads the original bytes
and splat's asm (functions, their %hi/%lo and jal references, rodata symbols) and derives:

Text boundaries (an object starts at a function start):
  pad       a run of zero words after a function that ends exactly on the next 16-byte boundary
            (the object alignment; functions inside an object follow each other without a gap)
  rodata    two rodata objects (below) are used by functions of one text object; the boundary lies
            between the last user of the first and the first user of the second; exactly one
            function start in that range is 16-aligned with no gap before it
  choice/N  as `rodata`, but N candidates; the one with the fewest calls crossing it is taken
            (low confidence; the build cannot tell, every candidate links the same bytes)

Rodata boundaries (an object's rodata starts):
  start     first rodata byte after the text (overlay) / first game rodata (main exe)
  jtbl-pad  a jump table followed by zero words up to the next 16-byte boundary (table words are
            never zero, so the zeros are the object's padding)
  jtbl-end  a jump table followed by something that is not a jump table: tables end an object
  str-pad   a string followed by more zero bytes than its 4-byte alignment needs, up to the next
            16-byte boundary
  owner     the symbols on both sides are used by different text objects; exactly one 16-aligned
            symbol start lies between them
  owner-choice/N  as `owner` with N candidates (the first is taken)

Rodata end (overlays: start of .data in the same blob): the end of the last jump table's object,
extended by following padding-delimited chunks while they hold strings, no symbol in them is written
by code or holds a pointer, and their users stay in text order (`.data` starts again at the first
object). Main exe: the end of the game rodata, 0x800B23B0, where the SDK libraries' rodata starts
(wiki/source-files.md).

An object's rodata chunk can be an island (the C object provides it, tools/rodata_pieces.py) when
the order asm-processor reproduces equals the original order: strings and constants owned by
(first mentioned by) functions in function order, symbols no function mentions placed with
INCLUDE_RODATA between them, all jump tables after all other symbols in function order, and every
INCLUDE_ASM function long enough for asm-processor to emit its tables (late_rodata_fits).
Otherwise the chunk stays asm and the reason is written.

--write stores the result as config/objects/<UNIT>.txt (read by tools/split_objects.py). Without it
the script prints a summary per unit; --verbose lists the objects. Tests: tools/test_objects.py.
"""
import argparse
import bisect
import glob
import os
import re
import sys
from typing import NamedTuple

INSN_RE = re.compile(r'^\s*/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+[0-9A-Fa-f]{8}\s*\*/\s+(\w+)\s*(.*?)\s*$')
NONMATCHING_RE = re.compile(r'^nonmatching\s+(\w+),\s*(0x[0-9A-Fa-f]+|\d+)', re.M)
REF_RE = re.compile(r'%(?:hi|lo)\((\w+)')
DLABEL_RE = re.compile(r'^dlabel\s+(\w+)')
LINE_ADDR_RE = re.compile(r'/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})[\s*]')
WORD_SYM_RE = re.compile(r'\.word\s+([A-Za-z_]\w*)')
STORES = {"sb", "sh", "sw", "swl", "swr"}

MAIN_TEXT = (0x80041000, 0x80086810)      # game code; the SDK starts at 0x80086810 (T-0010)
MAIN_RODATA = (0x800AF340, 0x800B23B0)    # game rodata; SDK rodata follows (T-0012, T-0301)
MAIN_FILE_OFF = 0x800 - 0x80041000        # file offset = vram + this


class Func(NamedTuple):
    name: str
    addr: int
    size: int
    refs: frozenset     # symbols named by %hi/%lo
    stores: frozenset   # symbols written (store with %lo)
    calls: frozenset    # jal target names
    at_groups: tuple = ()   # symbol sets stored through one shared `lui $at` (T-9010)


class Item(NamedTuple):
    name: str
    addr: int
    kind: str           # jtbl, str or data
    wrefs: frozenset    # symbols named by .word


class Unit(NamedTuple):
    name: str           # main or the overlay name
    binary: str
    file_off: int       # file offset = vram + file_off
    text: tuple         # (start, end) vram
    rodata_lo: int
    rodata_hi: int      # main: fixed end of the game rodata; overlays: end of the file
    func_glob: str
    item_glob: str


class Obj(NamedTuple):
    text_start: int
    text_end: int
    ro_start: object    # int or None
    ro_end: object
    island: str         # "yes" or "no:<reason>"
    text_ev: str
    ro_ev: str


class Result(NamedTuple):
    unit: str
    objects: list
    rodata_end: int
    rodata_end_ev: str
    orphans: list       # [(start, end, reason)] rodata chunks no single text object owns
    problems: list


def units(root="."):
    out = [Unit("main", os.path.join(root, "disc/files/SLPM_86.053"), MAIN_FILE_OFF, MAIN_TEXT,
                MAIN_RODATA[0], MAIN_RODATA[1], "asm/*matchings/main/**/*.s", "asm/data/**/*.s")]
    with open(os.path.join(root, "config/overlays.txt")) as f:
        rows = f.read().splitlines()
    for line in rows:
        if line.strip() and not line.startswith("#"):
            n, b, t = line.split()
            base, text = int(b, 16), int(t, 16)
            out.append(Unit(n, os.path.join(root, "disc/files/CDROM/EXEDIR/%s.EXN" % n), -base,
                            (base, base + text), base + text, base + 0x30000,
                            "asm/ovl/%s/*matchings/**/*.s" % n, "asm/ovl/%s/data/**/*.s" % n))
    return out


def parse_function(text):
    """Func from the text of one splat function file, or None."""
    m = NONMATCHING_RE.search(text[:400])
    if not m:
        return None
    addr = None
    refs, stores, calls = set(), set(), set()
    for line in text.splitlines():
        im = INSN_RE.match(line)
        if not im:
            continue
        if addr is None:
            addr = int(im.group(1), 16)
        syms = REF_RE.findall(im.group(3))
        refs.update(syms)
        if im.group(2) in STORES:
            stores.update(syms)
        elif im.group(2) == "jal":
            calls.add(im.group(3).strip())
    if addr is None:
        return None
    return Func(m.group(1), addr, int(m.group(2), 0), frozenset(refs), frozenset(stores), frozenset(calls),
                at_groups(text))


AT_LUI_RE = re.compile(r'^lui\s+\$at,\s*%hi\((\w+)\)')
AT_STORE_RE = re.compile(r'%lo\((\w+)\)\(\$at\)')
AT_WRITE_RE = re.compile(r'^\w+\s+\$at\b')


def at_groups(text):
    """Symbol sets of two or more symbols that one `lui $at` serves for stores (T-9010): from a
    `lui $at, %hi(X)` to the next label or other write of $at, the symbols of the stores through
    $at. IDO shares `$at` only for stores through one symbol defined in the same file, so every
    group is one variable of the function's own object."""
    out = []
    cur = None
    for line in text.splitlines():
        if line.lstrip().startswith(".L"):
            cur = None
            continue
        im = INSN_RE.match(line)
        if not im:
            continue
        ins = "%s %s" % (im.group(2), im.group(3))
        m = AT_LUI_RE.match(ins)
        if m:
            if cur and len(cur) > 1:
                out.append(frozenset(cur))
            cur = set()
            continue
        if im.group(2) in STORES:
            sm = AT_STORE_RE.search(im.group(3))
            if sm and cur is not None:
                cur.add(sm.group(1))
            continue
        if AT_WRITE_RE.match(ins):
            if cur and len(cur) > 1:
                out.append(frozenset(cur))
            cur = None
    if cur and len(cur) > 1:
        out.append(frozenset(cur))
    return tuple(out)


def parse_items(text):
    """[Item] of one splat data file (dlabel blocks)."""
    out = []
    cur = None
    for line in text.splitlines():
        m = DLABEL_RE.match(line)
        if m:
            cur = [m.group(1), None, "data", set()]
            continue
        if cur is None:
            continue
        if line.startswith("enddlabel"):
            if cur[1] is not None:
                out.append(Item(cur[0], cur[1], "jtbl" if cur[0].startswith("jtbl_") else cur[2],
                                frozenset(cur[3])))
            cur = None
            continue
        am = LINE_ADDR_RE.search(line)
        if am and cur[1] is None:
            cur[1] = int(am.group(1), 16)
        if ".asciz" in line or ".ascii" in line:
            cur[2] = "str"
        cur[3].update(WORD_SYM_RE.findall(line))
    return out


def load(unit, root="."):
    """(funcs sorted by address, items sorted by address) of a unit from the split asm."""
    funcs = {}
    for p in glob.glob(os.path.join(root, unit.func_glob), recursive=True):
        with open(p, errors="replace") as f:
            fn = parse_function(f.read())
        if fn and unit.text[0] <= fn.addr < unit.text[1]:
            funcs[fn.name] = fn
    items = {}
    for p in glob.glob(os.path.join(root, unit.item_glob), recursive=True):
        with open(p, errors="replace") as f:
            for it in parse_items(f.read()):
                if unit.rodata_lo <= it.addr < unit.rodata_hi:
                    items[it.addr] = it
    return sorted(funcs.values(), key=lambda f: f.addr), [items[a] for a in sorted(items)]


def content_end(item, blob):
    """Offset in `blob` (the item's bytes up to the next item) after its real content."""
    if item.kind == "jtbl":
        k = len(blob)
        while k >= 4 and blob[k - 4:k] == b"\0\0\0\0":
            k -= 4
        return k
    if item.kind == "str":
        k = len(blob.rstrip(b"\0")) + 1
        return min(len(blob), (k + 3) & ~3)
    return len(blob)


def text_pad_bounds(funcs, word):
    """({start: 'pad'} of object starts proven by alignment padding, [ambiguous gaps])."""
    bounds, amb = {}, []
    for f, g in zip(funcs, funcs[1:]):
        end = f.addr + f.size
        if end == g.addr:
            continue
        zero = all(word(x) == 0 for x in range(end, g.addr, 4))
        if zero and g.addr % 16 == 0 and g.addr - end == (-end) % 16:
            bounds[g.addr] = "pad"
        else:
            amb.append((end, g.addr))
    return bounds, amb


def prev_end(funcs, fstart, a):
    k = fstart.index(a)
    return funcs[k - 1].addr + funcs[k - 1].size if k else a


def best_cut(funcs, cands, lo, hi):
    """The candidate text boundary with the fewest calls crossing it among the functions in
    [lo, hi]; ties go to the first."""
    addr = {f.name: f.addr for f in funcs}
    edges = [(f.addr, addr[c]) for f in funcs for c in f.calls if c in addr]
    best = None
    for c in cands:
        cross = sum(1 for a, b in edges if min(a, b) < c <= max(a, b) and lo <= min(a, b) and max(a, b) <= hi)
        if best is None or cross < best[0]:
            best = (cross, c)
    return best[1]


def late_rodata_fits(words, ninstr):
    """True when asm-processor (IDO -O2 -mips1, no -KPIC) can make the compiler emit `words` words
    of .late_rodata from an INCLUDE_ASM block of `ninstr` instructions: one dummy float costs
    3 instructions, and from the second word on a dummy switch covers the rest when at least 5
    words remain and 12 instructions are left (asm_processor.py, Function.finish)."""
    lines, extra_nop = 0, False
    for i in range(words):
        if i >= 1 and words - i >= 5 and ninstr - lines >= 12:
            lines += 11
            extra_nop = i != 2
            break
        lines += 3
        extra_nop = True
    return lines + extra_nop <= ninstr - 1


def island_check(items, ks, users, funcs, jtbl_words=None):
    """'yes' when asm-processor can rebuild the chunk `ks` (item indices, address order) from the
    functions of its object, else 'no:<reason>' (see the module docstring). jtbl_words: {item
    index: table words}, for the late rodata size check."""
    fns = {f.addr for f in funcs}
    for f in funcs:
        words = sum((jtbl_words or {}).get(k, 0) for k in ks if items[k].kind == "jtbl"
                    and f.addr == min(users[k] or [0]))
        if words and not late_rodata_fits(words, f.size // 4):
            return "no:%s_too_short_for_its_jump_tables" % f.name
    owner = {}
    for k in ks:
        us = [u for u in users[k] if u in fns]
        if users[k] and len(us) != len(users[k]):
            return "no:%s_used_from_another_object" % items[k].name
        owner[k] = min(us) if us else None
    kinds = [items[k].kind for k in ks]
    if "jtbl" in kinds and any(x != "jtbl" for x in kinds[kinds.index("jtbl"):]):
        return "no:data_after_a_jump_table"
    seq = [(k, owner[k]) for k in ks if items[k].kind != "jtbl"]
    last = -1
    for k, o in seq:
        if o is None:
            continue
        if o < last:
            return "no:%s_out_of_function_order" % items[k].name
        last = o
    # an unowned symbol between two symbols of one function cannot be placed
    for i, (k, o) in enumerate(seq):
        if o is not None:
            continue
        before = next((x for _, x in reversed(seq[:i]) if x is not None), None)
        after = next((x for _, x in seq[i + 1:] if x is not None), None)
        if before is not None and before == after:
            return "no:unowned_%s_inside_one_function's_symbols" % items[k].name
    last = -1
    for k in ks:
        if items[k].kind != "jtbl":
            continue
        if owner[k] is None:
            return "no:jump_table_%s_without_user" % items[k].name
        if owner[k] < last:
            return "no:%s_out_of_function_order" % items[k].name
        last = owner[k]
    return "yes"


def layout_bounds(items, data, off, hi, problems):
    """({address: evidence} of rodata object starts proven by the layout, items). An object's
    rodata ends at its last item's content rounded up to 16, with zeros in between; zeros beyond
    that belong to what follows, so a boundary can fall between two items: then a synthetic
    item D_<address> (the name splat gives the first symbol of a subsegment) starts there."""
    ends = [it.addr for it in items[1:]] + [hi]
    rb, extra = {}, []
    for k, it in enumerate(items[:-1]):
        nxt = items[k + 1]
        blob = data[it.addr + off:ends[k] + off]
        cend = it.addr + content_end(it, blob)
        pend = (cend + 15) & ~15
        if any(data[cend + off:min(pend, nxt.addr) + off]):
            if it.kind == "jtbl" and nxt.kind != "jtbl":
                problems.append("jump table %s is followed by non-zero bytes before %08X" % (it.name, pend))
            continue
        if it.kind == "jtbl" and (nxt.kind != "jtbl" or cend < nxt.addr):
            ev = "jtbl-pad" if pend > cend else "jtbl-end"
        elif it.kind == "str" and cend < pend <= nxt.addr:
            ev = "str-pad"
        else:
            continue
        if pend > nxt.addr:
            problems.append("%s at %08X starts inside the padding after %s" % (nxt.name, nxt.addr, it.name))
            continue
        rb[pend] = ev
        if pend < nxt.addr:
            extra.append(Item("D_%08X" % pend, pend, "data", frozenset()))
    return rb, sorted(items + extra, key=lambda i: i.addr)


def analyze(unit, funcs, items, data):
    """Result for one unit. data: the unit's original file bytes."""
    off = unit.file_off
    word = lambda a: int.from_bytes(data[a + off:a + off + 4], "little")
    problems = []
    fstart = [f.addr for f in funcs]
    tbounds, amb = text_pad_bounds(funcs, word)
    for a, b in amb:
        problems.append("text gap %08X-%08X is not object padding (not used)" % (a, b))
    tbounds[funcs[0].addr] = "start"

    rb, items = layout_bounds(items, data, off, unit.rodata_hi, problems)
    ends = [it.addr for it in items[1:]] + [unit.rodata_hi]
    blobs = [data[it.addr + off:e + off] for it, e in zip(items, ends)]
    byname = {it.name: k for k, it in enumerate(items)}
    users = [set() for _ in items]      # function addresses that name item k
    writes = [False] * len(items)
    for f in funcs:
        for r in f.refs:
            if r in byname:
                users[byname[r]].add(f.addr)
        for r in f.stores:
            if r in byname:
                writes[byname[r]] = True
    ptr = [bool(it.wrefs) for it in items]

    # rodata end
    if unit.name == "main":
        ro_end, ro_end_ev = unit.rodata_hi, "sdk-start"
    else:
        last_j = max([k for k, it in enumerate(items) if it.kind == "jtbl"] or [-1])
        if last_j >= 0:
            ro_end = next((a for a in sorted(rb) if a > items[last_j].addr), unit.rodata_hi)
            ro_end_ev = "last-jtbl"
        else:
            ro_end, ro_end_ev = unit.rodata_lo, "no-jtbl"
        hi_user = max([u for k, it in enumerate(items) if it.addr < ro_end for u in users[k]] or [0])
        lo = ro_end
        for nxt in sorted(a for a in rb if a > ro_end) + [unit.rodata_hi]:
            ks = [k for k, it in enumerate(items) if lo <= it.addr < nxt]
            us = [u for k in ks for u in users[k]]
            if not ks or not us or any(writes[k] or ptr[k] for k in ks) \
                    or not any(items[k].kind == "str" for k in ks) or min(us) < hi_user:
                break
            hi_user = max(us)
            ro_end, ro_end_ev, lo = nxt, "monotone-strings", nxt
    items_ro = [k for k, it in enumerate(items) if it.addr < ro_end]
    for k in items_ro:
        if writes[k]:
            problems.append("%s is written by code but lies in rodata" % items[k].name)

    rbounds = dict((a, e) for a, e in rb.items() if a < ro_end)
    rbounds[unit.rodata_lo] = "start"

    def chunks():
        rs = sorted(rbounds)
        return [(s, rs[c + 1] if c + 1 < len(rs) else ro_end) for c, s in enumerate(rs)]

    # refine text and rodata boundaries against each other until stable
    for _ in range(1000):
        changed = False
        tstarts = sorted(tbounds)
        tobj = lambda a: bisect.bisect_right(tstarts, a) - 1
        # 1. a chunk used by two text objects is two objects: cut where the user changes
        for s, e in chunks():
            ks = [k for k in items_ro if s <= items[k].addr < e]
            seq = [(k, tobj(min(users[k]))) for k in ks if users[k]]
            for (k1, t1), (k2, t2) in zip(seq, seq[1:]):
                if t1 >= t2:
                    continue
                lo = items[k1].addr + content_end(items[k1], blobs[k1])
                cands = [items[k].addr for k in range(k1 + 1, k2 + 1)
                         if items[k].addr % 16 == 0 and items[k].addr >= lo
                         and not any(data[lo + off:items[k].addr + off])]
                if cands:
                    rbounds[cands[0]] = "owner" if len(cands) == 1 else "owner-choice/%d" % len(cands)
                    changed = True
                    break
            if changed:
                break
        if changed:
            continue
        # 2. two chunks used by one text object: a hidden text boundary between their users
        info = []
        for s, e in chunks():
            us = sorted(u for k in items_ro if s <= items[k].addr < e for u in users[k])
            if us:
                info.append((s, us[0], us[-1]))
        for (s1, lo1, hi1), (s2, lo2, hi2) in zip(info, info[1:]):
            if tobj(hi1) != tobj(lo2) or hi1 >= lo2:
                continue
            cands = [a for a in fstart if hi1 < a <= lo2 and a % 16 == 0 and a not in tbounds
                     and prev_end(funcs, fstart, a) == a]
            if cands:
                tbounds[cands[0] if len(cands) == 1 else best_cut(funcs, cands, hi1, lo2)] = \
                    "rodata" if len(cands) == 1 else "choice/%d" % len(cands)
                changed = True
                break
        if not changed:
            break

    # objects: each text object gets the one chunk only its functions use
    tstarts = sorted(tbounds)
    tobj = lambda a: bisect.bisect_right(tstarts, a) - 1
    owned, orphans = {}, []
    for s, e in chunks():
        ks = [k for k in items_ro if s <= items[k].addr < e]
        ts = sorted({tobj(u) for k in ks for u in users[k]})
        if not ts:
            orphans.append((s, e, "no_code_reference"))
        elif len(ts) > 1:
            orphans.append((s, e, "used_by_%d_text_objects" % len(ts)))
            problems.append("rodata %08X-%08X is used by text objects %s"
                            % (s, e, " ".join("%08X" % tstarts[t] for t in ts)))
        elif ts[0] in owned:
            orphans.append((s, e, "second_chunk_of_%08X" % tstarts[ts[0]]))
            problems.append("rodata %08X-%08X is a second chunk of text object %08X"
                            % (s, e, tstarts[ts[0]]))
        else:
            owned[ts[0]] = (s, e, ks)
    objects = []
    for t, s in enumerate(tstarts):
        e = tstarts[t + 1] if t + 1 < len(tstarts) else unit.text[1]
        if t in owned:
            rs, re_, ks = owned[t]
            jw = {k: content_end(items[k], blobs[k]) // 4 for k in ks if items[k].kind == "jtbl"}
            why = island_check(items, ks, users, [f for f in funcs if s <= f.addr < e], jw)
            objects.append(Obj(s, e, rs, re_, why, tbounds[s], rbounds[rs]))
        else:
            objects.append(Obj(s, e, None, None, "no:no_rodata", tbounds[s], "-"))
    return Result(unit.name, objects, ro_end, ro_end_ev, orphans, problems)


# ------------------------------------------------------------------ .data and .bss (T-9010)

MAIN_DATA = (0x800B3220, 0x800E3800)      # main exe .data (game and SDK), splat `data`
MAIN_BSS = (0x800E3800, 0x8012B538)       # main exe .bss, splat `bss`
OVERLAY_SIZE = 0x30000                    # every overlay file is one 192 KiB slot; no .bss


class DataRange(NamedTuple):
    kind: str           # data or bss
    obj: int            # text start of the object
    start: int
    end: int
    start_ev: str
    end_ev: str
    evidence: str       # at:<n>,ref:<m> items that placed the object


def data_regions(unit, ro_end):
    """[(kind, lo, hi)] of the unit's .data/.bss address ranges."""
    if unit.name == "main":
        return [("data",) + MAIN_DATA, ("bss",) + MAIN_BSS]
    return [("data", ro_end, unit.text[0] + OVERLAY_SIZE)]


COMMENT_RE = re.compile(r'/\*([^*]*)\*/')


def parse_data_items(text):
    """[(addr, name, wrefs)] of one splat data or bss file (dlabel blocks; a bss line has only
    the address in its comment)."""
    out = []
    cur = None

    def close():
        if cur is not None and cur[1] is not None:
            out.append((cur[1], cur[0], frozenset(cur[2])))
    for line in text.splitlines():
        m = DLABEL_RE.match(line)
        if m:
            close()
            cur = [m.group(1), None, set()]
            continue
        if cur is None:
            continue
        if line.startswith("enddlabel"):
            close()
            cur = None
            continue
        cm = COMMENT_RE.search(line)
        if cm and cur[1] is None:
            toks = cm.group(1).split()
            vr = [t for t in toks[:2] if len(t) == 8 and t.upper().startswith("80")]
            if vr:
                cur[1] = int(vr[0], 16)
        cur[2].update(WORD_SYM_RE.findall(line))
    close()
    return out


def load_data_items(unit, root, lo, hi):
    """[(addr, name, wrefs)] in [lo, hi) from the unit's split data files, sorted."""
    pat = "asm/data/**/*.s" if unit.name == "main" else "asm/ovl/%s/data/**/*.s" % unit.name
    found = {}
    for p in glob.glob(os.path.join(root, pat), recursive=True):
        with open(p, errors="replace") as f:
            for a, n, w in parse_data_items(f.read()):
                if lo <= a < hi:
                    found.setdefault(a, (a, n, w))
    return [found[a] for a in sorted(found)]


def data_evidence(funcs, obj_starts, items):
    """({name: (object index, weight)}, problems). Weight 10: the name is in a shared-`$at`
    group of a function of that object (IDO shares `$at` only for one variable defined in the
    same file). Weight 1: the functions of exactly one object name it, or it is a pointer
    table whose function pointers all point into one object."""
    oi = lambda a: bisect.bisect_right(obj_starts, a) - 1
    fobj = {f.name: oi(f.addr) for f in funcs}
    users = {}
    for f in funcs:
        for r in f.refs:
            users.setdefault(r, set()).add(fobj[f.name])
    for _a, n, w in items:
        for x in w:
            if x in fobj:
                users.setdefault(n, set()).add(fobj[x])
    strong, problems = {}, []
    for f in funcs:
        for g in f.at_groups:
            for n in g:
                if n in strong and strong[n] != fobj[f.name]:
                    problems.append("%s shares $at in two objects" % n)
                strong[n] = fobj[f.name]
    ev = {}
    for _a, n, _w in items:
        if n in strong:
            ev[n] = (strong[n], 10)
        elif len(users.get(n, ())) == 1:
            ev[n] = (next(iter(users[n])), 1)
    return ev, problems


def _chain(seq):
    """The heaviest subsequence of [(k, (object, weight))] whose objects never decrease."""
    best, prev = [], []
    for i, (_k, (o, w)) in enumerate(seq):
        b, p = w, -1
        for j in range(i):
            if seq[j][1][0] <= o and best[j] + w > b:
                b, p = best[j] + w, j
        best.append(b)
        prev.append(p)
    chain = []
    i = max(range(len(seq)), key=lambda x: best[x]) if seq else -1
    while i >= 0:
        chain.append(seq[i])
        i = prev[i]
    return chain[::-1]


def data_bounds(items, ev, lo, hi, kind="data"):
    """([(object index, start, end, start ev, end ev, evidence)], problems) for one region.

    items: [(addr, name, ...)] sorted; ev: {name: (object index, weight)}. The original link
    put every object's .data (and .bss) in text order, each 16-aligned (IDO aligns and pads the
    sections to 16). The heaviest chain of evidence items whose objects never decrease in
    address order places the objects; items off the chain are globals used elsewhere. Between
    the last item of one object and the first of the next, a boundary is a 16-aligned item
    start: `owner` when there is exactly one, else `owner-min/N` (the tightest range for each
    side; the bytes between stay without owner). The first object starts at the last 16-aligned
    item start before its first item (`owner-min/N`, `start` when that is the region start);
    the last ends at the first one after its last item (`end` when that is the region end)."""
    addr = [it[0] for it in items]
    aligned = lambda a, b: [x for x in addr if a < x <= b and x % 16 == 0]
    dropped, hard = set(), []
    while True:
        chain = _chain([(k, ev[it[1]]) for k, it in enumerate(items)
                        if it[1] in ev and k not in dropped])
        per = {}
        for k, (o, w) in chain:
            f, l, na, nr = per.get(o, (k, k, 0, 0))
            per[o] = (min(f, k), max(l, k), na + (w >= 10), nr + (w < 10))
        order = sorted(per)
        # two neighbouring objects without a 16-aligned item start between them: the lighter
        # evidence item is a global of the other object (used elsewhere); drop it and retry
        clash = next(((per[a][1], per[b][0]) for a, b in zip(order, order[1:])
                      if not aligned(addr[per[a][1]], addr[per[b][0]])), None)
        if clash is None:
            break
        la, fb = clash
        wa, wb = ev[items[la][1]][1], ev[items[fb][1]][1]
        if wa >= 10 and wb >= 10:
            hard.append("%s: shared-$at items %s and %s of two objects have no 16-aligned "
                        "boundary between them" % (kind, items[la][1], items[fb][1]))
        weight = lambda k: (ev[items[k][1]][1], sum(per[ev[items[k][1]][0]][2:]))
        dropped.add(la if weight(la) < weight(fb) else fb)
    problems = hard
    for k, it in enumerate(items):
        if it[1] in ev and ev[it[1]][1] >= 10 and not any(k == c[0] for c in chain):
            problems.append("%s %s: shared-$at item is out of object order" % (kind, it[1]))
    if dropped:
        problems.append("%s: %d single-user items are globals of another object (%s)" % (
            kind, len(dropped), " ".join(items[k][1] for k in sorted(dropped)[:5])))
    bounds = {}
    for a, b in zip(order, order[1:]):
        la, fb = per[a][1], per[b][0]
        cands = aligned(addr[la], addr[fb])
        ev_ = "owner" if len(cands) == 1 else "owner-min/%d" % len(cands)
        bounds[(a, "end")] = (cands[0], ev_)
        bounds[(b, "start")] = (cands[-1], ev_)
    if order:
        first, last = order[0], order[-1]
        cands = [x for x in addr if lo <= x <= addr[per[first][0]] and x % 16 == 0]
        if cands:
            bounds[(first, "start")] = (cands[-1], "start" if cands == [lo] else
                                        "owner-min/%d" % len(cands))
        else:
            bounds[(first, "start")] = None
        nxt = per[last][1] + 1
        cands = [x for x in addr[nxt:] if x % 16 == 0] + ([hi] if hi % 16 == 0 else [])
        bounds[(last, "end")] = (cands[0], "end" if cands[0] == hi else "owner-min") if cands else None
    out = []
    for o in order:
        s, e = bounds.get((o, "start")), bounds.get((o, "end"))
        if s is None or e is None:
            continue
        out.append((o, s[0], e[0], s[1], e[1], "at:%d,ref:%d" % per[o][2:]))
    return out, problems


def analyze_data(unit, funcs, objs, ro_end, root="."):
    """([DataRange], problems) for one unit."""
    starts = [o.text_start for o in objs]
    ranges, problems = [], []
    for kind, lo, hi in data_regions(unit, ro_end):
        items = load_data_items(unit, root, lo, hi)
        ev, p = data_evidence(funcs, starts, items)
        problems += p
        res, p = data_bounds(items, ev, lo, hi, kind)
        problems += p
        ranges += [DataRange(kind, starts[o], s, e, se, ee, n) for o, s, e, se, ee, n in res]
    return ranges, problems


def format_data(ranges):
    return "".join("%s %08X %08X %08X %s %s %s\n" % (r.kind, r.obj, r.start, r.end, r.start_ev,
                                                       r.end_ev, r.evidence) for r in ranges)


def read_data(path):
    """[DataRange] from a config/objects file."""
    out = []
    with open(path) as f:
        for line in f:
            p = line.split()
            if p and p[0] in ("data", "bss"):
                out.append(DataRange(p[0], int(p[1], 16), int(p[2], 16), int(p[3], 16), p[4], p[5], p[6]))
    return out


DATA_HEADER = """# data|bss <text start> <start> <end> <start evidence> <end evidence> <items> (T-9010): the
# object's .data/.bss range; written by tools/object_boundaries.py --data --write.
"""


def write_data(path, ranges):
    """Replace the data/bss lines (and their header) of a config/objects file."""
    with open(path) as f:
        lines = [l for l in f if not l.split()[:1] or l.split()[0] not in ("data", "bss")]
    text = "".join(lines).replace(DATA_HEADER, "")
    with open(path, "w") as f:
        f.write(text + (DATA_HEADER + format_data(ranges) if ranges else ""))


def objects_path(unit, root="."):
    return os.path.join(root, "config/objects/%s.txt" % unit)


HEADER = """# Original objects of {what} (T-0500). Written by tools/object_boundaries.py --write
# (do not edit by hand; re-run it); read by tools/split_objects.py.
# object <text start> <text end> <rodata start|-> <rodata end|-> <island> <text evidence> <rodata evidence>
# island: yes = the C object provides the rodata chunk; no:<reason> = it stays asm.
# Evidence names: tools/object_boundaries.py docstring and wiki/source-files.md.
"""


def format_result(res):
    what = "the main game code" if res.unit == "main" else "overlay %s" % res.unit
    out = [HEADER.format(what=what)]
    out.append("rodata_end %08X %s\n" % (res.rodata_end, res.rodata_end_ev))
    for o in res.objects:
        out.append("object %08X %08X %s %s %s %s %s\n" % (
            o.text_start, o.text_end, "%08X" % o.ro_start if o.ro_start is not None else "-",
            "%08X" % o.ro_end if o.ro_end is not None else "-", o.island, o.text_ev, o.ro_ev))
    for s, e, why in res.orphans:
        out.append("orphan %08X %08X %s\n" % (s, e, why))
    return "".join(out)


def read_objects(path):
    """(objects, rodata_end, orphans) from a config/objects file."""
    objs, orphans, ro_end = [], [], None
    h = lambda x: None if x == "-" else int(x, 16)
    with open(path) as f:
        for line in f:
            p = line.split()
            if not p or p[0].startswith("#"):
                continue
            if p[0] == "rodata_end":
                ro_end = int(p[1], 16)
            elif p[0] == "object":
                objs.append(Obj(int(p[1], 16), int(p[2], 16), h(p[3]), h(p[4]), p[5], p[6], p[7]))
            elif p[0] == "orphan":
                orphans.append((int(p[1], 16), int(p[2], 16), p[3]))
    return objs, ro_end, orphans


def summary(res):
    ev = {}
    for o in res.objects:
        k = o.text_ev.split("/")[0]
        ev[k] = ev.get(k, 0) + 1
    isl = sum(1 for o in res.objects if o.island == "yes")
    ro = sum(1 for o in res.objects if o.ro_start is not None)
    return ("%-9s objects %3d (%s)  rodata chunks %3d, islands %3d, orphans %d, rodata end %08X (%s), problems %d"
            % (res.unit, len(res.objects), " ".join("%s=%d" % kv for kv in sorted(ev.items())), ro, isl,
               len(res.orphans), res.rodata_end, res.rodata_end_ev, len(res.problems)))


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("units", nargs="*")
    ap.add_argument("--root", default=".")
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--verbose", "-v", action="store_true")
    ap.add_argument("--data", action="store_true",
                    help="only the .data/.bss ranges, from the objects already in config/objects")
    a = ap.parse_args(argv)
    for u in units(a.root):
        if a.units and u.name not in a.units:
            continue
        funcs, items = load(u, a.root)
        if not funcs:
            sys.exit("object_boundaries.py: no functions for %s (run ninja first)" % u.name)
        if a.data:
            path = objects_path(u.name, a.root)
            objs, ro_end, _o = read_objects(path)
            ranges, problems = analyze_data(u, funcs, objs, ro_end, a.root)
            print("%-9s data ranges %3d of %3d objects (%s), problems %d" % (
                u.name, len(ranges), len(objs), " ".join(
                    "%s=%d" % (k, sum(1 for r in ranges if r.start_ev.split("/")[0] == k))
                    for k in ("start", "owner", "owner-min")), len(problems)))
            if a.verbose:
                print(format_data(ranges), end="")
                for p in problems:
                    print("  problem: " + p)
            if a.write:
                write_data(path, ranges)
            continue
        with open(u.binary, "rb") as f:
            data = f.read()
        res = analyze(u, funcs, items, data)
        print(summary(res))
        if a.verbose:
            print("".join(l for l in format_result(res).splitlines(True) if not l.startswith("#")), end="")
            for p in res.problems:
                print("  problem: " + p)
        if a.write:
            path = objects_path(u.name, a.root)
            os.makedirs(os.path.dirname(path), exist_ok=True)
            with open(path, "w") as f:
                f.write(format_result(res))
    return 0


if __name__ == "__main__":
    sys.exit(main())
