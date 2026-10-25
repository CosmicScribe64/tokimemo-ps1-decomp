#!/usr/bin/env python3
"""Inventory literal text in main/overlay sources and verify binary extents (T-28001).

The inventory is evidence from generated splat assembly, not a NUL-string scan.
An extent is emitted as binary-backed only when its address and bytes agree with
the selected executable/overlay. C literals are references, not proof of an
address; byte-identical candidates remain grouped as ambiguous.
"""
from __future__ import annotations

import argparse
from bisect import bisect_left, bisect_right
from functools import lru_cache
import hashlib
import json
import re
from pathlib import Path

from .text_renderers import string_argument_role

REPOSITORY = Path(__file__).resolve().parents[2]
DEFAULT_OUTPUT = REPOSITORY / ".asset-cache" / "text" / "source-inventory.json"
ASM_DIRECTIVE = re.compile(r"\.(asciz|ascii|string)\s+(\"(?:\\.|[^\"\\])*\")")
ADDR_COMMENT = re.compile(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})(?:\s+[0-9A-Fa-f]{8})?\s*\*/")
PATH_TEXT = re.compile(r"(?i)(?:cdrom:|(?:[A-Z0-9_]+/)+[A-Z0-9_.-]+(?:;1)?)")
CALL = re.compile(r"([A-Za-z_][A-Za-z_0-9]*)\s*\([^()]*$")
SYMBOL_TOKEN = re.compile(r"[A-Za-z_][A-Za-z0-9_]*")
FUNCTION_DEF_PATTERNS = (
    re.compile(r"(?m)^\s*(?:[A-Za-z_][\w\s*]*\s+)([A-Za-z_]\w*)\s*\([^;{}]*\)\s*\{"),
    re.compile(r"(?m)^\s*(?:[A-Za-z_]\w*(?:\s+\*?)*\s+)?([A-Za-z_]\w*)\s*\([^;{}]*\)"
               r"(?:\s*\n\s*(?:[A-Za-z_]\w*\s+)+[^;{}]*;\s*)+\s*\{"),
)


class InventoryError(ValueError):
    """The requested source tree or binary evidence is invalid."""


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _decoded_view(raw: bytes, nul_terminated: bool) -> dict:
    """Return a strict Shift-JIS view without discarding embedded NUL bytes."""
    payload = raw[:-1] if nul_terminated and raw.endswith(b"\0") else raw
    try:
        value = payload.decode("shift_jis", errors="strict")
        encoded = value.encode("shift_jis", errors="strict")
    except (UnicodeDecodeError, UnicodeEncodeError):
        return {"source_decoded_text": None, "source_decode_status": "invalid_shift_jis",
                "source_roundtrip": False, "source_terminal_nul_excluded": bool(nul_terminated and raw.endswith(b"\0"))}
    valid = encoded == payload and "\ufffd" not in value
    return {"source_decoded_text": value if valid else None,
            "source_decode_status": "ok" if valid else "shift_jis_roundtrip_failed",
            "source_roundtrip": valid,
            "source_terminal_nul_excluded": bool(nul_terminated and raw.endswith(b"\0"))}


def _literal_extent_candidates(site: dict) -> tuple[bytes, ...]:
    """Return byte extents permitted by literal bytes and any declared array capacity."""
    raw = bytes.fromhex(site["raw_hex"])
    if not raw:
        return ()
    capacity = site.get("source_site", {}).get("array_length")
    if capacity == len(raw):
        return (raw,)
    if capacity is not None and capacity < len(raw):
        return ()
    return (raw + b"\0",)


def _read_original(path: Path, root: Path) -> tuple[bytes | None, str | None]:
    try:
        resolved = path.resolve(strict=True)
    except FileNotFoundError:
        return None, None
    if not resolved.is_relative_to(root) or not resolved.is_file():
        return None, "source input resolves outside the selected extracted root or is not a file"
    return resolved.read_bytes(), None


def _string_bytes(literal: str) -> bytes:
    """Decode assembler/C string escapes, treating source Unicode as Shift-JIS."""
    body = literal[1:-1]
    result = bytearray()
    i = 0
    escapes = {"a": 7, "b": 8, "f": 12, "n": 10, "r": 13, "t": 9, "v": 11,
               "\\": 92, '"': 34, "'": 39, "?": 63}
    while i < len(body):
        if body[i] != "\\":
            start = i
            while i < len(body) and body[i] != "\\":
                i += 1
            try:
                result.extend(body[start:i].encode("shift_jis", errors="strict"))
            except UnicodeEncodeError as exc:
                raise InventoryError("source literal is not encodable as Shift-JIS") from exc
            continue
        i += 1
        if i >= len(body):
            raise InventoryError("trailing backslash in literal")
        char = body[i]
        if char in "01234567":
            start = i
            i += 1
            while i < len(body) and i - start < 3 and body[i] in "01234567":
                i += 1
            value = int(body[start:i], 8)
            if value > 0xFF:
                raise InventoryError("octal escape exceeds one byte")
            result.append(value)
        elif char == "x":
            i += 1
            start = i
            while i < len(body) and body[i] in "0123456789abcdefABCDEF":
                i += 1
            if i == start:
                raise InventoryError("empty hexadecimal escape")
            value = int(body[start:i], 16)
            if value > 0xFF:
                raise InventoryError("hexadecimal escape exceeds one byte")
            result.append(value)
        elif char == "\n":
            i += 1
        else:
            result.append(escapes.get(char, ord(char)))
            i += 1
    return bytes(result)


def _category(raw: bytes, context: str = "", function: str | None = None,
              call: str | None = None, argument_index: int | None = None,
              unit: str | None = None) -> tuple[str, str]:
    try:
        value = raw.decode("shift_jis", errors="strict")
    except UnicodeError:
        value = ""
    if PATH_TEXT.search(value):
        return "path", "path-shaped literal"
    if raw in (b"", b"\0"):
        return "empty_string_or_terminator", "empty string literal or terminator-only extent"
    role = string_argument_role(call, argument_index, unit)
    if role in {"format", "diagnostic"}:
        return "debug_or_format", "format/diagnostic call site"
    if function in {"get_g_name", "get_p_name"}:
        return "name", "literal inside a name-construction routine"
    if role == "display":
        return "display", "known text display call site"
    if re.search(r"\b(?:strcpy|strcat|strncat)\s*\(", context):
        return "unknown", "string manipulation; semantic use not established"
    return "unknown", "no category proved from source context"


def _mask_c_literals(text: str, tokens) -> str:
    out = list(text)
    for token in tokens:
        if token["kind"] in ("string", "character"):
            for i in range(token["start"], token["end"]):
                if out[i] != "\n":
                    out[i] = " "
    return "".join(out)


def _call_containing(code: str, position: int) -> str | None:
    calls = []
    for match in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", code):
        depth = 1
        end = match.end()
        while end < len(code) and depth:
            if code[end] == "(":
                depth += 1
            elif code[end] == ")":
                depth -= 1
            end += 1
        if depth == 0 and match.end() <= position < end - 1:
            calls.append((end - match.start(), match.group(1)))
    return min(calls)[1] if calls else None


@lru_cache(maxsize=8)
def _call_argument_index(code: str) -> tuple[tuple, tuple[int, ...], tuple[int, ...]]:
    """Cache source call argument spans once per masked C translation unit."""
    calls = []
    for match in re.finditer(r"\b([A-Za-z_]\w*)\s*\(", code):
        open_pos = match.end() - 1
        parens, brackets, braces = 1, 0, 0
        separators = []
        close_pos = None
        i = open_pos + 1
        while i < len(code):
            char = code[i]
            if char == "(": parens += 1
            elif char == ")":
                parens -= 1
                if parens == 0:
                    close_pos = i
                    break
            elif char == "[": brackets += 1
            elif char == "]": brackets = max(0, brackets - 1)
            elif char == "{": braces += 1
            elif char == "}": braces = max(0, braces - 1)
            elif char == "," and parens == 1 and brackets == 0 and braces == 0:
                separators.append(i)
            i += 1
        if close_pos is None:
            continue
        starts = [open_pos + 1, *[comma + 1 for comma in separators]]
        ends = [*separators, close_pos]
        for index, (start, end) in enumerate(zip(starts, ends)):
            if start < end:
                calls.append((start, end, close_pos - match.start(), match.start(), match.group(1), index))
    calls.sort(key=lambda item: item[0])
    starts = tuple(item[0] for item in calls)
    prefix_max_end = []
    high = -1
    for item in calls:
        high = max(high, item[1])
        prefix_max_end.append(high)
    return tuple(calls), starts, tuple(prefix_max_end)


