"""Lossless fixed-renderer and C-format text views (T-28001).

Direct glyph loading: src/main/8004E500.c:81-99,323-353.
Narration wrapping/substitutions/styles: src/main/8004F870.c:512-663.
Runtime binding lengths remain symbolic unless provided; this reader never
executes formatting or invents a name value.
"""
from __future__ import annotations

import re

from .text import TextFormatError
from .text_renderers import FORMAT_ARGUMENT_INDEX

_FORMAT = re.compile(rb"%(?P<position>[1-9][0-9]*\$)?(?P<flags>[-+ #0]*)(?P<width>\*(?:[1-9][0-9]*\$)?|[0-9]+)?(?P<precision>\.(?:\*(?:[1-9][0-9]*\$)?|[0-9]*))?(?P<length>hh|ll|[hlLjzt])?(?P<conversion>[diouxXfFeEgGaAcspn%])")


def _unicode(raw):
    try:
        value = raw.decode("shift_jis", errors="strict")
        return value if value.encode("shift_jis", errors="strict") == raw else None
    except UnicodeError:
        return None


def _token(kind, start, raw, **fields):
    return {"kind": kind, "offset": start, "raw_hex": raw.hex(), **fields}


def _source_units(raw):
    """Source-codepoint units; these are not claims about renderer stepping."""
    units = []
    pos = 0
    while pos < len(raw):
        size = 2 if 0x81 <= raw[pos] <= 0x9F or 0xE0 <= raw[pos] <= 0xEF else 1
        chunk = raw[pos:pos + size]
        text = _unicode(chunk)
        units.append(_token("source_character" if text is not None else "unknown_source_bytes",
                            pos, chunk, text=text))
        pos += len(chunk)
    return units


def _direct(raw, renderer):
    tokens = []
    stopped = False
    for pos in range(0, len(raw), 2):
        chunk = raw[pos:pos + 2]
        if stopped:
            token = _token("not_rendered_after_rejected_pair", pos, chunk)
        elif len(chunk) == 1:
            # The supplied implicit NUL is read as this pair's low byte.
            token = _token("terminal_byte_before_nul", pos, chunk,
                           rendered=False, stop_reason="low_byte_nul")
        elif int.from_bytes(chunk, "big") < 0x8140:
            token = _token("renderer_rejected_pair", pos, chunk, rendered=False,
                           reason="below_first_font_range", numeric_code=int.from_bytes(chunk, "big"))
            stopped = True
        else:
            text = _unicode(chunk)
            valid = text is not None and len(text) == 1
            token = _token("glyph" if valid else "unmapped_numeric_glyph", pos, chunk,
                           text=text if valid else None, numeric_code=int.from_bytes(chunk, "big"),
                           font_resolution="requires_font_range_table")
        tokens.append(token)
    # set_kanji_string suppresses the entire entry when str[0] or str[1] is NUL.
    return {"tokens": tokens, "entry_active": len(raw) >= 2 if renderer == "set_kanji_string" else None,
            "glyph_stop": "rejected_pair" if stopped else "implicit_nul",
            "substitutions": [], "row_splitting": False}


def _style(context):
    col = context.get("col")
    y = context.get("y")
    if not isinstance(col, int) or not isinstance(y, int):
        return {"status": "runtime_context_required", "inputs": ["y", "col"],
                "evidence": "src/main/8004F870.c:627-663"}
    prefix = "none"
    if y == 0:
        prefix = {0xF: "player_quote_open", 0x1F: "parenthesis_open", 0x2F: "none"}.get(col, "girl_quote_open")
    return {"status": "resolved", "row": min(y, 2) if y >= 0 else 2,
            "screen_y": 0x32 if y == 0 else 0x42 if y == 1 else 0x52,
            "screen_x": -0x80 if col == 0xFF else -0x56 if y == 0 else -0x48,
            "prefix": prefix, "draw_color": 0, "speaker_label": None,
            "evidence": "src/main/8004F870.c:627-663"}


def _sndi(raw, context, start=0):
    # The first pass tests original byte offsets, not an expanding token count.
    p_pos = next((i for i, byte in enumerate(raw) if byte == ord("P") and i % 2 == 0), None)
    col = context.get("col")
    g_enabled = (col & 15) == 15 if isinstance(col, int) else None
    bindings = context.get("bindings", {})
    p_binding = bindings.get("player_form")
    if p_binding is not None and not isinstance(p_binding, bytes):
        raise TextFormatError("runtime bindings must be raw bytes")
    g_positions = []
    if g_enabled is not False:
        for i, byte in enumerate(raw):
            if byte != ord("G"):
                continue
            stage_pos = i
            uncertain = p_pos is not None and i > p_pos and p_binding is None
            if p_pos is not None and i > p_pos and p_binding is not None:
                stage_pos += len(p_binding) - 1
            if uncertain or stage_pos % 2 == 0:
                g_positions.append((i, uncertain or g_enabled is None))
                # An unresolved earlier candidate might fail its parity test.
                if not uncertain:
                    break
    g_map = dict(g_positions)
    candidates = set(g_map)
    if p_pos is not None:
        candidates.add(p_pos)
    boundaries = sorted({0, len(raw), *candidates, *(pos + 1 for pos in candidates)})
    units = []
    for left, right in zip(boundaries, boundaries[1:]):
        for unit in _source_units(raw[left:right]):
            unit["offset"] += left
            units.append(unit)
    tokens = []
    for token in units:
        pos = token["offset"]
        byte = raw[pos:pos + 1]
        if pos == p_pos:
            token.update(kind="substitution", code="P", name="player_form",
                         occurrence="first_even_original_byte_offset",
                         binding_selector="player_full_name_form_15" if g_enabled else "current_girl_address_form" if g_enabled is False else "col_low_nibble_15_or_current_girl")
        elif pos in g_map and byte == b"G":
            token.update(kind="conditional_substitution" if g_map[pos] else "substitution",
                         code="G", name="girl_name", occurrence="first_even_stage_byte_offset",
                         condition="col_low_nibble_is_15_and_first_even_G_after_P_expansion",
                         binding_selector="current_girl")
        token["offset"] += start
        tokens.append(token)
    return {"tokens": tokens, "style": _style(context),
            "layout_status": "symbolic_runtime_substitutions" if p_pos is not None or g_positions else "direct_glyph_loader",
            "copy_limit": 64, "copy_terminated": len(raw) < 64,
            "runtime_status": "bounded_original_copy" if len(raw) < 64 else "unterminated_bounded_copy_runtime_read_unproven",
            "evidence": "src/main/8004F870.c:562-663"}


