#!/usr/bin/env python3
"""T-39300: field-by-field report of the port's game memory against an emulator RAM dump.

Both inputs are 2 MiB main-RAM images (offset = address & 0x1FFFFF): the port's from
tests/support/game_memory_dump.gd (<route-out>/game-memory/<key>.ram.bin) and the
emulator's from tools/emu/route_driver/run.sh (<run>/<checkpoint>.ram.bin) or
`tools/emu/run.sh --ram-dump`, taken at the same route key. The field names, offsets,
widths and signedness come from the layout that tools/port/gen_game_memory.py computes
from include/game/*.h and include/main/*.h, so the report names fields as the C does
(`girls[3].tokimeki`, `scene_flags[10]`).

Compared: GameState (0x800E6280, 0x1A90 bytes) field by field (a field is a top-level
member; a differing one lists the leaf paths that differ), SystemData's seed, and the
globals the bridge maps (BRIDGED_GLOBALS). `--all-globals` adds a byte-level sweep of every
generated global. Fields of the "clock" group depend on boot/title/file-select timing
the port does not run (absolute frame_count, timer_ticks, the main-loop seed); they are
listed but counted separately, as tools/port/compare_newgame_golden.py does.

Exit 0 when everything compared is equal (clock fields excepted), 1 otherwise.

usage (Docker): python3 -I tools/port/compare_game_memory.py <port.ram.bin> <emulator.ram.bin>
                [--json out.json] [--limit N] [--known known-diffs.json] [--all-globals]
"""
import argparse
import json
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import gen_game_memory as gen  # noqa: E402

RAM_SIZE = 0x200000
GAME_STATE = 0x800E6280
CLOCK_FIELDS = {"frame_count", "timer_ticks", "seed"}
BRIDGED_GLOBALS = ["schedule_last_picked_girl", "scene_work", "bg_lba_cache", "dec_bg_show", "icon_enabled"]
GLOBAL_EXTENT = {"dec_bg_show": 1, "icon_enabled": 15}      # declared without an extent in the headers


class Leaf:
    __slots__ = ("path", "address", "size", "signed", "struct", "note")

    def __init__(self, path, address, size, signed=False, struct=None, note=""):
        self.path, self.address, self.size, self.signed, self.struct, self.note = path, address, size, signed, struct, note


def has_bit_fields(parser, struct):
    return any(m.bits is not None for m in struct.members)


def struct_leaves(parser, struct, base, prefix):
    """Leaves of one struct instance: scalars, and blobs for unions and bit-field records."""
    if struct.is_union or has_bit_fields(parser, struct):
        return [Leaf(prefix or struct.name, base, struct.size, struct=struct, note="union" if struct.is_union else "bit-fields")]
    out = []
    for m in struct.members:
        out.extend(member_leaves(parser, m, base, prefix))
    return out


def member_leaves(parser, m, base, prefix):
    out = []
    name = (prefix + "." if prefix else "") + m.name
    for i in range(m.count):
        elem = base + m.offset + i * m.element_size
        suffix = "" if not m.dims else "[%d]" % i if len(m.dims) == 1 else "[%d]" % i
        path = name + suffix
        if isinstance(m.type, gen.StructRef):
            out.extend(struct_leaves(parser, parser.structs[m.type.name], elem, path))
        else:
            out.append(Leaf(path, elem, m.element_size, m.type.signed))
    return out


def read(ram, address, size, signed=False):
    off = address & 0x1FFFFF
    return int.from_bytes(ram[off:off + size], "little", signed=signed)


def fmt(value, size):
    return "0x%0*X" % (size * 2, value & ((1 << (size * 8)) - 1)) if size <= 8 else value.hex()


def blob(ram, leaf):
    off = leaf.address & 0x1FFFFF
    return bytes(ram[off:off + leaf.size])


def bit_field_names(parser, struct, port, emu, prefix=""):
    """Bit-fields that differ between two byte strings holding `struct` (unions: their record members)."""
    names = []
    for m in struct.members:
        if isinstance(m.type, gen.StructRef):
            sub = parser.structs[m.type.name]
            if struct.is_union or has_bit_fields(parser, sub) or sub.is_union:
                names.extend(bit_field_names(parser, sub, port[m.offset:], emu[m.offset:], prefix + m.name + "."))
            continue
        if m.bits is None:
            continue
        mask = (1 << m.bits) - 1
        a = int.from_bytes(port[m.offset:m.offset + m.element_size], "little") >> m.bit_shift & mask
        b = int.from_bytes(emu[m.offset:m.offset + m.element_size], "little") >> m.bit_shift & mask
        if a != b:
            names.append("%s%s port=%d emu=%d" % (prefix, m.name, a, b))
    return names


