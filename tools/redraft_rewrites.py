#!/usr/bin/env python3
"""Type-driven rewrites of m2c drafts: the library behind tools/redraft.py (T-20800).

The types-first passes (T-16550 .. T-18800) each copied the last driver and added a few rewrites for
their overlay; this module is the union, read from the headers of the unit instead of constants:
`Env(unit)` parses the header closure (libgte.h, psx_types.h, main_api.h, game.h and, for an overlay,
include/ovl/<UNIT>.h) once and every rewrite takes the `Env` and the text. Nothing here compiles or
scores; tools/redraft.py does, and keeps whatever the compiler prefers.

Three stages, in the order the passes used them:
  fold        (asm, before m2c) a splat label inside a typed aggregate becomes `base + offset` so m2c prints
              fields: `%lo(D_80149974)` -> `%lo(D_80149968+0xC)`. The aggregates are the typed globals of the
              headers, GameState D_800E6280 and the record views (D_8011ECD0 as DisplayActor44[]). Modes:
              `bare` (every `%hi` takes the bare base, T-16550), `keep` (a `%hi` whose `%lo` uses all name one
              symbol, or that is indexed, keeps its offset, T-17000, T-17300), `named` (bare, but only labels at
              a named field, T-17100).
  PRE_STEPS   (m2c's raw function text) views, display-records, stack-table, stack-elements, fptab.
  POST_STEPS  (after the bulk_m2c cleanup) the rest; then field-symbols (tools/migrate_globals.py).
The names are the `rewrites` entries of a result.json. Every rewrite is a candidate: the call sites compare
scores. Unit-specific rewrites live in tools/redraft_extras.py and are listed in POST_STEPS too.
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import migrate_globals as mg  # noqa: E402

TYPE_HEADERS = ("include/libgte.h", "include/psx_types.h", "include/main_api.h", "include/game.h")
API = "include/main_api.h"
SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4}
ESIZE = dict(SIZES, **{"void (*": 4})
GAME_STATE = ("D_800E6280", 0x800E6280, 0x1A90)
# element counts of typed tables the headers declare without one (evidence: T-17000, T-16560, T-17600)
COUNTS_MAIN = {"D_800B0A04": 12, "D_801217D0": 12, "D_80125130": 48}
COUNTS_UNIT = {"TACO": {"D_80127480": 64, "D_80127080": 64, "D_80128880": 64}, "GYOZI": {"D_8012CA40": 54}}
TYPE_COUNTS = {"Tc14": 16, "TacoParticle14": 16}
# main-exe u8[] declarations that m2c should see as arrays of records: (type, record count). The display pool
# D_8011ECD0 is DisplayActor44[160] everywhere; EVENT and GYOZI have their own pools (T-17400, T-17600) and
# MASTER reads the pool and the sprites at D_80120650 in the fixed-point view DisplayFixActor44 (T-18600; the
# MasterSprite44 of that pass, folded into game.h by T-20900).
RECORD_VIEWS = {"*": {"D_8011ECD0": ("DisplayActor44", 160)},
                "NAME_ENT": {"D_80123120": ("NameRecordF00", 2)},
                "EVENT": {"D_800E9620": ("DisplayActor44", 160), "D_800EC190": ("Rec24", 0x43)},
                "GYOZI": {"D_80129F40": ("DisplayActor44", 160)},
                "MASTER": {"D_8011ECD0": ("DisplayFixActor44", 160), "D_80120650": ("DisplayFixActor44", 64)}}
# scalars that are whole objects (the headers declare a word, the unit reads the struct)
OBJ_VIEWS = {"NAME_ENT": {"D_800E7D10": ("NameMetricWork", 0xED8)}, "ETC": {"D_801230D0": ("EtcWork40", 0x40)}}
# extents that start inside a main-exe object (T-17600: GyoziWork's girl records, 11 x 0x38 at +0x1A8)
UNIT_RANGES = {"GYOZI": [(0x800F53A0 + 0x1A8, 0x800F53A0 + 0x1A8 + 11 * 0x38, "D_800F53A0")]}
UNIT_NO_AGG = {"GYOZI": {"D_800F53A0"}}      # its scalars keep their main_api.h names
# main-exe functions that every caller calls without arguments (T-17900); m2c would read stale registers
CONTEXT_PROTOS = {"NAME_ENT": ("void func_80054E6C(void);",)}
# T-20900: per-unit views folded into one shared type. An archived body (drafts/*) that names a retired type
# reads the shared one; the field renames apply where the old view had other names (as every rewrite, a
# candidate: the compiler and the score decide).
_FIX_HALVES = ((r"\.word\b", ".w"), (r"\.half\.high\b", ".h[1]"), (r"\.half\.low\b", ".h[0]"))
TYPE_ALIASES = {
    "DisplayRecord44": ("DisplayActor44", ()), "ShoSprite44": ("DisplayActor44", ()),
    "ShoCounter44": ("DisplayActor44", ()), "NameActorHead44": ("DisplayActor44", ()),
    "EventFlagRecord44": ("DisplayActor44", ()), "RpgObjectRecord44": ("DisplayActor44", ()),
    "RpgHealth44": ("DisplayActor44", ()), "RpgFlagRecord44": ("DisplayActor44", ()),
    "TaiValue44": ("DisplayActor44", ()), "GyoziRec44": ("DisplayActor44", ()),
    "GyoziInitSprite44": ("DisplayActor44", ()), "NameRecord44": ("DisplayActor44", ()),
    "OptActor44": ("DisplayActor44", ()), "OptSpr44": ("DisplayActor44", ()),
    "OptDraw44": ("DisplayActor44", ((r"\bunk_00\.b\[3\]", "unk_03"),)),
    "DateScene44": ("DisplayActor44", ((r"\.bytes\[7\]", ".unk_07"), (r"\.flags\b", ".unk_38"))),
    "NameAssetActor44": ("DisplayActor44", ((r"(->|\.)image\b", r"\1unk_34"),)),
    "TaiRecord44": ("DisplayActor44", ((r"(->|\.)y\b", r"\1unk_26"), (r"(->|\.)active\b", r"\1unk_02"),
                                       (r"(->|\.)flags\b", r"\1unk_03"))),
    "EtcRec44": ("DisplayActor44", ()), "Ev44": ("DisplayActor44", ()), "DateWords44": ("DisplayActor44", ()),
    "MasterSprite44": ("DisplayFixActor44", ((r"(->|\.)unk_26\b", r"\1unk_24.h[1]"),)),
    "KangeiSprite44": ("DisplayFixActor44", ((r"(->|\.)unk_26\b", r"\1unk_24.h[1]"),)),
    "DateMotion44": ("DisplayFixActor44", ()), "EventMotion44": ("DisplayFixActor44", ()),
    "DateParticleView44": ("DisplayFixActor44", ()), "EventParticleView44": ("DisplayFixActor44", ()),
    "DateMover44": ("DisplayFixActor44", ()), "TelRec44": ("DisplayFixActor44", ()),
    "TaiikuScene44": ("DisplayFixActor44", ((r"(->|\.)position\b", r"\1unk_24"),)),
    "NameMotionActor44": ("DisplayFixActor44", ((r"(->|\.)x\b", r"\1unk_24"), (r"(->|\.)y\b", r"\1unk_28"),
                                                (r"(->|\.)phase\b", r"\1unk_07"), (r"(->|\.)flags\b", r"\1unk_03"))
                          + _FIX_HALVES),
    "MasterFixed32": ("Fix32", ()), "KangeiFixed32": ("Fix32", ()), "DateFix32": ("Fix32", ()),
    "TaiikuPosition": ("Fix32", ()), "NameMotionWord": ("Fix32", _FIX_HALVES),
    "BunkaReturnFlags": ("Flags7D4F", ()),
    "BunkasaiModeFlags": ("Flags7D4F", ((r"->mode\b", "->unk_2_4"), (r"->pad\b", "->unk_5_7"))),
    "BunkakenModeFlags": ("Flags7D4F", ((r"->mode\b", "->unk_2_4"), (r"->pad\b", "->unk_5_7"))),
    "BunkSelectWords": ("BunkaDispatch8", ()), "BunkModePair": ("BunkaDispatch8", ()),
    "EtcMat": ("MATRIX", ()), "EtcMat2": ("MATRIX", ()), "TaiikuMat": ("MATRIX", ()),
    "EtcVec": ("VECTOR", ()), "EtcVec2": ("VECTOR", ()), "TaiikuVec": ("VECTOR", ()),
    # GameState words: +0x54C entries, +0x66C club words, +0xF4, +0xF8
    "TelEntryFlags": ("ScheduleEntryFlags", ((r"->nib\b", "->unk_25"), (r"->b29\b", "->active"))),
    "EntryFlags54C": ("ScheduleEntryFlags", ((r"->nib\b", "->unk_25"), (r"->b29\b", "->active"))),
    "EntryFlags29": ("ScheduleEntryFlags", ((r"->idx\b", "->unk_00"), (r"->nib\b", "->unk_25"),
                                            (r"->b29\b", "->active"))),
    "OptionScheduleBits": ("ScheduleEntryFlags", ((r"->f0\b", "->unk_00"), (r"->f25\b", "->unk_25"))),
    "KgSlot": ("ScheduleClubFlags", ((r"->grade\b", "->unk_25"),)),
    "EtcSlot": ("ScheduleClubFlags", ((r"->grade\b", "->unk_25"), (r"->flag\b", "->b30"))),
    "MasterClubBits": ("ScheduleClubFlags", ((r"->count\b", "->unk_25"),)),
    "TelClubFlags": ("ScheduleClubFlags", ()),
    "DateFlag17": ("ScheduleFlags", ()), "DateFlags0F4": ("ScheduleFlags", ()),
    "OptionPlayerBits": ("ScheduleFlags", ((r"->f8\b", "->unk_08"), (r"->f12\b", "->unk_12"))),
    "NameDate": ("GsDateBits", ((r"->day\b", "->@MONTH@"), (r"->month\b", "->day"), (r"->@MONTH@", "->month"),
                                (r"->week\b", "->unk_09"))),
}

DECL_RE = re.compile(r"^\s*extern\s+(?:volatile\s+)?(\w+)\s*(\*?)\s*(D_[0-9A-F]{8})\s*(\[(\w*)\])?\s*;"
                     r"(?:\s*/\*\s*(\d+) records)?", re.M)
RANGE_RE = re.compile(r"^\s*extern\s+(?:volatile\s+)?(\w+)\s*(\*?)\s*(D_([0-9A-F]{8}))\s*"
                      r"((?:\[(?:0x[0-9A-Fa-f]+|\d+)\])*)\s*;", re.M)
TABLE_RE = re.compile(r"typedef\s+struct\s+\w*\s*\{\s*(?:/\*[^*]*\*/\s*)*(\w+|void \(\*)\s*(\w+)((?:\[\w+\])+)\)?"
                      r"(?:\(\))?;\s*\}\s*(\w+)\s*;")
SYM_RE = re.compile(r"%(hi|lo)\(\s*(D_([0-9A-F]{8}))\s*(?:([+-])\s*(0x[0-9A-Fa-f]+|\d+)\s*)?\)")
LUI_RE = re.compile(r"\blui\s+(\$\w+),\s*%hi\(\s*(D_[0-9A-F]{8})")
LUI_LINE = re.compile(r"\blui\s+(\$\w+),\s*%hi\(")
INS_RE = re.compile(r"^\s*/\*[^*]*\*/\s+(\w+)\s+(.*?)\s*$")
OFF_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(?(&?)(D_[0-9A-F]{8})(?:\[([^\[\]]+)\])?\)? \+ \((\d+)\)\)")
OFF_PTR_RE = r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)%s \+ \((\d+)\)\)"
LOCAL_PTR_RE = re.compile(r"^\s*(\w+) (\*+)(\w+);\s*$", re.M)
SCALAR_SIZES = {"s16": 2, "u16": 2, "s32": 4, "u32": 4}
CAST = r"\*\((?:[^()]|\([^()]*\))*\)"


def type_aliases(text, types=None):
    """T-20900: rename the retired per-unit types of TYPE_ALIASES in an archived body or its declarations to
    the shared type (and the old field names to the shared ones). A type the text defines itself (a local
    typedef) or that the headers still have (`types`) is left alone."""
    for old, (new, fields) in TYPE_ALIASES.items():
        if not re.search(r"\b%s\b" % old, text) or (types and old in types):
            continue
        if re.search(r"\}\s*%s\s*;" % old, text):
            continue
        text = re.sub(r"\b%s\b" % old, new, text)
        for pat, rep in fields:
            text = re.sub(pat, rep, text)
    return text


# ------------------------------------------------------------------------------------------ layout


def leaf_at(t, rel, want_size):
    """(C path ".a.b[2]", leaf type) of the scalar of `want_size` bytes that starts at `rel` inside `t`."""
    for off, size, path, lt in mg.walk_layout(t):
        if lt.kind == "array":
            elem = lt.elem
            while elem.kind == "array":
                elem = elem.elem
            if off <= rel < off + size and (rel - off) % elem.size == 0 and elem.size == want_size:
                idx, rest = rel - off, []
                dims, e = [], lt
                while e.kind == "array":
                    dims.append(e.count)
                    e = e.elem
                stride = [elem.size] * len(dims)
                for i in range(len(dims) - 2, -1, -1):
                    stride[i] = stride[i + 1] * dims[i + 1]
                for s in stride:
                    rest.append(idx // s)
                    idx %= s
                return path + "".join("[%d]" % i for i in rest), elem
        elif off == rel and size == want_size and lt.kind in ("scalar", "ptr"):
            return path, lt
    return None, None


def path_leaf(t, path):
    """(offset, leaf type) of a C member path (`.unk_66C[0].unk_03`) inside struct type `t`, or None."""
    off = 0
    for name, idx in re.findall(r"\.(\w+)|\[(\d+)\]", path):
        if name:
            if t.kind not in ("struct", "union"):
                return None
            hit = [f for f in t.fields if f[0] == name]
            if not hit or hit[0][3]:              # unknown member or bit-field
                return None
            off += hit[0][1]
            t = hit[0][2]
        else:
            if t.kind != "array":
                return None
            off += int(idx) * t.elem.size
            t = t.elem
    return off, t


def named_offsets(t, count=1):
    """Offsets inside `count` records of `t` where a named (non-pad) scalar, pointer or array element starts."""
    out = set()
    for k in range(count):
        for off, size, path, lt in mg.walk_layout(t):
            if "pad" in path:
                continue
            base = k * t.size + off
            if lt.kind == "array":
                elem = lt.elem
                while elem.kind == "array":
                    elem = elem.elem
                out.update(range(base, base + size, elem.size))
            else:
                out.add(base)
    return out


class TypedAggregate(mg.Aggregate):
    """An aggregate built from a declaration instead of config/migrate_globals.txt."""

    def __init__(self, base, typ, types, header):
        self.base, self.tname, self.header = base, typ.spell(), header
        self.addr = int(base[2:], 16)
        self.types, self.type = types, typ
        self.end = self.addr + typ.size


# ------------------------------------------------------------------------------------------ environment

class Env:
    """What the rewrites know about one unit: types, typed globals, aggregate extents, tables."""

    def __init__(self, unit, root="."):
        self.unit, self.root = unit, root
        self.header = None if unit == "main" else "include/ovl/%s.h" % unit
        self.decl_files = [API] + ([self.header] if self.header else [])
        files = list(TYPE_HEADERS) + ([self.header] if self.header else [])
        self.text = {rel: self.read(rel) for rel in files}
        self.decl_text = "\n".join(self.text[rel] for rel in self.decl_files)
        all_text = "\n".join(self.text[rel] for rel in files)
        stripped = re.sub(r"\bstruct\s+(\w+)\s*\*", r"\1 *", mg.strip_comments_keep_offsets(all_text))
        # the layout parser does not read function-pointer arrays: model them as pointer arrays
        stripped = re.sub(r"void \(\*(\w+)\[(\d+)\]\)\(\);", r"void *\1[\2];", stripped)
        self.types = {k: v for k, v in mg.parse_types(stripped).items() if not isinstance(v, mg.LayoutError)}
        self.sizes = {n: int(v, 16) for n, v in re.findall(r"\}\s*(\w+)\s*;\s*/\*\s*size\s+(0x[0-9A-Fa-f]+)", all_text)}
        self.counts = dict(COUNTS_MAIN, **COUNTS_UNIT.get(unit, {}))
        self.flags = set()                       # "pass1": no stack-array typing (T-17300 typed1)
        views = dict(RECORD_VIEWS["*"], **RECORD_VIEWS.get(unit, {}))
        self.record_views = {s: v for s, v in views.items() if v[0] in self.types}
        self.obj_views = {s: v for s, v in OBJ_VIEWS.get(unit, {}).items() if v[0] in self.types}
        self.globals = self._typed_globals()
        self.records = self._records()
        self.ranges = self._ranges()
        self.named = self._named()
        self.tables, self.fn_tables = self._tables()
        self.ptrs, self.arrs, self.arr_dims, self.fn_arrs = self._scalar_globals()
        self.aggs = self._aggregates()
        self.decls = mg.declarations(self.decl_text)

    def read(self, rel):
        try:
            return mg.read(os.path.join(self.root, rel))
        except OSError:
            return ""

    def tsize(self, tname):
        """Size of a type name: the `/* size 0x.. */` comment, the parsed layout, or None."""
        if tname in self.sizes:
            return self.sizes[tname]
        if tname in self.types:
            return self.types[tname].size
        try:
            return mg.base_type(tname, self.types).size
        except (mg.LayoutError, KeyError):
            return None

    # -- typed globals ------------------------------------------------------------------------

    def _typed_globals(self):
        """SYM -> (struct Type, 'ptr' | 'array' | 'obj', count or None) for the headers' struct globals."""
        out = {}
        for rel in self.decl_files:
            for m in DECL_RE.finditer(self.text[rel]):
                tname, star, sym, arr, n, note = m.groups()
                t = self.types.get(tname)
                if t is None or t.kind != "struct":
                    continue
                count = int(n, 0) if n else int(note) if note else self.counts.get(sym) or TYPE_COUNTS.get(tname)
                out[sym] = (t, "ptr" if star else "array" if arr is not None else "obj", count)
        for sym, (tname, count) in self.record_views.items():
            out[sym] = (self.types[tname], "array", count)
        return out

    def _records(self):
        """SYM -> (type name, C expression of the record array) for the record rewrites."""
        out = {}
        for sym, (t, kind, _count) in self.globals.items():
            if kind == "array":
                out[sym] = (t.name, sym)
        for sym, (tname, _count) in self.record_views.items():
            out[sym] = (tname, "((%s *)%s)" % (tname, sym))
        return out

    def _ranges(self):
        """(start, end, base): the typed aggregates whose extent is known, GameState, the record and object views.
        Sorted innermost first (a label belongs to the smallest range around it, T-18600); a sized scalar array
        inside a larger range is dropped, so the aggregate keeps its fields."""
        out = []

        def add(start, end, base):
            if (start, end, base) not in out:
                out.append((start, end, base))
        gs = self.types.get("GameState")
        add(GAME_STATE[1], GAME_STATE[1] + (gs.size if gs is not None else GAME_STATE[2]), GAME_STATE[0])
        for sym, (tname, count) in self.record_views.items():
            add(int(sym[2:], 16), int(sym[2:], 16) + self.types[tname].size * count, sym)
        for sym, (tname, size) in self.obj_views.items():
            add(int(sym[2:], 16), int(sym[2:], 16) + size, sym)
        for sym, (t, kind, count) in self.globals.items():
            if kind == "ptr" or (kind == "array" and not count) or sym in self.record_views:
                continue
            add(int(sym[2:], 16), int(sym[2:], 16) + t.size * (count if kind == "array" else 1), sym)
        for start, end, base in UNIT_RANGES.get(self.unit, ()):
            add(start, end, base)
        # sized scalar arrays and scalar objects larger than a word
        for m in RANGE_RE.finditer(self.decl_text):
            tname, star, base, addr, dims = m.groups()
            if star or base in self.globals or base in self.record_views or base in self.obj_views:
                continue
            count = 1
            for d in re.findall(r"\[(\w+)\]", dims):
                count *= int(d, 0)
            size = self.tsize(tname)
            if size is None or not dims:
                continue
            start, end = int(addr, 16), int(addr, 16) + size * count
            if not any(a < end and start < b for a, b, _ in out):
                add(start, end, base)
        return sorted(out, key=lambda r: (r[1] - r[0], r[0]))

    def _named(self):
        """base -> offsets of named fields, for the `named` fold; None (fold everything) for GameState."""
        out = {}
        for sym, (t, kind, count) in self.globals.items():
            if kind != "ptr" and sym not in self.record_views:
                out[sym] = named_offsets(t, count if kind == "array" and count else 1)
        return out

    def _tables(self):
        """sym -> (type name, field, dims, element size) for globals of a one-array-field struct (stack
        tables); the set of table type names whose elements are function pointers."""
        field, fn = {}, set()
        for m in TABLE_RE.finditer(self.decl_text):
            if m.group(1) in ESIZE:
                field[m.group(4)] = (m.group(2), [int(d, 0) for d in re.findall(r"\[(\w+)\]", m.group(3))],
                                     ESIZE[m.group(1)])
                if m.group(1).startswith("void"):
                    fn.add(m.group(4))
        out = {}
        for m in DECL_RE.finditer(self.decl_text):
            tname, star, sym, arr = m.group(1), m.group(2), m.group(3), m.group(4)
            if tname in field and not star and not arr:
                out[sym] = (tname,) + field[tname]
        return out, fn

    def _scalar_globals(self):
        ptrs, arrs, dims, fns = {}, {}, [], []
        for m in re.finditer(r"^\s*extern\s+(u8|s8|u16|s16|u32|s32)\s*\*\s*(D_[0-9A-F]{8})\s*;", self.decl_text, re.M):
            ptrs[m.group(2)] = m.group(1)
        for m in re.finditer(r"^\s*extern\s+(u8|s8|u16|s16|u32|s32)\s+(D_[0-9A-F]{8})\s*\[\w+\]\s*;", self.decl_text, re.M):
            arrs[m.group(2)] = m.group(1)
        for m in re.finditer(r"^\s*extern\s+(u8|s8|u16|s16|u32|s32)\s+(D_([0-9A-F]{8}))\s*((?:\[\w+\])+)\s*;",
                             self.decl_text, re.M):
            dims.append((m.group(2), int(m.group(3), 16), SIZES[m.group(1)],
                         [int(d, 0) for d in re.findall(r"\[(\w+)\]", m.group(4))]))
        for m in re.finditer(r"^\s*extern\s+void\s*\(\*\s*(D_[0-9A-F]{8})\s*\[\w*\]\)\s*\(void\)\s*;", self.decl_text, re.M):
            fns.append(m.group(1))
        return ptrs, arrs, dims, fns

    def _aggregates(self):
        """mg aggregates for field-symbols: config/migrate_globals.txt plus the typed objects and sized arrays."""
        try:
            aggs, _keep = mg.load_config(self.root)
        except (OSError, SystemExit):
            aggs = []
        have = {a.base for a in aggs}
        for sym, (t, kind, count) in self.globals.items():
            if sym in have or kind == "ptr" or sym in self.record_views or sym in UNIT_NO_AGG.get(self.unit, ()):
                continue
            if kind == "array":
                if not count:
                    continue
                t = mg.Type("array", size=t.size * count, align=t.align, elem=t, count=count)
            aggs.append(TypedAggregate(sym, t, self.types, self.header or API))
        return aggs

    # -- the m2c context ----------------------------------------------------------------------

    def context(self, text):
        """The m2c context with the record views and object views declared as the draft reads them (only where
        the context declares the symbol the way main_api.h does: a different declaration is left alone)."""
        for sym, (tname, _count) in self.record_views.items():
            text, n = re.subn(r"\bextern\s+u8\s+%s\s*\[\s*\]\s*;" % sym, "", text)
            if n:
                text += "\nextern %s %s[];\n" % (tname, sym)
        for sym, (tname, _size) in self.obj_views.items():
            text, n = re.subn(r"\bextern\s+\w+\s+%s\s*;" % sym, "", text)
            if n:
                text += "\nextern %s %s;\n" % (tname, sym)
        for proto in CONTEXT_PROTOS.get(self.unit, ()):
            text += "\n%s\n" % proto
        return text


