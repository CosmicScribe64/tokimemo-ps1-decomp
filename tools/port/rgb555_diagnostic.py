#!/usr/bin/env python3
"""T-39008/T-39020: reversible RGB555 colour-code diagnosis; never a parity acceptance gate.

compare_capture_pair.py stays the only exact RGBA gate. This tool decodes both
images back to 5-bit colour codes under encodings the caller declares from
source (zero_extended = v<<3, bit_replicated = (v<<3)|(v>>2), or
source_proven_mixed = either form per channel) and reports what still differs.

The projection report (tokimemo.rgb555-projection-report.v1) records:

* the declared encodings and their source basis;
* the exact RGBA8 changed-pixel count, its bounding box and changed 16x16 tiles;
* the RGB555 residual count, bounding box, per-tile counts and delta histogram;
* named domain exclusions (the emulator x=0 column and y=0 row), each with its
  own pixel, exact-change and residual counts, so nothing is dropped silently;
* the sha256 of each input file and of its decoded RGBA bytes.

A channel value outside the declared encoding is not rounded or tolerated: the
report is rejected (status "rejected", exit 2) with per-side counts and the
first offending pixels. There is no tolerance, masking or phase search. The
residual numbers feed the known_diff bookkeeping of run_differential_suite.py
(T-39021); they are not parity acceptance on their own.

Exit codes: 0 report written (any residual), 2 invalid or rejected input.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys

from PIL import Image

ENCODINGS = ("zero_extended", "bit_replicated", "source_proven_mixed")
TILE = 16
EXAMPLE_LIMIT = 8
# Named domain exclusions. Each is a reviewed capture-domain artifact from
# T-39008 C1; a caller opts in by name and the report still counts its pixels.
EXCLUSIONS = {
    "emulator_x0_column": {
        "contains": lambda x, y: x == 0,
        "reason": "T-39008 C1: the native x=0 column is (0,0,0) in every PCSX-Redux shot; source of the column is undetermined",
    },
    "emulator_y0_row": {
        "contains": lambda x, y: y == 0,
        "reason": "T-39008 C1: the native y=0 row is black when gs_buffer_index (D_80129628) is 1 at the shot; emulator artifact or real output undetermined",
    },
}


class Rejected(ValueError):
    """Input is outside the declared domain; carries the machine-readable detail."""

    def __init__(self, message, detail):
        super().__init__(message)
        self.detail = detail


def decode_channel(value, encoding):
    code = value >> 3
    forms = {"zero_extended": {code << 3}, "bit_replicated": {(code << 3) | (code >> 2)},
             "source_proven_mixed": {code << 3, (code << 3) | (code >> 2)}}
    if encoding not in forms or value not in forms[encoding]:
        raise ValueError("channel is outside declared reversible RGB555 encoding")
    return code


def _decodable(encoding):
    """Lookup table value -> code (or None) for one declared encoding."""
    table = []
    for value in range(256):
        try:
            table.append(decode_channel(value, encoding))
        except ValueError:
            table.append(None)
    return table


def compare_codes(native, port, native_encoding, port_encoding):
    if native.size != port.size:
        raise ValueError("RGB555 diagnostic requires identical dimensions; no rescaling")
    changed = 0
    deltas = {}
    for a, b in zip(native.convert('RGBA').getdata(), port.convert('RGBA').getdata()):
        if a[3] != 255 or b[3] != 255:
            raise ValueError("opaque final frames required; STP/alpha cannot be inferred")
        left = tuple(decode_channel(c, native_encoding) for c in a[:3])
        right = tuple(decode_channel(c, port_encoding) for c in b[:3])
        delta = tuple(y - x for x, y in zip(left, right))
        changed += left != right
        deltas[delta] = deltas.get(delta, 0) + 1
    return {"acceptance": False, "scope": "RGB555 color-code diagnostic only; original exact RGBA gate unchanged; bit15/STP not reconstructed",
            "native_encoding": native_encoding, "port_encoding": port_encoding,
            "pixels": native.width * native.height, "different_color_codes": changed,
            "deltas": [{"rgb": list(delta), "pixels": count} for delta, count in sorted(deltas.items())]}


class _Box:
    def __init__(self):
        self.box = None
        self.count = 0
        self.tiles = {}

    def add(self, x, y):
        self.count += 1
        if self.box is None:
            self.box = [x, y, x, y]
        else:
            b = self.box
            b[0], b[1], b[2], b[3] = min(b[0], x), min(b[1], y), max(b[2], x), max(b[3], y)
        tile = (x // TILE * TILE, y // TILE * TILE)
        self.tiles[tile] = self.tiles.get(tile, 0) + 1

    def report(self):
        # bbox is inclusive [x0, y0, x1, y1]; null when nothing differs.
        return {"pixels": self.count, "bbox": self.box,
                "tiles": [{"x": x, "y": y, "pixels": n} for (x, y), n in sorted(self.tiles.items())]}


def projection_report(native, port, native_encoding, port_encoding, exclusions=()):
    """Return the projection report dict, or raise Rejected / ValueError."""
    for encoding in (native_encoding, port_encoding):
        if encoding not in ENCODINGS:
            raise ValueError("undeclared encoding: " + str(encoding))
    unknown = [name for name in exclusions if name not in EXCLUSIONS]
    if unknown or len(set(exclusions)) != len(exclusions):
        raise ValueError("unknown or repeated domain exclusion: " + ", ".join(unknown or exclusions))
    if native.size != port.size:
        raise ValueError("RGB555 projection requires identical dimensions; no rescaling")
    width, height = native.size
    left_rgba, right_rgba = native.convert("RGBA"), port.convert("RGBA")
    tables = {"native": _decodable(native_encoding), "port": _decodable(port_encoding)}
    exact, residual = _Box(), _Box()
    excluded = {name: {"pixels": 0, "exact_changed": 0, "rgb555_changed": 0} for name in exclusions}
    deltas = {}
    bad = {"native": [0, []], "port": [0, []]}
    alpha = 0
    for index, (a, b) in enumerate(zip(left_rgba.getdata(), right_rgba.getdata())):
        x, y = index % width, index // width
        if a[3] != 255 or b[3] != 255:
            alpha += 1
            continue
        codes = {}
        for side, pixel in (("native", a), ("port", b)):
            table = tables[side]
            decoded = tuple(table[c] for c in pixel[:3])
            if None in decoded:
                bad[side][0] += 1
                if len(bad[side][1]) < EXAMPLE_LIMIT:
                    bad[side][1].append({"x": x, "y": y, "rgb": list(pixel[:3])})
            codes[side] = decoded
        domain = next((name for name in exclusions if EXCLUSIONS[name]["contains"](x, y)), None)
        if domain is not None:
            row = excluded[domain]
            row["pixels"] += 1
            row["exact_changed"] += a != b
            row["rgb555_changed"] += codes["native"] != codes["port"]
            continue
        if a != b:
            exact.add(x, y)
        if codes["native"] != codes["port"]:
            residual.add(x, y)
            if None not in codes["native"] and None not in codes["port"]:
                delta = tuple(r - l for l, r in zip(codes["native"], codes["port"]))
                deltas[delta] = deltas.get(delta, 0) + 1
    if alpha:
        raise ValueError("opaque final frames required; %d pixels have alpha below 255" % alpha)
    if bad["native"][0] or bad["port"][0]:
        detail = {side: {"encoding": native_encoding if side == "native" else port_encoding,
                         "pixels_outside_encoding": count, "examples": examples}
                  for side, (count, examples) in bad.items()}
        raise Rejected("channel values outside the declared RGB555 encoding", detail)
    return {
        "schema": "tokimemo.rgb555-projection-report.v1",
        "status": "reported",
        "acceptance": False,
        "scope": "diagnostic and known_diff residual bookkeeping only; compare_capture_pair.py stays the exact RGBA gate; bit15/STP not reconstructed",
        "size": [width, height],
        "encodings": {"native": native_encoding, "port": port_encoding},
        "exact_rgba8": exact.report(),
        "rgb555_residual": dict(residual.report(),
                                deltas=[{"rgb": list(d), "pixels": n} for d, n in sorted(deltas.items())]),
        "domain_exclusions": [dict(excluded[name], name=name, reason=EXCLUSIONS[name]["reason"]) for name in exclusions],
        "compared_pixels": width * height - sum(row["pixels"] for row in excluded.values()),
        "decoded_rgba_sha256": {"native": hashlib.sha256(left_rgba.tobytes()).hexdigest(),
                                "port": hashlib.sha256(right_rgba.tobytes()).hexdigest()},
    }


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('native', type=Path)
    parser.add_argument('port', type=Path)
    for side in ('native', 'port'):
        parser.add_argument('--' + side + '-encoding', required=True, choices=list(ENCODINGS))
    parser.add_argument('--basis', required=True, help='Source proof for the declared encodings; numeric membership alone is not provenance')
    parser.add_argument('--exclude', action='append', default=[], choices=sorted(EXCLUSIONS),
                        help='named capture-domain exclusion; its pixels are still counted in the report')
    parser.add_argument('--out', type=Path, help='new report file (refuses to overwrite)')
    args = parser.parse_args(argv)
    inputs = {side: {'path': str(path), 'sha256': hashlib.sha256(path.read_bytes()).hexdigest()}
              for side, path in (('native', args.native), ('port', args.port))}
    code = 0
    try:
        with Image.open(args.native) as native, Image.open(args.port) as port:
            if native.format != "PNG" or port.format != "PNG":
                raise ValueError("capture images must be PNG")
            result = projection_report(native, port, args.native_encoding, args.port_encoding, args.exclude)
    except Rejected as error:
        result, code = {"schema": "tokimemo.rgb555-projection-report.v1", "status": "rejected",
                        "acceptance": False, "error": str(error), "outside_encoding": error.detail}, 2
    except (ValueError, OSError) as error:
        result, code = {"schema": "tokimemo.rgb555-projection-report.v1", "status": "invalid",
                        "acceptance": False, "error": str(error)}, 2
    result['basis'] = args.basis
    result['inputs'] = inputs
    rendered = json.dumps(result, sort_keys=True, indent=2) + "\n"
    if args.out:
        with args.out.open("x") as output:
            output.write(rendered)
    else:
        sys.stdout.write(rendered)
    return code


if __name__ == '__main__':
    raise SystemExit(main())
