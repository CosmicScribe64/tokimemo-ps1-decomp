#!/usr/bin/env python3
"""Free relink of the main executable and the overlays (T-39502, T-39505, wiki/english-text-runtime.md,
wiki/relink.md).

Two variants use these steps:
  build/en/  the English build: objects whose C source uses TOKIMEMO_ENGLISH are rebuilt with the flag,
             the English objects (src/english/, the font) are added, entry hooks are written;
  build/jr/  the relink check: no flag and no new objects, but padding is inserted in the code and the
             data of every unit, so that nearly every address moves. Its disc must play like the original.

Every unit is linked from its default linker script with one change: an anchor symbol
(`__en_<unit>_<n> = ALIGN(., 2);`) before each input section, so the new address of any default address
can be named. Nothing is pinned: objects that grow move everything after them.

Addresses the linker cannot see are rewritten from the default build's view:
  - absolute definitions in the side scripts (undefined_*_auto.txt, *_data_syms.ld, main_names.ld) become
    `anchor + offset` (own unit) or the number the relinked main executable gives (overlays);
  - the overlay entry points the main executable names (ovl_load_enter) come from the overlay links
    (config/english_overlay_entries.txt; a second main link uses them, and must keep every other symbol);
  - raw words and lui/low pairs without a relocation (tools/raw_addrs.py) are patched after the link:
    automatically when the value is the start of a function, otherwise as config/english_raw_addrs.txt
    says (`move` or `keep`); an unclassified one fails the build.

Usage (in Docker, run by ninja):
  english_link.py ld UNIT DIR DEFAULT_LD DEFAULT_MAP OUT_LD [--redirect OBJ ...] [--extra OBJ ...]
                  [--shift TEXT:DATA]
  english_link.py aux UNIT DIR OUT [--main-elf ELF] [--entries LD]
      Side script with the translated absolute definitions. Overlays need --main-elf (the first main
      link); the second main link takes --entries.
  english_link.py entries DIR OUT
      Overlay entry symbols of the main executable, from the overlay links.
  english_link.py exe DIR ELF RAW FIRST_ELF HOOKS OUT
      Main executable: check the link, patch raw addresses, apply hooks, header, size checks.
  english_link.py ovl NAME DIR ELF RAW MAIN_ELF HOOKS OUT
      Overlay: check the link and the size against the RAM map, patch raw addresses, hooks, checksum.
  english_link.py copy NAME DEFAULT_BIN OUT
      An overlay the game never loads (EVENT, GYOZI): the default binary, unchanged.
Exits non-zero on any failed check; prints one summary line on success.
"""
import os
import re
import shutil
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import elf32  # noqa: E402
import raw_addrs  # noqa: E402

EXE = "SLPM_86.053"
EXE_HEADER = 0x800
EXE_VRAM = 0x80041000
# Sectors 24-399 of the disc hold SLPM_86.053; KONAMIC3.STR starts at LBA 400. tools/patch/disc.py moves a
# larger file to the end of the track (relocate=True), so this only guards against runaway growth.
EXE_SLOT_BYTES = (400 - 24) * 2048
OVERLAY_SLOT = 0x80132000
OVERLAY_BYTES = 0x30000
# RAM map (wiki/ram-map.md): the main executable and its bss must end below the overlay slot; an overlay
# image must end at the slot end, because 0x80162000 is the background still buffer
# (bg_still_buffers_set) and the scene data loads above it.
MAIN_LIMIT = OVERLAY_SLOT
OVERLAY_LIMIT = OVERLAY_SLOT + OVERLAY_BYTES
MAIN_RANGE = (0x80041000, 0x8012B540)
SLOT_RANGE = (OVERLAY_SLOT + 1, OVERLAY_LIMIT - 1)  # the base and the end are fixed addresses
TRAILER = b"\x12\x10"
UNLOADED = ("EVENT", "GYOZI")  # no LBA of theirs appears anywhere (wiki/overlays.md)
ENTRIES = "config/english_overlay_entries.txt"
RAW_DECISIONS = "config/english_raw_addrs.txt"

INPUT_LINE = re.compile(r"^(\s+)(build/\S+\.o)\((\.\w+|COMMON)\);$")
OUTPUT_SECTION = re.compile(r"^ {4}(\.\w+)\b[^=;]*:")
MAP_STATEMENT = re.compile(r"^ (build/\S+\.o)\((\.\w+|COMMON)\)$")
MAP_INPUT = re.compile(r"^ (\.\w+|COMMON)?\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (build/\S+\.o)$")
ABS_LINE = re.compile(r"^(\s*)(PROVIDE\()?\s*([A-Za-z_.$][\w.$]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*(\))?\s*;(.*)$")
SEGMENT_SYMBOL = re.compile(r"_(TEXT|DATA|RODATA|BSS)_(START|END|SIZE)$|_VRAM|_ROM_|^__en_")