# ------------------------------------------------------------------------------------------ fold

def hi_exact(lines):
    """Line indexes of each `lui $r, %hi(X)` whose %lo uses (until $r is set again) all name X.

    m2c reads a lui %hi(sym+k) as the address sym+k for every %lo paired with it, also across a branch.
    With the hi folded to the bare base, a %lo in the next block printed field 0 (`D_800E6280.unk0`), so
    a lui whose %lo uses all name its own symbol keeps the full offset (T-17300)."""
    exact = set()
    for i, line in enumerate(lines):
        m = LUI_RE.search(line)
        if not m:
            continue
        reg, sym = m.group(1), m.group(2)
        r = re.escape(reg)
        uses = []
        for later in lines[i + 1:i + 40]:
            for u in re.finditer(r"%%lo\(\s*(D_[0-9A-F]{8})[^)]*\)\(%s\)|\$\w+,\s*%s,\s*%%lo\(\s*(D_[0-9A-F]{8})" % (r, r),
                                 later):
                uses.append(u.group(1) or u.group(2))
            if re.search(r"\b(?:lui|lw|lh|lhu|lb|lbu|addiu|addu|subu|or|sll|srl|sra|andi|ori|slt|slti|sltiu|sltu)\s+%s\s*," % r,
                         later) and "%lo" not in later:
                break
        if uses and all(u == sym for u in uses):
            exact.add(i)
    return exact


