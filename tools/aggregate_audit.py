#!/usr/bin/env python3
"""How the original code reaches the fields of each aggregate of config/migrate_globals.txt (T-7010).

For every function of the original (matched and unmatched: `asm/**/{matchings,nonmatchings}`, main
exe and overlays) this tool follows the registers in one linear pass, like tools/type_recovery.py,
and classifies each load or store whose address lies inside an aggregate (`GameState D_800E6280`):

  direct    `lui rA,%hi(D_X); l/s rB,%lo(D_X)(rA)`: the %hi and %lo name the field's own address.
            IDO 5.3 emits exactly these words for a struct member too (`lui; lbu off+%lo(base)`
            relocates to the same instruction words), so this kind is NOT evidence for a separate
            symbol; it is what any constant-offset access looks like.
  sharedhi  `%lo(D_Y)(rA)` where rA holds `%hi(D_X)` of another address: one `lui` serves two fields.
  ptr       an immediate offset from a register that holds an address (`addiu rA,%lo(D_X)`, then
            `lbu rB,0x3A(rA)`): base-relative.
  ptridx    the same after an index was added (`addu rA,rA,t6`; strength-reduced loops).
  idx       `lui rA,%hi(D_X); addu rA,rA,t6; l rB,%lo(D_X)(rA)`: an indexed element. The field
            offset is folded into %lo, so it reads like `direct` for a member array.
  lo?       a %lo whose %hi register was not followed (set in another block).

sharedhi, ptr and ptridx are base-relative: the original reached the field from a register that
held another address of the same object. Per field (the top-level member of the aggregate) and per
original object (the asm subdirectory, one per source file) the report counts each kind.

It also measures the one thing that tells a struct member from a separate symbol in this
toolchain: as1 moves a load above a store to another symbol but not above a store through the same
symbol (wiki/data-types.md). For two read-modify-writes in one basic block (load X, store X from
it; load Y, store Y from it; loads and stores in that order, neither value derived from the other
load, different words, at most 0x100 bytes apart, `direct` accesses only), the pair is "kept" when
the load of Y follows the store of X and "hoisted" when it precedes it. Pairs are counted inside one
aggregate and, as the baseline, between addresses outside every aggregate. A hoisted pair inside an
aggregate would say that the unit reached the two fields through different symbols.

Last, a read-modify-write whose load goes to another register than its `lui` (`lui t9;
lbu t0,%lo(D_X)(t9); ori; sb`) is listed as a bit-field site: IDO computes the address of a
bit-field member apart from the load, while a scalar `D |= 0x40` loads into the `lui` register.

Usage (inside Docker, after configure.py so asm/ exists):
  python3 tools/aggregate_audit.py                 summary: kinds, pairs, units with base-relative
  python3 tools/aggregate_audit.py --fields        table per top-level field (kinds and units)
  python3 tools/aggregate_audit.py --units         table per original object
  python3 tools/aggregate_audit.py --pairs         every hoisted pair (inside and outside)
  python3 tools/aggregate_audit.py --bitfields     bit-field read-modify-write sites
  python3 tools/aggregate_audit.py --json OUT      everything as JSON
"""
import argparse
import json
import os
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import migrate_globals as mg  # noqa: E402
import type_recovery as tr  # noqa: E402

KINDS = ("direct", "sharedhi", "ptr", "ptridx", "idx", "lo?")
BASE_RELATIVE = ("sharedhi", "ptr", "ptridx")
PAIR_MAX_DIST = 0x100


class Ranges:
    """The aggregates as (base name, start, end) in the main-exe address space."""

    def __init__(self, items):
        self.items = list(items)

    def find(self, space, addr):
        if space != "main":
            return None
        for name, lo, hi in self.items:
            if lo <= addr < hi:
                return name, lo
        return None


class Access:
    __slots__ = ("addr", "kind", "store", "op", "pos", "reg", "hireg")

    def __init__(self, addr, kind, store, op, pos, reg, hireg):
        self.addr, self.kind, self.store, self.op = addr, kind, store, op
        self.pos, self.reg, self.hireg = pos, reg, hireg