def die(msg):
    sys.exit("english_link.py: " + msg)


def in_range(v, r):
    return r[0] <= v <= r[1]


def default_paths(unit):
    if unit == "main":
        return "build/%s.ld" % EXE, "build/%s.map" % EXE, "build/%s.elf" % EXE
    return "build/ovl/%s.ld" % unit, "build/ovl/%s.map" % unit, "build/ovl/%s.elf" % unit


def side_scripts(unit):
    """The default link's scripts of absolute definitions, in link order."""
    if unit == "main":
        return ["build/undefined_funcs_auto.txt", "build/undefined_syms_auto.txt",
                "build/%s_data_syms.ld" % EXE]
    return ["build/ovl/%s_undefined_funcs_auto.txt" % unit, "build/ovl/%s_undefined_syms_auto.txt" % unit,
            "build/ovl/%s_data_syms.ld" % unit, "build/main_names.ld"]


def variant_path(obj, out_dir):
    """build/<path> of a default object -> <out_dir>/<path> of its rebuilt copy."""
    return out_dir + "/" + obj[len("build/"):]


def read_map(path):
    """{(object, section): (address, size)} of the input sections with contents."""
    out = {}
    pending = None
    for line in open(path):
        line = line.rstrip("\n")
        m = MAP_STATEMENT.match(line)
        if m:
            pending = (m.group(1), m.group(2))
            continue
        m = MAP_INPUT.match(line)
        if m and pending and m.group(4) == pending[0]:
            size = int(m.group(3), 16)
            if pending not in out and size > 0:
                out[pending] = (int(m.group(2), 16), size)
    return out


def symbols(path):
    """{name: (address, is_function, defined_in_section, is_global)} of an ELF."""
    out = {}
    for s in elf32.Elf(path).symbols:
        if s.name and s.shndx != elf32.SHN_UNDEF:
            out.setdefault(s.name, (s.value, s.type == 2, s.shndx != elf32.SHN_ABS, s.bind == 1))
    return out


def section_symbols(elf, sec):
    """{name: offset} of the symbols defined in a section of an object."""
    return {sym.name: sym.value for sym in elf.symbols if sym.name and sym.shndx == sec.index}


def section_references(elf):
    """{(site section, site offset): (target section, target offset)} of the section-relative
    relocations of an object: R_MIPS_32 words, and LO16 sites with the addend of their HI16."""
    out = {}
    for sec in elf.sections:
        if sec.type == elf32.SHT_REL or not sec.flags & 2:
            continue
        data = elf.contents(sec)
        last_hi = {}
        for offset, kind, sym in sorted(elf.relocations(sec), key=lambda r: r[0]):
            if sym.type != 3 or sym.shndx in (elf32.SHN_UNDEF, elf32.SHN_ABS):  # STT_SECTION only
                continue
            target = elf.sections[sym.shndx].name
            if kind == elf32.R_MIPS_32:
                out[(sec.name, offset)] = (target, struct.unpack_from("<I", data, offset)[0])
            elif kind == elf32.R_MIPS_HI16:
                last_hi[sym.shndx] = struct.unpack_from("<H", data, offset)[0]
            elif kind == elf32.R_MIPS_LO16 and sym.shndx in last_hi:
                lo = struct.unpack_from("<h", data, offset)[0]
                out[(sec.name, offset)] = (target, ((last_hi[sym.shndx] << 16) + lo) & 0xFFFFFFFF)
    return out