def indexed_his(lines):
    """Indexes of `lui r, %hi(sym)` lines whose register gets an index added (`addu r, r, x`) before
    its %lo use: m2c needs the whole address in such a %hi, or it drops the field offset (T-17000)."""
    out = set()
    for i, line in enumerate(lines):
        m = LUI_LINE.search(line)
        if not m:
            continue
        reg = m.group(1)
        for line2 in lines[i + 1:i + 24]:
            if re.match(r"\s*\.L\w+:", line2):
                break
            ins = INS_RE.match(line2)
            if not ins:
                continue
            ops = [o.strip() for o in ins.group(2).split(",")]
            if ins.group(1) == "addu" and ops[0] == reg and reg in ops[1:]:
                out.add(i)
                break
            if "%%lo(" in line2 and "(%s)" % reg in line2:
                break
            if ops and ops[0] == reg:
                break
    return out


FOLD_MODES = ("bare", "keep", "late", "named")


def fold(env, text, mode="bare"):
    """`text` (asm) with every label inside a typed aggregate written `base+offset`.

    bare   (T-16550, run before tools/m2c.py's fix_shared_hi) a `%hi` takes the bare base when its high half
           equals the base's, because m2c reads every `%lo` paired with a `%hi(sym+k)` as `sym+k`; a `%hi` that
           is indexed keeps its offset (T-17000: m2c would drop the field offset).
    keep   bare, plus the `%hi` whose `%lo` uses all name its own symbol keeps its offset (T-17300).
    late   (T-17700, run after fix_shared_hi) every `%hi` keeps its offset: fix_shared_hi has already written
           the `%lo` that share a `%hi` as displacements, and a bare base made m2c print `D_800E6280.unk0`.
    named  bare, but only labels that sit at a named field (T-17100)."""
    lines = text.split("\n")
    keep = indexed_his(lines) if mode in ("bare", "keep", "named") else set()
    if mode == "keep":
        keep |= hi_exact(lines)

    def one(line, kept):
        def rep(m):
            addr = int(m.group(3), 16)
            if m.group(4):
                addr += int(m.group(5), 0) * (-1 if m.group(4) == "-" else 1)
            for start, end, base in env.ranges:
                if start <= addr < end:
                    if m.group(2) == base:
                        return m.group(0)               # the innermost base itself (T-18600)
                    anchor = int(base[2:], 16)          # a range may start inside its base object (T-17600)
                    off = addr - anchor
                    if mode == "named" and base in env.named and off not in env.named[base]:
                        return m.group(0)
                    if m.group(1) == "hi" and mode != "late" and not kept and (anchor + 0x8000) >> 16 == (addr + 0x8000) >> 16:
                        off = 0
                    return "%%%s(%s+0x%X)" % (m.group(1), base, off) if off else "%%%s(%s)" % (m.group(1), base)
            return m.group(0)
        return SYM_RE.sub(rep, line)
    return "\n".join(one(l, i in keep) for i, l in enumerate(lines))


