"""Export translator batches and validate returns against trusted source units.

T-28007. This is an editable projection, not a game-text assembler. Run in Docker.
The original JSONL remains authoritative; imports never modify it or its targets.
"""
from __future__ import annotations

import argparse
import csv
import gzip
import hashlib
import json
from pathlib import Path
import re
import shutil
import sys


FIELDS = ("id", "japanese", "translation", "translator_note", "copy_template",
          "context", "protected_tokens", "review_status")
EDITABLE = {"translation", "translator_note"}
TEXT_KINDS = {"glyph", "source_character", "format_literal"}
MARKER = re.compile(r"\[\[T[^\[\]\r\n]*\]\]")
BATCH_SIZE = 400
csv.field_size_limit(sys.maxsize)


class HandoffError(ValueError):
    """The handoff or returned batch violates the source contract."""


def digest(path):
    result = hashlib.sha256()
    with Path(path).open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            result.update(block)
    return result.hexdigest()


def json_text(value):
    return json.dumps(value, ensure_ascii=False, sort_keys=True, separators=(",", ":"))


def visible(text):
    """Make storage controls visible without losing their evidence representation."""
    return "".join(f"⟦U+{ord(char):04X}⟧" if ord(char) < 32 or ord(char) == 127
                   else char for char in text)


def template(unit):
    """Replace non-text byte spans with stable, ordered protected references."""
    source = unit["source_text"]
    if not isinstance(source, str):
        return "", [], "source_text_unavailable"
    raw = bytes.fromhex(unit["raw_hex"] or "")
    encoded = source.encode("shift_jis", errors="strict")
    if raw != encoded and raw != encoded + b"\0":
        return visible(source), [], "source_bytes_do_not_match_template_view"
    raw = encoded
    cursor, spans, parts, references = 0, [], [], []
    for token in unit["tokens"]:
        kind = token.get("kind")
        if kind in TEXT_KINDS:
            continue
        offset = token.get("offset")
        payload = bytes.fromhex(token.get("raw_hex", ""))
        if not isinstance(offset, int) or offset < cursor or not payload:
            return visible(source), [], "token_spans_not_projectable"
        if raw[offset:offset + len(payload)] != payload:
            return visible(source), [], "token_spans_not_projectable"
        # The marker is protected; the inline speaker label remains translatable.
        length = 1 if kind == "inline_label" else len(payload)
        name = token.get("name", token.get("role", kind))
        spans.append((offset, length, kind, name))
        cursor = offset + length
    for offset, byte in enumerate(raw):
        if (byte < 32 or byte == 127) and not any(start <= offset < start + length for start, length, _, _ in spans):
            name = {9: "tab", 10: "line_feed", 13: "carriage_return", 0: "nul"}.get(byte, f"storage_byte_{byte:02x}")
            spans.append((offset, 1, "source_storage_control", name))
    cursor = 0
    for offset, length, kind, name in sorted(spans):
        name = re.sub(r"[^a-zA-Z0-9_-]", "_", str(name))
        marker = f"[[T{len(references) + 1:03d}:{name}]]"
        parts.extend((raw[cursor:offset].decode("shift_jis", errors="strict"), marker))
        references.append({"marker": marker, "kind": kind, "offset": offset,
                           "raw_hex": raw[offset:offset + length].hex()})
        cursor = offset + length
    parts.append(raw[cursor:].decode("shift_jis", errors="strict"))
    text = "".join(parts)
    if any(ord(char) < 32 or ord(char) == 127 for char in text):
        return visible(source), [], "unmodelled_storage_control"
    if MARKER.findall(source):
        return visible(source), [], "source_marker_collision"
    return text, references, None


def short_context(unit):
    context = unit["context"]
    keys = ("section", "group", "line", "table", "index", "table_role", "category",
            "source_function", "source_line", "call", "conditions", "rule_id")
    facts = [str(unit["source_path"]), unit["kind"]]
    facts.extend(f"{key}={json_text(context[key])}" for key in keys if context.get(key) is not None)
    speaker = context.get("speaker")
    if speaker:
        facts.append("speaker=" + json_text(speaker))
    command = context.get("command")
    if command:
        facts.append("command=" + str(command.get("name")))
    if unit.get("parent_id"):
        facts.append("parent=" + unit["parent_id"])
    return "; ".join(facts)


def project(unit):
    if unit.get("target_text") is not None:
        raise HandoffError("original unit target is not null: " + str(unit.get("id")))
    copy_template, protected, blocker = template(unit)
    blockers = list(unit["readiness"]["blockers"])
    if blocker:
        blockers.append(blocker)
    ready = (unit["readiness"]["status"] == "ready_for_review" and
             unit["readiness"]["translation_ready"] is True and not blockers)
    status = "ready_for_review" if ready else "unresolved: " + ", ".join(blockers)
    return {"id": unit["id"], "japanese": visible(unit["source_text"] or ""),
            "translation": "", "translator_note": "", "copy_template": copy_template,
            "context": short_context(unit), "protected_tokens": json_text(protected),
            "review_status": status}