def _sndisp(raw, context):
    splits = []
    parity = 0
    for pos, byte in enumerate(raw):
        if byte == ord(" ") and parity % 2 == 0:
            splits.append(pos)
            parity = -1
        elif byte in (ord("P"), ord("G")) and parity % 2 == 0:
            parity = -1
        parity += 1
    segments = []
    tokens = []
    start = 0
    for index, stop in enumerate(splits + [len(raw)]):
        row_context = dict(context)
        if isinstance(context.get("y"), int):
            row_context["y"] += index
        row = _sndi(raw[start:stop], row_context, start)
        segments.append({"index": index, "offset": start, "length": stop - start,
                         "style": row["style"], "copy_terminated": row["copy_terminated"]})
        tokens.extend(row["tokens"])
        if stop < len(raw):
            tokens.append(_token("row_separator", stop, raw[stop:stop + 1],
                                 parity_counter_reset=True))
        start = stop + 1
    return {"tokens": tokens, "rows": segments, "copy_limit": 256,
            "copy_terminated": len(raw) < 256,
            "split_position_capacity": 5, "split_capacity_exceeded": len(splits) > 5,
            "evidence": "src/main/8004F870.c:512-543"}


def _printf(raw):
    tokens = []
    pos = 0
    while pos < len(raw):
        start = pos
        if raw[pos] != ord("%"):
            stop = raw.find(b"%", pos)
            pos = len(raw) if stop < 0 else stop
            tokens.append(_token("format_literal", start, raw[start:pos], text=_unicode(raw[start:pos])))
            continue
        match = _FORMAT.match(raw, pos)
        if match is None:
            pos += 1
            # Preserve the unknown conversion with its percent sign as one unit.
            if pos < len(raw):
                pos += 1
            tokens.append(_token("unknown_format", start, raw[start:pos], reason="unrecognized_or_incomplete_format"))
            continue
        pos = match.end()
        fields = {key: value.decode("ascii") if value is not None else None
                  for key, value in match.groupdict().items()}
        conversion = fields["conversion"]
        modern = (fields["length"] in ("hh", "ll", "j", "z", "t") or conversion in "aA" or
                  any("$" in (value or "") for value in fields.values()))
        stars = sum("*" in (fields[key] or "") for key in ("width", "precision"))
        tokens.append(_token("format_directive", start, raw[start:pos], **fields,
                             argument_count=stars + (conversion != "%"),
                             runtime_support="requires_libc_verification" if modern else "c89_format_syntax",
                             translatable=False))
    return {"tokens": tokens, "formats_executed": False,
            "grammar": "C_printf_syntax_with_explicit_runtime_support_limits"}


def tokenize_fixed_text(raw: bytes, renderer: str, context=None) -> dict:
    """Read an explicitly bounded non-NUL source string using its known consumer.

    renderer: load_string, set_kanji_string, sndi, sndisp, printf, sprintf,
    snprintf, vsprintf, or vsnprintf. Narration context supplies y/col, and may
    supply bindings={player_form: bytes} to resolve G's post-expansion parity.
    Missing context is represented symbolically, never assumed to be talk rules.
    """
    if not isinstance(raw, bytes) or b"\0" in raw:
        raise TextFormatError("fixed source must be bytes excluding its NUL terminator")
    context = {} if context is None else context
    if renderer in ("load_string", "set_kanji_string"):
        view = _direct(raw, renderer)
    elif renderer == "sndi":
        view = _sndi(raw, context)
    elif renderer == "sndisp":
        view = _sndisp(raw, context)
    elif renderer in FORMAT_ARGUMENT_INDEX:
        view = _printf(raw)
    else:
        view = {"tokens": [_token("unknown_renderer_bytes", 0, raw)],
                "unsupported": "renderer_semantics_not_verified"}
    source_text = _unicode(raw)
    return {"schema": "tokimemo.fixed_text.v1", "renderer": renderer,
            "encoding": "shift_jis", "cp932_fallback": False,
            "raw_hex": raw.hex(), "length": len(raw), "terminator_hex": "00",
            "source_decoded_text": source_text,
            "source_decode_status": "strict_roundtrip" if source_text is not None else "invalid_shift_jis",
            **view}
