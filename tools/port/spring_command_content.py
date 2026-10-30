"""Export source-backed April school-day media for commands 1–3 only.

This is an isolated T-29014 candidate. It deliberately does not claim club,
commands 5–6, vacation, sick-rest, or seasonal pack coverage.
"""
from __future__ import annotations

import hashlib
import argparse
import json
import os
import shutil
import sys
import tempfile
from pathlib import Path

TRACK_SHA = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
EXE_SHA = "bae1514fcc4bd8843c69803bf469632b004377b67f3e4408f0641c545fd843ae"
TABLES_SHA = "a1f30e57ef4f40ee7f9786043ccc5e007cfe316ba42fbd2c3e6896bf1d5f7060"
MEDIA_SHA = "0c85bbc54be82c7b2597528ccbfc915cd1fb5f99405bd18d845452026b56be37"
SCHEMA = "tokimemo.spring-school-day-content.v1"
COMMANDS = (1, 2, 3)
MAX_PNG = 2 * 1024 * 1024
MAX_JSON = 64 * 1024 * 1024


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def resolve_april_route(tables: dict, command: int, extra: int) -> dict:
    """Resolve the native non-summer table entry for an April school event."""
    if type(command) is not int or command not in COMMANDS:
        raise ValueError("only ordinary school commands 1–3 are profiled")
    if type(extra) is not int or not 0 <= extra <= 255:
        raise ValueError("event extra must be an unsigned byte")
    bg_table = tables.get("weekday_background_lbas") if isinstance(tables, dict) else None
    sd_sets = tables.get("weekday_sd_lbas") if isinstance(tables, dict) else None
    selectors = tables.get("sd_select_non_summer") if isinstance(tables, dict) else None
    if (not isinstance(bg_table, list) or len(bg_table) != 8
            or not isinstance(sd_sets, list) or len(sd_sets) != 25
            or not isinstance(selectors, list) or len(selectors) != 16
            or any(type(x) is not int for x in bg_table + sd_sets + selectors)):
        raise ValueError("native school-day tables do not match the reviewed selector shapes")
    variant = 0 if extra == 0 else (2 if command == 3 and extra == 5 else 1)
    sd_index = 2 * command + variant
    sd_set = selectors[sd_index]
    background_lba = bg_table[command]
    sd_lba = sd_sets[sd_set]
    return {"command": command, "event_extra": extra, "variant": variant,
            "background_lba": background_lba, "sd_table_index": sd_index,
            "sd_set_id": sd_set, "sd_lba": sd_lba,
            "season_domain": "months 3–5 (non-summer table)",
            "route_domain": "ordinary school day, non-vacation, non-sick-rest"}


def _safe_output(root: Path, value: str) -> Path:
    # Admit the lexical path before resolving it. A symlinked cache root must
    # not turn an otherwise valid-looking output into an outside write.
    supplied = Path(value).expanduser()
    if ".." in supplied.parts:
        raise ValueError("output path must not contain parent-directory segments")
    lexical = supplied if supplied.is_absolute() else root / supplied
    cache = root / ".asset-cache" / "port"
    try:
        lexical.relative_to(cache)
    except ValueError as exc:
        raise ValueError("output must be beneath repository .asset-cache/port") from exc
    cursor = root
    for part in lexical.relative_to(root).parts:
        cursor = cursor / part
        if cursor.is_symlink():
            raise ValueError("output path contains a symlink")
    out = lexical.resolve(strict=False)
    try:
        out.relative_to(cache.resolve(strict=False))
    except ValueError as exc:
        raise ValueError("resolved output escaped repository .asset-cache/port") from exc
    if out.exists():
        raise ValueError("output must be a new immutable directory")
    return out


def _copy_png(source: Path, destination: Path) -> dict:
    data = source.read_bytes()
    if not data.startswith(b"\x89PNG\r\n\x1a\n") or not data or len(data) > MAX_PNG:
        raise ValueError("reader output is missing, oversized, or not PNG")
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(data)
    return {"path": destination.name, "size": len(data), "sha256": _sha(data)}


def export_april_commands(input_path: str, table_manifest_path: str,
                          media_manifest_path: str, output_path: str, *, fresh_source: bool = False) -> dict:
    root_value = os.environ.get("TOKIMEMO_ROOT")
    if root_value:
        root = Path(root_value).resolve()
    else:
        root = next((p for p in Path(__file__).resolve().parents
                     if (p / "tools/assets/input.py").is_file()), None)
        if root is None:
            raise ValueError("repository root unavailable; set TOKIMEMO_ROOT")
    sys.path.insert(0, str(root))
    if fresh_source:
        # T-39002: regenerate proofs from the pinned original, not lost caches.
        from tools.port.school_day_tables import build_bundle
        tables_doc = build_bundle(input_path)
        tables_hash = _sha((json.dumps(tables_doc, sort_keys=True, indent=2) + "\n").encode())
        media_doc = None
    else:
        return _legacy_export(input_path, table_manifest_path, media_manifest_path, output_path, root)
    return _export_verified(input_path, output_path, root, tables_doc, tables_hash, media_doc)


