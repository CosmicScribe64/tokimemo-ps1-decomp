#!/usr/bin/env python3
"""T-39008: decode pinned native captures; no resynchronization or parity claim."""
import argparse
import hashlib
import json
from pathlib import Path


WIDTHS = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4}


def integer(value):
    if type(value) is int:
        return value
    if isinstance(value, str):
        return int(value, 0)
    raise ValueError("expected integer or address string")


def region(address, length):
    if address < 0 or length <= 0 or address + length > 0x100000000:
        raise ValueError("invalid memory range")
    return address, address + length


def validate_map(mapping):
    if mapping.get("schema") != "tokimemo.native-memory-map.v1":
        raise ValueError("unsupported memory map")
    fields = mapping.get("fields")
    if not isinstance(fields, list) or not fields:
        raise ValueError("empty field map")
    names = set()
    for field in fields:
        name, kind = field["name"], field["type"]
        length = integer(field["length"])
        region(integer(field["address"]), length)
        if not isinstance(name, str) or not name or name in names:
            raise ValueError("empty or duplicate field name")
        names.add(name)
        if kind != "hex" and (kind not in WIDTHS or length != WIDTHS[kind]):
            raise ValueError("invalid field type/length")
    return fields


def project_sample(sample, mapping, map_sha256):
    fields = validate_map(mapping)
    for key in ("frame", "cycles", "pc", "mode"):
        if type(sample.get(key)) is not int or sample[key] < 0:
            raise ValueError("missing or invalid sample " + key)
    if not isinstance(sample.get("phase"), str) or not sample["phase"]:
        raise ValueError("missing sample phase")
    dumps = sample.get("dumps")
    if not isinstance(dumps, list) or not dumps:
        raise ValueError("missing memory dumps")
    spans = []
    for dump in dumps:
        start, end = region(integer(dump["addr"]), integer(dump["length"]))
        data = dump.get("data")
        if dump.get("status") != "ok" or not isinstance(data, str):
            raise ValueError("unreadable dump")
        if len(data) != 2 * (end - start) or any(c not in "0123456789abcdefABCDEF" for c in data):
            raise ValueError("truncated or malformed dump")
        if any(start < old_end and old_start < end for old_start, old_end, _ in spans):
            raise ValueError("ambiguous overlapping dumps")
        spans.append((start, end, bytes.fromhex(data)))
    values = {}
    for field in fields:
        start, end = region(integer(field["address"]), integer(field["length"]))
        matches = [(base, data) for base, limit, data in spans if base <= start and end <= limit]
        if len(matches) != 1:
            raise ValueError("missing contiguous dump for " + field["name"])
        base, data = matches[0]
        raw = data[start - base:end - base]
        kind = field["type"]
        values[field["name"]] = raw.hex() if kind == "hex" else int.from_bytes(raw, "little", signed=kind.startswith("s"))
    return {"schema": "tokimemo.native-memory-snapshot.v1", "map_sha256": map_sha256,
            "capture": {key: value for key, value in sample.items() if key != "dumps"}, "fields": values}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--map", type=Path, required=True)
    parser.add_argument("--samples", type=Path, required=True)
    parser.add_argument("--out", type=Path, required=True)
    args = parser.parse_args(argv)
    raw_map = args.map.read_bytes()
    mapping = json.loads(raw_map)
    digest = hashlib.sha256(raw_map).hexdigest()
    # Validate the whole run before creating output; a failed run yields no partial evidence.
    raw_samples = args.samples.read_bytes()
    sample_digest = hashlib.sha256(raw_samples).hexdigest()
    rows = [project_sample(json.loads(line), mapping, digest)
            for line in raw_samples.splitlines()]
    if not rows:
        raise ValueError("empty sample stream")
    with args.out.open("x") as output:
        for row in rows:
            row["samples_sha256"] = sample_digest
            output.write(json.dumps(row, sort_keys=True, separators=(",", ":")) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
