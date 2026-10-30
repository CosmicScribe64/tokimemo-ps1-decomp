#!/usr/bin/env python3
"""T-39028: original-free lint of the committed English translation pack and artwork.

Reads only committed files under translations/en (no disc, no .asset-cache):

Text pack (``returned-v3.jsonl``, ``supplemental.jsonl``, ``manifest.json``,
``graphics/inventory_decisions.json``):
- every line parses, every record has an id, ids are unique across both files;
- the manifest row counts and non-empty target count equal the files;
- the recorded text approval covers exactly the record count
  (tools/port/build_text_pack.py ``approval``);
- every non-empty translation carries exactly the record's protected
  ``[[T...]]`` tokens: none added, none dropped (a translation that drops every
  marker fails too); an empty target is not yet translated and is skipped;
- report only (not a failure): characters without an original full-width code
  unit (build_text_pack ``code_for``), which the pack builder rejects per record.

Artwork (``graphics/artwork``):
- the snapshot audit (tools/translation_assets.py ``audit``): every current
  asset's bytes and dimensions match its manifest and the review history hash;
- a draft is never marked approved: a version or asset whose status claims an
  approval must have an exact, hash-bound approve event; a checkpoint ledger
  entry must have ``human_approved: false`` unless its status is ``approved``,
  and an entry with status ``approved`` or ``human_approved: true`` needs an
  approve event in ``style-decisions.jsonl`` bound to the hash of every image
  it lists (``output_sha256`` for a single path, each file's own hash for
  ``paths``), with the recorded ``output_sha256`` equal to the file bytes;
- every image has a review record: every PNG anywhere under the pack root is
  either a manifest version output (``graphics/artwork/batches``) or listed by
  a checkpoint ledger (``path`` or ``paths``) with a matching hash where one is
  recorded; a stray PNG anywhere else fails; every approve event names an
  image the manifest or a ledger knows.

This is CI hygiene for committed files. It is not translation quality review,
visual acceptance or runtime coverage.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re
import sys

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.port.build_text_pack import MARKER, PackError, approval, code_for  # noqa: E402
from tools.translation_assets import audit  # noqa: E402

APPROVAL_CLAIM = re.compile(r"(?<![a-z_])(?:user_style_)?approved(?![a-z_])")
PENDING = re.compile(r"pending|draft|unapproved|not_approved")


class LintFailure(ValueError):
    """Committed translation assets break a lint rule."""


def _sha256(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def read_jsonl(path: Path) -> list[dict]:
    rows = []
    for number, line in enumerate(path.read_bytes().decode("utf-8").split("\n"), 1):
        if not line.strip():
            continue
        try:
            value = json.loads(line)
        except ValueError as error:
            raise LintFailure(f"{path.name}:{number}: {error}") from None
        if not isinstance(value, dict) or not isinstance(value.get("id"), str) or not value["id"]:
            raise LintFailure(f"{path.name}:{number}: record without id")
        rows.append(value)
    return rows


def _token_markers(tokens) -> list[str]:
    if isinstance(tokens, str):
        tokens = json.loads(tokens) if tokens.strip() else []
    if tokens is None:
        return []
    if not isinstance(tokens, list) or not all(isinstance(t, dict) and isinstance(t.get("marker"), str) for t in tokens):
        raise LintFailure("protected_tokens must be a list of {marker, ...}")
    return [t["marker"] for t in tokens]


def lint_text(root: Path) -> dict:
    main = read_jsonl(root / "returned-v3.jsonl")
    extra = read_jsonl(root / "supplemental.jsonl")
    failures: list[str] = []
    seen: set[str] = set()
    for row in main + extra:
        if row["id"] in seen:
            failures.append("duplicate record id " + row["id"])
        seen.add(row["id"])
    manifest = json.loads((root / "manifest.json").read_text())
    nonempty = sum(1 for row in main if isinstance(row.get("target_text"), str) and row["target_text"])
    for key, actual in (("main_rows", len(main)), ("supplemental_rows", len(extra)), ("main_nonempty_targets", nonempty)):
        if manifest.get(key) != actual:
            failures.append(f"manifest {key} {manifest.get(key)} != {actual}")
    try:
        approval(json.loads((root / "graphics" / "inventory_decisions.json").read_text()), len(main) + len(extra))
    except PackError as error:
        failures.append("text approval: " + str(error))
    marker_errors = 0
    unrepresentable: dict[str, int] = {}
    for row, field in [(r, "target_text") for r in main] + [(r, "translation") for r in extra]:
        text = row.get(field) or ""
        try:
            allowed = _token_markers(row.get("protected_tokens"))
        except (LintFailure, ValueError) as error:
            failures.append(f"{row['id']}: {error}")
            continue
        drawn = MARKER.findall(text)
        if text and sorted(drawn) != sorted(allowed):
            marker_errors += 1
            if marker_errors <= 20:
                failures.append(f"{row['id']}: markers {drawn} differ from protected tokens {allowed}")
        for character in MARKER.sub("", text):
            if code_for(character) is None and not character.isspace():
                unrepresentable["U+%04X" % ord(character)] = unrepresentable.get("U+%04X" % ord(character), 0) + 1
    if marker_errors > 20:
        failures.append(f"... {marker_errors - 20} more marker mismatches")
    return {"records": len(main) + len(extra), "failures": failures,
            "report_only": {"characters_without_original_code_unit": dict(sorted(unrepresentable.items()))}}


def lint_artwork(artwork: Path, pack_root: Path | None = None) -> dict:
    """Lint the artwork snapshot; ``pack_root`` (default: ``artwork``) bounds the stray-image walk."""
    failures: list[str] = []
    try:
        report = audit(artwork)
    except (ValueError, KeyError, OSError) as error:
        return {"failures": ["snapshot audit: " + str(error)], "assets": 0}
    styles = {asset["id"]: asset for asset in report["assets"]}
    manifest = json.loads((artwork / "manifest.json").read_text())
    outputs = set()
    for version in manifest["versions"]:
        claim = APPROVAL_CLAIM.search(str(version.get("publication_status", "")))
        for asset in version["assets"]:
            outputs.add(asset["output"])
            if not version.get("current"):
                continue
            identity = "draft:" + asset["id"]
            status = str(asset.get("status", ""))
            claims = (claim or APPROVAL_CLAIM.search(status)) and not PENDING.search(status)
            if claims and styles.get(identity, {}).get("style") != "approve":
                failures.append(f"{identity}: marked approved without an exact approve event (draft marked approved)")
    reviewed: set[Path] = {(artwork / output).resolve() for output in outputs}
    history = [json.loads(line) for line in (artwork / "style-decisions.jsonl").read_text().splitlines() if line]
    latest_by_hash: dict[str, str] = {}
    for event in history:
        if event.get("category") == "style" and isinstance(event.get("output_sha256"), str):
            latest_by_hash[event["output_sha256"]] = event.get("decision")
    approved_hashes = {digest for digest, decision in latest_by_hash.items() if decision == "approve"}
    known_hashes = {asset["output_sha256"] for version in manifest["versions"] for asset in version["assets"]}
    checkpoints = artwork / "checkpoints"
    ledgered = 0
    for ledger_path in sorted(checkpoints.glob("*/ledger.json")) if checkpoints.is_dir() else []:
        base = ledger_path.parent
        ledger = json.loads(ledger_path.read_text())
        listed: set[str] = set()
        for entry in ledger.get("assets", []):
            paths = entry.get("paths") or ([entry["path"]] if "path" in entry else [])
            status = str(entry.get("status", ""))
            label = f"{base.name}/{entry.get('id')}"
            if entry.get("human_approved") is True and (status == "draft" or status.startswith("agent")):
                failures.append(f"{label}: {status} entry marked human_approved (draft marked approved)")
            if entry.get("human_approved") is not False and status != "approved":
                failures.append(f"{label}: human_approved must be false unless status is approved")
            claims = status == "approved" or entry.get("human_approved") is True
            if claims and not paths:
                failures.append(f"{label}: approved entry lists no image (draft marked approved)")
            if isinstance(entry.get("output_sha256"), str):
                known_hashes.add(entry["output_sha256"])
            for relative in paths:
                listed.add(relative)
                target = base / relative
                if not target.is_file():
                    failures.append(f"{base.name}/{relative}: ledger lists a missing image")
                    continue
                actual = _sha256(target.read_bytes())
                known_hashes.add(actual)
                if len(paths) == 1 and entry.get("output_sha256") and actual != entry["output_sha256"]:
                    failures.append(f"{base.name}/{relative}: image bytes differ from the ledger hash")
                if not claims:
                    continue
                if len(paths) == 1 and not isinstance(entry.get("output_sha256"), str):
                    failures.append(f"{label}: approved entry has no output_sha256 (draft marked approved)")
                elif actual not in approved_hashes:
                    failures.append(f"{base.name}/{relative}: marked approved without an exact approve event "
                                    "bound to its hash (draft marked approved)")
        reviewed.update((base / relative).resolve() for relative in listed)
        ledgered += len(listed)
    for event in history:
        if event.get("decision") == "approve" and event.get("output_sha256") not in known_hashes:
            failures.append(f"review event {event.get('event_id')}: approves an image the manifest does not know")
    walk_root = pack_root if pack_root is not None else artwork
    for path in sorted(walk_root.rglob("*.png")):
        if path.resolve() not in reviewed:
            relative = path.relative_to(walk_root).as_posix()
            failures.append(f"{relative}: image has no manifest, ledger or review record")
    return {"assets": report["known_current_assets"], "approved_exact": report["approved_exact"],
            "checkpoint_images": ledgered, "failures": failures}


def lint(root: Path) -> dict:
    text = lint_text(root)
    art = lint_artwork(root / "graphics" / "artwork", root)
    return {"schema": "tokimemo.translation-asset-lint.v1", "text": text, "artwork": art,
            "passed": not text["failures"] and not art["failures"],
            "scope": "committed-file hygiene only; not translation quality, visual or runtime acceptance"}


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--root", type=Path, default=ROOT / "translations" / "en")
    args = parser.parse_args(argv)
    try:
        result = lint(args.root)
    except (LintFailure, ValueError, OSError, KeyError) as error:
        print("FAIL: " + str(error), file=sys.stderr)
        return 1
    for section in ("text", "artwork"):
        for failure in result[section]["failures"]:
            print(f"FAIL {section}: {failure}")
    print(("PASS" if result["passed"] else "FAIL") + f": {result['text']['records']} text records, "
          f"{result['artwork'].get('assets', 0)} current artwork assets ({result['artwork'].get('approved_exact', 0)} exactly approved), "
          f"{result['artwork'].get('checkpoint_images', 0)} checkpoint images")
    return 0 if result["passed"] else 1


if __name__ == "__main__":
    sys.exit(main())