def _call_containing_argument(code: str, position: int) -> dict | None:
    """Return the innermost call argument containing a source position."""
    calls, starts, prefix_max_end = _call_argument_index(code)
    index = bisect_right(starts, position) - 1
    best = None
    while index >= 0 and prefix_max_end[index] > position:
        start, end, call_width, call_start, name, argument_index = calls[index]
        if start <= position < end:
            candidate = (call_width, end - start, name, argument_index, start, end, call_start)
            if best is None or candidate[:2] < best[:2]:
                best = candidate
        index -= 1
    if best is None:
        return None
    _, _, name, argument_index, start, end, call_start = best
    return {"call": name, "argument_index": argument_index,
            "argument_start": start, "argument_end": end,
            "argument_expression": code[start:end], "call_start": call_start}


def _call_arguments_using_identifier(code: str, identifier: str, start: int, end: int) -> set[tuple[str, int]]:
    calls = set()
    pattern = re.compile(r"\b" + re.escape(identifier) + r"\b")
    for ident in pattern.finditer(code, start, end):
        info = _call_containing_argument(code, ident.start())
        if info:
            calls.add((info["call"], info["argument_index"]))
    return calls


def _identifier_is_value_operand(expression: str | None, symbol: str) -> bool:
    """Reject identifiers used as array indexes; AST analysis handles harder cases."""
    if not expression:
        return False
    match = re.search(r"\b" + re.escape(symbol) + r"\b", expression)
    if not match:
        return False
    depth = 0
    for char in expression[:match.start()]:
        if char == "[":
            depth += 1
        elif char == "]":
            depth = max(0, depth - 1)
    return depth == 0


def _mask_c_comments(text: str) -> str:
    """Blank C comments while retaining offsets and newlines for literal scans."""
    out = list(text)
    i = 0
    state = "code"
    while i < len(text):
        if state == "code":
            if text.startswith("/*", i):
                out[i:i + 2] = "  "; i += 2; state = "block"; continue
            if text.startswith("//", i):
                out[i:i + 2] = "  "; i += 2; state = "line"; continue
            if text[i] in ('"', "'"):
                quote = text[i]; i += 1
                while i < len(text):
                    if text[i] == "\\": i += 2; continue
                    if text[i] == quote: i += 1; break
                    i += 1
                continue
            i += 1; continue
        if state == "block":
            if text.startswith("*/", i): out[i:i + 2] = "  "; i += 2; state = "code"
            else:
                if text[i] != "\n": out[i] = " "
                i += 1
            continue
        if state == "line":
            if text[i] == "\n": state = "code"
            else: out[i] = " "
            i += 1
    return "".join(out)


def _lex_c_tokens(text: str):
    """Yield C string and character tokens while skipping comments and code."""
    i = 0
    while i < len(text):
        if text.startswith("//", i):
            end = text.find("\n", i + 2)
            i = len(text) if end < 0 else end
            continue
        if text.startswith("/*", i):
            end = text.find("*/", i + 2)
            if end < 0:
                yield {"kind": "lex_error", "start": i, "end": len(text), "error": "unterminated C comment"}
                return
            i = end + 2
            continue
        prefix = next((p for p in ("u8", "L", "u", "U")
                       if text.startswith(p, i) and i + len(p) < len(text) and text[i + len(p)] in ('"', "'")), None)
        if prefix is not None:
            start = i
            quote_pos = i + len(prefix)
            quote = text[quote_pos]
            i = quote_pos + 1
            while i < len(text):
                if text[i] == "\\": i += 2
                elif text[i] == quote:
                    i += 1
                    yield {"kind": "lex_error", "start": start, "end": i,
                           "error": "prefixed C literals require an explicit non-Shift-JIS encoding profile"}
                    break
                else: i += 1
            else:
                yield {"kind": "lex_error", "start": start, "end": len(text),
                       "error": "unterminated prefixed C literal"}
            continue
        if text[i] in ('"', "'"):
            start, quote = i, text[i]
            i += 1
            while i < len(text):
                if text[i] == "\\":
                    i += 2
                elif text[i] == quote:
                    i += 1
                    yield {"kind": "string" if quote == '"' else "character",
                           "start": start, "end": i, "literal": text[start:i]}
                    break
                else:
                    i += 1
            else:
                yield {"kind": "lex_error", "start": start, "end": len(text), "error": "unterminated C literal"}
            continue
        if text[i].isspace():
            i += 1
            continue
        # Keep every non-literal token as a boundary so distant strings do not concatenate.
        start = i
        i += 1
        while i < len(text) and (text[i].isalnum() or text[i] == "_"):
            i += 1
        yield {"kind": "other", "start": start, "end": i}


def _c_unit(path: Path, src_root: Path) -> str:
    relative = path.relative_to(src_root).as_posix()
    if relative.startswith("main/"):
        return "main"
    if relative.startswith("ovl/"):
        return relative.split("/", 2)[1]
    return "unknown"


def _matching_index(repo: Path) -> dict[tuple[str, str], list[Path]]:
    index = {}
    roots = [("main", repo / "asm" / "matchings" / "main")]
    for overlay in (repo / "asm" / "ovl").iterdir() if (repo / "asm" / "ovl").is_dir() else []:
        match_root = overlay / "matchings" / overlay.name
        if match_root.is_dir():
            roots.append((overlay.name, match_root))
    for unit, root in roots:
        if not root.is_dir():
            continue
        for path in root.rglob("*.s"):
            index.setdefault((unit, path.stem), []).append(path)
    return index


def _function_code_extent(asm_path: Path, binary: bytes | None, base: int, bias: int) -> dict:
    """Hash a function's original code extent using generated instruction addresses."""
    if binary is None:
        return {"status": "original_binary_missing"}
    try:
        source_bytes = asm_path.read_bytes()
        source = source_bytes.decode("utf-8", errors="strict")
    except (OSError, UnicodeDecodeError) as exc:
        return {"status": "function_assembly_unreadable", "reason": str(exc)}
    if source.encode("utf-8") != source_bytes or "\ufffd" in source:
        return {"status": "function_assembly_roundtrip_failed"}
    instructions = []
    for line in source.splitlines():
        match = re.search(r"/\*\s*[0-9A-Fa-f]+\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s*\*/", line)
        if match:
            instructions.append((int(match.group(1), 16), bytes.fromhex(match.group(2))))
    if not instructions:
        return {"status": "function_address_unavailable"}
    start = min(address for address, _ in instructions)
    declared = re.search(r"^\s*nonmatching\s+[^,]+,\s*(0x[0-9A-Fa-f]+)", source, re.M)
    length = int(declared.group(1), 16) if declared else max(address for address, _ in instructions) + 4 - start
    offset = start - base + bias
    if offset < 0 or offset + length > len(binary):
        return {"status": "function_extent_out_of_bounds", "address": start,
                "offset": offset, "length": length, "file_size": len(binary)}
    matches = sum(binary[address - base + bias:address - base + bias + 4] == word
                  for address, word in instructions
                  if 0 <= address - base + bias <= len(binary) - 4)
    return {"status": "verified_instruction_bytes" if matches == len(instructions) else "instruction_byte_mismatch",
            "address": start, "offset": offset, "length": length,
            "end_offset_exclusive": offset + length, "file_size": len(binary),
            "sha256": _sha(binary[offset:offset + length]),
            "instruction_words": len(instructions), "verified_instruction_words": matches,
            "extent_basis": "nonmatching_declared_size" if declared else "instruction_comment_min_max"}


