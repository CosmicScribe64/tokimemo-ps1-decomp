"""Resolve literal background reads to original input extents, without guesses.

T-28002/T-28004. The background reader accepts two layers whose decoder body
sets 320x88 and 320x160 output rectangles. Only literal read calls are mapped
here; table-driven or computed LBAs remain unresolved. Run in Docker.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

if __package__ in (None, ""):
    sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from assets.manifest import write_manifest

SCHEMA = "tokimemo.background-context.v1"
_C_NONCODE = re.compile(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'', re.S)
_NUMBER = r"(0[xX][0-9a-fA-F]+|0|[1-9][0-9]*)[uUlL]*"
_READ = re.compile(r"\bdec_bg_cd_read\s*\(\s*" + _NUMBER + r"\s*,\s*" + _NUMBER + r"\s*\)")
_RECT = re.compile(r"\bfunc_800536AC\s*\(\s*" + _NUMBER + r"\s*,\s*" + _NUMBER
                   + r"\s*,\s*" + _NUMBER + r"\s*,\s*" + _NUMBER + r"\s*\)")
_WRAPPER = re.compile(r"\bdec_bg_decode_to_vram\s*\(\s*" + _NUMBER + r"\s*\)")
_SETUP = re.compile(r"\bfunc_80053650\s*\(\s*" + _NUMBER + r"\s*,\s*" + _NUMBER
                    + r"\s*,\s*" + _NUMBER + r"\s*\)")
_RAW_READ = re.compile(r"\b(cd_read_sectors(?:_nowait)?)\s*\(\s*" + _NUMBER
                       + r"\s*,\s*" + _NUMBER + r"\s*,\s*" + _NUMBER + r"\s*\)")
_READ_BOUNDARY = re.compile(r"\bdec_bg_cd_read\s*\(")
_RAW_READ_BOUNDARY = re.compile(r"\bcd_read_sectors(?:_nowait)?\s*\(")
_READ_PROTOCOL = re.compile(r"\bcd_read_sectors(?:_nowait)?\s*\(\s*" + _NUMBER
                            + r"\s*,\s*" + _NUMBER + r"\s*,\s*([A-Za-z_]\w*)\s*\)")


def _balanced(code, opening):
    depth = 1
    for end in range(opening + 1, len(code)):
        if code[end] == "{":
            depth += 1
        elif code[end] == "}":
            depth -= 1
            if depth == 0:
                return end
    raise ValueError("unterminated C body")


def _functions(code):
    # Same ANSI/K&R declaration guards as tools/rename.py:function_spans,
    # applied to all names in one pass instead of rescanning a large C object
    # once per function. Comments and literals have already been blanked.
    functions = []
    pattern = r"^[A-Za-z_][\w \t*]*[ \t*]([A-Za-z_]\w*)[ \t]*\("
    for match in re.finditer(pattern, code, re.M):
        depth, end = 1, match.end()
        while end < len(code) and depth:
            if code[end] == "(":
                depth += 1
            elif code[end] == ")":
                depth -= 1
            end += 1
        opening = code.find("{", end)
        between = code[end:opening] if opening >= 0 else ";"
        if (depth or opening < 0 or re.match(r"^\s*;", between)
                or not re.match(r"^(?:\s*[A-Za-z_][^;{}()]*;)*\s*$", between)):
            continue
        closing = _balanced(code, opening)
        functions.append({"name": match.group(1), "start": match.start(), "end": closing,
                          "opening": opening, "body": code[opening + 1:closing]})
    return functions


def _rectangles(code, function, file):
    return [{"file": file, "function": function["name"],
             "line": code.count("\n", 0, function["opening"] + 1 + match.start()) + 1,
             "width": _number(match.group(3)), "height": _number(match.group(4))}
            for match in _RECT.finditer(function["body"])]


def _read_window(function, start, end, target=None):
    """Limit decoder evidence to after this read and before the next replacing read.

    This is a lexical association, not a C control-flow analysis. A raw read
    into a known different buffer does not replace another raw read's input.
    For wrapped reads the target is unspecified, so every raw read is a boundary.
    """
    body = function["body"]
    stops = [match.start() for match in _READ_BOUNDARY.finditer(body) if match.start() > start]
    for match in _RAW_READ_BOUNDARY.finditer(body):
        if match.start() <= start:
            continue
        replacement = _read_target(body, match.end())
        if target is None or replacement is None or replacement == target:
            stops.append(match.start())
    stop = min(stops) if stops else len(body)
    return dict(function, opening=function["opening"] + end, body=body[end:stop])


def _read_target(body, opening_end):
    """Read only a literal second argument; unknown destinations stop attribution."""
    depth, argument_start, arguments = 1, opening_end, []
    for index in range(opening_end, len(body)):
        char = body[index]
        if char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                break
        elif char == "," and depth == 1:
            arguments.append(body[argument_start:index].strip())
            argument_start = index + 1
            if len(arguments) == 2:
                match = re.fullmatch(_NUMBER, arguments[1])
                return _number(match.group(1)) if match else None
    return None


def _layer_geometry(code, function, file):
    signature = code[function["start"]:function["opening"]]
    parameter = re.search(r"\(\s*(?:[A-Za-z_]\w*\s+)*([A-Za-z_]\w*)\s*\)", signature)
    if not parameter:
        return {}, []
    layers, evidence = {}, []
    pattern = r"\bif\s*\(\s*" + re.escape(parameter.group(1)) + r"\s*==\s*([01])\s*\)\s*\{"
    for branch in re.finditer(pattern, function["body"]):
        opening = function["opening"] + 1 + branch.end() - 1
        end = _balanced(code, opening)
        branch_fn = dict(function, opening=opening, body=code[opening + 1:end])
        refs = _rectangles(code, branch_fn, file)
        shapes = {(ref["width"], ref["height"]) for ref in refs}
        if len(shapes) == 1:
            layers[int(branch.group(1))] = next(iter(shapes))
            evidence.extend(dict(ref, layer=int(branch.group(1))) for ref in refs)
    return layers, evidence


def _code_only(text):
    return _C_NONCODE.sub(lambda match: "\n" * match.group().count("\n")
                         + " " * (len(match.group()) - match.group().count("\n")), text)


def _number(text):
    return int(text, 16 if text.lower().startswith("0x") else 10)


def build_background_contexts(inventory, sources):
    """Return context evidence from {relative source path: C text} and a media manifest."""
    if not isinstance(inventory, dict) or inventory.get("schema") != "tokimemo.media.v1" or not isinstance(inventory.get("assets"), list):
        raise ValueError("expected tokimemo.media.v1 original-input inventory")
    extents = []
    for asset in inventory["assets"]:
        if (not isinstance(asset, dict) or not isinstance(asset.get("id"), str)
                or not isinstance(asset.get("source"), dict)
                or not isinstance(asset["source"].get("path"), str)
                or not isinstance(asset["source"].get("location", {}), dict)):
            raise ValueError("inventory assets require id, source path and location objects")
        source = asset.get("source", {})
        lba = source.get("location", {}).get("lba")
        size = source.get("size")
        if isinstance(lba, int) and isinstance(size, int) and size > 0:
            extents.append((lba, lba + (size + 2047) // 2048, asset))
    calls, hashes, geometry_evidence = [], {}, []
    parsed, layer_geometry, default_input_buffers, input_setup_evidence = {}, {}, set(), []
    read_protocol = []
    for name, text in sorted(sources.items()):
        hashes[name] = hashlib.sha256(text.encode("utf-8")).hexdigest()
        code = _code_only(text)
        functions = _functions(code) if any(symbol in code for symbol in
            ("dec_bg_cd_read", "func_800536AC", "cd_read_sectors", "dec_bg_decode_to_vram")) else []
        parsed[name] = (code, functions)
        for function in functions:
            if function["name"] == "dec_bg_cd_read":
                for match in _READ_PROTOCOL.finditer(function["body"]):
                    read_protocol.append({"file": name, "function": function["name"],
                        "line": code.count("\n", 0, function["opening"] + 1 + match.start()) + 1,
                        "sectors": _number(match.group(1)), "input_buffer": _number(match.group(2))})
            if function["name"] == "dec_bg_decode_to_vram":
                layers, refs = _layer_geometry(code, function, name)
                for layer, shape in layers.items():
                    if layer in layer_geometry and layer_geometry[layer] != shape:
                        raise ValueError("conflicting background layer decoder definitions")
                    layer_geometry[layer] = shape
                geometry_evidence.extend(refs)
                for match in _SETUP.finditer(function["body"]):
                    buffer = _number(match.group(3))
                    default_input_buffers.add(buffer)
                    input_setup_evidence.append({"file": name, "function": function["name"],
                        "line": code.count("\n", 0, function["opening"] + 1 + match.start()) + 1,
                        "input_buffer": buffer})
    sector_counts = {ref["sectors"] for ref in read_protocol}
    wrapper_sector_count = next(iter(sector_counts)) if len(sector_counts) == 1 else None
    for name, (code, functions) in parsed.items():
        for match in _READ.finditer(code):
            lba, layer = (_number(value) for value in match.groups())
            enclosing = [function for function in functions if function["start"] <= match.start() < function["end"]]
            refs, wrapper_layers, function_name = [], set(), None
            if len(enclosing) == 1:
                function = enclosing[0]
                function_name = function["name"]
                relative_start = match.start() - function["opening"] - 1
                relative_end = match.end() - function["opening"] - 1
                window = _read_window(function, relative_start, relative_end)
                refs = _rectangles(code, window, name)
                wrapper_layers = {_number(value.group(1)) for value in _WRAPPER.finditer(window["body"])}
            shapes = {(ref["width"], ref["height"]) for ref in refs}
            binding = "direct_decoder_in_same_function"
            if not shapes:
                if layer in wrapper_layers and layer in layer_geometry:
                    shapes = {layer_geometry[layer]}
                    refs = [ref for ref in geometry_evidence if ref["layer"] == layer]
                    binding = "layer_wrapper_in_same_function"
                else:
                    binding = "actual_decoder_unresolved"
            calls.append({"file": name, "line": code.count("\n", 0, match.start()) + 1,
                          "function": function_name, "symbol": "dec_bg_cd_read", "lba": lba,
                          "layer": layer, "read_sector_count": wrapper_sector_count,
                          "read_protocol_evidence": read_protocol, "geometry_binding": binding,
                          "decoder_evidence": refs,
                          "shape_candidates": [{"width": width, "height": height} for width, height in sorted(shapes)]})
        for function in functions:
            source_buffers = {_number(match.group(3)) for match in _SETUP.finditer(function["body"])}
            for match in _RAW_READ.finditer(function["body"]):
                symbol, sectors, target, lba = match.groups()
                sectors, target, lba = _number(sectors), _number(target), _number(lba)
                if target not in source_buffers and target not in default_input_buffers:
                    continue
                window = _read_window(function, match.start(), match.end(), target)
                refs = _rectangles(code, window, name)
                setups = {_number(value.group(3)) for value in _SETUP.finditer(window["body"])}
                if target not in setups and target not in default_input_buffers:
                    refs = []
                shapes = {(ref["width"], ref["height"]) for ref in refs}
                calls.append({"file": name, "line": code.count("\n", 0, function["opening"] + 1 + match.start()) + 1,
                              "function": function["name"], "symbol": symbol, "lba": lba,
                              "layer": None, "read_sector_count": sectors, "input_buffer": target,
                              "input_setup_scope": "same_function" if target in source_buffers else "background_decoder_default",
                              "input_setup_evidence": [] if target in source_buffers else input_setup_evidence,
                              "geometry_binding": "direct_read_setup_decoder_in_same_function" if refs else "actual_decoder_unresolved",
                              "decoder_evidence": refs,
                              "shape_candidates": [{"width": width, "height": height} for width, height in sorted(shapes)]})
    geometry_proved = set(layer_geometry) == {0, 1}
    streams, unmapped, ambiguous = {}, [], []
    for call in calls:
        matches = [(start, asset) for start, end, asset in extents if start <= call["lba"] < end]
        if not matches:
            unmapped.append(call)
            continue
        if len(matches) != 1:
            ambiguous.append(dict(call, reason="multiple source extents contain this LBA"))
            continue
        start, asset = matches[0]
        source = asset["source"]
        offset = (call["lba"] - start) * 2048
        key = (asset["id"], offset)
        if key not in streams:
            streams[key] = {"parent_asset_id": asset["id"], "source": source,
                            "source_offset": offset, "lba": call["lba"], "contexts": []}
        shape = call["shape_candidates"][0] if len(call["shape_candidates"]) == 1 else {}
        context = dict(call, width=shape.get("width"), height=shape.get("height"),
                       file_bytes_available=source["size"] - offset,
                       physical_read_bytes=call["read_sector_count"] * 2048 if call["read_sector_count"] is not None else None,
                       physical_read_exceeds_logical_file=(call["read_sector_count"] * 2048 > source["size"] - offset)
                           if call["read_sector_count"] is not None else None)
        streams[key]["contexts"].append(context)
    for stream in streams.values():
        shapes = sorted({(c["width"], c["height"]) for c in stream["contexts"] if c["width"] is not None})
        stream["shape_candidates"] = [{"width": width, "height": height} for width, height in shapes]
        unknown = any(not call["shape_candidates"] for call in stream["contexts"])
        stream["context_status"] = ("proved_single_shape_with_unresolved_uses" if len(shapes) == 1 and unknown else
                                    "proved_single_shape" if len(shapes) == 1 else
                                    "conflicting_shapes" if len(shapes) > 1 else "shape_unresolved")
    represented = {stream["parent_asset_id"] for stream in streams.values()}
    unresolved_bs = [{"parent_asset_id": asset["id"], "source": asset["source"],
                      "reason": "no literal background read resolved; computed/table-driven reads not expanded"}
                     for asset in inventory["assets"]
                     if asset["source"].get("path", "").upper().endswith(".BS") and asset["id"] not in represented]
    result = sorted(streams.values(), key=lambda stream: (stream["source"]["path"], stream["source_offset"]))
    return {"schema": SCHEMA, "original_input": inventory.get("input"),
            "geometry_body_proved": geometry_proved,
            "layer_geometry": {str(layer): {"width": shape[0], "height": shape[1]} for layer, shape in sorted(layer_geometry.items())},
            "geometry_evidence": geometry_evidence,
            "read_protocol_evidence": read_protocol,
            "default_input_setup_evidence": input_setup_evidence,
            "source_hashes": hashes, "streams": result, "unmapped_calls": unmapped,
            "ambiguous_calls": ambiguous, "unresolved_bs_sources": unresolved_bs,
            "counts": {"literal_calls": len(calls), "streams": len(result),
                       "proved_single_shape_streams": sum(s["context_status"] == "proved_single_shape" for s in result),
                       "proved_shape_with_unresolved_uses": sum(s["context_status"] == "proved_single_shape_with_unresolved_uses" for s in result),
                       "unresolved_bs_sources": len(unresolved_bs), "unmapped_calls": len(unmapped),
                       "ambiguous_calls": len(ambiguous)},
            "source_coverage_complete": False,
            "limitations": ["only literal LBAs/layers; C expressions and data tables are not evaluated",
                            "decoder evidence must follow the read and precede the next replacing read; lexical association is not a full C control-flow proof",
                            "default input-buffer bindings rely on the background setup protocol; local reset calls are identified separately",
                            "fixed sector reads can include padding or adjacent files; file ownership is only the logical extent",
                            "this report supplies caller geometry, not bitstream validity or hardware pixel equivalence"]}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--inventory", required=True)
    parser.add_argument("--source-root", default="src")
    parser.add_argument("--output", required=True)
    args = parser.parse_args(argv)
    try:
        root = Path(args.source_root)
        sources = {path.as_posix(): path.read_text(encoding="utf-8") for path in sorted(root.rglob("*.c"))}
        report = build_background_contexts(json.loads(Path(args.inventory).read_text()), sources)
        write_manifest(args.output, report)
        print(json.dumps(report["counts"], sort_keys=True))
        return 0
    except (OSError, ValueError) as error:
        print("media_background_context.py: %s" % error, file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