class Layout:
    """A unit's default placement: anchors (script order of the input sections), where each input section
    was, and which objects the variant rebuilds."""

    def __init__(self, unit, out_dir, redirect=()):
        self.unit = unit
        self.out_dir = out_dir
        ld, map_path, elf = default_paths(unit)
        self.order = []
        for line in open(ld):
            m = INPUT_LINE.match(line.rstrip("\n"))
            if m:
                self.order.append((m.group(2), m.group(3)))
        self.index = {key: n for n, key in enumerate(self.order)}
        self.placed = read_map(map_path)
        self.spans = sorted((a, a + size, key) for key, (a, size) in self.placed.items())
        self.redirect = set(redirect)
        self.default_syms = symbols(elf)
        self.names_at = {}
        for name, (addr, _f, in_section, is_global) in self.default_syms.items():
            if in_section and is_global and not SEGMENT_SYMBOL.search(name) and "." not in name:
                self.names_at.setdefault(addr, []).append(name)
        self.functions = {a for a, f, s, _g in self.default_syms.values() if f and s}
        self._same = {}

    def referenced_offset(self, key, offset):
        """Offset in the variant's section of the byte that the default object's own code or data
        reaches at `offset` (an anonymous literal in a rebuilt object): the variant's relocation at
        the same site gives it. None unless every such site agrees and the site sections kept their
        size."""
        old, new = elf32.Elf(key[0]), elf32.Elf(variant_path(key[0], self.out_dir))
        refs_old, refs_new = section_references(old), section_references(new)
        found = set()
        for site, target in refs_old.items():
            if target != (key[1], offset):
                continue
            a, b = old.section(site[0]), new.section(site[0])
            if a is None or b is None or a.size != b.size or site not in refs_new \
                    or refs_new[site][0] != key[1]:
                return None
            found.add(refs_new[site][1])
        return found.pop() if len(found) == 1 else None

    def unchanged(self, key):
        """True when the variant's copy of a rebuilt object keeps this section's size and the offset of
        every symbol in it, so an offset into it still names the same place. English literals change
        .rodata; in .data they change bytes of fixed-size arrays and the addends of pointers."""
        if key[0] not in self.redirect:
            return True
        if key not in self._same:
            old, new = elf32.Elf(key[0]), elf32.Elf(variant_path(key[0], self.out_dir))
            a, b = old.section(key[1]), new.section(key[1])
            self._same[key] = a is not None and b is not None and a.size == b.size \
                and section_symbols(old, a) == section_symbols(new, b)
        return self._same[key]

    def anchor(self, key):
        return "__en_%s_%d" % (self.unit, self.index[key])

    def containing(self, v):
        """(object, section) whose default span holds v (an end address counts for the section it ends,
        unless another one starts there)."""
        end_match = None
        for start, end, key in self.spans:
            if start <= v < end:
                return key
            if v == end:
                end_match = key
        return end_match

    def where(self, v):
        """('anchor', key, offset) or ('name', symbol) for default address v; None if v is outside every
        input section; dies when v lies in a rebuilt object and no global symbol names it."""
        key = self.containing(v)
        if key is None:
            return None
        if self.unchanged(key):
            return ("anchor", key, v - self.placed[key][0])
        names = sorted(self.names_at.get(v, []))
        if not names and v - self.placed[key][0] == self.placed[key][1]:  # the end of the section
            sec = elf32.Elf(variant_path(key[0], self.out_dir)).section(key[1])
            return ("anchor", key, sec.size)
        if not names:
            moved = self.referenced_offset(key, v - self.placed[key][0])
            if moved is not None:
                return ("anchor", key, moved)
            die("%s: 0x%08X lies in %s(%s), which this variant rebuilds with other bytes, and no global "
                "symbol names it" % (self.unit, v, key[0], key[1]))
        return ("name", names[0])

    def expr(self, v):
        w = self.where(v)
        if w is None:
            return None
        if w[0] == "name":
            return w[1]
        return "%s + 0x%X" % (self.anchor(w[1]), w[2]) if w[2] else self.anchor(w[1])

    def moved_to(self, v, new_syms):
        """New address of default address v, or None when it cannot be found: outside every input
        section, or inside a rebuilt object's changed section with neither a global symbol there nor a
        relocation of the object's own that reaches it. For tools that patch the relinked binaries
        (T-39507); they must still check the bytes they find there."""
        key = self.containing(v)
        if key is None:
            return None
        offset = v - self.placed[key][0]
        if not self.unchanged(key):
            for name in sorted(self.names_at.get(v, [])):
                if name in new_syms:
                    return new_syms[name][0]
            offset = self.referenced_offset(key, offset)
            if offset is None:
                return None
        anchor = self.anchor(key)
        return new_syms[anchor][0] + offset if anchor in new_syms else None

    def value(self, v, new_syms):
        """New address of default address v, from the relinked ELF's symbols."""
        w = self.where(v)
        if w is None:
            die("%s: 0x%08X is not inside any input section" % (self.unit, v))
        if w[0] == "name":
            if w[1] not in new_syms:
                die("%s: %s (0x%08X) is missing from the relinked image" % (self.unit, w[1], v))
            return new_syms[w[1]][0]
        anchor = self.anchor(w[1])
        if anchor not in new_syms:
            die("%s: anchor %s missing from the relinked image" % (self.unit, anchor))
        return new_syms[anchor][0] + w[2]


# ---- ld ----

def translator(unit, out_dir):
    """Function default address -> address in the relinked unit (None if it has none), from the variant's
    link in out_dir (build/en)."""
    lay = Layout(unit, out_dir, redirected_objects(out_dir, unit))
    elf = "%s/%s.elf" % (out_dir, EXE) if unit == "main" else "%s/ovl/%s.elf" % (out_dir, unit)
    syms = symbols(elf)
    return lambda v: lay.moved_to(v, syms)