def batches(units_path):
    """Build bounded files preserving source order within each source/category."""
    groups, seen, previous = {}, set(), {}
    with Path(units_path).open(encoding="utf-8") as stream:
        for line in stream:
            unit = json.loads(line)
            identifier = unit.get("id")
            if not isinstance(identifier, str) or not identifier or identifier in seen:
                raise HandoffError("missing or duplicate original ID")
            seen.add(identifier)
            row = project(unit)
            sequence = (str(unit["source_path"]), unit["kind"])
            if sequence in previous:
                row["context"] += "; previous_id=" + previous[sequence]["id"]
                previous[sequence]["context"] += "; next_id=" + row["id"]
            previous[sequence] = row
            queue = "ready" if row["review_status"] == "ready_for_review" else "unresolved"
            group = (queue, str(unit["source_path"]), unit["kind"])
            groups.setdefault(group, []).append(row)
    result = {}
    for group, rows in sorted(groups.items()):
        queue, source, kind = group
        slug = re.sub(r"[^a-zA-Z0-9_.-]+", "_", source + "_" + kind)[:110]
        suffix = hashlib.sha256(json_text(group).encode()).hexdigest()[:10]
        for start in range(0, len(rows), BATCH_SIZE):
            result[f"{queue}/{slug}_{suffix}_{start // BATCH_SIZE + 1:03d}.tsv"] = rows[start:start + BATCH_SIZE]
    return result


def safe_destination(path, repository):
    path = Path(path).absolute()
    root = Path(repository).resolve() / ".asset-cache" / "text"
    if not path.is_relative_to(root) or not path.resolve().is_relative_to(root):
        raise HandoffError("output must remain beneath repository .asset-cache/text")
    for part in (path, *path.parents):
        if part.is_symlink():
            raise HandoffError("output path contains a symlink")
        if part == root:
            break
    if path.exists():
        raise HandoffError("output already exists; choose a fresh destination")
    return path


def compress(source, target):
    with Path(source).open("rb") as src, Path(target).open("xb") as dst:
        with gzip.GzipFile(filename="", mode="wb", fileobj=dst, mtime=0) as compressed:
            shutil.copyfileobj(src, compressed)


README = """# Translator handoff

Start in `ready/`. Each UTF-8 TSV has at most 400 records from one source/category.
Section, group and line remain in context, with adjacent exported IDs across batch
and queue edges. Export order is evidence order; it does not prove runtime sequence.
Import a file into your spreadsheet editor as tab-separated UTF-8, with every column
set to Text. Keep the `.tsv` name when exporting it back as UTF-8 tab-separated text.
Japanese and quoted fields may contain punctuation; use a proper TSV/spreadsheet
editor rather than splitting lines by hand.

Edit only `translation` and `translator_note`. For a row you translate, copy
`copy_template` into `translation`, then replace the Japanese around its markers.
Keep every `[[T...]]` marker exactly once in its original order. These represent
names, row controls, format arguments or other protected source operations. The
`protected_tokens` column explains each marker; the engineering evidence keeps its
full semantics. Inline speaker labels in the template are translatable. Leave
translation blank to defer a row. Put questions in `translator_note`.

Do not edit IDs, Japanese, templates, context, token references or status. Keep all
rows of each returned file, including unfinished rows. You may return any selection
of complete batch files. Duplicated wording may have different IDs and contexts;
do not delete repeated rows. Counts describe records, not unique dialogue lines.

`unresolved/` is a separate browsing queue. These rows need engineering/context
review; any returned wording remains an unresolved draft. SPEAKTST rows are layout
candidates whose runtime use, speakers and voice associations are unproved. A ready
row is ready for translation review; it does not prove an actual runtime scene.
Context gives source table/group/line and symbolic speakers only where available.

Original textbox sizes impose no translation length limit. This package does not
assemble translated game files. Rendering, font coverage, runtime substitutions,
unresolved consumer context, and image text remain separate engineering work.

`evidence/units.jsonl.gz` preserves every source unit, raw byte, token, context and
null target. `evidence/catalog.json.gz` preserves the complete engineering catalog,
including information outside the editable units. These files are for reference;
the maintainer validates returns against their trusted original export.

Maintainer return validation (run in the existing Docker image from the repository):

    python3 -m tools.assets.text_handoff import --units ORIGINAL/units.jsonl --batch RETURNED.tsv --output .asset-cache/text/returned.jsonl --repository .

Repeat `--batch` for more files. Validation rejects changed protected fields,
unknown/duplicate/missing IDs, malformed columns, and altered or reordered markers.
It writes a separate JSONL translation overlay only after every supplied file passes.
The overlay also retains translator notes and review status. It never edits source
units or claims runtime readiness. Return files to the project maintainer for this
check; no Python or Docker setup is needed for the translator.
"""


