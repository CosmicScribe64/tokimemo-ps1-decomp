"""Shared scanning of the game C sources and splat asm headers (T-0012).

Used by progress.py, list_leaves.py and funcdiff.py so the INCLUDE_ASM / definition / size
parsing exists once. The game code lives in src/main/<address>.c; each INCLUDE_ASM names the
folder asm/nonmatchings/main/<address> that holds the function's .s file.
"""
import re
from pathlib import Path
from typing import NamedTuple

SRC_DIR = "src/main"
INCLUDE_ASM_RE = re.compile(r'^\s*INCLUDE_ASM\(\s*"([^"]+)"\s*,\s*(\w+)\s*\)', re.M)
DEF_RE = re.compile(r'^[A-Za-z_][\w \t*]*[ \t*](\w+)\(.*\)\s*\{', re.M)
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
    return set(DEF_RE.findall(path.read_text()))


def nonmatching_size(asm_path):
    """(name, size) from the splat `nonmatching <name>, <size>` header of a .s file, or None."""
    m = NONMATCHING_RE.search(Path(asm_path).read_text(errors="replace")[:400])
    return (m.group(1), int(m.group(2), 0)) if m else None
