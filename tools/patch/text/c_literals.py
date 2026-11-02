"""Writer for the C string literals of src/ (T-39503).

The executable and overlays are built from the decomp, so a record of kind
``source_literal`` (ids ``c:<unit>:<function address>:<n>``) is written into the C
source, not into the binary: the compiler and linker place the new bytes. A literal
site is the run of adjacent string tokens that starts at the unit's
``source_character_offset``; the writer replaces that run with one literal rendered
from the payload bytes.

``render_literal`` writes Shift-JIS pairs as their UTF-8 characters (the form the
sources use; ``tools/cc.py`` turns them back into octal escapes), printable ASCII as
itself and every other byte as a three-digit octal escape, so
``string_bytes(render_literal(b)) == b`` for every byte string.
"""
from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

from tools.assets.text_source import _lex_c_tokens, _string_bytes

ESCAPES = {0x0A: "\\n", 0x09: "\\t", 0x0D: "\\r", 0x22: '\\"', 0x5C: "\\\\"}


class LiteralError(ValueError):
    """A literal site cannot be found or does not hold the expected bytes."""


@dataclass
class LiteralSite:
    id: str
    path: str
    start: int
    end: int
    payload: bytes
    array_length: int | None = None


def _sjis_char(pair: bytes) -> str | None:
    try:
        char = pair.decode("shift_jis", errors="strict")
    except UnicodeDecodeError:
        return None
    if len(char) != 1 or ord(char) < 0x80 or char.encode("shift_jis") != pair:
        return None
    return char


def render_literal(payload: bytes) -> str:
    """One C string literal whose bytes are exactly ``payload`` (NUL not included)."""
    out, pos = ['"'], 0
    while pos < len(payload):
        byte = payload[pos]
        if (0x81 <= byte <= 0x9F or 0xE0 <= byte <= 0xEF) and pos + 1 < len(payload):
            char = _sjis_char(payload[pos:pos + 2])
            if char is not None:
                out.append(char)
                pos += 2
                continue
        if byte in ESCAPES:
            out.append(ESCAPES[byte])
        elif byte == 0x3F and out[-1].endswith("?"):
            out.append("\\?")  # no trigraphs
        elif 0x20 <= byte <= 0x7E:
            out.append(chr(byte))
        else:
            out.append("\\%03o" % byte)
        pos += 1
    out.append('"')
    return "".join(out)


def string_bytes(literal_text: str) -> bytes:
    """Bytes a run of adjacent C literals compiles to (Shift-JIS for non-ASCII)."""
    tokens = [t for t in _lex_c_tokens(literal_text) if t["kind"] == "string"]
    return b"".join(_string_bytes(t["literal"]) for t in tokens)


def locate_sites(repository: str | Path, units: list[dict]) -> list[LiteralSite]:
    """Find every ``source_literal`` unit in the current sources and check its bytes."""
    repository = Path(repository)
    by_path: dict[str, list[dict]] = {}
    for unit in units:
        if unit["kind"] != "source_literal":
            continue
        locator = unit["context"].get("source_locator_id") or unit["id"]
        path = locator.split(":")[1] if locator.startswith("c:src/") else None
        if path is None:
            raise LiteralError(f"{unit['id']}: no source locator")
        by_path.setdefault(path, []).append(unit)
    sites = []
    for path, items in sorted(by_path.items()):
        text = (repository / path).read_text(encoding="utf-8")
        groups = {}
        tokens = list(_lex_c_tokens(text))
        i = 0
        while i < len(tokens):
            if tokens[i]["kind"] != "string":
                i += 1
                continue
            start = tokens[i]["start"]
            end = tokens[i]["end"]
            literals = [tokens[i]["literal"]]
            i += 1
            while i < len(tokens) and tokens[i]["kind"] == "string":
                literals.append(tokens[i]["literal"])
                end = tokens[i]["end"]
                i += 1
            groups[start] = (end, b"".join(_string_bytes(lit) for lit in literals))
        for unit in items:
            start = unit["context"].get("source_character_offset")
            if start not in groups:
                raise LiteralError(f"{unit['id']}: no literal at {path} character {start}")
            end, raw = groups[start]
            payload = bytes.fromhex(unit["raw_hex"] or "")
            if raw != payload:
                raise LiteralError(f"{unit['id']}: source literal bytes differ from the record")
            sites.append(LiteralSite(unit["id"], path, start, end, payload))
    return sites


def rewrite_sources(repository: str | Path, sites: list[LiteralSite], payloads: dict[str, bytes],
                    *, keep_unchanged: bool = True) -> dict[str, str]:
    """New text of every source file with a written literal.

    ``keep_unchanged`` leaves a literal's original spelling when its payload is the
    original one; ``False`` renders every literal (the identity proof uses both).
    """
    repository = Path(repository)
    by_path: dict[str, list[LiteralSite]] = {}
    for site in sites:
        by_path.setdefault(site.path, []).append(site)
    out = {}
    for path, items in sorted(by_path.items()):
        text = (repository / path).read_text(encoding="utf-8")
        pieces, cursor, changed = [], 0, False
        for site in sorted(items, key=lambda s: s.start):
            payload = payloads.get(site.id, site.payload)
            pieces.append(text[cursor:site.start])
            if keep_unchanged and payload == site.payload:
                pieces.append(text[site.start:site.end])
            else:
                pieces.append(render_literal(payload))
                changed = True
            cursor = site.end
        pieces.append(text[cursor:])
        if changed:
            out[path] = "".join(pieces)
    return out


def identity_check(repository: str | Path, sites: list[LiteralSite]) -> dict:
    """Render every literal from its original bytes; the compiled bytes must not change."""
    rendered = [s for s in sites if string_bytes(render_literal(s.payload)) == s.payload]
    files = rewrite_sources(repository, sites, {}, keep_unchanged=False)
    same_files = 0
    for path, text in files.items():
        original = (Path(repository) / path).read_text(encoding="utf-8")
        old = [t for t in _lex_c_tokens(original) if t["kind"] != "string"]
        new = [t for t in _lex_c_tokens(text) if t["kind"] != "string"]
        old_code = [original[t["start"]:t["end"]] for t in old]
        new_code = [text[t["start"]:t["end"]] for t in new]
        if old_code == new_code:
            same_files += 1
    return {"sites": len(sites), "render_roundtrip": len(rendered), "files": len(files),
            "files_with_unchanged_code": same_files,
            "status": "identical" if len(rendered) == len(sites) and same_files == len(files) else "different"}