# ------------------------------------------------------------------------------------------ PRE: raw m2c text

def display_records(env, body):
    """m2c prints a record view's arithmetic in bytes; write records of the typed view."""
    for sym, (tname, _count) in env.record_views.items():
        size = env.types[tname].size
        view = "((%s *)%s)" % (tname, sym)
        body = re.sub(r"\b%s \+ \(([^()]+) \* 0x%X\)" % (sym, size), lambda m: "&%s[%s]" % (view, m.group(1)), body)
        # T-17900: m2c also prints the product first (`(i * 0xF00) + D_80123120`)
        body = re.sub(r"\(([^()]+) \* 0x%X\) \+ %s\b" % (size, sym), lambda m: "&%s[%s]" % (view, m.group(1)), body)
        # a byte offset computed into a temporary first (`temp_lo = i * 0x44; p = D_8011ECD0 + temp_lo;`)
        for name in re.findall(r"^\s+(\w+) = [^;]+ \* 0x%X;$" % size, body, re.M):
            body = re.sub(r"(?<!\(u8 \*\))\b%s \+ %s\b" % (sym, name),
                          lambda m, tname=tname, name=name: "(%s *)((u8 *)@@DISP@@ + %s)" % (tname, name), body)
            body = re.sub(r"&%s\[%s\]" % (sym, name),
                          lambda m, tname=tname, name=name: "(%s *)((u8 *)@@DISP@@ + %s)" % (tname, name), body)

        def disp(m, size=size):
            n = int(m.group(1), 0)
            return "@@REC%d@@" % (n // size) if n % size == 0 else "@@BYTE%d@@" % n
        body = re.sub(r"(?<!\*\)\()\b%s \+ (0x[0-9A-Fa-f]+|\d+)\b" % sym, disp, body)
        body = re.sub(r"(?<!\*)(?<!\*\))(?<!\*\)\()\b%s\b(?!\)\[)" % sym, lambda m: view, body)
        body = re.sub(r"@@REC(\d+)@@", lambda m: "&%s[%s]" % (view, m.group(1)), body)
        body = body.replace("@@DISP@@", sym)
        body = re.sub(r"@@BYTE(\d+)@@", lambda m: "(%s *)(%s + 0x%X)" % (tname, sym, int(m.group(1))), body)
    return body


def obj_views(env, body):
    """T-17900: m2c sees `D_800E7D10` as a NameMetricWork object; the unit declares an s32 (main_api.h), so
    the draft reads it as the matched code does, ((NameMetricWork *)&D_800E7D10)->field."""
    for sym, (tname, _size) in env.obj_views.items():
        view = "((%s *)&%s)" % (tname, sym)
        body = re.sub(r"(?<![\w.>&])%s\.(\w)" % sym, lambda m: view + "->" + m.group(1), body)
        body = re.sub(r"&%s\.(\w)" % sym, lambda m: "&" + view + "->" + m.group(1), body)
        body = re.sub(r"= &%s;" % sym, lambda m: "= %s;" % view, body)
    return body


def stack_tables(env, body):
    """Copies of header tables into stack locals become one struct assignment; element reads too.
    Returns (body, {spXX: (type, field, dims, element size)})."""
    locals_ = {}
    for sym, (tname, fld, dims, esize) in env.tables.items():
        lines = body.split("\n")
        hits = []
        for i, line in enumerate(lines):
            if re.search(r"\b%s\b" % sym, line) and ("=" in line or "M2C_MEMCPY" in line):
                m = re.search(r"\b(sp[0-9A-F]+)\b", line)
                if m:
                    hits.append((i, m.group(1)))
        if not hits:
            continue
        sp = hits[0][1]
        drop = {i for i, s in hits if s == sp}
        indent = re.match(r"\s*", lines[hits[0][0]]).group(0)
        out = []
        for i, line in enumerate(lines):
            if i == hits[0][0]:
                out.append("%s%s = %s;" % (indent, sp, sym))
            elif i not in drop:
                out.append(line)
        body = "\n".join(out)
        body, n = re.subn(r"^(\s+)(?:\?|\w+) \*?%s(?:\[\d+\])*;$" % sp, r"\g<1>%s %s;" % (tname, sp), body,
                          count=1, flags=re.M)
        if not n:  # m2c printed no declaration (the bulk cleanup would synthesize an s32 array)
            body = re.sub(r"^(.*\)\s*\{\n)", r"\g<1>    %s %s;\n" % (tname, sp), body, count=1)
        locals_[sp] = (tname, fld, dims, esize)
    for sp, (tname, fld, dims, esize) in locals_.items():
        base = int(sp[2:], 16)
        rows = [1] * len(dims)
        for i in range(len(dims) - 2, -1, -1):
            rows[i] = rows[i + 1] * dims[i + 1]
        row = rows[0] * esize
        total = row * dims[0]

        def addr_elem(expr, k, off, sp=sp, fld=fld, base=base, rows=rows, esize=esize, row=row, total=total):
            rel = off - base
            if not (0 <= rel < total) or k != row:
                return None
            first, r = divmod(rel, row)
            rest = []
            for s in rows[1:]:
                rest.append(r // (s * esize))
                r %= s * esize
            if r:
                return None
            head = "%s + %d" % (expr, first) if first else expr
            return "%s.%s[%s]%s" % (sp, fld, head, "".join("[%d]" % i for i in rest))

        def read(m, addr_elem=addr_elem):
            got = addr_elem(m.group(1), int(m.group(2), 0), int(m.group(3), 0))
            return got if got else m.group(0)
        # T-18300: a scaled index kept in its own temporary (`temp_s2 = i * 2;` ... `(sp + temp_s2)`)
        scaled = {m.group(1): (m.group(2), m.group(3))
                  for m in re.finditer(r"^\s+(\w+) = (\w+) \* (\d+);$", body, re.M)}

        def unscale(m, scaled=scaled):
            got = scaled.get(m.group(1))
            return "(sp + (%s * %s))" % got if got else m.group(0)
        body = re.sub(r"\(sp \+ (\w+)\)", unscale, body)
        body = re.sub(CAST + r"\(\(unsigned char \*\)\(sp \+ \(([^()]+) \* (\d+)\)\) \+ \((\d+)\)\)", read, body)
        if len(dims) == 2 and esize == 1:
            body = re.sub(r"\*\(&%s \+ \(([^()]+) \* (0x[0-9A-Fa-f]+|\d+)\) \+ ([^()]+?)\)" % sp,
                          lambda m, sp=sp, fld=fld, dims=dims: "%s.%s[%s][%s]" % (sp, fld, m.group(1), m.group(3))
                          if int(m.group(2), 0) == dims[1] else m.group(0), body)
        body = re.sub(r"\(&%s(?:\[0\])?\)\[" % sp, lambda m, sp=sp, fld=fld: "%s.%s[" % (sp, fld), body)
        # T-17500: m2c's array views of the copied struct (`&sp40[0]`, `((s16 *)sp30)[i]`)
        body = re.sub(r"\(\(\w+ \*\)%s\)\[" % sp, lambda m, sp=sp, fld=fld: "%s.%s[" % (sp, fld), body)
        body = re.sub(r"&%s\[0\]" % sp, lambda m, sp=sp, fld=fld: "%s.%s" % (sp, fld), body)
        body = re.sub(r"(?<![.\w&])%s\[" % sp, lambda m, sp=sp, fld=fld: "%s.%s[" % (sp, fld), body)
        body = re.sub(r"&%s\b(?![.\[])" % sp, lambda m, sp=sp, fld=fld: "%s.%s" % (sp, fld), body)
    return body, locals_


def stack_elements(env, body, locals_):
    """T-17700: m2c names single elements of a copied table as their own stack locals (`s16 sp1A;` inside
    `GekoMonthDays sp18`) and reaches a neighbour of an indexed element through a byte offset
    (`*(s16 *)((unsigned char *)(&sp18.day[i]) + (-2))`); both are elements of the table. T-18800: a row of
    a copied 2-D table named as its own local (`&sp65` inside `Date2Labels4x21 sp50` is `sp50.v[1]`)."""
    for sp, (tname, fld, dims, esize) in locals_.items():
        if len(dims) == 2:
            row = dims[1] * esize
            for m in list(re.finditer(r"^\s+(?:\w+|\?) (sp([0-9A-F]+))(?:\[\d*\])?;\n", body, re.M)):
                off = int(m.group(2), 16) - int(sp[2:], 16)
                if m.group(1) == sp or not (0 < off < dims[0] * row) or off % row:
                    continue
                body = body.replace(m.group(0), "", 1)
                body = re.sub(r"\(&%s\)\[" % m.group(1), lambda k, sp=sp, fld=fld, off=off, row=row: "%s.%s[%d][" % (
                    sp, fld, off // row), body)
                body = re.sub(r"&%s\b(?!\[)" % m.group(1), lambda k, sp=sp, fld=fld, off=off, row=row: "%s.%s[%d]" % (
                    sp, fld, off // row), body)
            continue
        if len(dims) != 1:
            continue
        base, total = int(sp[2:], 16), dims[0] * esize
        for m in list(re.finditer(r"^\s+\w+ (sp([0-9A-F]+));\n", body, re.M)):
            off = int(m.group(2), 16) - base
            if m.group(1) == sp or not (0 < off < total) or off % esize:
                continue
            body = body.replace(m.group(0), "", 1)
            body = re.sub(r"(?<![.\w&])%s\b" % m.group(1), lambda k, sp=sp, fld=fld, off=off, esize=esize: "%s.%s[%d]" % (
                sp, fld, off // esize), body)

        def shift(m, sp=sp, fld=fld, esize=esize):
            k = int(m.group(3))
            if SIZES[m.group(1)] != esize or k % esize:
                return m.group(0)
            n = k // esize
            return "%s.%s[%s %s %d]" % (sp, fld, m.group(2), "-" if n < 0 else "+", abs(n))
        body = re.sub(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(&%s\.%s\[([^\[\]]+)\]\) \+ \((-?\d+)\)\)"
                      % (sp, fld), shift, body)
        # the table's own name read as a value is its first element (`var_s0 = sp4C;`)
        lines = body.split("\n")
        for i, line in enumerate(lines):
            if re.match(r"\s+(?:\w+ )?%s(?: = D_\w+)?;$" % sp, line):
                continue
            lines[i] = re.sub(r"(?<![.\w&])%s\b(?![.\[\w])" % sp, lambda m, sp=sp, fld=fld: "%s.%s[0]" % (sp, fld), line)
        body = "\n".join(lines)
    return body


def fptab(env, body, locals_):
    """T-3330: one scalar declared above the step table; the call indexes it through `idx`."""
    for sp, (tname, fld, dims, esize) in locals_.items():
        if tname not in env.fn_tables and not tname.startswith(("FnTbl", "DateDispatch")):
            continue
        call = re.compile(r"^(\s+)\(?%s\.%s\[([^\[\]]+)\]\)?\((.*)\);$" % (sp, fld), re.M)
        if not call.search(body) or re.search(r"\bidx\b", body):
            continue
        body = call.sub(lambda m, sp=sp, fld=fld: "%sidx = %s;\n%s%s.%s[idx](%s);" % (
            m.group(1), m.group(2), m.group(1), sp, fld, m.group(3)), body)
        body = re.sub(r"^(\s+)%s %s;$" % (tname, sp), lambda m, tname=tname, sp=sp: (
            "%ss32 idx; /* declared before the table: T-3330 */\n%s%s %s;" % (m.group(1), m.group(1), tname, sp)),
            body, count=1, flags=re.M)
    return body


def pre_steps(env, body, log=None):
    """The PRE rewrites on m2c's raw function text, in order; `log` collects the names that changed it."""
    def note(name, new, old):
        if new != old and log is not None:
            log.append(name)
        return new
    body = note("views", obj_views(env, body), body)
    body = note("display-records", display_records(env, body), body)
    new, locals_ = stack_tables(env, body)
    body = note("stack-table", new, body)
    body = note("stack-elements", stack_elements(env, body, locals_), body)
    body = note("fptab", fptab(env, body, locals_), body)
    return body


# ------------------------------------------------------------------------------------------ POST: cleaned body

def display_offsets(env, body):
    """`*(u8 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (6535))` is record 96's field; the same for
    every record array (T-17900: also without the view cast, the form m2c prints for constant offsets)."""
    for sym, (tname, view) in env.records.items():
        t = env.types.get(tname)
        if t is None or sym not in body:
            continue
        pat = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)(?:%s|%s) \+ \((\d+)\)\)"
                         % (re.escape(view), re.escape(sym)))

        def repl(m, t=t, view=view):
            ctype, n = m.group(1), int(m.group(2))
            k, r = divmod(n, t.size)
            path, lt = leaf_at(t, r, SIZES[ctype])
            if path is None:
                return m.group(0)
            expr = "%s[%d]%s" % (view, k, path)
            store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
            if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
                return expr
            return "*(%s *)&%s" % (ctype, expr)
        body = pat.sub(repl, body)
    return body


def record_temps(env, body):
    """A local set to `&SYM[i]` (or `SYM + lo` with `lo = i * size`, or the array itself) takes the record
    pointer type and its byte offsets become `p[k].field`; the display and record views too."""
    body = display_offsets(env, body)
    for sym, (tname, view) in env.records.items():
        t = env.types.get(tname)
        if t is None or sym not in body:
            continue
        # p = D_801217D0 + temp_lo; with temp_lo = i * 0x24
        for var, lo in re.findall(r"^\s+(\w+) = %s \+ (\w+);$" % sym, body, re.M):
            m = re.search(r"^\s+%s = (.+) \* 0x%X;$" % (lo, t.size), body, re.M)
            use = re.search(r"^\s+%s = %s \+ %s;$" % (var, sym, lo), body, re.M)
            # the index must not change between the product and its use (`lo = i * 0x24; i += 1;`)
            if m and use and re.fullmatch(r"\w+", m.group(1)) and re.search(
                    r"\b%s\s*(?:[-+*/|&]?=(?!=)|\+\+|--)" % m.group(1), body[m.end():use.start()]):
                body = re.sub(r"^(\s+)%s = %s \+ %s;$" % (var, sym, lo),
                              lambda k, var=var, tname=tname, sym=sym, lo=lo: "%s%s = (%s *)((u8 *)%s + %s);" % (
                                  k.group(1), var, tname, sym, lo), body, flags=re.M)
                continue
            if m:
                body = re.sub(r"^(\s+)%s = %s \+ %s;$" % (var, sym, lo),
                              lambda k, var=var, view=view, idx=m.group(1): "%s%s = &%s[%s];" % (k.group(1), var, view, idx),
                              body, flags=re.M)
        byte_form = "(%s *)((u8 *)%s + " % (tname, sym)
        ptrs = set(re.findall(r"^\s+(\w+) = &%s\[" % re.escape(view), body, re.M))
        ptrs |= set(re.findall(r"^\s+(\w+) = %s" % re.escape(byte_form), body, re.M))
        # display-base (T-17300, T-18200): a walk that starts at the first record (`var_s0 = D_8011ECD0;`)
        ptrs |= set(re.findall(r"^\s+(\w+) = (?:%s|%s);$" % (re.escape(view), re.escape(sym)), body, re.M))
        for var in ptrs:
            assigns = re.findall(r"^\s+%s = (.+);$" % var, body, re.M)
            if any(not (a.startswith("&" + view + "[") or a.startswith(byte_form) or a in (view, sym)) for a in assigns):
                continue
            if sym in env.record_views:
                body = re.sub(r"^(\s+)%s = %s;$" % (var, sym), lambda k, var=var, view=view: "%s%s = %s;" % (
                    k.group(1), var, view), body, flags=re.M)

            def repl(m, var=var, t=t):
                ctype, n = m.group(1), int(m.group(2))
                k, r = divmod(n, t.size)
                path, lt = leaf_at(t, r, SIZES[ctype])
                store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
                if path is None:
                    return "*(%s *)((u8 *)&%s[%d] + %d)" % (ctype, var, k, r) if k else m.group(0)
                expr = "%s[%d]%s" % (var, k, path) if k else "%s->%s" % (var, path[1:])
                if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
                    return expr
                return "*(%s *)&%s" % (ctype, expr)
            new = re.sub(OFF_PTR_RE % re.escape(var), repl, body)
            if new != body:
                body = re.sub(r"^(\s+)(?:\?|\w+) \*?%s;$" % var, lambda k, var=var, tname=tname: "%s%s *%s;" % (
                    k.group(1), tname, var), new, count=1, flags=re.M)
    return body


REC_RE = re.compile(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)(\(\(\w+ \*\)D_[0-9A-F]{8}\)|\w+) \+ \((\d+)\)\)")


def record_offsets(env, body):
    """T-17100: `*(T *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (N))` and the same on a local `S *p`
    of a struct type: the element and field (m2c and the cleanup print byte offsets past the first element)."""
    ptrs = {name: env.types[t] for t, stars, name in LOCAL_PTR_RE.findall(body)
            if stars == "*" and t in env.types and env.types[t].kind == "struct"}
    views = {"((%s *)%s)" % (tname, sym): env.types[tname] for sym, (tname, _c) in env.record_views.items()}

    def repl(m):
        ctype, base, n = m.group(1), m.group(2), int(m.group(3))
        if base in views:
            t, arr = views[base], True
        elif base in ptrs:
            t, arr = ptrs[base], False
        else:
            return m.group(0)
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        if path is None or "pad" in path:
            return m.group(0)
        expr = ("%s[%d]" % (base, k) + path) if (arr or k) else base + "->" + path[1:]
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr
        return "*(%s *)&%s" % (ctype, expr)
    return REC_RE.sub(repl, body)


def actor_offsets(env, body):
    """`*(T *)((unsigned char *)SYM + (N))` and `SYM + N` on a typed global table or pointer: written as
    the element and field, `D_8015EDB4[8].unk2` (m2c prints byte offsets past the first element)."""
    def elem(sym, idx, k):
        if idx is None:
            return "%s[%d]" % (sym, k)
        return "%s[%s + %d]" % (sym, idx, k) if k else "%s[%s]" % (sym, idx)

    def repl(m):
        ctype, amp, sym, idx, n = m.group(1), m.group(2), m.group(3), m.group(4), int(m.group(5))
        if sym not in env.globals:
            return m.group(0)
        t, kind, _count = env.globals[sym]
        if idx is not None and not amp:
            return m.group(0)
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        if path is None:
            return m.group(0)
        if idx is not None:                       # &SYM[i] + n
            expr = elem(sym, idx, k) + path
        elif amp and kind == "obj":               # &SYM + n
            if k:
                return m.group(0)
            expr = sym + path
        elif amp and kind == "ptr":               # address of the pointer itself
            return m.group(0)
        else:                                     # SYM + n (pointer value or array)
            expr = elem(sym, None, k) + path
        if kind == "ptr" and k == 0 and idx is None:
            expr = sym + "->" + path[1:]
        store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
        if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
            return expr                           # a store does not depend on the signedness
        return "*(%s *)&%s" % (ctype, expr)
    body = OFF_RE.sub(repl, body)

    def plus(m):
        sym, n = m.group(1), int(m.group(2), 0)
        if sym not in env.globals or env.globals[sym][1] == "obj":
            return m.group(0)
        t = env.globals[sym][0]
        if n % t.size:
            return m.group(0)
        return "&%s[%d]" % (sym, n // t.size)
    return re.sub(r"(?<![\w&])(D_[0-9A-F]{8}) \+ (0x[0-9A-Fa-f]+|\d+)\b(?!\s*[\[(])", plus, body)


def pointer_steps(env, body):
    """m2c prints pointer steps in bytes (`Tc14 *p; p += 0x14`, `s16 **q; q += 4`); C counts elements."""
    for tname, stars, name in LOCAL_PTR_RE.findall(body):
        if len(stars) > 1:
            size = 4
        elif tname in SCALAR_SIZES:
            size = SCALAR_SIZES[tname]
        else:
            t = env.types.get(tname)
            size = t.size if t is not None and t.kind == "struct" else 0
        if size <= 1:
            continue

        def step(m, size=size):
            n = int(m.group(3), 0)
            if n % size:
                return m.group(0)
            return "%s %s %d" % (m.group(1), m.group(2), n // size)
        body = re.sub(r"\b(%s) ([-+]=) (0x[0-9A-Fa-f]+|\d+)\b" % re.escape(name), step, body)
    return body


def struct_walks(env, body):
    """T-17000: m2c types a byte walk over a struct (`GameState *p; ... p += 4;`) with the struct's pointer, so
    the step counts whole structs. Such a local becomes `u8 *` and its member reads byte offsets."""
    for tname, name in re.findall(r"^\s*(\w+) \*(\w+);\s*$", body, re.M):
        t = env.types.get(tname)
        if t is None or t.kind != "struct" or t.size <= 1:
            continue
        steps = [int(n, 0) for n in re.findall(r"\b%s [-+]= (0x[0-9A-Fa-f]+|\d+);" % re.escape(name), body)]
        if not steps or all(n % t.size == 0 for n in steps) or re.search(r"\b%s\[" % re.escape(name), body):
            continue
        bad = []

        def member(m, t=t, name=name):
            got = path_leaf(t, "." + m.group(1))
            if got is None or got[1].kind not in ("scalar", "ptr"):
                bad.append(m.group(0))
                return m.group(0)
            ctype = got[1].name if got[1].kind == "scalar" else "s32"
            return "*(%s *)(%s + 0x%X)" % (ctype, name, got[0])
        new = re.sub(r"\b%s->(\w+(?:\[\d+\]|\.\w+)*)" % re.escape(name), member, body)
        if bad:
            continue
        new = re.sub(r"^(\s*)%s \*%s;" % (re.escape(tname), re.escape(name)), lambda m, name=name: "%su8 *%s;" % (
            m.group(1), name), new, count=1, flags=re.M)
        new = re.sub(r"\b(%s\s*(?:=|!=|==|<|>|<=|>=)\s*)(&?D_[0-9A-F]{8}\b)" % re.escape(name),
                     lambda m: m.group(1) + "(u8 *)" + m.group(2), new)
        body = new
    return body


PRIMS = {0x0C: "DR_MODE", 0x10: "TILE", 0x14: "SPRT", 0x18: "POLY_F4", 0x1C: "POLY_G3", 0x20: "POLY_FT3",
         0x24: "POLY_G4", 0x28: "POLY_FT4", 0x34: "POLY_GT4"}


def prim_field(t, var, deref, m):
    ctype, n = m.group(1), int(m.group(2))
    k, r = divmod(n, t.size)
    path, lt = leaf_at(t, r, SIZES[ctype])
    if path is None:
        return m.group(0)
    base = "(*%s)" % var if deref else var
    expr = "%s[%d]%s" % (base, k, path) if k else "%s->%s" % (base, path[1:])
    store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
    if lt.name == ctype or (store and lt.kind == "scalar" and lt.size == SIZES[ctype]):
        return expr
    return "*(%s *)&%s" % (ctype, expr)


def work_prims(env, body):
    """T-17300: `p = GetWorkBase(0x28, D_8011ECA0)` hands out a POLY_FT4; offsets on p become fields
    (T-17600: GYOZI's func_80054B60(size, ...) the same way)."""
    for var, alloc, size in re.findall(r"^\s+(\w+) = (?:\([^()]*\))?(GetWorkBase|func_80054B60)\((0x[0-9A-Fa-f]+|\d+),",
                                       body, re.M):
        tname = PRIMS.get(int(size, 0))
        t = env.types.get(tname)
        if t is None:
            continue
        assigns = re.findall(r"^\s+%s = (.+);$" % var, body, re.M)
        if any(alloc + "(" not in a for a in assigns):
            continue
        body = re.sub(r"^(\s+)%s = (?:\([^()]*\))?%s\(" % (var, alloc),
                      lambda m, var=var, tname=tname, alloc=alloc: "%s%s = (%s *)%s(" % (m.group(1), var, tname, alloc),
                      body, flags=re.M)
        body = re.sub(r"^(\s+)(?:\?|\w+) \*?%s;$" % var, lambda m, var=var, tname=tname: "%s%s *%s;" % (
            m.group(1), tname, var), body, count=1, flags=re.M)
        body = re.sub(OFF_PTR_RE % re.escape(var), lambda m, var=var, t=t: prim_field(t, var, False, m), body)
        # a pointer-to-pointer local that stores it: `*var_s0 = temp_v0;`, then `(*var_s0) + k`
        for pp in set(re.findall(r"^\s+\*(\w+) = %s;$" % var, body, re.M)):
            body = re.sub(OFF_PTR_RE % (r"\(\*%s\)" % pp), lambda m, pp=pp, t=t: prim_field(t, pp, True, m), body)
            body = re.sub(r"^(\s+)(?:\?|\w+) \*+%s;$" % pp, lambda m, pp=pp, tname=tname: "%s%s **%s;" % (
                m.group(1), tname, pp), body, count=1, flags=re.M)
            # the stack array it walks (`var_s0 = &sp6C;`) holds the same pointers
            sps = set() if "pass1" in env.flags else set(
                re.findall(r"^\s+%s = &(sp[0-9A-F]+)(?:\[0\])?;$" % pp, body, re.M))
            for sp in sps:
                body = re.sub(r"^(\s+)(?:\?|\w+) \*?%s((?:\[\d+\])?);$" % sp,
                              r"\g<1>%s *%s\g<2>;" % (tname, sp), body, count=1, flags=re.M)
    return body


def header_arrays(env, body):
    """(T-18500) bodies written against scalar declarations of a header array (the census) index it
    by hand: `*(&D_80155F0C + (i * 4))`, `*(&D_80155F00 + i)`, `((void (**)(void)) &D_80155ED0)[i]()`.
    With `s32 D_80155F0C[12]` in the header these are `D_80155F0C[i]`."""
    simple = r"([\w.\[\]]+(?:->\w+)?)"
    for sym, ctype in env.arrs.items():
        if sym not in body:
            continue
        size = SIZES[ctype]
        if size == 1:
            body = re.sub(r"\*\(&%s \+ %s\)" % (sym, simple), lambda m, sym=sym: "%s[%s]" % (sym, m.group(1)), body)
        else:
            body = re.sub(r"\*\(&%s \+ \(%s \* %d\)\)" % (sym, simple, size),
                          lambda m, sym=sym: "%s[%s]" % (sym, m.group(1)), body)
        body = re.sub(r"\*\(&%s\)" % sym, lambda m, sym=sym: "%s[0]" % sym, body)
    for sym in env.fn_arrs:
        body = re.sub(r"\(\(void \(\*\*\)\(void\)\) ?&%s\)\[" % sym, lambda m, sym=sym: "%s[" % sym, body)
    return body


def array_symbols(env, body):
    """(T-18500) a splat label inside a header array of scalars (`D_801562A4` in `s32 D_801562A0[3][13]`)
    is that element (`D_801562A0[0][1]`); m2c prints the label when it reads a constant index."""
    if not env.arr_dims:
        return body

    def repl(m):
        addr = int(m.group(1), 16)
        for base, start, size, dims in env.arr_dims:
            total = size
            for d in dims:
                total *= d
            rel = addr - start
            if 0 < rel < total and rel % size == 0:
                k, idx = rel // size, []
                for d in reversed(dims):
                    idx.append(k % d)
                    k //= d
                return base + "".join("[%d]" % i for i in reversed(idx))
        return m.group(0)
    body = re.sub(r"\bD_([0-9A-F]{8})\b(?!\[)", repl, body)
    for base, start, size, dims in env.arr_dims:  # the base itself read as a scalar
        if len(dims) > 1:
            body = re.sub(r"\b%s\b(?!\[)" % base, lambda m, base=base, dims=dims: base + "[0]" * len(dims), body)
    return body


def array_views(env, body):
    """(T-18800) m2c's views of a scalar header array (`s16 D_8013A5E4[3]`): `(&D_X)[i]` is `D_X[i]`,
    and a byte offset on `&D_X` of the element size is that element."""
    for sym, ctype in env.arrs.items():
        if sym not in body:
            continue
        size = SIZES[ctype]
        body = re.sub(r"\(&%s\)\[" % sym, lambda m, sym=sym: "%s[" % sym, body)

        def off(m, sym=sym, size=size, ctype=ctype):
            n = int(m.group(2))
            if SIZES[m.group(1)] != size or n % size:
                return m.group(0)
            store = re.match(r"\s*=(?!=)", m.string[m.end():]) is not None
            if m.group(1) != ctype and not store:
                return "*(%s *)&%s[%d]" % (m.group(1), sym, n // size)
            return "%s[%d]" % (sym, n // size)
        body = re.sub(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(&%s\) \+ \((\d+)\)\)" % sym, off, body)

        def shift(m, sym=sym, size=size):
            n = int(m.group(3))
            if SIZES[m.group(1)] != size or n % size:
                return m.group(0)
            return "%s[%d]" % (sym, int(m.group(2)) + n // size)
        body = re.sub(r"\*\((u8|s8|u16|s16|u32|s32) \*\)\(\(unsigned char \*\)\(&%s\[(\d+)\]\) \+ \((-?\d+)\)\)" % sym,
                      shift, body)
        # `t = i * 2; p = t + &D_X;` is `p = &D_X[i]`
        for var, idx in re.findall(r"^\s+(\w+) = (\w+) \* %d;$" % size, body, re.M):
            body = re.sub(r"\b%s \+ &%s\b" % (var, sym), lambda m, sym=sym, idx=idx: "&%s[%s]" % (sym, idx), body)
        # m2c's first element (`D_X->unk0`, after the cleanup `(*(s32 *)D_X)`)
        body = re.sub(r"\b%s->unk0\b" % sym, lambda m, sym=sym: "%s[0]" % sym, body)
        body = re.sub(r"\(\*\((?:u8|s8|u16|s16|u32|s32) \*\)%s\)" % sym, lambda m, sym=sym: "%s[0]" % sym, body)
        # a census body reads the array as a scalar (declared so): its first element, only where a
        # scalar is meant (a store, a scalar cast, a compare), not where the array is a pointer
        body = re.sub(r"^(\s+)%s( [-+*/|&^]?= )" % sym, lambda m, sym=sym: "%s%s[0]%s" % (m.group(1), sym, m.group(2)),
                      body, flags=re.M)
        body = re.sub(r"(\((?:u8|s8|u16|s16|u32|s32)\) )%s\b(?!\s*\[)" % sym,
                      lambda m, sym=sym: "%s%s[0]" % (m.group(1), sym), body)
        body = re.sub(r"(?<![&\w*.>])%s( (?:==|!=|<=|>=|<|>) )" % sym,
                      lambda m, sym=sym: "%s[0]%s" % (sym, m.group(1)), body)
    return body


def pointer_index(env, body):
    """(T-18100) `*(s32 *)((unsigned char *)D_8013C730 + (8))`, `(*(s32 *)D_8013C730)` and
    `D_8013C730->unk0` on a header global `s32 *D_8013C730`: `D_8013C730[2]`, `D_8013C730[0]`.
    m2c keeps the byte offsets although the context gives the pointee type."""
    for sym, ctype in env.ptrs.items():
        if sym not in body:
            continue
        size = SIZES[ctype]

        def repl(m, sym=sym, size=size):
            n = int(m.group(1))
            return "%s[%d]" % (sym, n // size) if n % size == 0 else m.group(0)
        body = re.sub(r"\*\(%s \*\)\(\(unsigned char \*\)%s \+ \((\d+)\)\)" % (ctype, sym), repl, body)
        body = body.replace("(*(%s *)%s)" % (ctype, sym), "%s[0]" % sym)
        body = re.sub(r"\b%s->unk0\b" % sym, lambda m, sym=sym: "%s[0]" % sym, body)
    return body


def struct_bytes(env, body):
    """T-18300: m2c prints a byte offset from GameState as `&D_800E6280 + i`, which C scales by the struct size;
    the byte pointer is `(u8 *)&D_800E6280 + i`."""
    return re.sub(r"(?<![)\w])&D_800E6280 \+ ", "(u8 *)&D_800E6280 + ", body)


REC38_RE = re.compile(r"\*\(&D_800E6280\.unk_1BC\[0\]\.(unk_\w+) \+ \(([^()]+) \* 0x38\)\)")


def gamestate_records(env, body):
    """T-17700: `*(&D_800E6280.unk_1BC[0].unk_0A + (i * 0x38))` is record i of GameState's Rec38 array (the
    Rec38Flags union at +0x0C is read through its byte view)."""
    return REC38_RE.sub(lambda m: "D_800E6280.unk_1BC[%s].%s%s" % (m.group(2), m.group(1),
                                                                   ".b[0]" if m.group(1) == "unk_0C" else ""), body)


GS_WORD_RE = re.compile(r"(?:\(s32\) )?\bD_800E6280\.(unk_(?:0FC|100|104|108|10C|110|114|118|11C|120))\b(?!\.)")


def gamestate_words(env, body):
    """T-18400: the GsRec0FC records at GameState +0xFC..+0x120 (two s16) are also read and written as one
    16.16 word (`+= 0x18000`); m2c prints `D_800E6280.unk_100 = (s32) D_800E6280.unk_100 + 0x18000`, which
    IDO rejects. Write the word view `*(s32 *)&D_800E6280.unk_100`."""
    return GS_WORD_RE.sub(lambda m: "*(s32 *)&D_800E6280.%s" % m.group(1), body)


RAW_RE = re.compile(r"\*\((void|u8|s8|u16|s16|u32|s32) \*\)0x(8011[EF][0-9A-F]{3}|8012[01][0-9A-F]{3})\b"
                    r"(\s*=\s*\((u8|s8|u16|s16|u32|s32)\))?")


def raw_display(env, body):
    """T-17800: `*(void *)0x80120653 = (u8) ...` (a folded label m2c printed as a number) is a record field."""
    if "D_8011ECD0" not in env.record_views:
        return body
    tname, count = env.record_views["D_8011ECD0"]
    t = env.types[tname]
    start = 0x8011ECD0

    def repl(m):
        ctype = m.group(1) if m.group(1) != "void" else m.group(4)
        n = int(m.group(2), 16) - start
        if ctype is None or not (0 <= n < t.size * count):
            return m.group(0)
        k, r = divmod(n, t.size)
        path, lt = leaf_at(t, r, SIZES[ctype])
        tail = m.group(3) or ""
        if path is None:
            return "*(%s *)&D_8011ECD0[0x%X]%s" % (ctype, n, tail)
        expr = "((%s *)D_8011ECD0)[%d]%s" % (tname, k, path)
        return (expr if lt.name == ctype else "*(%s *)&%s" % (ctype, expr)) + tail
    return RAW_RE.sub(repl, body)


PROTO_RE = re.compile(r"^(s32|u8|s8|u16|s16|u32) (func_[0-9A-F]{8})\(void\);$", re.M)


def return_type(env, body):
    """T-17800: m2c prints `void` for an `s32 f(void)` of the header that falls off its end."""
    m = re.match(r"void (func_[0-9A-F]{8})\(void\) \{", body)
    if m:
        for rt, name in PROTO_RE.findall(env.decl_text):
            if name == m.group(1):
                return rt + body[4:]
    return body


WALK_DECL_RE = re.compile(r"^\s*DisplayActor44 \*(\w+);\s*$", re.M)
WALK_OFF = r"\*\((u8|s8|u16|s16|u32|s32|void)( \*\*?)\)\(\(unsigned char \*\)%s \+ \((\d+)\)\)"


def record_walk(env, body):
    """(T-18100) A DisplayActor44 pointer that starts at D_8011ECD0 and steps one record per trip
    of a do-while: m2c prints its fields as byte offsets past the pool and the step in the middle of
    the body, where IDO scheduled it. The step (and a counter step right before it whose value no
    later statement of the body reads) moves to the end of the body, the offsets after it gain
    0x44, and the fields are printed as `p[96].unk_01` (the matched TAIIKU 80138950.c form)."""
    t = env.types.get("DisplayActor44")
    if t is None:
        return body
    for var in WALK_DECL_RE.findall(body):
        if not re.search(r"\b%s = (\(\(DisplayActor44 \*\))+D_8011ECD0\)+;" % var, body):
            continue
        lines = body.split("\n")
        steps = [i for i, l in enumerate(lines) if l.strip() in ("%s += 1;" % var, "%s += 0x44;" % var)]
        if len(steps) != 1:
            continue
        i = steps[0]
        ind = len(lines[i]) - len(lines[i].lstrip())
        end = next((j for j in range(i + 1, len(lines)) if lines[j].startswith(" " * (ind - 4) + "} while")), None)
        if end is None:
            continue
        moved = [lines[i].replace("+= 0x44;", "+= 1;")]
        k = i - 1
        m = re.match(r"^\s*(\w+) \+= 1;$", lines[k])
        if m and not any(re.search(r"\b%s\b" % m.group(1), l) for l in lines[i + 1:end]):
            moved.insert(0, lines[k])
        else:
            k = i
        tail = [re.sub(WALK_OFF % var, lambda mm, var=var: "*(%s%s)((unsigned char *)%s + (%d))" % (
            mm.group(1), mm.group(2), var, int(mm.group(3)) + t.size), l) for l in lines[i + 1:end]]
        lines = lines[:k] + tail + moved + lines[end:]
        body = "\n".join(lines)

        def repl(mm, var=var):
            n = int(mm.group(3))
            idx, r = divmod(n, t.size)
            path, lt = leaf_at(t, r, 4 if mm.group(2) == " **" else SIZES.get(mm.group(1), 0))
            if path is None:
                return mm.group(0)
            return "%s[%d]%s" % (var, idx, path)
        body = re.sub(WALK_OFF % var, repl, body)
        body = re.sub(r"\(s32 \*\) (D_[0-9A-F]{8})\b", lambda m: "(u8 *)" + m.group(1), body)
    return body


def implicit_int(env, body, row):
    """T-17900: a function the unit calls before its definition, with no prototype, was declared `int` by
    that call (K&R); a `void` definition then fails (\"Incompatible function return type\"). The original is
    an implicit-int function with no return value: write `s32` (as func_8013822C, T-16560)."""
    func = (row or {}).get("func")
    src = (row or {}).get("src")
    if not func or not src or not re.match(r"void %s\(" % re.escape(func), body):
        return body
    text = env.read(src)
    at = text.find('", %s);' % func)
    if at > 0 and re.search(r"\b%s\(" % re.escape(func), text[:at]) and not re.search(
            r"\b%s\s*\(" % re.escape(func), env.decl_text):
        return "s32" + body[4:]
    return body


GS_DECL = "    GameState *g = &D_800E6280;\n"


def gs_local(body):
    """(T-18500) the BUNKA family form of T-12701: every GameState access through one local pointer. Used by
    the driver as an extra scored candidate, not a step."""
    if "D_800E6280" not in body or re.search(r"\bg\b", body):
        return body
    new = re.sub(r"\(\*?&D_800E6280\)|&D_800E6280\b", "g", body)
    new = new.replace("D_800E6280.", "g->")
    new = re.sub(r"\bD_800E6280\b", "(*g)", new)
    return re.sub(r"(\)\s*\{\n)", lambda m: m.group(1) + GS_DECL, new, count=1)


def field_symbols(env, body, known):
    """(body, known) with splat names inside an aggregate written as fields; their declarations dropped."""
    views = {}
    for d in known.values():
        views.update(mg.declarations(d.rstrip() + "\n"))
    try:
        new, _problems = mg.rewrite_text(body, env.aggs, set(), None, lambda s: views.get(s) or env.decls.get(s))
    except Exception:  # a layout the rewriter cannot read: keep the body
        return body, known
    used = set(re.findall(r"\bD_[0-9A-F]{8}\b", new))
    known = {k: v for k, v in known.items() if not (k.startswith("D_") and k not in used)}
    return new, known


# ------------------------------------------------------------------------------------------ driver of the steps

def post_steps(env):
    """[(name, fn(env, body, ctx))] in the order the passes used them; ctx is the dict of redraft.py
    (`row`, `asm`). tools/redraft_extras.py adds the unit-specific ones."""
    import redraft_extras as extras

    def plain(fn):
        return lambda env, body, ctx: fn(env, body)
    steps = [
        ("implicit-int", lambda env, body, ctx: implicit_int(env, body, ctx.get("row"))),
        ("rect-view", plain(extras.rect_view)),
        ("header-arrays", plain(header_arrays)),
        ("array-symbols", plain(array_symbols)),
        ("array-views", plain(array_views)),
        ("struct-bytes", plain(struct_bytes)),
        ("gamestate-records", plain(gamestate_records)),
        ("gamestate-words", plain(gamestate_words)),
        ("raw-display", plain(raw_display)),
        ("pointer-index", plain(pointer_index)),
        ("record-walk", plain(record_walk)),
        ("return-type", plain(return_type)),
        ("menu-arrays", plain(extras.menu_arrays)),
        ("work-prim", plain(work_prims)),
        ("record-temp", plain(record_temps)),
        ("actor-offset", plain(actor_offsets)),
        ("record-offset", plain(record_offsets)),
        ("struct-walk", plain(struct_walks)),
        ("pointer-step", plain(pointer_steps)),
        ("matrix-copy", plain(extras.matrix_copies)),
        ("tcpos-args", lambda env, body, ctx: extras.tcpos_args(env, body, ctx.get("asm"))),
        ("rect-local", plain(extras.rect_locals)),
    ]
    return steps


def post_after_fields(env):
    """Steps that read the output of field-symbols (T-18600)."""
    import redraft_extras as extras
    return [("club-bits", extras.club_bits), ("record-copy", extras.record_copies), ("reroll", extras.reroll)]


def apply_post(env, body, known, ctx=None, log=None):
    """(body, known): the POST rewrites, then field-symbols, then the ones that read its output."""
    ctx = ctx or {}

    def note(name, new, old):
        if new != old and log is not None:
            log.append(name)
        return new
    for name, fn in post_steps(env):
        body = note(name, fn(env, body, ctx), body)
    new, known = field_symbols(env, body, known)
    body = note("field-symbols", new, body)
    for name, fn in post_after_fields(env):
        body = note(name, fn(env, body), body)
    return body, known


def rewrite_all(env, body, known=None, ctx=None):
    """(body, rewrite names): PRE and POST steps on one body, as `tools/redraft.py --rewrite` prints."""
    log = []
    body = pre_steps(env, body, log)
    body, _known = apply_post(env, body, known or {}, ctx, log)
    return body, log