def cmd_ld(argv):
    unit, out_dir, default_ld, default_map, out_ld = argv[:5]
    opts = {"--redirect": [], "--extra": [], "--shift": []}
    key = None
    for a in argv[5:]:
        if a in opts:
            key = a
        elif key is None:
            die("ld: unexpected argument %r" % a)
        else:
            opts[key].append(a)
    redirect = set(opts["--redirect"])
    extra = opts["--extra"]
    if extra and unit != "main":
        die("ld: new objects go into the main executable only")
    shift_text = shift_data = 0
    if opts["--shift"]:
        shift_text, shift_data = (int(x, 0) for x in opts["--shift"][0].split(":"))
    target = ".main" if unit == "main" else "." + unit
    lines = open(default_ld).read().split("\n")
    # last input line of each kind inside the unit's output section, for the new objects
    section = None
    last = {}
    first_data = None
    texts = []
    for i, line in enumerate(lines):
        m = OUTPUT_SECTION.match(line)
        if m:
            section = m.group(1)
        m = INPUT_LINE.match(line)
        if m and section == target:
            last[m.group(3)] = i
            if m.group(3) == ".data" and first_data is None:
                first_data = i
            if m.group(3) == ".text":
                texts.append(i)
    insert_before = {}
    if shift_text:
        if len(texts) < 2:
            die("ld %s: --shift needs two .text inputs" % unit)
        insert_before[texts[1]] = ". += 0x%X; /* relink check: move the code */" % shift_text
    if shift_data:
        if first_data is None:
            die("ld %s: no .data input to shift" % unit)
        insert_before[first_data] = ". += 0x%X; /* relink check: move the data */" % shift_data
    insert_after = {}
    if extra:
        for sec, kinds in ((".text", [".text"]), (".rodata", [".rodata"]),
                           (".data", [".data", ".bss", "COMMON"])):
            if sec not in last:
                die("ld main: no %s input to append the new objects to" % sec)
            insert_after[last[sec]] = ["        %s(%s);" % (o, k) for k in kinds for o in extra]
    out = []
    seen = set()
    n = 0
    for i, line in enumerate(lines):
        if i in insert_before:
            out.append("        " + insert_before[i])
        m = INPUT_LINE.match(line)
        if m:
            indent, obj, sec = m.groups()
            out.append("%s__en_%s_%d = ALIGN(., 2);" % (indent, unit, n))
            n += 1
            if obj in redirect:
                seen.add(obj)
                line = "%s%s(%s);" % (indent, variant_path(obj, out_dir), sec)
        out.append(line)
        out += insert_after.get(i, [])
    missing = redirect - seen
    if missing:
        die("ld: %s not in %s" % (", ".join(sorted(missing)), default_ld))
    with open(out_ld, "w") as f:
        f.write("/* free relink (T-39505): tools/english_link.py ld from %s */\n" % default_ld)
        f.write("\n".join(out))


# ---- aux ----

def read_entries():
    """{symbol: [overlay, ...]} of config/english_overlay_entries.txt."""
    out = {}
    for n, line in enumerate(open(ENTRIES), 1):
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        if len(words) < 2:
            die("%s:%d: expected `<symbol> <overlay> ...`" % (ENTRIES, n))
        out[words[0]] = words[1:]
    return out


def read_ld_numbers(path):
    out = {}
    for line in open(path):
        m = ABS_LINE.match(line)
        if m:
            out[m.group(3)] = int(m.group(4), 16)
    return out


def redirected_objects(out_dir, unit):
    """Objects the variant rebuilds, from its linker script (lines that name <out_dir>/...)."""
    path = "%s/%s.ld" % (out_dir, EXE) if unit == "main" else "%s/ovl/%s.ld" % (out_dir, unit)
    found = set()
    for line in open(path):
        m = INPUT_LINE.match(line.rstrip("\n"))
        if m and m.group(2).startswith(out_dir + "/"):
            found.add("build/" + m.group(2)[len(out_dir) + 1:])
    return found


def referenced_names(out_dir, unit, definitions_only=False):
    """Names defined or referenced by the selected input sections of this link.
    Discarded sections must not retain stale interior-entry references (T-39506).
    Defined names count too: a side-script assignment overrides
    an object's own label (`D_800AA178` in libgte_patchgte, whose code copies itself up to it)."""
    path = "%s/%s.ld" % (out_dir, EXE) if unit == "main" else "%s/ovl/%s.ld" % (out_dir, unit)
    objects = {}
    for line in open(path):
        m = INPUT_LINE.match(line.rstrip("\n"))
        if m:
            objects.setdefault(m.group(2), set()).add(m.group(3))
    names = set()
    for obj in sorted(objects):
        if os.path.exists(obj):
            names |= selected_names(elf32.Elf(obj), objects[obj], definitions_only)
    return names


