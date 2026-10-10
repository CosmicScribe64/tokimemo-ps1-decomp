"""Shared scanning of the game C sources and splat asm headers (T-0012, T-0500).

Used by configure.py, progress.py, queue.py, dupes.py, funcdiff.py, cc.py and others so the C file
list, the INCLUDE_ASM / definition / size parsing and the source-to-asm path rule exist once.

C files are the `c` subsegments of the splat configs (config/SLPM_86.053.yaml for the main exe,
config/overlays/<NAME>.yaml per overlay; c_files()). A subsegment name N maps to
  main exe: src/N.c (N = main/<address>), asm/{nonmatchings,matchings}/N, build/src/N.o
  overlay:  src/ovl/N.c (N = <NAME> or <NAME>/<address>, one file per original object, T-0500),
            asm/ovl/<NAME>/{nonmatchings,matchings}/N, build/ovl/<NAME>/src/ovl/N.o
A dot-prefixed `.rodata` subsegment with the same name is the file's rodata island (T-1340).
"""
import os
import re
from pathlib import Path
from typing import NamedTuple

SRC_DIR = "src/main"
INCLUDE_ASM_RE = re.compile(r'^\s*INCLUDE_ASM\(\s*"([^"]+)"\s*,\s*(\w+)\s*\)', re.M)
DEF_RE = re.compile(r'^[A-Za-z_][\w \t*]*[ \t*](\w+)\(.*\)\s*\{', re.M)
# K&R definition: `void f(a, b)` + parameter declarations + `{` on a later line (T-3300).
KR_DEF_RE = re.compile(
    r'^[A-Za-z_][\w \t*]*[ \t*](\w+)\([ \t]*(?:\w+[ \t]*(?:,[ \t]*\w+[ \t]*)*)?\)[ \t]*\n'
    r'(?:[ \t]*[A-Za-z_][^;{}()\n]*;[ \t]*\n)*[ \t]*\{', re.M)
NONMATCHING_RE = re.compile(r'^nonmatching\s+(\w+),\s*(0x[0-9A-Fa-f]+|\d+)', re.M)


class IncludeAsm(NamedTuple):
    folder: str   # path inside the INCLUDE_ASM macro
    name: str     # function name
    file: str     # stem of the src/main file, e.g. 80041000


def source_files(root="."):
    """Sorted src/main/*.c paths under `root`."""
    return sorted((Path(root) / SRC_DIR).glob("*.c"))


def include_asm_entries(path):
    """IncludeAsm entries (functions still in assembly) of one C file."""
    return [IncludeAsm(f, n, path.stem) for f, n in INCLUDE_ASM_RE.findall(path.read_text())]


def defined_functions(path):
    """Names of the functions with a C body in one C file."""
    text = path.read_text()
    return set(DEF_RE.findall(text)) | set(KR_DEF_RE.findall(text))


def nonmatching_size(asm_path):
    """(name, size) from the splat `nonmatching <name>, <size>` header of a .s file, or None."""
    m = NONMATCHING_RE.search(Path(asm_path).read_text(errors="replace")[:400])
    return (m.group(1), int(m.group(2), 0)) if m else None


EXE_YAML = "config/SLPM_86.053.yaml"


class CFile(NamedTuple):
    unit: str        # "main" or the overlay name
    name: str        # splat subsegment name: main/80041000, RENSYU or RENSYU/80132000
    src: Path        # the C source
    nonmatchings: Path
    matchings: Path
    obj: str         # built object, relative to the root
    island: bool     # the file has a `.rodata` island subsegment
    start: int       # vram of the first function

    @property
    def label(self):
        """Short name for tables and --files filters: 80041000, RENSYU, RENSYU/80132000."""
        return self.name[len("main/"):] if self.unit == "main" else self.name


def subsegments(cfg):
    """[(offset, type, name)] of the subsegments of every code segment of a parsed splat config,
    and the segment's vram minus its file offset as a dict {name: delta} (main/overlays differ)."""
    out = []
    for seg in cfg["segments"]:
        if not isinstance(seg, dict):
            continue
        delta = seg.get("vram", 0) - seg.get("start", 0)
        for sub in seg.get("subsegments", []):
            if isinstance(sub, dict):
                off, typ, name = sub["start"], sub["type"], sub.get("name")
            else:
                off, typ, name = sub[0], sub[1], (sub[2] if len(sub) > 2 else None)
            out.append((off, typ, name, delta))
    return out


def _load_yaml(path):
    import yaml
    with open(path) as f:
        return yaml.safe_load(f)


def overlay_names(root="."):
    """Overlay names in config/overlays.txt order."""
    out = []
    with open(Path(root) / "config/overlays.txt") as f:
        for line in f:
            if line.strip() and not line.startswith("#"):
                out.append(line.split()[0])
    return out


def unit_c_files(unit, root="."):
    """CFile list of one unit (main or an overlay), in link order, from its splat config."""
    root = Path(root)
    path = root / (EXE_YAML if unit == "main" else "config/overlays/%s.yaml" % unit)
    subs = subsegments(_load_yaml(path))
    islands = {n for _o, t, n, _d in subs if t == ".rodata"}
    out = []
    for off, typ, name, delta in subs:
        if typ != "c":
            continue
        if unit == "main":
            base, src = "asm", Path("src") / (name + ".c")
            obj = "build/src/%s.o" % name
        else:
            base, src = "asm/ovl/%s" % unit, Path("src/ovl") / (name + ".c")
            obj = "build/ovl/%s/src/ovl/%s.o" % (unit, name)
        out.append(CFile(unit, name, root / src, root / base / "nonmatchings" / name,
                         root / base / "matchings" / name, obj, name in islands, off + delta))
    return out


def c_files(root=".", units=None):
    """CFile list of the main exe and all overlays (or only `units`)."""
    out = []
    for u in ["main"] + overlay_names(root):
        if units is None or u in units:
            out += unit_c_files(u, root)
    return out


def asm_dirs_of_source(src):
    """[matchings dir, nonmatchings dir] of a C source path (relative or absolute, any layout),
    by the path rule in the module docstring; [] for files outside src/main and src/ovl."""
    parts = Path(os.path.normpath(str(src))).with_suffix("").parts
    if "src" not in parts:
        return []
    rel = parts[len(parts) - 1 - parts[::-1].index("src") + 1:]
    if len(rel) >= 2 and rel[0] == "main":
        return ["asm/%s/%s" % (k, "/".join(rel)) for k in ("matchings", "nonmatchings")]
    if len(rel) >= 2 and rel[0] == "ovl":
        name = "/".join(rel[1:])
        return ["asm/ovl/%s/%s/%s" % (rel[1], k, name) for k in ("matchings", "nonmatchings")]
    return []