class FuncAudit:
    def __init__(self, name):
        self.name = name
        self.accesses = []     # Access inside an aggregate
        self.pairs = []        # (where 'in'|'out', 'kept'|'hoisted', addr_x, addr_y)
        self.bitfields = []    # addresses with a bit-field style read-modify-write


def analyze_function(name, lines, unit, syms, ranges):
    """One linear pass over a function's asm; returns a FuncAudit."""
    res = FuncAudit(name)
    regs = {}             # reg -> (kind, (space, addr) | None, deps frozenset)
    nid = [0]
    block = {"loads": [], "stores": []}
    none = ("x", None, frozenset())

    def get(r):
        if r == "$zero":
            return ("const", 0, frozenset())
        return regs.get(r, none)

    def resolve(sym, add):
        r = syms.resolve(unit, sym)
        return (r[0], r[1] + tr.to_int(add)) if r else None

    def flush():
        rmw = []
        for st in block["stores"]:
            pos_s, (sp, a), deps, kind_s, _r = st
            if kind_s != "direct":
                continue
            for pos_l, (lsp, la), lid, kind_l, lreg, hreg in block["loads"]:
                if (lsp, la) == (sp, a) and lid in deps and pos_l < pos_s and kind_l == "direct":
                    rmw.append((pos_l, pos_s, sp, a, lid, deps))
                    if lreg != hreg and ranges.find(sp, a):
                        res.bitfields.append(a)
                    break
        for lx, sx, spx, x, idx_, dx in rmw:
            for ly, sy, spy, y, idy, dy in rmw:
                if spx != spy or not (sx < sy and lx < ly) or x // 4 == y // 4:
                    continue
                if abs(x - y) > PAIR_MAX_DIST or idy in dx or idx_ in dy:
                    continue
                gx, gy = ranges.find(spx, x), ranges.find(spy, y)
                if gx and gx == gy:
                    where = "in"
                elif not gx and not gy:
                    where = "out"
                else:
                    continue
                res.pairs.append((where, "hoisted" if ly < sx else "kept", x, y))
        block["loads"], block["stores"] = [], []

    pos = 0
    pending = None
    for line in lines:
        if tr.LABEL_RE.match(line):
            flush()
            continue
        m = tr.INSN_RE.match(line)
        if not m:
            continue
        pos += 1
        op, args = m.group(2), tr.split_args(m.group(3))
        delay_end, pending = pending, None
        if op in tr.LOADS or op in tr.STORES:
            store = op in tr.STORES
            rt, mem = args[0], args[1] if len(args) > 1 else ""
            target, kind, hreg = None, None, None
            lo = tr.LO_MEM_RE.match(mem)
            if lo:
                target = resolve(lo.group(1), lo.group(2))
                b = get(lo.group(3))
                hreg = lo.group(3)
                if b[0] == "hi":
                    kind = "direct" if b[1] == target else "sharedhi"
                elif b[0] == "hiidx":
                    kind = "idx"
                else:
                    kind = "lo?"
            else:
                im = tr.IMM_MEM_RE.match(mem)
                if im:
                    b = get(im.group(2))
                    if b[0] in ("addr", "idx") and b[1]:
                        target = (b[1][0], b[1][1] + tr.to_int(im.group(1)))
                        kind = "ptr" if b[0] == "addr" else "ptridx"
            if target and ranges.find(*target):
                res.accesses.append(Access(target[1], kind, store, op, pos, rt, hreg))
            if store:
                if target:
                    block["stores"].append((pos, target, get(rt)[2], kind, rt))
            elif op != "lwc2":
                nid[0] += 1
                if target:
                    block["loads"].append((pos, target, nid[0], kind, rt, hreg))
                regs[rt] = ("x", None, frozenset([nid[0]]))
        elif op == "lui":
            h = tr.HI_RE.match(args[1])
            regs[args[0]] = ("hi", resolve(h.group(1), h.group(2)), frozenset()) if h else none
        elif op == "addiu" and len(args) == 3 and tr.LO_RE.match(args[2]):
            lo = tr.LO_RE.match(args[2])
            s = get(args[1])
            t = resolve(lo.group(1), lo.group(2))
            if s[0] == "hi" and t:
                regs[args[0]] = ("addr", t, frozenset())
            elif s[0] == "hiidx" and t:
                regs[args[0]] = ("idx", t, frozenset())
            else:
                regs[args[0]] = none
        elif op == "addiu" and len(args) == 3:
            s = get(args[1])
            try:
                v = tr.to_int(args[2])
            except ValueError:
                v = None
            if s[0] in ("addr", "idx") and s[1] and v is not None:
                regs[args[0]] = (s[0], (s[1][0], s[1][1] + v), frozenset())
            else:
                regs[args[0]] = ("x", None, s[2])
        elif op in ("addu", "subu", "or", "move") and len(args) >= 2:
            a = get(args[1])
            b = get(args[2]) if len(args) > 2 else ("const", 0, frozenset())
            d = a[2] | b[2]
            v = ("x", None, d)
            if op == "addu":
                for x, y in ((a, b), (b, a)):
                    if x[0] == "hi" and y[0] not in ("hi", "addr", "idx", "hiidx"):
                        v = ("hiidx", x[1], d)
                        break
                    if x[0] in ("addr", "idx") and y[0] not in ("hi", "addr", "idx", "hiidx"):
                        v = ("idx", x[1], d)
                        break
                    if x[0] in ("hiidx",):
                        v = x
                        break
            elif op in ("or", "move"):
                if b[0] == "const" and b[1] == 0:
                    v = a
                elif a[0] == "const" and a[1] == 0:
                    v = b
            regs[args[0]] = v
        elif op in ("mult", "multu", "div", "divu") and len(args) == 2:
            regs["$hilo"] = ("x", None, get(args[0])[2] | get(args[1])[2])
        elif op in ("mflo", "mfhi"):
            regs[args[0]] = ("x", None, regs.get("$hilo", none)[2])
        elif op not in tr.NO_DEST and op not in tr.BRANCHES and args and args[0].startswith("$"):
            d = frozenset()
            for x in args[1:]:
                if x.startswith("$"):
                    d |= get(x)[2]
            regs[args[0]] = ("x", None, d)
        if op in ("jal", "jalr"):
            pending = "call"
        elif op in tr.BRANCHES:
            pending = "branch"
        if delay_end:
            flush()
            if delay_end == "call":
                for r in tr.CALLER_SAVED:
                    regs.pop(r, None)
    flush()
    return res