def compare_struct(parser, struct_name, base, port, emu, known):
    struct = parser.structs[struct_name]
    fields = []
    for m in struct.members:
        leaves = member_leaves(parser, m, base, "")
        differing = []
        for leaf in leaves:
            a, b = read_leaf(port, leaf), read_leaf(emu, leaf)
            if a != b:
                detail = {"path": leaf.path, "address": "0x%08X" % leaf.address, "port": fmt_leaf(a, leaf), "emulator": fmt_leaf(b, leaf)}
                if leaf.note:
                    detail["bit_fields"] = bit_field_names(parser, leaf.struct, a, b)
                differing.append(detail)
        fields.append({
            "name": m.name, "address": "0x%08X" % (base + m.offset), "bytes": m.element_size * m.count,
            "equal": not differing, "differing": differing, "clock": m.name in CLOCK_FIELDS,
            "reason": known.get("%s.%s" % (struct_name, m.name), ""),
        })
    return fields


def read_leaf(ram, leaf):
    if leaf.note:
        return blob(ram, leaf)
    return read(ram, leaf.address, leaf.size, leaf.signed)


def fmt_leaf(value, leaf):
    if isinstance(value, bytes):
        return value.hex()
    return ("%d (0x%0*X)" % (value, leaf.size * 2, value & ((1 << (leaf.size * 8)) - 1)))


def byte_diff_count(port, emu, base, size):
    o = base & 0x1FFFFF
    a, b = port[o:o + size], emu[o:o + size]
    return sum(1 for x, y in zip(a, b) if x != y)


def summarize(name, fields, base, size, port, emu):
    total = len(fields)
    counted = [f for f in fields if not f["clock"]]
    equal = sum(1 for f in counted if f["equal"])
    diff_bytes = byte_diff_count(port, emu, base, size)
    return {
        "struct": name, "base": "0x%08X" % base, "size": size,
        "bytes_equal": size - diff_bytes, "bytes_total": size,
        "fields_total": total, "fields_counted": len(counted), "fields_equal": equal,
        "fields_differing": [f["name"] for f in counted if not f["equal"]],
        "clock_fields": [{"name": f["name"], "equal": f["equal"], "differing": f["differing"]} for f in fields if f["clock"]],
    }


def compare_globals(parser, names, port, emu, known=None):
    known = known or {}
    out = []
    for name in names:
        g = parser.globals.get(name)
        if g is None or g.address is None:
            out.append({"name": name, "error": "unknown global"})
            continue
        count = 1
        for d in g.dims:
            count *= d or GLOBAL_EXTENT.get(name, 1)
        if isinstance(g.type, gen.StructRef):
            size = parser.structs[g.type.name].size
            signed = False
        else:
            size, signed = g.type.width, g.type.signed
        diffs = []
        for i in range(count):
            addr = g.address + i * size
            a, b = read(port, addr, size, signed), read(emu, addr, size, signed)
            if a != b:
                diffs.append({"index": i, "address": "0x%08X" % addr, "port": a, "emulator": b})
        out.append({"name": name, "address": "0x%08X" % g.address, "elements": count, "equal": not diffs, "differing": diffs,
                    "reason": known.get(name, "")})
    return out


def sweep_globals(parser, port, emu):
    """Byte-level sweep of every generated global (informational: the port runs no code that fills most)."""
    rows = []
    for g in sorted(parser.globals.values(), key=lambda g: (g.address or 0, g.name)):
        if g.address is None:
            continue
        g.type = gen.resolve_type(parser, g.type)
        size = parser.structs[g.type.name].size if isinstance(g.type, gen.StructRef) else g.type.width
        count = 1
        for d in g.dims:
            count *= d or GLOBAL_EXTENT.get(g.name, 1)
        total = size * count
        if g.address < GAME_STATE or g.address >= GAME_STATE + 0x1A90:
            rows.append({"name": g.name, "address": "0x%08X" % g.address, "bytes": total,
                         "differing_bytes": byte_diff_count(port, emu, g.address, total)})
    return rows