def _labels_referenced_by_asm(path: Path, known_labels: set[str]) -> tuple[set[str], str | None]:
    try:
        raw = path.read_bytes()
        source = raw.decode("utf-8", errors="strict")
    except (OSError, UnicodeDecodeError) as exc:
        return set(), str(exc)
    if source.encode("utf-8") != raw or "\ufffd" in source:
        return set(), "assembly source UTF-8 round-trip failed"
    refs = set()
    for line in source.splitlines():
        code = re.sub(r"/\*.*?\*/", "", line)
        code = code.split("#", 1)[0]
        code = re.sub(r'"(?:\\.|[^"\\])*"', '""', code)
        if re.match(r"\s*(?:dlabel|glabel|jlabel|nonmatching|\.globl)\s+", code):
            continue
        refs.update(set(SYMBOL_TOKEN.findall(code)) & known_labels)
    return refs, None


def _function_spans(definitions, tokens):
    spans = []
    braces = [t for t in tokens if t["kind"] == "other" and t.get("punct") in ("{", "}")]
    positions = [token["start"] for token in braces]
    openings = {}
    for definition in definitions:
        opening = bisect_left(positions, definition.start())
        if opening < len(braces) and positions[opening] < definition.end() and braces[opening]["punct"] == "{":
            openings[opening] = definition.group(1)
    active = None
    depth = 0
    for i, token in enumerate(braces):
        if i in openings:
            active = (token["start"], openings[i])
            depth = 1
        elif active:
            depth += 1 if token["punct"] == "{" else -1
            if depth == 0:
                spans.append((active[0], token["end"], active[1]))
                active = None
    return spans


def _function_definitions(text: str):
    matches = []
    control = {"if", "for", "while", "switch", "do", "else"}
    for pattern in FUNCTION_DEF_PATTERNS:
        for match in pattern.finditer(text):
            if match.group(1) not in control:
                matches.append(match)
    return sorted(matches, key=lambda m: m.start())


def _enclosing_function(spans, offset: int) -> str | None:
    return next((name for start, end, name in reversed(spans) if start < offset < end), None)


def _c_literals(src_root: Path) -> tuple[list[dict], list[dict], list[dict]]:
    records, blocked, excluded = [], [], []
    file_infos = []
    function_literal_ordinals = {}
    matching_index = _matching_index(src_root.parent)
    for path in sorted(list((src_root / "main").rglob("*.c")) + list((src_root / "ovl").rglob("*.c"))):
        source_path = path.relative_to(src_root.parent).as_posix()
        try:
            source_bytes = path.read_bytes()
            text = source_bytes.decode("utf-8", errors="strict")
        except UnicodeDecodeError as exc:
            blocked.append({"source_path": source_path, "status": "invalid_utf8",
                            "byte_offset": exc.start, "reason": "C source is not strict UTF-8"})
            continue
        if text.encode("utf-8") != source_bytes or "\ufffd" in text:
            blocked.append({"source_path": source_path, "status": "utf8_roundtrip_failed",
                            "reason": "source has a replacement character or does not round-trip"})
            continue
        try:
            tokens = list(_lex_c_tokens(text))
        except Exception as exc:
            blocked.append({"source_path": source_path, "status": "lexer_error", "reason": str(exc)})
            continue
        char_ordinal = 0
        for token in tokens:
            if token["kind"] == "lex_error":
                blocked.append({"source_path": source_path, "line": text.count("\n", 0, token["start"]) + 1,
                                "status": "lexer_error", "reason": token["error"]})
            elif token["kind"] == "character":
                char_ordinal += 1
                line = text.count("\n", 0, token["start"]) + 1
                excluded.append({"id": f"char:{source_path}:{line}:{char_ordinal}",
                                 "source_site": {"path": source_path, "line": line},
                                 "status": "character_constant_excluded"})
        tokens = [token for token in tokens if token["kind"] != "lex_error"]
        for token in tokens:
            if token["kind"] == "other" and text[token["start"]:token["end"]] in ("{", "}"):
                token["punct"] = text[token["start"]:token["end"]]
        unit = _c_unit(path, src_root)
        masked = _mask_c_comments(text)
        code_mask = _mask_c_literals(masked, tokens)
        definitions = _function_definitions(masked)
        spans = _function_spans(definitions, tokens)
        file_infos.append((source_path, unit, text, masked, code_mask, tokens, spans))
        ordinal = 0
        build_ordinal = 0
        i = 0
        while i < len(tokens):
            token = tokens[i]
            if token["kind"] != "string":
                i += 1
                continue
            start, end = token["start"], token["end"]
            literals = [token["literal"]]
            i += 1
            while i < len(tokens) and tokens[i]["kind"] == "string":
                literals.append(tokens[i]["literal"])
                end = tokens[i]["end"]
                i += 1
            line = text.count("\n", 0, start) + 1
            line_start = text.rfind("\n", 0, start) + 1
            line_end = text.find("\n", end)
            source_line = text[line_start:] if line_end < 0 else text[line_start:line_end]
            if re.match(r"\s*#\s*include\b", source_line) or re.search(r"\bINCLUDE_(?:RODATA|ASM)\s*\(", source_line):
                build_ordinal += 1
                excluded.append({"id": f"build:{source_path}:{line}:{build_ordinal}",
                                 "source_site": {"path": source_path, "line": line},
                                 "status": "build_only_path_literal"})
                continue
            ordinal += 1
            try:
                raw = b"".join(_string_bytes(literal) for literal in literals)
            except (ValueError, SyntaxError, InventoryError) as exc:
                blocked.append({"source_path": source_path, "line": line,
                                "status": "literal_decode_failed", "reason": str(exc)})
                continue
            prefix = masked[line_start:start]
            call_info = _call_containing_argument(code_mask, start)
            call_name = call_info["call"] if call_info else None
            call_argument_index = call_info["argument_index"] if call_info else None
            call_argument_expression = call_info["argument_expression"] if call_info else None
            function = _enclosing_function(spans, start)
            prefix_context = masked[max(0, start - 500):start]
            array_match = re.search(
                r"\b(?:u8|s8|char|unsigned\s+char)\s+(\w+)\s*\[\s*(\d+)\s*\]\s*=\s*$",
                prefix_context)
            array_symbol = array_match.group(1) if array_match else None
            array_length = int(array_match.group(2)) if array_match else None
            consumer_calls: set[tuple[str, int]] = set()
            if array_symbol and function:
                span = next(((a, b) for a, b, name in spans if name == function and a < start < b), None)
                if span:
                    consumer_calls = _call_arguments_using_identifier(code_mask, array_symbol, end, span[1])
            direct_call = call_name
            if call_info is None and len(consumer_calls) == 1:
                call_name, call_argument_index = next(iter(consumer_calls))
                call_info = {"call": call_name, "argument_index": call_argument_index,
                             "argument_expression": None}
            function_asm = matching_index.get((unit, function), []) if function else []
            function_ordinal = None
            if function is not None:
                function_key = (unit, function)
                function_literal_ordinals[function_key] = function_literal_ordinals.get(function_key, 0) + 1
                function_ordinal = function_literal_ordinals[function_key]
            context = code_mask[line_start:] if line_end < 0 else code_mask[line_start:line_end]
            category, reason = _category(raw, context, function, call_name, call_argument_index, unit)
            declaration_prefix = prefix_context
            brace_start = declaration_prefix.rfind("{")
            table_symbol = None
            if brace_start >= 0:
                declaration = declaration_prefix[:brace_start]
                table = re.search(r"([A-Za-z_]\w*)\s*\[[^\]]*\]\s*=\s*$", declaration)
                if table:
                    table_symbol = table.group(1)
            locator_id = f"c:{source_path}:{line}:{ordinal}"
            records.append({"id": locator_id, "source_locator_id": locator_id,
                            "stable_id": None, "stable_id_status": "unresolved_original_provenance",
                            "link_unit": unit,
                            "source_site": {"path": source_path,
                                            "line": line, "ordinal_on_file": ordinal,
                                            "byte_offset": len(text[:start].encode("utf-8")),
                                            "character_offset": start,
                                            "byte_offset_basis": "utf8_bytes",
                                            "function_literal_ordinal": function_ordinal,
                                            "call": call_name,
                                            "call_argument_index": call_argument_index,
                                            "call_argument_expression": call_argument_expression,
                                            "call_basis": "literal_call_site" if direct_call else
                                                          "unique_array_consumer" if len(consumer_calls) == 1 else "unresolved",
                                            "function": function, "literal_count": len(literals),
                                            "table_symbol": table_symbol,
                                            "array_symbol": array_symbol, "array_length": array_length,
                                            "array_consumer_calls": sorted({call for call, _ in consumer_calls}),
                                            "array_consumer_call_arguments": [{"call": call, "argument_index": index}
                                                                               for call, index in sorted(consumer_calls)],
                                            "function_assembly_candidates": [p.relative_to(src_root.parent).as_posix()
                                                                              for p in function_asm],
                                            "function_assembly_status": "unique" if len(function_asm) == 1 else
                                                                       "ambiguous" if function_asm else
                                                                       "function_unresolved" if function is None else "missing"},
                            "raw_hex": raw.hex(), "raw_length": len(raw),
                            **_decoded_view(raw, False),
                            "category": category, "category_basis": reason,
                            "binary_candidates": []})
            if records[-1]["source_decode_status"] != "ok":
                blocked.append({"id": records[-1]["id"], "source_path": source_path,
                                "line": line, "status": records[-1]["source_decode_status"],
                                "reason": "source literal bytes do not have a strict Shift-JIS round-trip"})
    table_sites = [site for site in records if site["source_site"]["table_symbol"]]
    table_symbols = {(site["link_unit"], site["source_site"]["table_symbol"]) for site in table_sites}
    table_ref_index = {}
    for source, unit, text, masked, code_mask, tokens, spans in file_infos:
        for token in tokens:
            if token["kind"] != "other":
                continue
            symbol = text[token["start"]:token["end"]]
            if (unit, symbol) not in table_symbols:
                continue
            line = text.count("\n", 0, token["start"]) + 1
            call_info = _call_containing_argument(code_mask, token["start"])
            call_name = call_info["call"] if call_info else None
            argument_index = call_info["argument_index"] if call_info else None
            function = _enclosing_function(spans, token["start"])
            role = string_argument_role(call_name, argument_index, unit)
            is_value = _identifier_is_value_operand(
                call_info.get("argument_expression") if call_info else None, symbol)
            semantic = "display" if role == "display" else \
                "debug_or_format" if role in {"format", "diagnostic"} else None
            if not is_value:
                semantic = None
            table_ref_index.setdefault((unit, symbol), []).append(
                {"path": source, "line": line, "kind": "table_symbol_occurrence_unclassified",
                 "call": call_name, "call_argument_index": argument_index,
                 "function": function, "semantic_category": None,
                 "semantic_category_candidate": semantic})
    for site in table_sites:
        symbol = site["source_site"]["table_symbol"]
        refs = table_ref_index.get((site["link_unit"], symbol), [])
        candidates = {ref["semantic_category_candidate"] for ref in refs
                      if ref.get("semantic_category_candidate")}
        if len(candidates) == 1:
            site["category_candidate"] = next(iter(candidates))
            site["category_candidate_basis"] = "untyped C table-use occurrence; AST value flow required"
        site["source_site"]["table_reference_sites"] = refs
    return records, blocked, excluded