def selected_names(elf, selected, definitions_only=False):
    """Side-script symbols needed by selected sections, including their relocations."""
    sections = [sec for sec in elf.sections if sec.name in selected]
    indices = {sec.index for sec in sections}
    if definitions_only:
        return {sym.name for sym in elf.symbols
                if sym.name and sym.bind == 1 and sym.shndx in indices}
    names = {sym.name for sym in elf.symbols if sym.name and
             (sym.shndx in indices or sym.shndx == elf32.SHN_ABS or
              (sym.shndx == elf32.SHN_COMMON and "COMMON" in selected))}
    for sec in sections:
        names.update(sym.name for _, _, sym in elf.relocations(sec) if sym.name)
    return names


def cmd_aux(argv):
    unit, out_dir, out = argv[:3]
    main_elf = entries = None
    rest = argv[3:]
    while rest:
        if rest[0] == "--main-elf":
            main_elf, rest = rest[1], rest[2:]
        elif rest[0] == "--entries":
            entries, rest = rest[1], rest[2:]
        else:
            die("aux: unexpected argument %r" % rest[0])
    own = Layout(unit, out_dir, redirected_objects(out_dir, unit))
    own_range = MAIN_RANGE if unit == "main" else SLOT_RANGE
    main = main_syms = default_main_syms = None
    if unit != "main":
        if not main_elf:
            die("aux %s: needs --main-elf" % unit)
        main = Layout("main", out_dir, redirected_objects(out_dir, "main"))
        main_syms = symbols(main_elf)
        default_main_syms = main.default_syms
    entry_values = read_ld_numbers(entries) if entries else {}
    used = referenced_names(out_dir, unit)
    defined = referenced_names(out_dir, unit, definitions_only=True)
    lines = ["/* free relink (T-39505): tools/english_link.py aux %s */" % unit]
    imported = set()
    for path in side_scripts(unit):
        lines.append("/* from %s */" % path)
        for line in open(path):
            line = line.rstrip("\n")
            m = ABS_LINE.match(line)
            if not m:
                lines.append(line)
                continue
            indent, provide, name, number, _close, tail = m.groups()
            imported.add(name)
            v = int(number, 16)
            new = None
            if name not in used:  # no object of this link refers to it (main_names.ld is shared)
                lines.append(line)
                continue
            if in_range(v, own_range):
                if name in defined:
                    # The input's relocatable definition is authoritative. An
                    # absolute side-script alias must not pin it to the old address.
                    lines.append("/* %s is defined by a selected input section. */" % name)
                    continue
                new = own.expr(v)
            elif unit == "main" and in_range(v, SLOT_RANGE) and entries:
                if name not in entry_values:
                    die("aux main: %s (0x%08X) is in the overlay slot but not in %s" % (name, v, ENTRIES))
                new = "0x%08X" % entry_values[name]
            elif unit != "main" and in_range(v, MAIN_RANGE):
                new = "0x%08X" % main.value(v, main_syms)
            if new is None:
                lines.append(line)
                continue
            body = "%s = %s" % (name, new)
            lines.append("%s%s;%s" % (indent, "PROVIDE(%s)" % body if provide else body, tail))
    if unit != "main":
        # T-39536: include existing main functions newly called by English overlays,
        # such as SDK routines absent from the original overlay's imports.
        lines.append("/* main-executable imports */")
        for name, (addr, _f, in_section, is_global) in sorted(main_syms.items()):
            if in_section and is_global and not name.startswith("__en_") and \
                    (name not in default_main_syms or (name in used and name not in imported)):
                lines.append("%s = 0x%08X;" % (name, addr))
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")


# ---- entries ----

def cmd_entries(argv):
    out_dir, out = argv
    lines = ["/* overlay entry points for the main link (T-39505): tools/english_link.py entries */"]
    main_numbers = read_ld_numbers("build/undefined_syms_auto.txt")
    for name, overlays in sorted(read_entries().items()):
        if name not in main_numbers:
            die("entries: %s is not an undefined slot symbol of the main executable" % name)
        v = main_numbers[name]
        values = set()
        for ovl in overlays:
            lay = Layout(ovl, out_dir, redirected_objects(out_dir, ovl))
            if v not in lay.functions:
                die("entries: %s (0x%08X) is not a function start in %s" % (name, v, ovl))
            values.add(lay.value(v, symbols("%s/ovl/%s.elf" % (out_dir, ovl))))
        if len(values) != 1:
            die("entries: %s lands at %s in %s; the main executable has one address for them"
                % (name, ", ".join("0x%08X" % x for x in sorted(values)), " ".join(overlays)))
        lines.append("%s = 0x%08X; /* %s */" % (name, values.pop(), " ".join(overlays)))
    with open(out, "w") as f:
        f.write("\n".join(lines) + "\n")


# ---- raw addresses ----

def symbol_extent(syms, name):
    """[start, end) of a symbol: up to the next symbol (or 4 bytes for the last one)."""
    start = syms[name][0]
    later = sorted({a for a, _f, s, _g in syms.values() if s and a > start})
    return start, later[0] if later else start + 4