# ------------------------------------------------------------------------------------- tree

def load_ranges(root):
    aggs, _keep = mg.load_config(root)
    return aggs, Ranges((g.base, g.addr, g.end) for g in aggs)


def audit_tree(root="."):
    aggs, ranges = load_ranges(root)
    syms = tr.load_labels(tr.data_files(root))
    out = []
    for path in tr.function_files(root):
        rel = os.path.relpath(path, root).replace(os.sep, "/")
        m = tr.ASM_PATH_RE.match(rel)
        unit = m.group(1) if m and m.group(1) else "main"
        obj = "%s/%s" % (unit, os.path.basename(os.path.dirname(path)))
        name, lines = tr.parse_function_file(path)
        r = analyze_function(name, lines, unit, syms, ranges)
        r.unit, r.obj, r.matched = unit, obj, "/matchings/" in rel
        out.append(r)
    if not out:
        raise SystemExit("aggregate_audit: no function asm under %s/asm (run configure.py first)" % root)
    return aggs, out


def top_field(agg, addr):
    subs = mg.subobjects(agg.type, addr - agg.addr)
    if not subs:
        return "?"
    path = subs[0][0]
    return path.split(".")[1].split("[")[0]


def summarize(aggs, results):
    by_agg = {g.base: g for g in aggs}
    fields = defaultdict(lambda: {"kinds": Counter(), "objs": defaultdict(Counter), "funcs": set()})
    objs = defaultdict(Counter)
    pairs = Counter()
    hoisted = []
    bit = []
    for r in results:
        for a in r.accesses:
            g = next(x for x in aggs if x.addr <= a.addr < x.end)
            f = fields[(g.base, top_field(g, a.addr))]
            f["kinds"][a.kind] += 1
            f["objs"][r.obj][a.kind] += 1
            f["funcs"].add("%s:%s" % (r.unit, r.name))
            objs[r.obj][a.kind] += 1
        for where, how, x, y in r.pairs:
            pairs[(where, how)] += 1
            if how == "hoisted":
                hoisted.append((where, r.unit, r.name, r.matched, x, y))
        for a in sorted(set(r.bitfields)):
            bit.append((r.unit, r.name, r.matched, a))
    return by_agg, fields, objs, pairs, hoisted, bit


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--root", default=".")
    ap.add_argument("--fields", action="store_true")
    ap.add_argument("--units", action="store_true")
    ap.add_argument("--pairs", action="store_true")
    ap.add_argument("--bitfields", action="store_true")
    ap.add_argument("--json")
    a = ap.parse_args(argv)
    aggs, results = audit_tree(a.root)
    _by, fields, objs, pairs, hoisted, bit = summarize(aggs, results)
    total = Counter()
    for c in objs.values():
        total.update(c)
    print("accesses inside %s: %s" % (", ".join("%s %s" % (g.tname, g.base) for g in aggs),
                                      ", ".join("%s %d" % (k, total[k]) for k in KINDS)))
    rel_objs = [o for o, c in objs.items() if any(c[k] for k in BASE_RELATIVE)]
    print("objects with accesses: %d; with base-relative accesses: %d" % (len(objs), len(rel_objs)))
    print("read-modify-write pairs inside an aggregate: kept %d, hoisted %d; outside (baseline): "
          "kept %d, hoisted %d" % (pairs[("in", "kept")], pairs[("in", "hoisted")],
                                   pairs[("out", "kept")], pairs[("out", "hoisted")]))
    print("bit-field read-modify-write sites: %d" % len(bit))
    if a.fields:
        print("\nfield                     " + " ".join("%8s" % k for k in KINDS) + "  objs  base-rel objs")
        for (base, fld), f in sorted(fields.items(), key=lambda kv: (kv[0][0], kv[0][1])):
            nrel = sum(1 for c in f["objs"].values() if any(c[k] for k in BASE_RELATIVE))
            print("%-25s " % ("%s.%s" % (base, fld)) + " ".join("%8d" % f["kinds"][k] for k in KINDS)
                  + "  %4d  %4d" % (len(f["objs"]), nrel))
    if a.units:
        print("\nobject              " + " ".join("%8s" % k for k in KINDS))
        for o in sorted(objs):
            print("%-20s" % o + " ".join("%8d" % objs[o][k] for k in KINDS))
    if a.pairs:
        for where, unit, name, matched, x, y in hoisted:
            print("hoisted %-3s %s:%s%s  load %08X above store %08X" % (where, unit, name,
                                                                     " (matched)" if matched else "", y, x))
    if a.bitfields:
        for unit, name, matched, addr in bit:
            print("bit-field %s:%s%s  D_%08X" % (unit, name, " (matched)" if matched else "", addr))
    if a.json:
        data = {
            "fields": {"%s.%s" % k: {"kinds": dict(v["kinds"]),
                                     "objects": {o: dict(c) for o, c in v["objs"].items()},
                                     "functions": sorted(v["funcs"])} for k, v in fields.items()},
            "objects": {o: dict(c) for o, c in objs.items()},
            "pairs": {"%s/%s" % k: n for k, n in pairs.items()},
            "hoisted": [list(h) for h in hoisted],
            "bitfields": [list(b) for b in bit],
        }
        with open(a.json, "w") as f:
            json.dump(data, f, indent=1, sort_keys=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