def render(report, limit):
    lines = []
    for s in report["structs"]:
        sm = s["summary"]
        lines.append("%s @%s size 0x%X: bytes equal %d/%d, fields equal %d/%d%s" % (
            sm["struct"], sm["base"], sm["size"], sm["bytes_equal"], sm["bytes_total"], sm["fields_equal"], sm["fields_counted"],
            " (%d clock fields not counted)" % len(sm["clock_fields"]) if sm["clock_fields"] else ""))
        shown = 0
        for f in s["fields"]:
            if f["equal"] or f["clock"]:
                continue
            if shown >= limit:
                lines.append("  ... %d more differing fields" % (len(sm["fields_differing"]) - shown))
                break
            shown += 1
            first = f["differing"][0]
            more = " (+%d more leaves)" % (len(f["differing"]) - 1) if len(f["differing"]) > 1 else ""
            bits = " bit-fields: " + ", ".join(first["bit_fields"]) if first.get("bit_fields") else ""
            reason = "  [" + f["reason"] + "]" if f["reason"] else ""
            lines.append("  %s %s: %s port=%s emu=%s%s%s%s" % (f["address"], f["name"], first["path"], first["port"], first["emulator"], more, bits, reason))
        for c in sm["clock_fields"]:
            lines.append("  clock %s: %s" % (c["name"], "equal" if c["equal"] else "; ".join("port=%s emu=%s" % (d["port"], d["emulator"]) for d in c["differing"])))
    lines.append("globals:")
    for g in report["globals"]:
        if "error" in g:
            lines.append("  %s: %s" % (g["name"], g["error"]))
        elif g["equal"]:
            lines.append("  %s @%s: equal (%d elements)" % (g["name"], g["address"], g["elements"]))
        else:
            d = g["differing"][0]
            lines.append("  %s @%s: %d of %d elements differ, first [%d] port=%d emu=%d%s" % (
                g["name"], g["address"], len(g["differing"]), g["elements"], d["index"], d["port"], d["emulator"],
                "  [" + g["reason"] + "]" if g.get("reason") else ""))
    if "sweep" in report:
        different = [r for r in report["sweep"] if r["differing_bytes"]]
        lines.append("global sweep (outside GameState): %d of %d globals differ, %d bytes" % (
            len(different), len(report["sweep"]), sum(r["differing_bytes"] for r in different)))
    return "\n".join(lines)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("port_ram", type=Path)
    ap.add_argument("emulator_ram", type=Path)
    ap.add_argument("--json", type=Path)
    ap.add_argument("--limit", type=int, default=40, help="differing fields to print per struct")
    ap.add_argument("--known", type=Path, help="JSON {\"GameState.field\": \"reason\"} printed beside differing fields")
    ap.add_argument("--all-globals", action="store_true")
    args = ap.parse_args(argv)
    port = args.port_ram.read_bytes()
    emu = args.emulator_ram.read_bytes()
    for label, data in (("port", port), ("emulator", emu)):
        if len(data) != RAM_SIZE:
            print("%s RAM image is %d bytes, expected %d" % (label, len(data), RAM_SIZE), file=sys.stderr)
            return 2
    known = json.loads(args.known.read_text()) if args.known else {}
    parser = gen.load_model()
    state = parser.structs["GameState"]
    fields = compare_struct(parser, "GameState", GAME_STATE, port, emu, known)
    sysdata_base = parser.globals["system_data"].address
    sysdata = compare_struct(parser, "SystemData", sysdata_base, port, emu, known)
    # SystemData is 0xEE0 bytes of mostly save/album data the port never builds; the seed is the part the bridge maps.
    seed = [f for f in sysdata if f["name"] == "seed"]
    report = {
        "port_ram": str(args.port_ram), "emulator_ram": str(args.emulator_ram),
        "structs": [
            {"summary": summarize("GameState", fields, GAME_STATE, state.size, port, emu), "fields": fields},
            {"summary": summarize("SystemData.seed", seed, sysdata_base, 4, port, emu), "fields": seed},
        ],
        "globals": compare_globals(parser, BRIDGED_GLOBALS, port, emu, known),
        "system_data_summary": summarize("SystemData", sysdata, sysdata_base, parser.structs["SystemData"].size, port, emu),
    }
    if args.all_globals:
        report["sweep"] = sweep_globals(parser, port, emu)
    print(render(report, args.limit))
    sd = report["system_data_summary"]
    print("SystemData (all %d fields, informational): bytes equal %d/%d, fields equal %d/%d" % (
        sd["fields_total"], sd["bytes_equal"], sd["bytes_total"], sd["fields_equal"], sd["fields_counted"]))
    if args.json:
        args.json.write_text(json.dumps(report, indent=1) + "\n")
    bad = report["structs"][0]["summary"]["fields_differing"] or any(not g.get("equal", False) for g in report["globals"])
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