def read_decisions(unit, default_syms, new_syms):
    """[(start, end, action, in_default)] of config/english_raw_addrs.txt for `unit` (end exclusive). A
    symbol of the default build, or an address, names default addresses; a symbol that only the relinked
    build has (new code and data) names relinked addresses."""
    out = []
    for n, line in enumerate(open(RAW_DECISIONS), 1):
        words = line.split("#", 1)[0].split(None, 3)
        if not words or words[0] != unit:
            continue
        if len(words) < 4 or words[2] not in ("move", "keep"):
            die("%s:%d: expected `<unit> <symbol|0xADDR[-0xEND]> <move|keep> <reason>`" % (RAW_DECISIONS, n))
        what = words[1]
        if what.startswith("0x"):
            a, _, b = what.partition("-")
            start = int(a, 16)
            out.append((start, int(b, 16) if b else start + 4, words[2], True))
        elif what in default_syms:
            out.append(symbol_extent(default_syms, what) + (words[2], True))
        elif what in new_syms:
            out.append(symbol_extent(new_syms, what) + (words[2], False))
        elif what.startswith("english_"):
            continue  # English code and data: only the English build has them
        else:
            die("%s:%d: %s is not a symbol of %s" % (RAW_DECISIONS, n, what, unit))
    return out


def raw_sites(unit, out_dir, new_map, own, main_layout, main_syms_new, own_syms_new):
    """[(address, kind, old value, new value)] of the raw addresses to patch in the relinked unit."""
    placed_new = raw_addrs.placed_sections(new_map)
    decisions = read_decisions(unit, own.default_syms, own_syms_new)
    used = set()
    ranges = [MAIN_RANGE, SLOT_RANGE]
    new_sizes = {}
    out = []
    for path in sorted({o for o, _s in placed_new}):
        default_obj = "build/" + path[len(out_dir) + 1:] if path.startswith(out_dir + "/") else path
        rebuilt = default_obj != path or default_obj in own.redirect
        for f in raw_addrs.scan_object(path, placed_new, ranges, {}):
            if f["kind"] == "abs" or f["address"] is None:
                continue
            v = f["value"]
            site_default = None
            p = own.placed.get((default_obj, f["section"]))
            if p and (not rebuilt or p[1] == new_sizes.setdefault((path, f["section"]),
                                                               elf32.Elf(path).section(f["section"]).size)):
                site_default = p[0] + f["offset"]  # a rebuilt object whose section kept its size counts too
            target = main_layout if in_range(v, MAIN_RANGE) else own
            if unit == "main" and in_range(v, SLOT_RANGE):
                die("main: raw overlay address 0x%08X at %s %s+0x%X" % (v, path, f["section"], f["offset"]))
            action = None
            for i, (start, end, act, in_default) in enumerate(decisions):
                at = site_default if in_default else f["address"]
                if at is not None and start <= at < end:
                    action = act
                    used.add(i)
                    break
            if action is None and f["kind"] == "word" and v in target.functions:
                action = "move"
            if action is None:
                die("%s: unclassified raw address 0x%08X (%s) at %s %s+0x%X (default 0x%s); add it to %s"
                    % (unit, v, f["kind"], path, f["section"], f["offset"],
                       "%08X" % site_default if site_default is not None else "?", RAW_DECISIONS))
            if action == "keep":
                continue
            if f["kind"] in ("jal", "j"):
                die("%s: raw %s to 0x%08X at %s" % (unit, f["kind"], v, path))
            new = target.value(v, main_syms_new if target is main_layout else own_syms_new)
            out.append((f["address"], f["kind"], v, new))
    unused = [d for i, d in enumerate(decisions) if i not in used]
    if unused:
        die("%s: %s entries match no raw address: %s" % (unit, RAW_DECISIONS,
                                                          ", ".join("0x%08X" % d[0] for d in unused)))
    return out


def sext16(v):
    return v - 0x10000 if v & 0x8000 else v


