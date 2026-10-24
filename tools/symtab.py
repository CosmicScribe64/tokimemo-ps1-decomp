"""Symbol tables of every link unit, read from the splat configs (T-27002).

One resolver for (unit, name) -> address and back, for tools/rename.py, tools/check_symbols.py,
tools/migrate_globals.py and configure.py. Nothing here reads game data.

A unit is `main` (config/SLPM_86.053.yaml) or an overlay of config/overlays.txt
(config/overlays/<NAME>.yaml). The symbol files of a unit are the `symbol_addrs_path` list of its
yaml, in order; main also has config/symbol_addrs_types.txt, which only tools/syms_to_ld.py reads.

Scope rule (the same as tools/sync_protos.is_main_symbol): in unit U, an address A is U's own when
A lies in U's file range (load address + 0x30000), otherwise it is a main-exe address. EVENT loads at
0x800F6000, so its range covers the main bss 0x800F6000-0x80126000: a main name there must not be in a
file that EVENT loads.

Where a new name goes (placement()):
  main address outside every overlay range   config/symbol_addrs.txt (every yaml loads it)
  main address inside EVENT's range          config/symbol_addrs_main.txt (main yaml only; overlays
                                             link it through build/main_names.ld)
  overlay address                            config/symbols_<NAME>.txt
A hypothesis goes into the file of the same role with `_hypotheses` before `.txt`
(config/symbol_addrs_hypotheses.txt, config/symbol_addrs_main_hypotheses.txt,
config/symbols_<NAME>_hypotheses.txt), whose first line says HYPOTHESES.

problems() is the consistency check (tools/check_symbols.py runs it in the build):
  1. one name per address and one address per name in every unit (a line whose comment holds the word
     `alias-view` is a deliberate second name and is exempt);
  2. a file that several units load holds only addresses that are main addresses in each of them;
  3. a main address has one name across all symbol files of all units;
  4. names are unique without regard to case in every unit (splat writes one .s file per function,
     and macOS volumes are case-insensitive).
"""
import os
import re
from typing import NamedTuple

EXE_YAML = "config/SLPM_86.053.yaml"
TYPES_FILE = "config/symbol_addrs_types.txt"
SHARED_FILE = "config/symbol_addrs.txt"
MAIN_ONLY_FILE = "config/symbol_addrs_main.txt"
OVERLAY_SIZE = 0x30000
MAIN_LO, MAIN_HI = 0x80041000, 0x8012B538
PLACEHOLDER_RE = re.compile(r"^(?:func|D|jtbl)_([0-9A-F]{8})$")
LINE_RE = re.compile(r"^\s*([A-Za-z_]\w*)\s*=\s*(0[xX][0-9A-Fa-f]+|\d+)\s*;(.*)$")
ALIAS_WORD = "alias-view"
HYP_SUFFIX = "_hypotheses"


class SymLine(NamedTuple):
    path: str       # repo-relative symbol file
    lineno: int     # 1-based
    name: str
    addr: int
    comment: str    # text after `//` (splat attributes and notes), stripped

    @property
    def is_func(self):
        return "type:func" in self.comment.split()

    @property
    def alias(self):
        return ALIAS_WORD in self.comment.split()


class Unit(NamedTuple):
    name: str
    yaml: str
    base: int       # load address (main: MAIN_LO)
    end: int        # end of the file range (main: MAIN_HI)


def placeholder_addr(name):
    """Address of a splat placeholder name (func_/D_/jtbl_ + 8 hex digits), else None."""
    m = PLACEHOLDER_RE.match(name or "")
    return int(m.group(1), 16) if m else None


def parse_lines(text, path):
    """SymLine list of a symbol file's text (comment and blank lines skipped)."""
    out = []
    for n, line in enumerate(text.splitlines(), 1):
        if line.lstrip().startswith("//"):
            continue
        m = LINE_RE.match(line)
        if not m:
            continue
        comment = m.group(3).strip()
        comment = comment[2:].strip() if comment.startswith("//") else ""
        out.append(SymLine(path, n, m.group(1), int(m.group(2), 0), comment))
    return out


def read_units(root="."):
    """[Unit] for main and every overlay of config/overlays.txt, in that order."""
    units = [Unit("main", EXE_YAML, MAIN_LO, MAIN_HI)]
    path = os.path.join(root, "config/overlays.txt")
    if not os.path.exists(path):
        return units
    with open(path) as f:
        for line in f:
            parts = line.split()
            if parts and not line.startswith("#"):
                base = int(parts[1], 16)
                units.append(Unit(parts[0], "config/overlays/%s.yaml" % parts[0], base, base + OVERLAY_SIZE))
    return units


