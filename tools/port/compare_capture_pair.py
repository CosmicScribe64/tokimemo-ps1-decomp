#!/usr/bin/env python3
"""T-39008: exact capture-pair diagnostics, never resize or infer synchronization.

Exit 0: declared checkpoint/synchronization and compared fields/pixels agree.
Exit 1: observed differences or unverified/incompatible comparison semantics.
Exit 2: missing, malformed or changed evidence. Identity is not producer truth.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys

from PIL import Image

try:
    from tools.port.native_memory_projection import project_sample
except ModuleNotFoundError:
    from native_memory_projection import project_sample


def select(value, path):
    if not isinstance(path, list):
        raise ValueError("record selectors must be explicit path arrays")
    for key in path:
        value = value[key]
    return value


def pinned(root, spec):
    if not isinstance(spec, dict):
        raise ValueError("evidence reference must be an object")
    path = root / spec["path"]
    if not path.is_file() or path.stat().st_size > 64 * 1024 * 1024:
        raise ValueError("missing or oversized evidence: " + str(path))
    raw = path.read_bytes()
    digest = hashlib.sha256(raw).hexdigest()
    if digest != spec["sha256"]:
        raise ValueError("evidence hash changed: " + str(path))
    return path, raw


def pixels(native_path, port_path):
    with Image.open(native_path) as source, Image.open(port_path) as target:
        if source.format != "PNG" or target.format != "PNG":
            raise ValueError("capture images must be PNG")
        if max(*source.size, *target.size) > 4096 or min(*source.size, *target.size) < 1:
            raise ValueError("invalid capture image dimensions")
        result = {"native_size": list(source.size), "port_size": list(target.size),
                  "comparison": "exact decoded RGBA; no scaling, cropping, masks or tolerance"}
        if source.size != target.size:
            return dict(result, status="dimension_mismatch", differing_pixels=None)
        a, b = source.convert("RGBA"), target.convert("RGBA")
        count, maximum, tiles = 0, 0, {}
        for index, (left, right) in enumerate(zip(a.getdata(), b.getdata())):
            if left != right:
                count += 1
                maximum = max(maximum, max(abs(x-y) for x, y in zip(left, right)))
                tile = (index % a.width // 16 * 16, index // a.width // 16 * 16)
                tiles[tile] = tiles.get(tile, 0) + 1
        return dict(result, status="different" if count else "equal", differing_pixels=count,
                    max_channel_difference=maximum,
                    changed_tiles=[{"x": x, "y": y, "pixels": count} for (x, y), count in sorted(tiles.items())])


def compare(pair, root):
    if not isinstance(pair, dict):
        raise ValueError("pair manifest must be an object")
    if pair.get("schema") != "tokimemo.capture-pair.v1":
        raise ValueError("unsupported pair manifest")
    records, paths, pins = {}, {}, {}
    for side in ("native", "port"):
        spec = pair[side]
        if not isinstance(spec, dict):
            raise ValueError("capture side must be an object")
        record_path, raw = pinned(root, spec["record"])
        records[side] = select(json.loads(raw), spec["selector"])
        if not isinstance(records[side], dict):
            raise ValueError("selected capture record must be an object")
        paths[side], _ = pinned(root, spec["image"])
        pins[side] = {"record": spec["record"], "image": spec["image"]}
    native, port = records["native"], records["port"]
    image_status = native.get("image_status")
    if image_status == "ok":
        if native.get("png") != paths["native"].name or native.get("png_sha256") != pair["native"]["image"]["sha256"]:
            raise ValueError("native PNG receipt identity differs")
    elif image_status is not None:
        raise ValueError("native GPU image is unavailable")
    elif not pair["native"].get("legacy_image_validation"):
        raise ValueError("legacy PNG requires explicit independent validation declaration")
    if Path(native["path"]).stem != paths["native"].stem:
        raise ValueError("native image does not belong to selected shot")
    if select(port, pair["port"]["image_sha_path"]) != pair["port"]["image"]["sha256"]:
        raise ValueError("port image is not bound to selected record")
    mapping = pair["native_map"]
    if not isinstance(mapping, dict):
        raise ValueError("native map must be an object")
    map_digest = hashlib.sha256(json.dumps(mapping, sort_keys=True).encode()).hexdigest()
    projected = project_sample(native, mapping, map_digest)
    fields = projected["fields"]
    semantic, blocked = [], []
    if image_status is None:
        blocked.append("legacy PNG decoded and hash checked; original raw conversion receipt unavailable")
    mappings = pair["mappings"]
    if not isinstance(mappings, list) or not mappings:
        raise ValueError("explicit semantic mappings are required")
    names = set()
    for item in mappings:
        if not isinstance(item, dict):
            raise ValueError("semantic mapping must be an object")
        name = item["name"]
        if not isinstance(name, str) or not name or name in names or not item.get("meaning"):
            raise ValueError("mapping needs unique name and source meaning")
        names.add(name)
        left, right = fields[item["native_field"]], select(port, item["port_path"])
        compatibility = item["compatibility"]
        if compatibility not in ("equivalent", "incompatible"):
            raise ValueError("mapping must declare semantic compatibility")
        if compatibility == "incompatible" and not item.get("reason"):
            raise ValueError("incompatible mapping needs reason")
        # JSON numbers remain exact declared types: no bool/int or float/int
        # coercion, including integral floats. Producers must emit native ints.
        if compatibility == "equivalent" and type(right) is not type(left):
            raise ValueError("equivalent field has incompatible value type: " + name)
        status = "incompatible" if compatibility == "incompatible" else ("equal" if left == right else "different")
        semantic.append(dict(item, native=left, port=right, status=status))
    checkpoint = pair["checkpoint"]
    if not isinstance(checkpoint, dict) or not checkpoint.get("name") or not isinstance(checkpoint.get("expected"), dict) or not checkpoint["expected"]:
        raise ValueError("explicit checkpoint identity and expected values required")
    for key, expected in checkpoint["expected"].items():
        rows = [row for row in semantic if row["name"] == key]
        if len(rows) != 1 or rows[0]["compatibility"] != "equivalent":
            raise ValueError("checkpoint must reference equivalent mapped fields")
        if type(expected) is not type(rows[0]["native"]):
            raise ValueError("checkpoint expected value type differs: " + key)
        if rows[0]["native"] != expected or rows[0]["port"] != expected:
            blocked.append("checkpoint differs: " + key)
    sync = pair["synchronization"]
    if not isinstance(sync, dict) or sync.get("status") not in ("verified", "unverified") or not sync.get("basis"):
        raise ValueError("explicit synchronization status/basis required")
    if sync["status"] == "verified":
        pinned(root, sync["evidence"])
        if native.get("state_load_phase") not in ("restored", "cold_boot"):
            blocked.append("native capture lifecycle is unavailable or before restore")
    else:
        blocked.append("checkpoint synchronization remains unverified")
    if not isinstance(pair["port"]["clock_paths"], dict):
        raise ValueError("port clock paths must be an object")
    clock = {"native": {key: native.get(key, "not_recorded") for key in
                         ("frame", "cycles", "pc", "phase", "state_load_phase")},
             "port": {key: select(port, path) for key, path in pair["port"]["clock_paths"].items()},
             "policy": "different clock domains retained verbatim; no shift or inferred equivalence"}
    if not clock["port"]:
        raise ValueError("port clock evidence required")
    pixel_report = pixels(paths["native"], paths["port"])
    if pixel_report["native_size"] != [native["width"], native["height"]]:
        raise ValueError("native shot receipt dimensions differ from PNG")
    mismatch = pixel_report["status"] != "equal" or any(row["status"] != "equal" for row in semantic) or bool(blocked)
    return {"schema": "tokimemo.capture-pair-report.v1", "status": "not_equal_or_unverified" if mismatch else "equal_declared_scope",
            "scope": pair.get("scope", "single declared capture pair only"), "evidence": pins,
            "checkpoint": checkpoint, "synchronization": sync, "acceptance_blockers": blocked,
            "synchronization_trust": "externally reviewed declaration; comparator does not prove synchronization",
            "pixels": pixel_report, "semantics": semantic, "clocks": clock}, 1 if mismatch else 0


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("pair", type=Path)
    parser.add_argument("--out", type=Path)
    args = parser.parse_args(argv)
    try:
        raw = args.pair.read_bytes()
        report, code = compare(json.loads(raw), args.pair.parent)
        report["pair_sha256"] = hashlib.sha256(raw).hexdigest()
    except (ValueError, KeyError, TypeError, IndexError, OSError) as error:
        report, code = {"status": "invalid_evidence", "error": str(error)}, 2
    rendered = json.dumps(report, sort_keys=True, indent=2) + "\n"
    if args.out:
        with args.out.open("x") as output:
            output.write(rendered)
    else:
        sys.stdout.write(rendered)
    return code


if __name__ == "__main__":
    raise SystemExit(main())