def patch_raw(data, base, sites):
    """Rewrite raw words and lui/low pairs (every low that uses the lui's register) in place."""
    for addr, kind, old, new in sites:
        at = addr - base
        if kind == "word":
            if struct.unpack_from("<I", data, at)[0] != old:
                die("raw word at 0x%08X is not 0x%08X" % (addr, old))
            struct.pack_into("<I", data, at, new)
            continue
        lui = struct.unpack_from("<I", data, at)[0]
        rt = (lui >> 16) & 31
        hi = (lui & 0xFFFF) << 16
        delta = new - old
        new_hi = None
        k = at + 4
        lows = 0
        while k < min(len(data), at + 160):
            x = struct.unpack_from("<I", data, k)[0]
            op = x >> 26
            if op in raw_addrs.LOW_OPS and (x >> 21) & 31 == rt:
                imm = x & 0xFFFF
                full = (hi | imm) if op == 0x0D else (hi + sext16(imm)) & 0xFFFFFFFF
                want = (full + delta) & 0xFFFFFFFF
                if op == 0x0D:
                    h, lo = want >> 16, want & 0xFFFF
                else:
                    h, lo = ((want + 0x8000) >> 16) & 0xFFFF, want & 0xFFFF
                if new_hi is not None and h != new_hi:
                    die("lui at 0x%08X feeds lows that need different upper halves" % addr)
                new_hi = h
                struct.pack_into("<I", data, k, (x & 0xFFFF0000) | lo)
                lows += 1
            if x == 0x03E00008:  # jr $ra: the function ends after the delay slot
                k += 4
                continue
            if k > at + 4 and struct.unpack_from("<I", data, k - 4)[0] == 0x03E00008:
                break
            if (op == 0x0F and (x >> 16) & 31 == rt) or \
               (op == 0 and (x >> 11) & 31 == rt and x & 0x3F not in (0x08, 0x09)) or \
               (op in (0x08, 0x09, 0x0D, 0x0C, 0x0A, 0x0B, 0x0E, 0x20, 0x21, 0x23, 0x24, 0x25)
                    and (x >> 16) & 31 == rt):
                break
            k += 4
        if not lows:
            die("lui at 0x%08X: no low part found" % addr)
        struct.pack_into("<I", data, at, (lui & 0xFFFF0000) | new_hi)


def check_anchors(unit, lay, new_map, syms_new, out_dir):
    """Every anchor equals the address where its input section was placed."""
    placed_new = read_map(new_map)
    for key, n in lay.index.items():
        obj = variant_path(key[0], out_dir) if key[0] in lay.redirect else key[0]
        if (obj, key[1]) in placed_new:
            anchor = "__en_%s_%d" % (unit, n)
            if syms_new[anchor][0] != placed_new[(obj, key[1])][0]:
                die("%s: anchor %s is 0x%08X but %s(%s) starts at 0x%08X" % (
                    unit, anchor, syms_new[anchor][0], obj, key[1], placed_new[(obj, key[1])][0]))


# ---- hooks ----

def read_hooks(path, unit):
    """[(original, replacement)] for `unit`; path `-` means no hooks (the relink check)."""
    hooks = []
    if path == "-":
        return hooks
    for n, line in enumerate(open(path), 1):
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        if len(words) != 3:
            die("%s:%d: expected `<unit> <original> <replacement>`" % (path, n))
        if words[0] == unit:
            hooks.append((words[1], words[2]))
    return hooks


def apply_hooks(data, base, file_offset, syms, hooks, unit):
    for original, replacement in hooks:
        if original not in syms or replacement not in syms:
            die("%s hook %s -> %s: symbol not found" % (unit, original, replacement))
        src, dst = syms[original][0], syms[replacement][0]
        if (src ^ dst) & 0xF0000000:
            die("%s hook %s -> %s: out of j range" % (unit, original, replacement))
        at = src - base + file_offset
        if not 0 <= at <= len(data) - 8:
            die("%s hook %s at 0x%08X is outside the binary" % (unit, original, src))
        data[at:at + 8] = struct.pack("<II", 0x08000000 | ((dst >> 2) & 0x03FFFFFF), 0)


# ---- exe ----

def cmd_exe(argv):
    out_dir, elf, raw, first_elf, hooks_path, out = argv
    syms = symbols(elf)
    first = symbols(first_elf)
    entries = read_entries()
    bad = ["%s 0x%08X -> 0x%08X" % (n, first[n][0], syms[n][0])
           for n in first if n in syms and n not in entries and first[n][0] != syms[n][0]]
    if bad:
        die("exe: the second main link moved %d symbols, first: %s" % (len(bad), "; ".join(bad[:5])))
    lay = Layout("main", out_dir, redirected_objects(out_dir, "main"))
    new_map = "%s/%s.map" % (out_dir, EXE)
    check_anchors("main", lay, new_map, syms, out_dir)
    end = syms["main_VRAM_END"][0]
    if end > MAIN_LIMIT:
        die("exe: the executable and its bss end at 0x%08X, past 0x%08X (wiki/ram-map.md)" % (end, MAIN_LIMIT))
    data = bytearray(open(raw, "rb").read())
    image = data[EXE_HEADER:]
    sites = raw_sites("main", out_dir, new_map, lay, lay, syms, syms)
    patch_raw(image, EXE_VRAM, [s for s in sites if s[0] >= EXE_VRAM])
    data[EXE_HEADER:] = image
    apply_hooks(data, EXE_VRAM, EXE_HEADER, syms, read_hooks(hooks_path, "main"), "main")
    data += bytes(-len(data) % 2048)
    if len(data) > EXE_SLOT_BYTES:
        die("exe: %d bytes, more than the %d-byte limit" % (len(data), EXE_SLOT_BYTES))
    struct.pack_into("<I", data, 0x10, syms["boot_start"][0])
    struct.pack_into("<I", data, 0x1C, len(data) - EXE_HEADER)
    with open(out, "wb") as f:
        f.write(data)
    print("relink exe: %s %d bytes, ends at 0x%08X (limit 0x%08X), %d raw addresses patched"
          % (out, len(data), end, MAIN_LIMIT, len(sites)))


