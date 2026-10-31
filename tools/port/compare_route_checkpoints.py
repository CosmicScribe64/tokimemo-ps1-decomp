"""Route driver: pair the port's and the original's checkpoints of one input script by id.

Inputs
  port run    <out>/route.json, states/<id>.json, checkpoints/<id>.png
              (port/godot/tests/route/run_installed_long_route.gd)
  native run  route.json, <id>.decoded.json, <id>.ram.bin, <id>.png
              (tools/emu/route_driver/run.sh)

For every checkpoint id the native run captured, in native order, the report gives
  - whether the port reached it (the first id the port lacks is where the port route stops);
  - the state comparison of tools/port/compare_newgame_golden.py (same field projection: GameState,
    calendar, parameters, girls, names, icon and HUD globals) with the number of matching state fields
    and the first differing field;
  - the clock relation: native frame_count minus port frame_count, and its change since the first
    paired checkpoint (0 means both sides took the same number of iterations between the two). With
    fewer than two paired checkpoints there is no drift to measure and it is reported as n/a;
  - pixels: exact RGB equality of the two PNGs when both exist and have the same size, else the reason.
    The native screenshot is the display at the boundary (the frame drawn before that iteration), the
    port PNG the frame published by it, so pixel equality is expected only on settled screens.
It also prints both runs' stops and compares their unsampled pulse presses (route.json "unsampled",
presses the game did not sample after an unsampled_ok loading state): each entry is placed in the
interval after the last checkpoint that fired at or before it on its own side, and a port entry with
no native entry for the same button in the same interval is reported (port_only_unsampled). Port
entries in an interval the native run never started (its checkpoint is not in the native run) are
listed as uncompared. Exit status: 0 when every paired checkpoint matches on state fields, 1 when any
differs, the port has a checkpoint the native run lacks or a port-only unsampled press, 2 on unusable
input (a missing or truncated route.json, a checkpoint listed in route.json whose files are missing).

usage (Docker): python3 -I tools/port/compare_route_checkpoints.py <port-out> <native-run> [--json report.json]
"""
import argparse
import json
from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).resolve().parent))
import compare_newgame_golden as golden  # noqa: E402


# Every route checkpoint is at or after the first command-screen build, so compare_newgame_golden's
# a7 rules apply (the icon_enabled/menu rows are build output, not boot values, and get no boot note).
ROUTE_TAG = "a7"


class InputError(ValueError):
    pass


def load_run(path: Path, side: str) -> dict:
    route = path / "route.json"
    if not route.is_file():
        raise InputError("%s run %s has no route.json (truncated or not a route-driver run)" % (side, path))
    try:
        data = json.loads(route.read_text())
    except json.JSONDecodeError as error:
        raise InputError("%s route.json is truncated or invalid: %s" % (side, error))
    if data.get("side") != side or "checkpoints" not in data:
        raise InputError("%s route.json is not a %s tokimemo.route-run.v1 record" % (side, side))
    return data


def pixel_compare(port_png: Path, native_png: Path) -> dict:
    if not port_png.is_file() or not native_png.is_file():
        return {"status": "missing", "port": port_png.is_file(), "native": native_png.is_file()}
    try:
        from PIL import Image
    except ImportError:
        return {"status": "unavailable", "reason": "PIL is not installed"}
    a = Image.open(port_png).convert("RGB")
    b = Image.open(native_png).convert("RGB")
    if a.size != b.size:
        return {"status": "size_differs", "port": list(a.size), "native": list(b.size)}
    pa, pb = a.tobytes(), b.tobytes()
    differing = sum(1 for i in range(0, len(pa), 3) if pa[i:i + 3] != pb[i:i + 3])
    return {"status": "exact" if differing == 0 else "differs", "differing_pixels": differing,
            "total_pixels": a.size[0] * a.size[1]}