def _legacy_export(input_path, table_manifest_path, media_manifest_path, output_path, root):
    for path, expected, label in ((Path(table_manifest_path), TABLES_SHA, "rule-table manifest"),
                                  (Path(media_manifest_path), MEDIA_SHA, "media manifest")):
        if path.is_symlink() or not path.is_file() or path.stat().st_size > MAX_JSON or _sha(path.read_bytes()) != expected:
            raise ValueError(label + " does not match the reviewed source publication")
    tables_doc = json.loads(Path(table_manifest_path).read_text(encoding="utf-8"))
    media_doc = json.loads(Path(media_manifest_path).read_text(encoding="utf-8"))
    return _export_verified(input_path, output_path, root, tables_doc, TABLES_SHA, media_doc)


def _export_verified(input_path, output_path, root, tables_doc, tables_hash, media_doc):
    from tools.assets.input import open_input
    from tools.assets.image_packs_sd import export_sd_ui
    from tools.assets.image_mdec import export_mdec
    from tools.assets.bs_cohort_profile import build_profile_context
    from tools.assets.manifest import asset_id
    if tables_doc.get("schema") != "tokimemo.port-rule-tables.v1" or tables_doc.get("provenance", {}).get("release_executable", {}).get("sha256") != EXE_SHA:
        raise ValueError("rule tables do not identify the reviewed original executable")
    tables = tables_doc.get("tables")
    if not isinstance(tables, dict):
        raise ValueError("rule-table manifest is malformed")
    if media_doc is not None and (media_doc.get("schema") != "tokimemo.media.v1" or media_doc.get("input", {}).get("source_sha256") != TRACK_SHA):
        raise ValueError("reviewed media inventory has the wrong source identity")
    original = {a["source"]["path"]: a for a in (media_doc or {}).get("assets", [])
                if isinstance(a, dict) and isinstance(a.get("source"), dict)}
    out = _safe_output(root, output_path)
    out.parent.mkdir(parents=True, exist_ok=True)
    stage = Path(tempfile.mkdtemp(prefix=".t29014-spring-content-", dir=out.parent))
    try:
        components: dict[str, dict] = {}
        with open_input(input_path) as source:
            desc = source.describe()
            if desc.get("format") != "mode2-2352" or desc.get("source_sha256") != TRACK_SHA:
                raise ValueError("original input is not the pinned MODE2 release")
            entries = source.entries()
            by_path = {e.get("path"): e for e in entries if isinstance(e, dict)}
            exe_entry = by_path.get("SLPM_86.053")
            if exe_entry is None or _sha(source.read(exe_entry)) != EXE_SHA:
                raise ValueError("original executable hash mismatch")
            profile = build_profile_context(source, entries, root, "weekly")
            bg_by_command = {int(a["schedule_girl_index"]): a for a in profile["assets"]}
            routes = []
            for command in COMMANDS:
                # Normal opening-week actor event carries extra 0xF (no companion).
                route = resolve_april_route(tables, command, 0xF)
                bg_profile = bg_by_command.get(command)
                if bg_profile is None or bg_profile.get("lba_word") != route["background_lba"]:
                    raise ValueError("background table selection disagrees with trusted weekly source profile")
                bg_path = bg_profile["path"]
                bg_entry = by_path.get(bg_path)
                bg_data = source.read(bg_entry) if bg_entry else b""
                indexed_bg = original.get(bg_path)
                if media_doc is None:
                    # Whole-track identity, source-code pins and table/profile
                    # agreement above bind this exact entry without a census.
                    if bg_entry is None or bg_entry.get("location", {}).get("lba") != route["background_lba"]:
                        raise ValueError("source background LBA differs from the route table")
                    indexed_bg = {"id": asset_id(bg_path, _sha(bg_data)),
                                  "source": {"sha256": _sha(bg_data), "location": bg_entry["location"]}}
                if (not indexed_bg or indexed_bg["source"].get("sha256") != _sha(bg_data)
                        or indexed_bg["source"].get("location", {}).get("lba") != route["background_lba"]):
                    raise ValueError("background differs from the reviewed original media inventory")
                bg_context = {"width": 320, "height": 88, "padded_height": 96,
                              "source_profile": profile["source_profile"], "geometry": profile["geometry"],
                              "evidence": bg_profile["source_evidence"]}
                bg_id = indexed_bg["id"]
                bg_dir = stage / "reader" / bg_id
                bg_result = export_mdec(bg_data, bg_dir, bg_id, source_path=bg_path, context=bg_context)
                bgfiles = {name: _copy_png(bg_dir / name, stage / "assets" / name)
                           for name in bg_result["files"]}

                sd_entry = next((e for e in entries if e.get("kind") == "file"
                                 and e.get("location", {}).get("lba") == route["sd_lba"]
                                 and str(e.get("path", "")).startswith("CDROM/MAIN_SD/")), None)
                if sd_entry is None:
                    raise ValueError("native SD table LBA has no exact 41-sector MAIN_SD source")
                sd_data = source.read(sd_entry)
                if len(sd_data) != 41 * 2048:
                    raise ValueError("native SD table LBA has no exact 41-sector MAIN_SD source")
                sd_id = "source-" + _sha(sd_data)
                sd_dir = stage / "reader" / sd_id
                sd_result = export_sd_ui(sd_data, source.read(exe_entry), sd_dir, sd_id, source_entry=sd_entry)
                if sd_result.get("status") != "exported":
                    raise ValueError("existing SD reader rejected the source-selected pack")
                frames = []
                for frame in sd_result["images"]:
                    files = {"png": _copy_png(sd_dir / frame["png"], stage / "assets" / frame["png"]),
                             "stp_png": _copy_png(sd_dir / frame["stp_png"], stage / "assets" / frame["stp_png"])}
                    frames.append({"frame_index": frame["frame_index"], "bounds": [*frame["bounds"][:2], frame["width"], frame["height"]],
                                   "origin": frame["origin"], "parts": frame["parts"], "source_palette_words": frame["source_palette_words"],
                                   **files})
                meta = sd_result["metadata"]
                if not meta.get("source_sequence_prefix"):
                    raise ValueError("selected source pack has no decoded sequence prefix")
                key = f"command_{command}"
                components[key] = {"route": route, "background": {"source": bg_profile["source"],
                    "geometry_context": bg_context, "decoder_model": bg_result["metadata"]["decoder_model"], "reader": "tools/assets/image_mdec.py:export_mdec", "files": bgfiles},
                    "sd": {"source": {"path": sd_entry["path"], "lba": route["sd_lba"], "size": len(sd_data), "sha256": _sha(sd_data), "exe_sha256": EXE_SHA},
                    "source_id": sd_id, "profile_index": route["sd_set_id"], "frames": frames,
                    "sequences": meta["source_sequence_prefix"], "palette_mode": meta["actor_runtime_palette_mode"],
                    "stp_policy": meta["stp_policy"], "reader": "tools/assets/image_packs_sd.py:export_sd_ui"}}
                routes.append(route)
        manifest = {"schema": SCHEMA, "version": 1,
                    "provenance": {"original_input_sha256": TRACK_SHA, "original_executable_sha256": EXE_SHA,
                                   "rule_tables_manifest_sha256": tables_hash, "reviewed_media_publication_sha256": MEDIA_SHA if media_doc is not None else None,
                                   "readers": ["tools/assets/input.py:open_input", "tools/assets/image_packs_sd.py:export_sd_ui",
                                               "tools/assets/bs_cohort_profile.py:build_profile_context", "tools/assets/image_mdec.py:export_mdec"]},
                    "route_capabilities": ["command_1_april_school_day", "command_2_april_school_day", "command_3_april_school_day"],
                    "unsupported": {"command_4": "club/club-id dependent profile not included", "commands_5_6": "separate BS cohort not profiled",
                                    "seasonal": "only non-summer table; summer selector variants not included", "vacation": "not included",
                    "sick_rest": "not included", "conditional_companion_variants": "only opening-week event extra 0xF; companion and athletics-special variants not included",
                    "scene_composition": "not established", "full_route": "not claimed"},
                    "routes": routes, "components": components}
        (stage / "manifest.json").write_text(json.dumps(manifest, sort_keys=True, separators=(",", ":")) + "\n", encoding="utf-8")
        # Reader scratch contains duplicate renderings and is not runtime input.
        shutil.rmtree(stage / "reader", ignore_errors=True)
        if out.exists():
            raise ValueError("output appeared during export; refusing overwrite")
        os.rename(stage, out)
        return {"path": str(out), "manifest_sha256": _sha((out / "manifest.json").read_bytes()),
                "route_capabilities": manifest["route_capabilities"]}
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="original MODE2 track or accepted input container")
    parser.add_argument("--tables", help="exact reviewed school-day table manifest")
    parser.add_argument("--media-manifest", help="exact reviewed original media inventory")
    parser.add_argument("--fresh-source", action="store_true", help="regenerate source proofs from the pinned original disc")
    parser.add_argument("--output", required=True, help="new directory under ignored .asset-cache/port")
    args = parser.parse_args()
    if not args.fresh_source and (not args.tables or not args.media_manifest):
        parser.error("provide --fresh-source or both --tables and --media-manifest")
    print(json.dumps(export_april_commands(args.input, args.tables, args.media_manifest, args.output, fresh_source=args.fresh_source), sort_keys=True))


if __name__ == "__main__":
    main()