# ---- ovl ----

# bytes counted for a label inside the zero run after an overlay's last nonzero byte: the variables
# there are at most 16 bytes (RPG_BAT's RpgRewardSelectionFlags; ETC and RENSYU have s32s)
ZERO_RUN_LABEL_BYTES = 16


def used_end(name):
    """Address just past the default overlay's data: its last nonzero byte before the 4-byte trailer, or
    16 bytes past the last label in the zero run after it (ETC, RENSYU and RPG_BAT keep zero-valued
    variables there), whichever is later."""
    d = open("build/ovl/%s.bin" % name, "rb").read()
    end = len(d) - 4
    while end > 0 and d[end - 1] == 0:
        end -= 1
    end += OVERLAY_SLOT
    last_nonzero = end
    for sym in elf32.Elf("build/ovl/%s.elf" % name).symbols:
        if sym.name and sym.shndx not in (elf32.SHN_UNDEF, elf32.SHN_ABS) and not SEGMENT_SYMBOL.search(sym.name) \
                and last_nonzero <= sym.value < OVERLAY_LIMIT - 4:
            end = max(end, sym.value + ZERO_RUN_LABEL_BYTES)
    return min(end, OVERLAY_LIMIT - 4)


def cmd_ovl(argv):
    name, out_dir, elf, raw, main_elf, hooks_path, out = argv
    syms = symbols(elf)
    lay = Layout(name, out_dir, redirected_objects(out_dir, name))
    main_lay = Layout("main", out_dir, redirected_objects(out_dir, "main"))
    new_map = "%s/ovl/%s.map" % (out_dir, name)
    check_anchors(name, lay, new_map, syms, out_dir)
    data = bytearray(open(raw, "rb").read())
    # The zero run after the default overlay's last data (and its old trailer) holds nothing: no symbol and
    # no address points into it and no code writes it (wiki/ram-map.md). It may shrink as code grows.
    old_end = used_end(name)
    new_end = lay.value(old_end, syms) if old_end < OVERLAY_LIMIT - 4 else None
    trailer = lay.value(OVERLAY_LIMIT - 4, syms) - OVERLAY_SLOT
    data[trailer:trailer + 4] = bytes(4)
    if new_end is None:
        new_end = OVERLAY_SLOT + len(data)
    if new_end > OVERLAY_LIMIT - 4:
        die("ovl %s: data ends at 0x%08X, past 0x%08X (slot end minus the checksum trailer; the RAM "
            "above is the background still buffer, wiki/ram-map.md)" % (name, new_end, OVERLAY_LIMIT - 4))
    if any(data[new_end - OVERLAY_SLOT:]):
        die("ovl %s: nonzero bytes after the data end 0x%08X" % (name, new_end))
    data = data[:OVERLAY_BYTES - 4] + bytes(max(0, OVERLAY_BYTES - 4 - len(data))) + bytes(4)
    sites = raw_sites(name, out_dir, new_map, lay, main_lay, symbols(main_elf), syms)
    patch_raw(data, OVERLAY_SLOT, sites)
    hooks = read_hooks(hooks_path, name)
    if hooks:
        apply_hooks(data, OVERLAY_SLOT, 0, syms, hooks, name)
    total = sum(data[:-4]) & 0xFFFF
    data[-4:] = bytes([total >> 8, total & 0xFF]) + TRAILER
    with open(out, "wb") as f:
        f.write(data)
    print("relink ovl: %s data to 0x%08X (%d bytes free), %d raw addresses patched, checksum %04X"
          % (out, new_end, OVERLAY_LIMIT - 4 - new_end, len(sites), total))


def cmd_copy(argv):
    name, default_bin, out = argv
    if name not in UNLOADED:
        die("copy: %s is loaded by the game and must be relinked" % name)
    shutil.copyfile(default_bin, out)


def main(argv):
    cmds = {"ld": cmd_ld, "aux": cmd_aux, "entries": cmd_entries, "exe": cmd_exe, "ovl": cmd_ovl,
            "copy": cmd_copy}
    if len(argv) < 2 or argv[1] not in cmds:
        sys.exit(__doc__)
    cmds[argv[1]](argv[2:])


if __name__ == "__main__":
    main(sys.argv)