def _asm_groups(asm_root: Path):
    """Yield logical string extents assembled from adjacent .ascii/.asciz lines."""
    paths = sorted(list((asm_root / "data").rglob("*.s")) + list((asm_root / "ovl").rglob("*.s")))
    for path in paths:
        try:
            source_bytes = path.read_bytes()
            source = source_bytes.decode("utf-8", errors="strict")
        except UnicodeDecodeError as exc:
            yield {"path": path, "line": 1, "blocked_reason": f"invalid UTF-8 at byte {exc.start}"}
            continue
        if source.encode("utf-8") != source_bytes or "\ufffd" in source:
            yield {"path": path, "line": 1, "blocked_reason": "assembler source UTF-8 round-trip failed"}
            continue
        lines = source.splitlines()
        current = None
        pending = None
        active_label = None
        for line_no, line in enumerate(lines, 1):
            match = ASM_DIRECTIVE.search(line)
            address = ADDR_COMMENT.search(line)
            line_address = int(address.group(1), 16) if address else None
            code = re.sub(r"/\*.*?\*/", "", line).split("#", 1)[0].strip()

            declaration = re.match(r"^(dlabel|nonmatching)\s+([A-Za-z_][A-Za-z_0-9]*)\b", code)
            end_label = re.match(r"^enddlabel\s+([A-Za-z_][A-Za-z_0-9]*)\b", code)
            if declaration or end_label:
                if pending is not None:
                    pending.pop("next_address", None)
                    yield pending
                    pending = None
            if declaration:
                kind, name = declaration.groups()
                # `nonmatching` is a segment boundary hint, not an exact data
                # label. The following dlabel supplies the usable symbol.
                active_label = ({"label": name, "address": line_address,
                                 "alignment_seen": False} if kind == "dlabel" else None)
                if line_address is not None:
                    current = line_address
                continue
            if end_label:
                if active_label and active_label["label"] == end_label.group(1):
                    active_label = None
                continue
            if active_label and active_label["address"] is None and line_address is not None and \
                    code.startswith(".") and not re.match(r"^\.(?:align|balign|p2align)\b", code) and \
                    not active_label["alignment_seen"]:
                active_label["address"] = line_address
            if pending is not None and line_address is not None and pending.get("next_address") is not None:
                if line_address != pending["next_address"]:
                    pending.pop("next_address", None)
                    yield pending
                    pending = None
            if match is None and code and pending is not None:
                pending.pop("next_address", None)
                yield pending
                pending = None
            if match:
                try:
                    chunk = _string_bytes(match.group(2))
                except InventoryError as exc:
                    yield {"path": path, "line": line_no,
                           "blocked_reason": f"assembler string decode failed: {exc}"}
                    continue
                if match.group(1) in ("asciz", "string"):
                    chunk += b"\0"
                if pending is None:
                    start_address = line_address if line_address is not None else current
                    if active_label and active_label["address"] is None and line_address is not None and \
                            not active_label["alignment_seen"]:
                        active_label["address"] = line_address
                    exact_labels = [active_label["label"]] if active_label and \
                        active_label["address"] == start_address else []
                    containing_labels = []
                    if active_label and isinstance(active_label["address"], int) and \
                            isinstance(start_address, int) and active_label["address"] < start_address:
                        containing_labels = [{"label": active_label["label"],
                                              "address": active_label["address"],
                                              "relative_offset": start_address - active_label["address"]}]
                    pending = {"path": path, "line": line_no, "address": start_address,
                               "chunks": [], "raw": bytearray(), "labels": exact_labels,
                               "containing_labels": containing_labels,
                               "next_address": start_address}
                pending["chunks"].append({"line": line_no, "directive": match.group(1)})
                pending["raw"].extend(chunk)
                if pending["next_address"] is not None:
                    pending["next_address"] += len(chunk)
                    current = pending["next_address"]
                if match.group(1) in ("asciz", "string"):
                    pending.pop("next_address", None)
                    yield pending
                    pending = None
                continue
            if code and re.match(r"^\.(?:byte|word|half|short|long|float|double|space|zero|align|balign|p2align)\b", code):
                # The next address cannot be inferred safely through an unparsed data directive.
                if active_label and re.match(r"^\.(?:align|balign|p2align)\b", code):
                    active_label["alignment_seen"] = True
                current = None
            elif line_address is not None and code:
                current = line_address
        if pending is not None:
            pending.pop("next_address", None)
            yield pending


