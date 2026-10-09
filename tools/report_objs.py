#!/usr/bin/env python3
"""Prepare the objdiff project that feeds the decomp.dev progress report (T-0902).

Usage (inside Docker, after a full `ninja`):
    python3 tools/report_objs.py [out_dir]          # default build/report

The build links the original bytes whether a function is C or `INCLUDE_ASM`, so
comparing the build against itself would call every function matched. This
script writes two copies of every unit object listed in objdiff.json:

  <out>/target/<unit>.o   the object as built (the original code)
  <out>/base/<unit>.o     the same object with the body of every function that
                          is still `INCLUDE_ASM` in the unit's C file overwritten
                          with 0xFF bytes, which no real instruction matches

and an objdiff.json next to them. `objdiff-cli report generate -p <out>` then
counts a function as matched only when its C compiles to the original bytes.
Only the object's .text bytes change; symbols, sizes and relocations stay.
"""
import json
import os
import re
import shutil
import struct
import sys

INCLUDE_ASM_RE = re.compile(r'INCLUDE_ASM\s*\(\s*"[^"]*"\s*,\s*(\w+)\s*\)')
SHT_SYMTAB = 2
STT_FUNC = 2


def _sections(data):
    """(name, type, offset, size, link) for every section of a 32-bit little-endian ELF."""
    if data[:4] != b"\x7fELF" or data[4] != 1 or data[5] != 1:
        raise ValueError("not a 32-bit little-endian ELF object")
    shoff, = struct.unpack_from("<I", data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x2E)
    raw = []
    for i in range(shnum):
        name, typ, _flags, _addr, off, size, link, _info, _al, _es = \
            struct.unpack_from("<IIIIIIIIII", data, shoff + i * shentsize)
        raw.append((name, typ, off, size, link))
    stroff = raw[shstrndx][2]

    def cstr(base, idx):
        end = data.index(b"\0", base + idx)
        return data[base + idx:end].decode("ascii")

    return [(cstr(stroff, n), t, o, s, l) for n, t, o, s, l in raw], cstr


def undecompiled_names(c_path):
    """Names of the functions a C file still includes from assembly."""
    with open(c_path, encoding="utf-8", errors="replace") as f:
        return set(INCLUDE_ASM_RE.findall(f.read()))


def blank_functions(obj, names):
    """Return (patched bytes, number of functions overwritten)."""
    data = bytearray(obj)
    secs, cstr = _sections(obj)
    text_idx = next(i for i, s in enumerate(secs) if s[0] == ".text")
    text_off, text_size = secs[text_idx][2], secs[text_idx][3]
    sym = next(s for s in secs if s[1] == SHT_SYMTAB)
    str_off = secs[sym[4]][2]
    done = set()
    for i in range(sym[3] // 16):
        st_name, value, size, info, _other, shndx = struct.unpack_from("<IIIBBH", obj, sym[2] + i * 16)
        if shndx != text_idx or info & 0xF != STT_FUNC or size == 0:
            continue
        name = cstr(str_off, st_name)
        if name in names and name not in done:
            if value + size > text_size:
                raise ValueError("%s runs past .text" % name)
            data[text_off + value:text_off + value + size] = b"\xff" * size
            done.add(name)
    return bytes(data), len(done)


def main():
    out = sys.argv[1] if len(sys.argv) > 1 else "build/report"
    with open("objdiff.json") as f:
        cfg = json.load(f)
    shutil.rmtree(out, ignore_errors=True)
    units = []
    for u in cfg["units"]:
        built = u["base_path"]
        if not os.path.exists(built):
            sys.exit("report_objs.py: %s is missing, run a full ninja first" % built)
        with open(built, "rb") as f:
            obj = f.read()
        src = u["metadata"]["source_path"]
        patched, n = blank_functions(obj, undecompiled_names(src))
        for kind, blob in (("target", obj), ("base", patched)):
            dest = os.path.join(out, kind, u["name"] + ".o")
            os.makedirs(os.path.dirname(dest), exist_ok=True)
            with open(dest, "wb") as f:
                f.write(blob)
        nu = dict(u)
        nu["target_path"] = "target/%s.o" % u["name"]
        nu["base_path"] = "base/%s.o" % u["name"]
        units.append(nu)
    rep = {"$schema": cfg["$schema"], "build_target": False, "build_base": False,
           "progress_categories": cfg.get("progress_categories", []), "units": units}
    with open(os.path.join(out, "objdiff.json"), "w") as f:
        json.dump(rep, f, indent=2)
        f.write("\n")
    print("report_objs.py: %d units -> %s" % (len(units), out))


if __name__ == "__main__":
    main()