def export_handoff(units_path, catalog_path, output, repository):
    output = safe_destination(output, repository)
    projected = batches(units_path)
    unit_hash, catalog_hash = digest(units_path), digest(catalog_path)
    from tools.assets.text_units import iter_units
    catalog = json.loads(Path(catalog_path).read_text(encoding="utf-8"))
    with Path(units_path).open(encoding="utf-8") as stream:
        for expected in iter_units(catalog):
            line = stream.readline()
            if not line or json.loads(line) != expected:
                raise HandoffError("source units do not exactly match catalog projection")
        if stream.readline():
            raise HandoffError("source units contain rows outside catalog projection")
    del catalog
    output.mkdir(parents=True)
    files, counts = {}, {"ready": 0, "unresolved": 0}
    for relative, rows in projected.items():
        path = output / relative
        path.parent.mkdir(exist_ok=True)
        with path.open("x", encoding="utf-8-sig", newline="") as stream:
            writer = csv.DictWriter(stream, fieldnames=FIELDS, delimiter="\t", lineterminator="\n")
            writer.writeheader()
            writer.writerows(rows)
        files[relative] = {"rows": len(rows), "sha256": digest(path)}
        counts[relative.split("/")[0]] += len(rows)
    evidence = output / "evidence"
    evidence.mkdir()
    compress(units_path, evidence / "units.jsonl.gz")
    compress(catalog_path, evidence / "catalog.json.gz")
    if digest(units_path) != unit_hash or digest(catalog_path) != catalog_hash:
        raise HandoffError("source changed while exporting")
    manifest = {"schema": "tokimemo.text.handoff.v1", "source_units_sha256": unit_hash,
                "source_catalog_sha256": catalog_hash, "counts": counts, "files": files,
                "maximum_batch_rows": BATCH_SIZE, "comprehensive_coverage": False}
    (output / "manifest.json").write_text(json_text(manifest) + "\n", encoding="utf-8")
    (output / "README.md").write_text(README, encoding="utf-8")
    with (output / "INDEX.tsv").open("x", encoding="utf-8-sig", newline="") as stream:
        writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        writer.writerow(("file", "records", "queue"))
        for relative, details in files.items():
            writer.writerow((relative, details["rows"], relative.split("/")[0]))
    return manifest


def import_returns(units_path, batch_paths, output, repository):
    output = safe_destination(output, repository)
    source_hash = digest(units_path)
    expected = batches(units_path)
    by_name = {Path(path).name: rows for path, rows in expected.items()}
    result, seen_files = [], set()
    for batch in batch_paths:
        batch = Path(batch)
        if batch.name not in by_name or batch.name in seen_files:
            raise HandoffError("unknown or duplicate batch filename: " + batch.name)
        seen_files.add(batch.name)
        originals = {row["id"]: row for row in by_name[batch.name]}
        seen = set()
        with batch.open(encoding="utf-8-sig", newline="") as stream:
            reader = csv.DictReader(stream, delimiter="\t", strict=True)
            if tuple(reader.fieldnames or ()) != FIELDS:
                raise HandoffError("batch columns changed: " + batch.name)
            for row in reader:
                identifier = row.get("id")
                if identifier not in originals or identifier in seen:
                    raise HandoffError("unknown or duplicate returned ID")
                seen.add(identifier)
                original = originals[identifier]
                if None in row or any(row.get(key) is None for key in FIELDS):
                    raise HandoffError("malformed row: " + identifier)
                if any(row[key] != original[key] for key in FIELDS if key not in EDITABLE):
                    raise HandoffError("protected field changed: " + identifier)
                target = row["translation"]
                references = [item["marker"] for item in json.loads(original["protected_tokens"])]
                if target and MARKER.findall(target) != references:
                    raise HandoffError("protected translation markers changed: " + identifier)
                remainder = MARKER.sub("", target)
                if "[[T" in remainder:
                    raise HandoffError("malformed translation marker: " + identifier)
                if target or row["translator_note"]:
                    result.append({"id": identifier, "target_text": target if target else None,
                                   "translator_note": row["translator_note"],
                                   "review_status": original["review_status"],
                                   "protected_tokens": json.loads(original["protected_tokens"]),
                                   "source_units_sha256": source_hash})
        if seen != set(originals):
            raise HandoffError("returned batch is missing rows: " + batch.name)
    if digest(units_path) != source_hash:
        raise HandoffError("source changed during validation")
    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open("x", encoding="utf-8") as stream:
        for row in result:
            stream.write(json_text(row) + "\n")
    return {"batches": len(seen_files), "returned_rows": len(result),
            "translated_rows": sum(row["target_text"] is not None for row in result),
            "source_units_sha256": source_hash}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("export", "import"))
    parser.add_argument("--units", type=Path, required=True)
    parser.add_argument("--catalog", type=Path)
    parser.add_argument("--batch", type=Path, action="append", default=[])
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--repository", type=Path, default=Path.cwd())
    args = parser.parse_args()
    try:
        if args.command == "export":
            if not args.catalog:
                parser.error("export requires --catalog")
            result = export_handoff(args.units, args.catalog, args.output, args.repository)
            print(json_text({"counts": result["counts"], "batches": len(result["files"])}))
        else:
            if not args.batch:
                parser.error("import requires at least one --batch")
            print(json_text(import_returns(args.units, args.batch, args.output, args.repository)))
    except (HandoffError, OSError, ValueError, csv.Error) as exc:
        parser.exit(1, "text handoff: " + str(exc) + "\n")


if __name__ == "__main__":
    main()