def compare_runs(port_dir: Path, native_dir: Path) -> dict:
    port = load_run(port_dir, "port")
    native = load_run(native_dir, "native")
    port_ids = [c["id"] for c in port["checkpoints"]]
    native_ids = [c["id"] for c in native["checkpoints"]]
    rows = []
    first_offset = None
    for cp in native["checkpoints"]:
        cid = cp["id"]
        decoded = native_dir / ("%s.decoded.json" % cid)
        ram_path = native_dir / ("%s.ram.bin" % cid)
        if not decoded.is_file() or not ram_path.is_file():
            raise InputError("native checkpoint %s is listed but %s or %s is missing" % (cid, decoded.name, ram_path.name))
        if cid not in port_ids:
            rows.append({"id": cid, "status": "not_reached_by_port"})
            continue
        state_path = port_dir / "states" / ("%s.json" % cid)
        if not state_path.is_file():
            raise InputError("port checkpoint %s is listed but states/%s.json is missing" % (cid, cid))
        result = golden.compare(json.loads(state_path.read_text()), json.loads(decoded.read_text()), ram_path.read_bytes(), ROUTE_TAG)
        state_rows = [r for r in result["rows"] if r["group"] == "state"]
        port_cp = port["checkpoints"][port_ids.index(cid)]
        offset = int(cp["frame_count"]) - int(port_cp["frame_count"])
        if first_offset is None:
            first_offset = offset
        rows.append({"id": cid, "status": "match" if not result["mismatches"] else "differs",
                     "state_fields": len(state_rows), "state_matching": len(state_rows) - len(result["mismatches"]),
                     "first_difference": result["mismatches"][0] if result["mismatches"] else None,
                     "mismatches": result["mismatches"],
                     "clock": {"native_frame_count": cp["frame_count"], "port_frame_count": port_cp["frame_count"],
                               "offset": offset, "offset_drift": offset - first_offset},
                     "pixels": pixel_compare(port_dir / "checkpoints" / ("%s.png" % cid), native_dir / ("%s.png" % cid))})
    extra = [cid for cid in port_ids if cid not in native_ids]
    reached = [r for r in rows if r["status"] != "not_reached_by_port"]
    if len(reached) < 2:
        for r in reached:
            r["clock"]["offset_drift"] = None  # one pair: no second point to measure drift against
    first_missing = next((r["id"] for r in rows if r["status"] == "not_reached_by_port"), None)
    unsampled = compare_unsampled(port, native)
    return {"schema": "tokimemo.route-compare.v1", "port_script": port.get("script"), "native_script": native.get("script"),
            "port_stop": {k: port.get(k) for k in ("reason", "stop", "stop_pinned", "passed", "final_key")},
            "native_stop": {k: native.get(k) for k in ("reason", "stop", "stop_pinned", "passed", "final_key", "segment_at_stop")},
            "paired": len(reached), "native_checkpoints": len(native_ids), "first_not_reached_by_port": first_missing,
            "port_only_checkpoints": extra, "checkpoints": rows,
            "clock": {"pairs": len(reached), "offset_first": reached[0]["clock"]["offset"] if reached else None,
                      "max_abs_drift": max(abs(r["clock"]["offset_drift"]) for r in reached) if len(reached) >= 2 else None},
            "unsampled": unsampled,
            "all_paired_match": all(r["status"] == "match" for r in reached)}


def _interval(checkpoints: list, frame_count: int) -> str:
    """The id of the last checkpoint that fired at or before frame_count ("start" before the first)."""
    label = "start"
    for cp in sorted(checkpoints, key=lambda c: int(c["frame_count"])):
        if int(cp["frame_count"]) > frame_count:
            break
        label = cp["id"]
    return label


def compare_unsampled(port: dict, native: dict) -> dict:
    native_ids = {c["id"] for c in native["checkpoints"]} | {"start"}
    pool = {}
    for entry in native.get("unsampled") or []:
        key = (_interval(native["checkpoints"], int(entry["frame_count"])), entry["button"])
        pool[key] = pool.get(key, 0) + 1
    port_only, uncompared, matched = [], [], 0
    for entry in port.get("unsampled") or []:
        interval = _interval(port["checkpoints"], int(entry["frame_count"]))
        row = {"interval": interval, "button": entry["button"], "port_frame_count": entry["frame_count"]}
        if interval not in native_ids:
            uncompared.append(row)
        elif pool.get((interval, entry["button"]), 0) > 0:
            pool[(interval, entry["button"])] -= 1
            matched += 1
        else:
            port_only.append(row)
    return {"port": len(port.get("unsampled") or []), "native": len(native.get("unsampled") or []), "matched": matched,
            "port_only_unsampled": port_only, "port_uncompared": uncompared}


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("port_out", type=Path)
    parser.add_argument("native_run", type=Path)
    parser.add_argument("--json", type=Path)
    args = parser.parse_args(argv)
    try:
        report = compare_runs(args.port_out, args.native_run)
    except (InputError, OSError, KeyError, ValueError) as error:
        print("compare_route_checkpoints: %s" % error, file=sys.stderr)
        return 2
    if report["port_script"] != report["native_script"]:
        print("note: scripts differ (port %s, native %s)" % (report["port_script"], report["native_script"]))
    print("port stop: %s (%s)" % (report["port_stop"]["reason"], report["port_stop"]["stop"]))
    print("native stop: %s (%s) at %s" % (report["native_stop"]["reason"], report["native_stop"]["stop"],
                                          report["native_stop"]["segment_at_stop"]))
    for row in report["checkpoints"]:
        if row["status"] == "not_reached_by_port":
            continue
        first = row["first_difference"]
        drift = row["clock"]["offset_drift"]
        print("%-16s %s %d/%d state fields; offset %d, drift %s; pixels %s%s" % (
            row["id"], row["status"], row["state_matching"], row["state_fields"], row["clock"]["offset"],
            "n/a (one pair)" if drift is None else "%+d" % drift,
            row["pixels"]["status"], "" if first is None else "; first difference %s: port %s native %s" % (
                first["field"], json.dumps(first["port"])[:60], json.dumps(first["golden"])[:60])))
    print("paired %d of %d native checkpoints; first not reached by the port: %s" % (
        report["paired"], report["native_checkpoints"], report["first_not_reached_by_port"]))
    unsampled = report["unsampled"]
    print("unsampled presses: port %d, native %d, matched %d, port-only %d, uncompared %d" % (
        unsampled["port"], unsampled["native"], unsampled["matched"], len(unsampled["port_only_unsampled"]),
        len(unsampled["port_uncompared"])))
    for row in unsampled["port_only_unsampled"]:
        print("  port-only unsampled %s at port frame_count %s (after %s)" % (row["button"], row["port_frame_count"], row["interval"]))
    if args.json:
        args.json.write_text(json.dumps(report, indent=1))
    ok = report["all_paired_match"] and not report["port_only_checkpoints"] and not unsampled["port_only_unsampled"]
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