def _asm_source_snapshot(asm_root: Path) -> dict[str, str]:
    """Hash every assembly input used by the inventory for concurrent-change detection."""
    paths = sorted(set((list((asm_root / "data").rglob("*.s")) if (asm_root / "data").is_dir() else []) +
                       (list((asm_root / "ovl").rglob("*.s")) if (asm_root / "ovl").is_dir() else []) +
                       (list((asm_root / "matchings" / "main").rglob("*.s"))
                        if (asm_root / "matchings" / "main").is_dir() else [])))
    result = {}
    for path in paths:
        try:
            result[path.relative_to(asm_root.parent).as_posix()] = _sha(path.read_bytes())
        except OSError as exc:
            raise InventoryError(f"assembly input changed or disappeared during inventory: {path}") from exc
    return result


def _binary_for_asm(path: Path, main_bin: bytes | None, overlays: dict[str, tuple[bytes, int]]):
    rel = path.as_posix()
    if "/asm/data/" in rel and "/asm/data/ovl/" not in rel:
        return "SLPM_86.053", main_bin, 0x80041000, 0x800
    match = re.search(r"/ovl/([A-Z0-9_]+)/", rel)
    if match and match.group(1) in overlays:
        data, base = overlays[match.group(1)]
        return f"CDROM/EXEDIR/{match.group(1)}.EXN", data, base, 0
    return None, None, None, None


def _asm_unit(source: str) -> str:
    if source == "SLPM_86.053":
        return "main"
    match = re.search(r"EXEDIR/([A-Z0-9_]+)\.EXN$", source)
    return match.group(1) if match else "unknown"


def _assembly_source_unit(path: Path, repo: Path) -> str:
    """Return the link unit for a source/reference file, independent of its symbol names."""
    relative = path.relative_to(repo).as_posix()
    overlay_data = re.match(r"asm/data/ovl/([A-Z0-9_]+)/", relative)
    if overlay_data:
        return overlay_data.group(1)
    if relative.startswith(("asm/matchings/main/", "asm/data/")):
        return "main"
    match = re.match(r"asm/ovl/([A-Z0-9_]+)/", relative)
    return match.group(1) if match else "unknown"


def _main_label_scope_proven(label: str, record: dict, data_labels: dict) -> bool:
    """Prove an overlay reference names this main executable label by unique main VMA."""
    if record.get("source", {}).get("path") != "SLPM_86.053":
        return False
    addresses = {item["address"] for item in data_labels.get(label, [])
                 if item.get("source") == "SLPM_86.053" and item.get("address") is not None}
    return addresses == {record["source"].get("address")}


def _data_label_addresses(asm_root: Path, main_bin: bytes | None,
                          overlays: dict[str, tuple[bytes, int]]) -> dict[str, list[dict]]:
    """Map generated data labels to VRAM/file offsets without interpreting their payloads."""
    paths = sorted(list((asm_root / "data").rglob("*.s")) +
                   [p for root in (asm_root / "ovl").iterdir() if (root / "data").is_dir()
                    for p in (root / "data").rglob("*.s")])
    result = {}
    for path in paths:
        try:
            raw = path.read_bytes()
            text = raw.decode("utf-8", errors="strict")
        except UnicodeDecodeError:
            continue  # _asm_groups emits the exact invalid source path as a blocker.
        if text.encode("utf-8") != raw or "\ufffd" in text:
            continue
        pending_labels = []
        for line_no, line in enumerate(text.splitlines(), 1):
            label = re.search(r"\bdlabel\s+([A-Za-z_][A-Za-z_0-9]*)", line)
            if label:
                pending_labels = [label.group(1)]
            address = ADDR_COMMENT.search(line)
            if pending_labels and address:
                source, binary, base, bias = _binary_for_asm(path, main_bin, overlays)
                vram = int(address.group(1), 16)
                offset = vram - base + bias if source is not None and base is not None else None
                for symbol in pending_labels:
                    result.setdefault(symbol, []).append({
                        "source": source, "binary": binary, "address": vram, "offset": offset,
                        "assembly_path": path, "assembly_line": line_no})
                pending_labels = []
    return result


def _c_symbol_references(src_root: Path, labels: set[str]) -> tuple[list[dict], list[dict]]:
    """Find source identifier references to data labels without reading string contents."""
    refs, blocked = [], []
    if not labels:
        return refs, blocked
    for path in sorted(list((src_root / "main").rglob("*.c")) + list((src_root / "ovl").rglob("*.c"))):
        source_path = path.relative_to(src_root.parent).as_posix()
        unit = _c_unit(path, src_root)
        try:
            raw_source = path.read_bytes()
            text = raw_source.decode("utf-8", errors="strict")
        except UnicodeDecodeError as exc:
            blocked.append({"source_path": source_path, "status": "symbol_reference_invalid_utf8",
                            "byte_offset": exc.start})
            continue
        if text.encode("utf-8") != raw_source or "\ufffd" in text:
            blocked.append({"source_path": source_path, "status": "symbol_reference_roundtrip_failed"})
            continue
        tokens = list(_lex_c_tokens(text))
        for token in tokens:
            if token["kind"] == "other" and text[token["start"]:token["end"]] in ("{", "}"):
                token["punct"] = text[token["start"]:token["end"]]
        masked = _mask_c_comments(text)
        code_mask = _mask_c_literals(masked, tokens)
        definitions = _function_definitions(masked)
        spans = _function_spans(definitions, tokens)
        tokens = [t for t in tokens if t["kind"] == "other"]
        for token in tokens:
            token_text = text[token["start"]:token["end"]]
            for match in SYMBOL_TOKEN.finditer(token_text):
                symbol = match.group()
                if symbol not in labels:
                    continue
                position = token["start"] + match.start()
                line = text.count("\n", 0, position) + 1
                start = masked.rfind("\n", 0, position) + 1
                end = masked.find("\n", position)
                source_line = text[start:] if end < 0 else text[start:end]
                if re.match(r"\s*#\s*include\b", source_line) or re.search(r"\bINCLUDE_(?:RODATA|ASM)\s*\(", source_line):
                    continue
                call_info = _call_containing_argument(code_mask, position)
                call_name = call_info["call"] if call_info else None
                argument_index = call_info["argument_index"] if call_info else None
                function = _enclosing_function(spans, position)
                declaration = re.search(
                    r"\b(?:const\s+)?(?:u8|s8|u16|s16|u32|s32|char|int|short|long|void|struct\s+\w+)\s+\*?\s*" +
                    re.escape(symbol) + r"\b", source_line)
                if declaration or (call_name is None and function is None):
                    continue
                role = string_argument_role(call_name, argument_index, unit)
                semantic_category_candidate = None
                is_value = _identifier_is_value_operand(
                    call_info.get("argument_expression") if call_info else None, symbol)
                if is_value and role == "display":
                    semantic_category_candidate = "display"
                elif is_value and role in {"format", "diagnostic"}:
                    semantic_category_candidate = "debug_or_format"
                refs.append({"link_unit": unit, "label": symbol, "path": source_path,
                             "line": line, "call": call_name, "call_argument_index": argument_index,
                             "function": function,
                             "semantic_category": None,
                             "semantic_category_candidate": semantic_category_candidate})
    return refs, blocked