def yaml_symbol_files(text):
    """The symbol_addrs_path entries of a splat yaml text, in order (no yaml parser needed)."""
    out, inside = [], False
    for line in text.splitlines():
        s = line.strip()
        if s == "symbol_addrs_path:":
            inside = True
            continue
        if inside:
            if s.startswith("#") or not s:
                continue
            if s.startswith("- "):
                out.append(s[2:].split("#", 1)[0].strip())
                continue
            break
    return out


def add_yaml_symbol_file(text, path):
    """Yaml text with `path` appended to its symbol_addrs_path list (unchanged if present)."""
    if path in yaml_symbol_files(text):
        return text
    lines = text.splitlines(True)
    k = next((i for i, l in enumerate(lines) if l.strip() == "symbol_addrs_path:"), None)
    if k is None:
        o = next(i for i, l in enumerate(lines) if l.strip() == "options:")
        return "".join(lines[:o + 1] + ["  symbol_addrs_path:\n", "    - %s\n" % path] + lines[o + 1:])
    e = k + 1
    while e < len(lines) and (lines[e].startswith("    - ") or lines[e].startswith("    #")):
        e += 1
    return "".join(lines[:e] + ["    - %s\n" % path] + lines[e:])


def hypotheses_path(path):
    """config/x.txt -> config/x_hypotheses.txt."""
    return path[:-4] + HYP_SUFFIX + ".txt"


def is_hypotheses_file(path):
    return path.endswith(HYP_SUFFIX + ".txt") or path.endswith("symbol_addrs_obin.txt")


