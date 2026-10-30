#!/usr/bin/env python3
"""T-39025: English legibility and asset-coverage linter over E-mode draw receipts.

Input: one or more receipts, each either a ``tokimemo.english-draw-receipt.v1``
document or a checkpoint receipt that carries one under its ``english`` key
(the WP-02 observer receipt v2 is expected to embed it). A draw receipt lists
what the port actually drew at one checkpoint with language=en:

- ``text_runs``: ``unit_id`` (translation-unit id), ``text`` (Unicode) or
  ``cp932_hex`` (the drawn code units), ``source_box`` [x, y, w, h] from the
  source call site, ``rect`` (drawn extent), ``lines``/``max_lines``,
  ``pages``/``max_pages`` after reflow, ``missing_glyphs`` and
  ``fallback_glyphs`` (code points drawn with a substitute glyph);
- ``images``: ``asset_id``, ``has_text`` (required boolean), ``source_sha256``,
  ``drawn_sha256`` and ``replacement`` ({``status``, ``output_sha256``} or
  null). A replacement with status ``approved`` must carry ``output_sha256``
  and the image ``drawn_sha256``, both 64 lowercase hex digits;
- ``declared_fallbacks``: unit ids allowed to stay Japanese, each declared in
  the translation pack (a name, a proper noun, an untranslatable glyph).

Rules (all objective; a finding never needs a human verdict to exist):

- ``untranslated_cp932``: kana or kanji drawn while language=en in a unit that
  is not a declared fallback, listed by unit id;
- ``overflow_source_box``: a run extends past its source box;
- ``clipped_by_window``: a run extends past the window (canvas) rect;
- ``line_overflow`` / ``page_overflow``: reflowed lines or pages over the box limit;
- ``missing_glyph`` / ``font_fallback``: glyphs absent or substituted;
- art state per drawn image with text: ``approved-used``, ``approved-unused``,
  ``approved-unverified``, ``draft`` or ``none``. The receipt's own status is a
  claim, not evidence: a replacement counts as approved only when an approvals
  file is given (``--approvals``, the audit from tools/translation_assets.py)
  and that exact output hash has an exact approve event. Without one, a claimed
  approval is ``approved-unverified`` with an ``art_approval_unverified``
  finding; with one, a hash without an event is ``draft`` with
  ``art_unverified_approval``. Any other status is ``draft``: a draft never
  counts as approved.

Coverage per checkpoint: % English text (runs with no Japanese, over all runs),
% declared fallback text (Japanese runs the pack allows; reported apart and
never counted as English) and % English art (approved-used over images with
text). Receipts are local evidence under .asset-cache; the fixtures in
tools/port/tests/fixtures/english_lint/ are synthetic.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

SCHEMA = "tokimemo.english-lint-report.v1"
RECEIPT_SCHEMA = "tokimemo.english-draw-receipt.v1"
ART_STATES = ("approved-used", "approved-unused", "approved-unverified", "draft", "none")
RULES = ("untranslated_cp932", "overflow_source_box", "clipped_by_window", "line_overflow",
         "page_overflow", "missing_glyph", "font_fallback", "art_approved_unused", "art_draft",
         "art_missing", "art_unverified_approval", "art_approval_unverified")
HASH_RE = re.compile(r"^[0-9a-f]{64}$")


class LintError(ValueError):
    """A receipt does not follow the draw-receipt contract."""


def is_japanese(character: str) -> bool:
    """Kana, kanji or half-width katakana: script a reader of English cannot read."""
    value = ord(character)
    return (0x3040 <= value <= 0x30FF or 0x3400 <= value <= 0x4DBF or 0x4E00 <= value <= 0x9FFF
            or 0xF900 <= value <= 0xFAFF or 0xFF66 <= value <= 0xFF9F or 0x31F0 <= value <= 0x31FF)


def run_text(run: dict, where: str) -> str:
    if isinstance(run.get("text"), str):
        return run["text"]
    if isinstance(run.get("cp932_hex"), str):
        try:
            return bytes.fromhex(run["cp932_hex"]).decode("cp932")
        except ValueError as error:
            raise LintError(f"{where}: cp932_hex does not decode: {error}") from None
    raise LintError(f"{where}: run needs text or cp932_hex")


def _rect(value, where: str) -> tuple[int, int, int, int]:
    if not (isinstance(value, list) and len(value) == 4 and all(type(v) is int for v in value)
            and value[2] >= 0 and value[3] >= 0):
        raise LintError(f"{where}: rect must be [x, y, w, h] integers")
    return tuple(value)


def _inside(inner, outer) -> bool:
    return (inner[0] >= outer[0] and inner[1] >= outer[1]
            and inner[0] + inner[2] <= outer[0] + outer[2] and inner[1] + inner[3] <= outer[1] + outer[3])


def draw_receipt(document: dict, where: str) -> dict:
    if not isinstance(document, dict):
        raise LintError(where + ": receipt is not an object")
    english = document if document.get("schema") == RECEIPT_SCHEMA else document.get("english")
    if not isinstance(english, dict) or english.get("schema") != RECEIPT_SCHEMA:
        raise LintError(where + ": no " + RECEIPT_SCHEMA + " block")
    if english.get("language") != "en":
        raise LintError(where + ": linter applies to language=en receipts only")
    if not isinstance(english.get("checkpoint"), str) or not english["checkpoint"]:
        raise LintError(where + ": checkpoint key missing")
    return english


def art_state(image: dict, approvals: set[str] | None) -> tuple[str, str | None]:
    """Return (state, extra finding rule) for one drawn image."""
    replacement = image.get("replacement")
    if replacement is None:
        return "none", None
    if not isinstance(replacement, dict):
        raise LintError("replacement must be an object or null")
    output = replacement.get("output_sha256")
    if replacement.get("status") != "approved":
        return "draft", None
    for key, value in (("replacement output_sha256", output), ("drawn_sha256", image.get("drawn_sha256"))):
        if not isinstance(value, str) or not HASH_RE.fullmatch(value):
            raise LintError(f"approved replacement needs {key} as 64 lowercase hex digits")
    if approvals is None:
        return "approved-unverified", "art_approval_unverified"
    if output not in approvals:
        return "draft", "art_unverified_approval"
    return ("approved-used" if image["drawn_sha256"] == output else "approved-unused"), None


def lint_receipt(english: dict, approvals: set[str] | None = None) -> dict:
    checkpoint = english["checkpoint"]
    window = _rect(english.get("window", [0, 0, 320, 240]), checkpoint + " window")
    fallbacks = english.get("declared_fallbacks", [])
    if not isinstance(fallbacks, list) or not all(isinstance(f, str) for f in fallbacks):
        raise LintError(checkpoint + ": declared_fallbacks must be a list of unit ids")
    findings: list[dict] = []
    runs = english.get("text_runs", [])
    images = english.get("images", [])
    if not isinstance(runs, list) or not isinstance(images, list):
        raise LintError(checkpoint + ": text_runs and images must be lists")
    english_runs = fallback_runs = 0
    for index, run in enumerate(runs):
        where = f"{checkpoint} run {index}"
        if not isinstance(run, dict) or not isinstance(run.get("unit_id"), str) or not run["unit_id"]:
            raise LintError(where + ": unit_id missing")
        unit = run["unit_id"]
        text = run_text(run, where)
        japanese = sorted({"U+%04X" % ord(c) for c in text if is_japanese(c)})
        if japanese and unit not in fallbacks:
            findings.append({"rule": "untranslated_cp932", "unit_id": unit, "code_points": japanese[:16]})
        elif japanese:
            fallback_runs += 1
        else:
            english_runs += 1
        box = _rect(run.get("source_box"), where + " source_box")
        drawn = _rect(run.get("rect"), where + " rect")
        if not _inside(drawn, box):
            findings.append({"rule": "overflow_source_box", "unit_id": unit, "rect": list(drawn), "source_box": list(box)})
        if not _inside(drawn, window):
            findings.append({"rule": "clipped_by_window", "unit_id": unit, "rect": list(drawn), "window": list(window)})
        for count, limit, rule in (("lines", "max_lines", "line_overflow"), ("pages", "max_pages", "page_overflow")):
            if count in run or limit in run:
                if type(run.get(count)) is not int or type(run.get(limit)) is not int:
                    raise LintError(f"{where}: {count} and {limit} must both be integers")
                if run[count] > run[limit]:
                    findings.append({"rule": rule, "unit_id": unit, count: run[count], limit: run[limit]})
        for key, rule in (("missing_glyphs", "missing_glyph"), ("fallback_glyphs", "font_fallback")):
            glyphs = run.get(key, [])
            if not isinstance(glyphs, list):
                raise LintError(f"{where}: {key} must be a list")
            if glyphs:
                findings.append({"rule": rule, "unit_id": unit, "glyphs": sorted(set(map(str, glyphs)))[:16]})
    states = {state: 0 for state in ART_STATES}
    for index, image in enumerate(images):
        where = f"{checkpoint} image {index}"
        if not isinstance(image, dict) or not isinstance(image.get("asset_id"), str):
            raise LintError(where + ": asset_id missing")
        if type(image.get("has_text")) is not bool:
            raise LintError(where + ": has_text must be true or false")
        if not image["has_text"]:
            continue
        try:
            state, extra = art_state(image, approvals)
        except LintError as error:
            raise LintError(f"{where}: {error}") from None
        states[state] += 1
        if extra:
            findings.append({"rule": extra, "asset_id": image["asset_id"]})
        rule = {"approved-unused": "art_approved_unused", "draft": "art_draft", "none": "art_missing"}.get(state)
        if rule:
            findings.append({"rule": rule, "asset_id": image["asset_id"]})
    art_total = sum(states.values())
    for finding in findings:
        finding["objective"] = True
    return {"checkpoint": checkpoint,
            "english_text_percent": round(100.0 * english_runs / len(runs), 2) if runs else None,
            "declared_fallback_text_percent": round(100.0 * fallback_runs / len(runs), 2) if runs else None,
            "english_art_percent": round(100.0 * states["approved-used"] / art_total, 2) if art_total else None,
            "text_runs": len(runs), "art_states": states, "findings": findings}


def load_approvals(path: Path) -> set[str]:
    audit = json.loads(path.read_text())
    if audit.get("schema") != "tokimemo.translation-asset-audit.v1":
        raise LintError("approvals file is not a tokimemo.translation-asset-audit.v1 report")
    return {a["output_sha256"] for a in audit.get("assets", []) if a.get("style") == "approve" and a.get("exact_review_event")}


def lint(paths: list[Path], approvals: set[str] | None = None) -> dict:
    checkpoints, inputs = [], []
    for path in paths:
        data = path.read_bytes()
        inputs.append({"path": path.name, "sha256": hashlib.sha256(data).hexdigest()})
        checkpoints.append(lint_receipt(draw_receipt(json.loads(data), path.name), approvals))
    counts = {rule: sum(1 for c in checkpoints for f in c["findings"] if f["rule"] == rule) for rule in RULES}
    return {"schema": SCHEMA, "inputs": inputs, "approvals_verified": approvals is not None,
            "checkpoints": checkpoints, "finding_counts": counts,
            "passed": not any(counts.values()),
            "scope": "objective draw-receipt rules only; subjective readability and tone stay with the human review queue"}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("receipts", type=Path, nargs="+")
    parser.add_argument("--approvals", type=Path, help="tools/translation_assets.py audit JSON")
    parser.add_argument("--output", type=Path, help="new report path (stdout summary only when omitted)")
    args = parser.parse_args(argv)
    if args.output is not None and args.output.exists():
        print("output exists: " + str(args.output), file=sys.stderr)
        return 2
    try:
        approvals = load_approvals(args.approvals) if args.approvals else None
        report = lint(args.receipts, approvals)
    except (LintError, ValueError, OSError, KeyError) as error:
        print("FAIL: " + str(error), file=sys.stderr)
        return 2
    if args.output is not None:
        args.output.write_text(json.dumps(report, indent=1, sort_keys=True) + "\n")
    for checkpoint in report["checkpoints"]:
        print(f"{checkpoint['checkpoint']}: text {checkpoint['english_text_percent']}% "
              f"(fallback {checkpoint['declared_fallback_text_percent']}%) art "
              f"{checkpoint['english_art_percent']}% findings {len(checkpoint['findings'])}")
    return 0 if report["passed"] else 1


if __name__ == "__main__":
    sys.exit(main())