def inventory_source_text(repository=REPOSITORY, files_root=None, output_path=None) -> dict:
    """Build binary-backed inventory and source-reference candidates.

    Reads `asm/data/main` and `asm/ovl`, maps addresses using splat/overlay
    metadata, and scans C string literals for candidate references. Exact text
    appears only in the returned in-memory structure or the requested cache file.
    """
    repo = Path(repository).resolve(strict=True)
    src_root, asm_root = repo / "src", repo / "asm"
    asm_snapshot_before = _asm_source_snapshot(asm_root)
    extracted = Path(files_root).resolve(strict=True) if files_root else repo / "disc" / "files"
    exe_path = extracted / "SLPM_86.053"
    input_blockers = []
    main_bin, error = _read_original(exe_path, extracted)
    if error:
        input_blockers.append({"binary_path": "SLPM_86.053", "status": "source_input_escapes_root", "reason": error})
    overlay_bases = {}
    for line in (repo / "config" / "overlays.txt").read_text().splitlines():
        parts = line.split()
        if len(parts) >= 2 and not parts[0].startswith("#"):
            overlay_bases[parts[0]] = int(parts[1], 0)
    overlays = {}
    for name, base in overlay_bases.items():
        path = extracted / "CDROM" / "EXEDIR" / f"{name}.EXN"
        data, error = _read_original(path, extracted)
        if error:
            input_blockers.append({"binary_path": f"CDROM/EXEDIR/{name}.EXN",
                                   "status": "source_input_escapes_root", "reason": error})
        elif data is not None:
            overlays[name] = (data, base)
    c_sites, c_blocked, c_excluded = _c_literals(src_root)
    blocked = list(c_blocked) + input_blockers
    function_extent_cache = {}
    for site in c_sites:
        function_paths = site["source_site"]["function_assembly_candidates"]
        if len(function_paths) != 1:
            site["function_code_extent"] = {"status": site["source_site"]["function_assembly_status"]}
            continue
        function_path = repo / function_paths[0]
        unit = site["link_unit"]
        if unit == "main":
            code_binary, code_base, code_bias = main_bin, 0x80041000, 0x800
        elif unit in overlays:
            code_binary, code_base = overlays[unit]
            code_bias = 0
        else:
            code_binary, code_base, code_bias = None, 0, 0
        code_key = (function_path, site["link_unit"])
        if code_key not in function_extent_cache:
            function_extent_cache[code_key] = _function_code_extent(function_path, code_binary, code_base, code_bias)
        site["function_code_extent"] = function_extent_cache[code_key]
        extent = site["function_code_extent"]
        if extent.get("status") == "verified_instruction_bytes" and \
                isinstance(site["source_site"].get("function_literal_ordinal"), int):
            address = extent["address"]
            site["stable_id"] = f"c:{site['link_unit']}:{address:08x}:{site['source_site']['function_literal_ordinal']}"
            site["stable_id_status"] = "verified_matching_function_entry_and_local_ordinal"
            site["stable_id_evidence"] = {"function_entry_address": address,
                                           "function_code_sha256": extent["sha256"],
                                           "function_literal_ordinal": site["source_site"]["function_literal_ordinal"]}
        else:
            site["stable_id_status"] = "unresolved_original_function_or_object_address"
        if site["function_code_extent"].get("status") not in ("verified_instruction_bytes", "original_binary_missing"):
            blocked.append({"id": site["id"], "source_path": site["source_site"]["path"],
                            "line": site["source_site"]["line"], "function_assembly": function_paths[0],
                            "status": "function_code_extent_unverified",
                            "reason": site["function_code_extent"].get("status")})
    c_by_bytes = {}
    for site in c_sites:
        for candidate_bytes in _literal_extent_candidates(site):
            bucket = c_by_bytes.setdefault((site["link_unit"], candidate_bytes.hex()), [])
            if site not in bucket:
                bucket.append(site)
    records_by_extent = {}
    assembly_string_extents = 0
    hashes = {}
    for group in _asm_groups(asm_root):
        assembly_string_extents += 1
        asm_path = group["path"]
        if "blocked_reason" in group:
            blocked.append({"id": f"asm:{asm_path.relative_to(repo).as_posix()}:{group['line']}",
                            "assembly_path": asm_path.relative_to(repo).as_posix(),
                            "assembly_line": group["line"], "status": "string_decode_failed",
                            "reason": group["blocked_reason"]})
            continue
        source, binary, base, bias = _binary_for_asm(asm_path, main_bin, overlays)
        raw = bytes(group["raw"])
        address = group["address"]
        site_id = f"asm:{asm_path.relative_to(repo).as_posix()}:{group['line']}"
        if source is None:
            blocked.append({"id": site_id, "assembly_path": asm_path.relative_to(repo).as_posix(),
                            "assembly_line": group["line"], "status": "no_binary_mapping"})
            continue
        if binary is None:
            blocked.append({"id": site_id, "assembly_path": asm_path.relative_to(repo).as_posix(),
                            "assembly_line": group["line"], "binary_path": source,
                            "status": "source_binary_missing", "reason": "required local original input unavailable"})
            continue
        if address is None:
            blocked.append({"id": site_id, "assembly_path": asm_path.relative_to(repo).as_posix(),
                            "assembly_line": group["line"], "binary_path": source,
                            "status": "address_missing"})
            continue
        offset = address - base + bias
        if offset < 0 or offset + len(raw) > len(binary):
            status = "address_out_of_bounds"
        elif binary[offset:offset + len(raw)] != raw:
            status = "binary_bytes_disagree"
        else:
            status = "verified"
        unit = _asm_unit(source)
        candidates = c_by_bytes.get((unit, raw.hex()), [])
        alias_raw = raw + b"\0" if raw and not raw.endswith(b"\0") and status == "verified" and \
            offset + len(raw) < len(binary) and binary[offset + len(raw)] == 0 else None
        alias_candidates = c_by_bytes.get((unit, alias_raw.hex()), []) if alias_raw is not None else []
        category = "empty_extent_no_translatable_bytes" if not any(raw) else "unknown"
        refs = [{"kind": "byte_identical_c_literal_candidate", "id": site["id"],
                 "source_site": site["source_site"], "confirmed_reference": False} for site in candidates]
        extent_key = (source, offset, raw)
        record = records_by_extent.get(extent_key)
        if record is None:
            if source not in hashes:
                hashes[source] = _sha(binary)
            record_id = f"{source}:{offset:08x}:{_sha(raw)[:12]}"
            record = {"id": record_id, "stable_id": record_id,
                      "stable_id_status": "verified_original_binary_extent", "category": category,
                  "category_basis": "zero-only extent contains no translatable bytes" if not any(raw) else
                                    "byte-identical C candidates do not prove a semantic reference",
                  "source": {"path": source, "offset": offset, "address": address,
                             "length": len(raw), "end_offset_exclusive": offset + len(raw),
                             "file_size": len(binary), "within_file_bounds": True,
                             "sha256": hashes[source]},
                  "raw_hex": raw.hex(), "raw_length": len(raw), "extent_status": status,
                  **_decoded_view(raw, raw.endswith(b"\0")),
                  "terminator": {"nul_terminated": raw.endswith(b"\0"),
                                 "offset": offset + len(raw) - 1 if raw.endswith(b"\0") else None},
                  "assembly_sites": [], "reference_sites": [],
                  "reference_status": "unreferenced"}
            records_by_extent[extent_key] = record
            if record["source_decode_status"] != "ok":
                blocked.append({"id": record["id"], "binary_path": source, "offset": offset,
                                "assembly_path": asm_path.relative_to(repo).as_posix(),
                                "assembly_line": group["line"],
                                "status": record["source_decode_status"],
                                "reason": "binary extent does not have a strict Shift-JIS round-trip"})
        site = {"path": asm_path.relative_to(repo).as_posix(), "line": group["line"],
                "labels": group["labels"], "containing_labels": group.get("containing_labels", []),
                "chunks": group["chunks"]}
        if site not in record["assembly_sites"]:
            record["assembly_sites"].append(site)
        if status != "verified":
            record["extent_status"] = status
            blocked.append({"id": record["id"], "assembly_path": site["path"], "assembly_line": site["line"],
                            "binary_path": source, "status": status,
                            "reason": "assembler extent did not verify against original bytes"})
        for site in candidates:
            if record["id"] not in site["binary_candidates"]:
                site["binary_candidates"].append(record["id"])
            ref = {"kind": "byte_identical_c_literal_candidate", "id": site["id"],
                   "source_site": site["source_site"], "confirmed_reference": False}
            if ref not in record["reference_sites"]:
                record["reference_sites"].append(ref)
        if alias_raw is not None and alias_candidates:
            alias_key = (source, offset, alias_raw)
            alias = records_by_extent.get(alias_key)
            if alias is None:
                alias_id = f"{source}:{offset:08x}:{_sha(alias_raw)[:12]}"
                alias = {"id": alias_id, "stable_id": alias_id,
                         "stable_id_status": "verified_original_binary_extent",
                         "category": "unknown",
                         "category_basis": "exact C string including an original-byte-verified terminal NUL",
                         "source": {"path": source, "offset": offset, "address": address,
                                    "length": len(alias_raw), "end_offset_exclusive": offset + len(alias_raw),
                                    "file_size": len(binary), "within_file_bounds": True,
                                    "sha256": hashes[source]},
                         "raw_hex": alias_raw.hex(), "raw_length": len(alias_raw),
                         "extent_status": "verified_by_assembly_prefix_and_original_terminal_nul",
                         **_decoded_view(alias_raw, True),
                         "terminator": {"nul_terminated": True, "offset": offset + len(alias_raw) - 1,
                                        "evidence": "next_original_binary_byte_is_zero"},
                         "assembly_sites": [{"path": asm_path.relative_to(repo).as_posix(),
                                             "line": group["line"], "labels": group["labels"],
                                             "containing_labels": group.get("containing_labels", []),
                                             "kind": "canonical_nul_terminated_alias"}],
                         "reference_sites": [], "reference_status": "unreferenced"}
                records_by_extent[alias_key] = alias
                if alias["source_decode_status"] != "ok":
                    blocked.append({"id": alias["id"], "binary_path": source, "offset": offset,
                                    "assembly_path": asm_path.relative_to(repo).as_posix(),
                                    "assembly_line": group["line"],
                                    "status": alias["source_decode_status"],
                                    "reason": "canonical NUL-terminated source bytes do not round-trip as Shift-JIS"})
            for site in alias_candidates:
                if alias["id"] not in site["binary_candidates"]:
                    site["binary_candidates"].append(alias["id"])
                ref = {"kind": "byte_identical_c_literal_candidate", "id": site["id"],
                       "source_site": site["source_site"], "confirmed_reference": False}
                if ref not in alias["reference_sites"]:
                    alias["reference_sites"].append(ref)
    binary_records = sorted(records_by_extent.values(), key=lambda r: (r["source"]["path"], r["source"]["offset"], r["id"]))
    # Generated instruction/data references to labels establish call/table paths without interpreting the text.
    label_records = {}
    for record in binary_records:
        for asm_site in record["assembly_sites"]:
            for label in asm_site["labels"]:
                label_records.setdefault(label, []).append(record)
    data_labels = _data_label_addresses(asm_root, main_bin, overlays)
    function_literal_counts = {}
    for site in c_sites:
        key = (site["link_unit"], site["source_site"]["function"], site["raw_hex"])
        function_literal_counts[key] = function_literal_counts.get(key, 0) + 1
    function_ref_cache = {}
    for site in c_sites:
        if site["raw_length"] == 0:
            site["function_assembly_label_references"] = []
            site["function_binary_candidates"] = []
            site["function_reference_status"] = "empty_constant_no_translatable_bytes"
            continue
        candidates = site["source_site"]["function_assembly_candidates"]
        if len(candidates) != 1:
            site["function_binary_candidates"] = []
            site["function_reference_status"] = site["source_site"]["function_assembly_status"]
            continue
        assembly_path = repo / candidates[0]
        if assembly_path not in function_ref_cache:
            function_ref_cache[assembly_path] = _labels_referenced_by_asm(
                assembly_path, set(label_records) | set(data_labels))
        refs, error = function_ref_cache[assembly_path]
        if error:
            blocked.append({"id": site["id"], "source_path": site["source_site"]["path"],
                            "line": site["source_site"]["line"], "function_assembly": candidates[0],
                            "status": "function_reference_source_unreadable", "reason": error})
        candidate_records = []
        exact_raws = tuple(sorted(set(_literal_extent_candidates(site)), key=len, reverse=True))
        for label in sorted(refs):
            for record in label_records.get(label, []):
                if (_asm_unit(record["source"]["path"]) == site["link_unit"] and
                        bytes.fromhex(record["raw_hex"]) in exact_raws and record not in candidate_records):
                    candidate_records.append(record)
            for location in data_labels.get(label, []):
                if _asm_unit(location["source"]) != site["link_unit"]:
                    continue
                binary, offset = location["binary"], location["offset"]
                if binary is None or offset is None:
                    continue
                matched_raw = next((candidate for candidate in exact_raws
                                    if 0 <= offset <= len(binary) - len(candidate) and
                                    binary[offset:offset + len(candidate)] == candidate), None)
                if matched_raw is None:
                    continue
                extent_key = (location["source"], offset, matched_raw)
                record = records_by_extent.get(extent_key)
                if record is None:
                    if location["source"] not in hashes:
                        hashes[location["source"]] = _sha(binary)
                    record_id = f"{location['source']}:{offset:08x}:{_sha(matched_raw)[:12]}"
                    record = {"id": record_id, "stable_id": record_id,
                              "stable_id_status": "verified_original_binary_extent",
                              "category": site["category"],
                              "category_basis": "C literal exact bytes at data label referenced by its matching function",
                              "source": {"path": location["source"], "offset": offset,
                                         "address": location["address"], "length": len(matched_raw),
                                         "end_offset_exclusive": offset + len(matched_raw),
                                         "file_size": len(binary), "within_file_bounds": True,
                                         "sha256": hashes[location["source"]]},
                              "raw_hex": matched_raw.hex(), "raw_length": len(matched_raw),
                              **_decoded_view(matched_raw, matched_raw.endswith(b"\0")),
                              "extent_status": "verified_by_function_label_and_source_literal",
                              "terminator": {"nul_terminated": matched_raw.endswith(b"\0"),
                                             "offset": offset + len(matched_raw) - 1 if matched_raw.endswith(b"\0") else None},
                              "assembly_sites": [{"path": location["assembly_path"].relative_to(repo).as_posix(),
                                                  "line": location["assembly_line"], "labels": [label],
                                                  "kind": "referenced_data_label_exact_bytes"}],
                              "reference_sites": [], "reference_status": "direct_matching_function_reference"}
                    records_by_extent[extent_key] = record
                    binary_records.append(record)
                if record not in candidate_records:
                    candidate_records.append(record)
        site["function_assembly_label_references"] = sorted(refs)
        site["function_binary_candidates"] = [record["id"] for record in candidate_records]
        for record in candidate_records:
            if record["id"] not in site["binary_candidates"]:
                site["binary_candidates"].append(record["id"])
        count_key = (site["link_unit"], site["source_site"]["function"], site["raw_hex"])
        if len(candidate_records) == 1 and function_literal_counts[count_key] == 1:
            site["function_reference_status"] = "unique_same_function_label_and_exact_bytes"
            record = candidate_records[0]
            ref = {"kind": "matching_function_data_reference", "c_source_site": site["id"],
                   "function_assembly": candidates[0], "confirmed_reference": True}
            if ref not in record["reference_sites"]:
                record["reference_sites"].append(ref)
            c_ref = next((r for r in record["reference_sites"]
                          if r.get("kind") == "byte_identical_c_literal_candidate" and r.get("id") == site["id"]), None)
            if c_ref:
                c_ref["confirmed_reference"] = True
                c_ref["confirmation_basis"] = "same function assembly references an exact-byte data label"
            if site["category"] in ("display", "name", "system", "place", "debug_or_format"):
                record["category"] = site["category"]
                record["category_basis"] = "unique C literal matched a same-function referenced data label"
        elif candidate_records:
            site["function_reference_status"] = "ambiguous_same_function_data_labels_or_literals"
        else:
            site["function_reference_status"] = "no_exact_data_label_reference_in_matching_function"
    c_symbol_refs, c_ref_blocked = _c_symbol_references(src_root, set(label_records))
    blocked.extend(c_ref_blocked)
    for ref in c_symbol_refs:
        for record in label_records.get(ref["label"], []):
            record_unit = _asm_unit(record["source"]["path"])
            if record_unit != ref["link_unit"] and not (
                    record_unit == "main" and _main_label_scope_proven(ref["label"], record, data_labels)):
                continue
            site_ref = {"kind": "C_symbol_reference", "label": ref["label"],
                        "path": ref["path"], "line": ref["line"], "call": ref["call"],
                        "semantic_category": None,
                        "semantic_category_candidate": ref.get("semantic_category_candidate"),
                        "confirmed_reference": True}
            if site_ref not in record["reference_sites"]:
                record["reference_sites"].append(site_ref)
    if label_records:
        known_labels = set(label_records)
        reference_paths = sorted(list((asm_root / "matchings").rglob("*.s")) + list((asm_root / "ovl").rglob("*.s")) +
                                 list((asm_root / "data").rglob("*.s")))
        for path in reference_paths:
            reference_unit = _assembly_source_unit(path, repo)
            try:
                raw_source = path.read_bytes()
                source_text = raw_source.decode("utf-8", errors="strict")
            except UnicodeDecodeError as exc:
                blocked.append({"assembly_path": path.relative_to(repo).as_posix(),
                                "status": "reference_source_invalid_utf8", "byte_offset": exc.start})
                continue
            if source_text.encode("utf-8") != raw_source or "\ufffd" in source_text:
                blocked.append({"assembly_path": path.relative_to(repo).as_posix(),
                                "status": "reference_source_roundtrip_failed"})
                continue
            for line_no, line in enumerate(source_text.splitlines(), 1):
                code = re.sub(r"/\*.*?\*/", "", line)
                code = code.split("#", 1)[0]
                code = re.sub(r'"(?:\\.|[^"\\])*"', '""', code)
                if re.match(r"\s*(?:dlabel|glabel|jlabel|nonmatching|\.globl)\s+", code):
                    continue
                for label in sorted(set(SYMBOL_TOKEN.findall(code)) & known_labels):
                    for record in label_records.get(label, []):
                        record_unit = _asm_unit(record["source"]["path"])
                        if reference_unit != record_unit and not (
                                record_unit == "main" and _main_label_scope_proven(label, record, data_labels)):
                            continue
                        ref = {"kind": "assembler_symbol_reference", "label": label,
                               "path": path.relative_to(repo).as_posix(), "line": line_no}
                        if path.as_posix() in {str((repo / s["path"]).as_posix()) for s in record["assembly_sites"]}:
                            continue
                        if ref not in record["reference_sites"]:
                            record["reference_sites"].append(ref)
    asm_snapshot_after = _asm_source_snapshot(asm_root)
    if asm_snapshot_after != asm_snapshot_before:
        raise InventoryError("assembly source tree changed during inventory; rerun against a stable generated tree")
    for record in binary_records:
        candidates = [r for r in record["reference_sites"] if r.get("kind") == "byte_identical_c_literal_candidate"]
        asm_refs = [r for r in record["reference_sites"] if r.get("kind") == "assembler_symbol_reference"]
        record["reference_status"] = ("direct_assembler_symbol_refs_found" if asm_refs else
                                      "single_unconfirmed_byte_candidate" if len(candidates) == 1 else
                                      "ambiguous_byte_candidates" if candidates else "unreferenced_in_scanned_sources")
    binary_records = sorted(records_by_extent.values(), key=lambda r: (r["source"]["path"], r["source"]["offset"], r["id"]))
    verified = sum(r["extent_status"].startswith("verified") for r in binary_records)
    for site in c_sites:
        if site["raw_length"] == 0:
            site["binary_status"] = "empty_constant_no_translatable_bytes"
            site["binary_candidate_status"] = "not_applicable"
            site["binary_candidates"] = []
            continue
        if site["binary_candidates"]:
            site["binary_status"] = "candidate_found_unconfirmed"
            site["binary_candidate_status"] = "single" if len(site["binary_candidates"]) == 1 else "ambiguous_identical_bytes"
        else:
            site["binary_status"] = "unmapped_literal_no_exact_assembler_match"
            blocked.append({"id": site["id"], "source_path": site["source_site"]["path"],
                            "line": site["source_site"]["line"], "status": "C_literal_unmapped",
                            "reason": "no exact-byte literal extent found in the same link unit"})
    inventory = {"schema": "tokimemo.text.source-inventory.v1",
                 "sources": {"main": "SLPM_86.053", "overlays": sorted(f"CDROM/EXEDIR/{n}.EXN" for n in overlays),
                             "main_binary_available": main_bin is not None},
                 "coverage": {"assembly_string_extents_scanned": assembly_string_extents,
                              "assembly_source_files_scanned": len(asm_snapshot_before),
                              "assembly_source_tree_sha256": _sha(json.dumps(asm_snapshot_before, sort_keys=True).encode()),
                              "assembly_source_tree_stable_during_scan": True,
                              "binary_extents": len(binary_records), "verified_extents": verified,
                              "blocked_extents": sum(not b.get("status", "").startswith("C_literal") for b in blocked),
                              "unmapped_C_literals": sum(b.get("status") == "C_literal_unmapped" for b in blocked),
                              "blocked_inventory_items": len(blocked), "C_literal_sites": len(c_sites),
                              "C_literal_blocked": len(c_blocked), "C_literal_excluded": len(c_excluded),
                              "deduplicated_assembly_sites": assembly_string_extents - len(binary_records),
                              "C_sites_with_binary_candidates": sum(bool(s["binary_candidates"]) for s in c_sites),
                              "empty_constant_sites": sum(s["raw_length"] == 0 for s in c_sites),
                              "empty_extent_no_translatable_bytes": sum(r["category"] == "empty_extent_no_translatable_bytes"
                                                                         for r in binary_records),
                              "semantic_unknown_nonempty_extents": sum(r["category"] == "unknown" and any(bytes.fromhex(r["raw_hex"]))
                                                                        for r in binary_records),
                              "categories": {k: sum(r["category"] == k for r in binary_records)
                                             for k in ("display", "name", "system", "debug_or_format", "path", "unknown",
                                                       "empty_extent_no_translatable_bytes")}},
                 "records": binary_records, "c_literal_sites": c_sites, "excluded_c_sites": c_excluded, "blocked": blocked,
                 "coverage_matrix": {"assembler_source_roots": ["asm/data/**/*.s", "asm/ovl/**/*.s"],
                                     "assembler_extents": "literal directives joined through terminator, address checked against original bytes, duplicate path/offset/raw extents merged",
                                     "C_source_roots": ["src/main/**/*.c", "src/ovl/**/*.c"],
                                     "C_literals": "strict UTF-8, Shift-JIS byte view, adjacent strings combined; same-link-unit exact-byte matches are candidates, never asserted references",
                                     "C_literal_unmapped": "each site appears in blocked with path and line",
                                     "build_only_literals": "#include and INCLUDE_RODATA/INCLUDE_ASM paths excluded into excluded_c_sites",
                                     "character_constants": "excluded into excluded_c_sites by C lexer",
                                     "indirect_table_references": "array declaration symbol and C identifier occurrence sites retained where lexically proved",
                                     "C_label_references": "same-link-unit C references plus overlay-to-main references with a unique verified main VMA are recorded; known display-call uses are categorized",
                                     "runtime_name_substitutions": "not statically resolved; caller context retained",
                                     "data_without_string_directives": "not classified as text; requires format-specific evidence",
                                     "missing_binary_or_address": "listed in blocked with exact source site and reason",
                                     "unscanned_streams_and_external_files": "out of scope for this source inventory"}}
    if output_path is not None:
        out = Path(output_path).absolute()
        cache = repo / ".asset-cache" / "text"
        if not out.is_relative_to(cache):
            raise InventoryError("source inventory output must be under .asset-cache/text/")
        if out.is_symlink() or any(path.is_symlink() for path in (repo / ".asset-cache", cache, *out.parents)):
            raise InventoryError("inventory output cannot be a symlink")
        out.parent.mkdir(parents=True, exist_ok=True)
        if not out.resolve().is_relative_to(cache.resolve()):
            raise InventoryError("inventory output escapes .asset-cache/text/ through a symlink")
        out.write_text(json.dumps(inventory, ensure_ascii=False, sort_keys=True, indent=2) + "\n", encoding="utf-8")
    return inventory


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository", type=Path, default=REPOSITORY)
    parser.add_argument("--files-root", type=Path)
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args(argv)
    try:
        inventory = inventory_source_text(args.repository, args.files_root, args.output)
    except (OSError, ValueError, KeyError, TypeError) as exc:
        parser.exit(1, f"source inventory failed: {exc}\n")
    print(json.dumps(inventory["coverage"], sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
