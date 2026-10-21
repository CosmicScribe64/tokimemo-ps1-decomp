#!/usr/bin/env python3
"""Trim compiler-only text tail padding to the configured C subsegment (T-10200).

Runs uniformly after cc.py. A source split need not start/end on a 16-byte
boundary, although IDO and cc.py round each object's text size to 16. If an
object exceeds its configured span only by that rounding, remove the surplus
zero bytes, provided no non-section symbol or relocation occupies them. All
instructions, symbol values and relocations remain unchanged. Other size
differences are left for funcdiff and the unit checksum, never padded or fitted.
This is section placement, not an instruction or compiler code-generation pass.

An object with several `.data` islands (T-25005) then has its `.data` cut into their sections
(tools/data_split.py); an object with one or none is left as it is.
"""
from pathlib import Path
import struct
import sys

import yaml

import data_pieces as dp
import data_split as ds
import object_boundaries as ob
import srcscan
from trailing_pad import Elf, read_symbols


def trim(data, extent):
    """Return ELF bytes with only provably unused alignment beyond extent removed."""
    if extent <= 0 or extent % 4:
        raise ValueError("configured text span must be positive and word-aligned")
    elf = Elf(data)
    index = elf.find(".text")
    section = elf.sections[index]
    size = len(section["data"])
    if size <= extent or size != (extent + 15) // 16 * 16:
        return data
    if any(section["data"][extent:]):
        raise ValueError("nonzero text exceeds configured span")
    for name, value, length, info, _, shndx, _ in read_symbols(elf, elf.find(".symtab")):
        if shndx == index and info & 15 != 3 and (value >= extent or value + length > extent):
            raise ValueError("symbol %s exceeds configured text span" % name)
    for sec in elf.sections:
        if sec["type"] in (4, 9) and sec["info"] == index:
            step = 12 if sec["type"] == 4 else 8
            for pos in range(0, len(sec["data"]), step):
                if struct.unpack_from("<I", sec["data"], pos)[0] >= extent:
                    raise ValueError("relocation exceeds configured text span")
    del section["data"][extent:]
    return elf.to_bytes()


def source_config(source, root="."):
    """(unit, config path, parsed config, subsegment name) of a C source."""
    source = Path(source)
    if source.is_absolute():
        source = source.relative_to(Path(root).resolve())
    parts = source.parts
    wanted = parts[2] if len(parts) > 2 and parts[:2] == ("src", "ovl") else "main"
    config = Path(root) / ("config/SLPM_86.053.yaml" if wanted == "main"
                           else "config/overlays/%s.yaml" % wanted)
    with config.open() as f:
        cfg = yaml.safe_load(f)
    name = str(source.with_suffix("").relative_to("src" if wanted == "main" else "src/ovl"))
    return wanted, config, cfg, name


def data_regions(source, root="."):
    """[(section, start symbol, size)] of the `.data` islands of a C source when it has more than
    one (T-25005), else []. The start symbol is the first label of splat's island file."""
    _unit, config, cfg, name = source_config(source, root)
    own = [i for i in dp.islands(cfg) if i.name == name]
    if len(own) < 2:
        return []
    opts = cfg["options"]
    base = Path(config).parent / opts.get("base_path", ".")
    out = []
    for isl in own:
        path = dp.island_source(str(base / opts["asm_path"]), isl)
        with open(path, encoding="utf-8") as f:
            labels, _lines = dp.parse_island(f.read())
        if not labels:
            raise ValueError("%s has no label" % path)
        out.append((isl.section, labels[0][1], isl.end - isl.start))
    return out


def source_extent(source, root="."):
    """Configured text span for a C subsegment, including original trailing nops."""
    source = Path(source)
    if source.is_absolute():
        source = source.relative_to(Path(root).resolve())
    wanted, _config, cfg, name = source_config(source, root)
    subs = srcscan.subsegments(cfg)
    for k, (off, kind, stem, _delta) in enumerate(subs):
        if kind == "c" and stem == name and k + 1 < len(subs):
            return subs[k + 1][0] - off
    # A final C-only segment may have no following data subsegment.
    for unit in (u for u in ob.units(root) if u.name == wanted):
        files = srcscan.unit_c_files(unit.name, root)
        for k, cf in enumerate(files):
            if Path(cf.src) == Path(root) / source:
                end = files[k + 1].start if k + 1 < len(files) else unit.text[1]
                return end - cf.start
    raise ValueError("no configured C subsegment for %s" % source)


def main(argv):
    if len(argv) != 3:
        sys.exit("usage: object_layout.py SOURCE.c OBJECT.o")
    path = Path(argv[2])
    try:
        before = path.read_bytes()
        after = trim(before, source_extent(argv[1]))
        regions = data_regions(argv[1])
        if regions:
            after = ds.split(after, regions)
        if after != before:
            path.write_bytes(after)
    except (ValueError, dp.PieceError, ds.SplitError) as exc:
        sys.exit("object_layout.py: %s: %s" % (argv[1], exc))


if __name__ == "__main__":
    main(sys.argv)