class SymbolTable:
    """All symbol files of all units of a tree. Rebuild it after writing a symbol file."""

    def __init__(self, root=".", texts=None):
        """texts: optional {repo path: text} that overrides files on disk (for planning)."""
        self.root = root
        self.texts = dict(texts or {})
        self.units = {u.name: u for u in read_units(root)}
        self.order = [u for u in self.units]
        self.files = {}
        for u in self.units.values():
            files = yaml_symbol_files(self.read(u.yaml) or "")
            if u.name == "main" and os.path.exists(os.path.join(root, TYPES_FILE)):
                files = files + [TYPES_FILE]
            self.files[u.name] = files
        self.lines = {}
        for files in self.files.values():
            for p in files:
                if p not in self.lines:
                    self.lines[p] = parse_lines(self.read(p) or "", p)
        self.loaders = {}
        for unit, files in self.files.items():
            for p in files:
                self.loaders.setdefault(p, []).append(unit)

    def read(self, path):
        if path in self.texts:
            return self.texts[path]
        full = os.path.join(self.root, path)
        if not os.path.exists(full):
            return None
        with open(full, encoding="utf-8", errors="surrogateescape") as f:
            return f.read()

    # -------------------------------------------------------------- scope

    def owner(self, unit, addr):
        """`unit` when addr lies in its file range, else 'main'."""
        u = self.units[unit]
        if unit != "main" and u.base <= addr < u.end:
            return unit
        return "main"

    def in_overlay_range(self, addr, unit):
        u = self.units[unit]
        return unit != "main" and u.base <= addr < u.end

    def units_seeing_main(self, addr):
        """Units in which addr is a main address (main itself, and every overlay whose range misses it)."""
        return [u for u in self.order if self.owner(u, addr) == "main"]

    # -------------------------------------------------------------- lookups

    def unit_lines(self, unit):
        out = []
        for p in self.files[unit]:
            out += self.lines.get(p, [])
        return out

    def owned_lines(self, owner):
        """Lines naming addresses of `owner` in any file of a unit that sees them as `owner`'s."""
        out, seen = [], set()
        for unit in self.order:
            for l in self.unit_lines(unit):
                if self.owner(unit, l.addr) == owner and (l.path, l.lineno) not in seen:
                    seen.add((l.path, l.lineno))
                    out.append(l)
        return out

    def lines_at(self, owner, addr):
        return [l for l in self.owned_lines(owner) if l.addr == addr]

    def names_at(self, owner, addr):
        """Names given to (owner, addr) by the symbol files, in file order, without repeats."""
        out = []
        for l in self.lines_at(owner, addr):
            if l.name not in out:
                out.append(l.name)
        return out

    def resolve(self, unit, name):
        """(owner, address) of `name` as seen from `unit`, or None. Placeholders carry their address;
        other names come from the unit's own symbol files, then from main's."""
        a = placeholder_addr(name)
        if a is not None:
            return self.owner(unit, a), a
        for l in self.unit_lines(unit):
            if l.name == name:
                return self.owner(unit, l.addr), l.addr
        if unit != "main":
            for l in self.unit_lines("main"):
                if l.name == name and self.owner(unit, l.addr) == "main":
                    return "main", l.addr
        return None

    def current_name(self, owner, addr, kind_prefix="func"):
        """The name (owner, addr) has now: the first symbol-file name, else the placeholder."""
        names = [n for n in self.names_at(owner, addr) if placeholder_addr(n) is None]
        if names:
            return names[0]
        names = self.names_at(owner, addr)
        return names[0] if names else "%s_%08X" % (kind_prefix, addr)

    def all_names(self, unit):
        """{name: address} of every name a unit's symbol files hold."""
        return {l.name: l.addr for l in self.unit_lines(unit)}

    # -------------------------------------------------------------- placement

    def placement(self, owner, addr, hypothesis=False):
        """Symbol file for a new line naming (owner, addr) (see the module docstring)."""
        if owner != "main":
            path = "config/symbols_%s.txt" % owner
        elif any(self.in_overlay_range(addr, u) for u in self.order):
            path = MAIN_ONLY_FILE
        else:
            path = SHARED_FILE
        return hypotheses_path(path) if hypothesis else path

    def yamls_for(self, path, owner):
        """Yaml files that must list a placement file: the owner's for an overlay or main-only file,
        every yaml for the shared files."""
        if owner != "main":
            return [self.units[owner].yaml]
        if path in (MAIN_ONLY_FILE, hypotheses_path(MAIN_ONLY_FILE)):
            return [EXE_YAML]
        return [u.yaml for u in self.units.values()]

    def main_link_files(self):
        """Main-exe symbol files that the overlays do not load: tools/syms_to_ld.py inputs for
        build/main_names.ld, so overlay C can use those names."""
        shared = set.intersection(*[set(self.files[u]) for u in self.order if u != "main"]) \
            if len(self.order) > 1 else set()
        return [p for p in self.files["main"] if p not in shared]

    # -------------------------------------------------------------- consistency

    def problems(self):
        """Consistency findings (module docstring), one string each."""
        out = []
        for unit in self.order:
            by_addr, by_name, by_lower = {}, {}, {}
            for l in self.unit_lines(unit):
                by_addr.setdefault(l.addr, [])
                if all(o.name != l.name for o in by_addr[l.addr]):
                    by_addr[l.addr].append(l)
                by_name.setdefault(l.name, set()).add(l.addr)
                by_lower.setdefault(l.name.lower(), set()).add(l.name)
            for addr, ls in sorted(by_addr.items()):
                named = [l for l in ls if not l.alias]
                if len(named) > 1:
                    out.append("%s: 0x%08X has %d names (%s); keep one" % (
                        unit, addr, len(named), ", ".join("%s in %s:%d" % (l.name, l.path, l.lineno)
                                                          for l in named)))
            for name, addrs in sorted(by_name.items()):
                if len(addrs) > 1:
                    out.append("%s: %s names %d addresses (%s)" % (
                        unit, name, len(addrs), ", ".join("0x%08X" % a for a in sorted(addrs))))
            for low, names in sorted(by_lower.items()):
                if len(names) > 1:
                    out.append("%s: names differ only in case: %s" % (unit, ", ".join(sorted(names))))
        for path, units in sorted(self.loaders.items()):
            if len(units) < 2:
                continue
            for l in self.lines.get(path, []):
                bad = [u for u in units if self.owner(u, l.addr) != "main"]
                if bad:
                    out.append("%s:%d: %s = 0x%08X lies in the range of %s, but %s loads this file; "
                               "move it to %s" % (path, l.lineno, l.name, l.addr, ", ".join(bad),
                                                  "every unit" if len(units) == len(self.order)
                                                  else ", ".join(units), MAIN_ONLY_FILE))
        names = {}
        for l in self.owned_lines("main"):
            if not l.alias and l.name not in names.setdefault(l.addr, []):
                names[l.addr].append(l.name)
        for addr, ns in sorted(names.items()):
            real = [n for n in ns if placeholder_addr(n) is None]
            if len(real) > 1:
                out.append("main: 0x%08X has the names %s in different units' files" % (addr, ", ".join(real)))
        return out
