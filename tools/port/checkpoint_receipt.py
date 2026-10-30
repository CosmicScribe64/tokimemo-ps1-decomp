#!/usr/bin/env python3
"""T-39019 (T-39018 WP-02): Python check of tokimemo.checkpoint-receipt.v2 bindings.

Mirrors ``verify()`` in port/godot/tests/support/checkpoint_observer.gd, so that
downstream tools refuse a receipt whose fields or PNG changed after the
observer bound them:

* ``receipt_sha256`` is recomputed over the receipt without that field, with
  the observer's canonical form: integral finite floats below 9e15 become
  integers, dictionaries are written with sorted keys, and the text is what
  Godot 4.7 ``JSON.stringify(body, "", true)`` writes (no spaces; strings
  escaped by ``String.json_escape``; other floats as ``"%.{p}f"`` with
  ``p = max(1, 14 - floor(log10|x|))``, trailing zeros removed down to one
  decimal). The float and escape rules were checked against Godot 4.7.2 in
  Docker tokimemo-port and against real receipts.
* ``native_canvas.png_sha256`` is the hash of the PNG file bytes.
* ``native_canvas.rgba8_sha256`` is the hash of the decoded RGBA8 pixels.
"""
import hashlib
import math
from pathlib import Path

SCHEMA = "tokimemo.checkpoint-receipt.v2"
NATIVE_SIZE = (320, 240)
_ESCAPES = [("\\", "\\\\"), ("\b", "\\b"), ("\f", "\\f"), ("\n", "\\n"), ("\r", "\\r"), ("\t", "\\t"),
            ("\v", "\\v"), ('"', '\\"')]


class Unsupported(ValueError):
    """A value whose Godot JSON text this module does not reproduce."""


def canonical(value):
    """checkpoint_observer.gd _canonical(): string keys, integral floats as integers."""
    if isinstance(value, dict):
        return {str(key): canonical(item) for key, item in value.items()}
    if isinstance(value, list):
        return [canonical(item) for item in value]
    if isinstance(value, float) and math.isfinite(value) and value == math.floor(value) and abs(value) < 9.0e15:
        return int(value)
    return value


def _float_text(value):
    if not math.isfinite(value):
        raise Unsupported("non-finite float %r" % value)
    if value == 0.0:
        return "0.0"
    if abs(value) < 1e-30:
        raise Unsupported("float %r is below the range checked against Godot" % value)
    precision = max(1, 14 - math.floor(math.log10(abs(value))))
    text = format(value, ".%df" % precision)
    if "." in text:
        text = text.rstrip("0")
        if text.endswith("."):
            text += "0"
    return text


def _string_text(value):
    for raw, escaped in _ESCAPES:
        value = value.replace(raw, escaped)
    return '"' + value + '"'


def godot_json(value):
    """Godot 4.7 JSON.stringify(value, "", true) for the JSON value types."""
    if value is None:
        return "null"
    if value is True:
        return "true"
    if value is False:
        return "false"
    if isinstance(value, int):
        return str(value)
    if isinstance(value, float):
        return _float_text(value)
    if isinstance(value, str):
        return _string_text(value)
    if isinstance(value, list):
        return "[" + ",".join(godot_json(item) for item in value) + "]"
    if isinstance(value, dict):
        return "{" + ",".join(_string_text(key) + ":" + godot_json(value[key]) for key in sorted(value)) + "}"
    raise Unsupported("value of type %s" % type(value).__name__)


def receipt_digest(receipt):
    body = canonical(receipt)
    body.pop("receipt_sha256", None)
    return hashlib.sha256(godot_json(body).encode("utf-8")).hexdigest()


def rgba8_sha256(path):
    from PIL import Image
    with Image.open(path) as image:
        if image.format != "PNG":
            raise ValueError("not a PNG: %s" % path)
        return hashlib.sha256(image.convert("RGBA").tobytes()).hexdigest(), image.size


def verify(receipt, directory):
    """Empty string when the receipt and its PNG still match the binding, else the reason."""
    if not isinstance(receipt, dict) or receipt.get("schema") != SCHEMA:
        return "not a v2 receipt"
    try:
        digest = receipt_digest(receipt)
    except Unsupported as error:
        return "receipt digest cannot be recomputed: %s" % error
    if receipt.get("receipt_sha256") != digest:
        return "receipt fields changed after binding"
    canvas = receipt.get("native_canvas")
    if not isinstance(canvas, dict):
        return "receipt has no native_canvas"
    path = Path(directory) / str(canvas.get("png", ""))
    if not path.is_file():
        return "bound PNG is missing: %s" % path.name
    if hashlib.sha256(path.read_bytes()).hexdigest() != canvas.get("png_sha256"):
        return "PNG changed after binding: %s" % path.name
    try:
        pixels, size = rgba8_sha256(path)
    except (OSError, ValueError) as error:
        return "bound PNG cannot be decoded: %s" % error
    if size != NATIVE_SIZE:
        return "bound PNG is not a native canvas"
    if pixels != canvas.get("rgba8_sha256"):
        return "decoded pixels differ from the bound RGBA hash"
    return ""
