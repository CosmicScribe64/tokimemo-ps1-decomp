#!/usr/bin/env python3
"""Build a fresh, source-bound installed-runtime bundle from an original disc input.

This developer tool composes existing canonical exporters. It does not claim that
the resulting optional packs make every game route playable. Exporter output is
captured privately and never copied from a prior cache entry.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import shutil
import subprocess
import sys
import tempfile


ROOT = Path(__file__).resolve().parents[2]
PROFILE = "original-ja-ps-the-best"
SCHEMA = "tokimemo.installed-runtime.v1"
ROLE_KINDS = {
    "name_entry_rules": "file", "name_entry_content": "file",
    "font_root": "dir", "rule_tables": "file",
    "school_day_media": "dir", "spring_media": "dir",
    "weekday56_media": "dir", "club_media": "dir",
    "geko_dialogue": "dir",
    "hud_manifest": "file", "hud_context": "file",
    "holiday_scene": "file", "holiday_text": "file",
    "date_preview": "dir", "opening_pack": "dir",
    "prologue_text": "dir", "first_arrival_media": "dir",
    "prologue_hud": "dir", "opening_audio": "dir",
    "original_first_arrival_bgm": "dir",
}


class BundleError(ValueError):
    pass


def _sha(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def _sha_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _inside(path: Path, root: Path) -> bool:
    try:
        path.resolve(strict=False).relative_to(root.resolve(strict=True))
        return True
    except (OSError, ValueError):
        return False


def _safe_relative(value: object) -> bool:
    if not isinstance(value, str) or not value or "\\" in value or ":" in value:
        return False
    path = PurePosixPath(value)
    return (not path.is_absolute() and path.as_posix() == value
            and all(part not in ("", ".", "..") for part in path.parts))


def _safe_tree(root: Path) -> list[Path]:
    """Return regular files under root, rejecting symlinks and special files."""
    result = []
    for current, dirs, files in os.walk(root, followlinks=False):
        base = Path(current)
        for name in dirs + files:
            item = base / name
            if item.is_symlink():
                raise BundleError("generated output contains a symlink")
            if not _inside(item, root):
                raise BundleError("generated output escapes its bundle")
            if item.is_file():
                result.append(item)
            elif not item.is_dir():
                raise BundleError("generated output contains a special file")
    return sorted(result, key=lambda p: p.relative_to(root).as_posix())


def _manifest(root: Path, executable: Path, track: Path | None,
              roles: dict[str, str], coverage: dict) -> dict:
    root = root.resolve(strict=True)
    files = {}
    for path in _safe_tree(root):
        if path.name == "manifest.json" and path.parent == root:
            continue
        rel = path.relative_to(root).as_posix()
        if PurePosixPath(rel).is_absolute() or ".." in PurePosixPath(rel).parts:
            raise BundleError("unsafe relative file path")
        files[rel] = {"size": path.stat().st_size, "sha256": _sha(path)}
    for role, rel in roles.items():
        if not isinstance(role, str) or role not in ROLE_KINDS:
            raise BundleError("unknown installed-runtime role")
        if not _safe_relative(rel):
            raise BundleError("role locator must be a canonical relative path")
        target = root / rel
        if target.is_symlink() or not _inside(target, root) or not target.exists():
            raise BundleError("role target is absent or escapes bundle")
        if ROLE_KINDS[role] == "file" and not target.is_file():
            raise BundleError("file role does not name a file")
        if ROLE_KINDS[role] == "dir" and not target.is_dir():
            raise BundleError("directory role does not name a directory")
    source = {"executable_sha256": _sha(executable)}
    if track is not None:
        source["track_sha256"] = _sha(track)
    return {"schema": SCHEMA, "version": 1, "release_profile": PROFILE,
            "source": source, "files": files, "roles": roles,
            "coverage": coverage}


def _run(command: list[str], label: str) -> tuple[bool, str]:
    """Run a canonical exporter without exposing its output to the terminal."""
    try:
        proc = subprocess.run(command, cwd=ROOT, stdout=subprocess.DEVNULL,
                              stderr=subprocess.DEVNULL, check=False)
    except OSError:
        return False, label + ": exporter could not start"
    return proc.returncode == 0, ("" if proc.returncode == 0 else
                                  label + ": exporter rejected this source/profile")


def _copy_pack(source: Path, destination: Path) -> None:
    if not source.is_dir() or source.is_symlink():
        raise BundleError("exporter did not produce a regular pack directory")
    _safe_tree(source)
    if destination.exists():
        raise BundleError("duplicate role destination")
    shutil.copytree(source, destination, symlinks=False)
    _safe_tree(destination)


def _discard_partial_role(stage: Path, role_path: str) -> None:
    if not _safe_relative(role_path):
        raise BundleError("cannot clean an unsafe partial role path")
    partial = stage / role_path
    if partial.is_symlink():
        raise BundleError("partial role path became a symlink")
    if partial.exists():
        shutil.rmtree(partial)


def _disc_root() -> Path:
    for project in (ROOT, *ROOT.parents):
        candidate = project / "disc"
        if candidate.is_dir() and (candidate / "files" / "SLPM_86.053").is_file():
            return candidate.resolve(strict=True)
    raise BundleError("original disc/ source directory is unavailable")


def _check_track_binding(input_path: Path, track: Path | None) -> None:
    if track is not None and (not input_path.is_file() or input_path != track):
        raise BundleError("track SHA can be recorded only when --input is that same raw track file")


def _export_geko_sections(input_path: Path, cache: Path, token: str,
                          stage: Path, work_paths: list[Path]) -> None:
    """Build the twelve canonical GEKO sections directly from the input track."""
    sys.path.insert(0, str(ROOT))
    from tools.assets.input import open_input
    from tools.assets.script_profiles import PROFILES
    from tools.assets.text import parse_script
    from tools.assets.script_validation import validate_script
    from tools.port.dialogue_content import write_pack

    source_path = "CDROM/SERIFU/GEKO.BIN"
    matches = [item for item in PROFILES if item.get("path") == source_path]
    if len(matches) != 1 or len(matches[0].get("sections", [])) != 12:
        raise BundleError("GEKO source profile is missing or has changed cardinality")
    profile = matches[0]
    with open_input(str(input_path)) as source:
        entries = [item for item in source.entries() if item.get("path") == source_path]
        if len(entries) != 1:
            raise BundleError("original input does not contain one profiled GEKO file")
        raw = source.read(entries[0])
    parsed = parse_script(raw, source_path, profile)
    validation = validate_script(raw, source_path, profile, parsed)
    catalog = {"schema": "tokimemo.text.catalog.v1",
               "files": [{"path": source_path, "status": "exported",
                          "script": parsed, "validation": validation}]}
    temp_catalog = None
    destination_root = stage / "packs" / "geko_dialogue"
    working_root = stage / "packs" / (".geko-dialogue-" + token)
    working_root.mkdir(parents=True, exist_ok=False)
    index_sections = []
    try:
        with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8", suffix=".json",
                                         prefix=".runtime-geko-catalog-", dir=cache,
                                         delete=False) as stream:
            temp_catalog = Path(stream.name)
            json.dump(catalog, stream, ensure_ascii=False, sort_keys=True,
                      separators=(",", ":"))
            stream.write("\n")
        for section in profile["sections"]:
            section_id = section["id"]
            work = cache / (".runtime-" + token + "-geko-" + section_id + ".json")
            if work.exists() or work.is_symlink():
                raise BundleError("fresh GEKO exporter work path already exists")
            work_paths.append(work)
            write_pack(temp_catalog, source_path, section_id, input_path, work)
            target = working_root / section_id
            target.mkdir()
            shutil.copy2(work, target / "content.json")
            index_sections.append({"id": section_id, "group_count": section["group_count"],
                                   "path": section_id + "/content.json",
                                   "component_sha256": _sha(target / "content.json")})
        (working_root / "index.json").write_text(
            json.dumps({"schema": "tokimemo.installed-runtime.geko-dialogue.v1",
                        "source_path": source_path, "source_sha256": _sha_bytes(raw),
                        "section_count": len(index_sections), "sections": index_sections,
                        "semantic_coverage_complete": False,
                        "runtime_execution_proven": False},
                       sort_keys=True, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        if destination_root.exists() or destination_root.is_symlink():
            raise BundleError("GEKO role destination appeared during generation")
        os.replace(working_root, destination_root)
    finally:
        if temp_catalog is not None:
            temp_catalog.unlink(missing_ok=True)
        if working_root.exists() and not working_root.is_symlink():
            shutil.rmtree(working_root, ignore_errors=True)


def build(input_path: Path, executable: Path, output: Path,
          track: Path | None = None, *, opening_only: bool = False) -> Path:
    if input_path.is_symlink() or executable.is_symlink() or (track is not None and track.is_symlink()):
        raise BundleError("original inputs must not be symlinks")
    input_path = input_path.resolve(strict=True)
    executable = executable.resolve(strict=True)
    if not input_path.is_file() and not input_path.is_dir():
        raise BundleError("input must be a file or directory")
    if not executable.is_file() or executable.is_symlink():
        raise BundleError("executable must be a regular file")
    if track is not None:
        track = track.resolve(strict=True)
        if not track.is_file() or track.is_symlink():
            raise BundleError("track must be a regular file")
    _check_track_binding(input_path, track)
    original_root = _disc_root()
    for original in (input_path, executable, track):
        if original is None:
            continue
        if not _inside(original, original_root):
            raise BundleError("original inputs must stay beneath the detected disc/ directory")

    cache = ROOT / ".asset-cache" / "port"
    if cache.is_symlink() or cache.parent.is_symlink():
        raise BundleError("port cache path must not contain symlinks")
    cache.mkdir(parents=True, exist_ok=True)
    cache = cache.resolve(strict=True)
    if cache.parent != (ROOT / ".asset-cache").resolve(strict=True):
        raise BundleError("port cache escaped repository")
    if output.is_symlink():
        raise BundleError("output destination must not be a symlink")
    output = output.resolve(strict=False)
    if not _inside(output.parent, cache) or output == cache or output.exists():
        raise BundleError("output must be a new direct bundle under .asset-cache/port")
    if not _inside(cache, ROOT):
        raise BundleError("port cache escapes repository")

    # Exporters require their own immutable pack directories directly below the
    # cache. These fresh sibling packs are composed into the requested bundle.
    token = hashlib.sha256((str(output) + _sha(executable)).encode()).hexdigest()[:12]
    work_dirs = {key: cache / (".runtime-" + token + "-" + key)
                 for key in ("rules", "entry_rules", "entry", "weekday", "club",
                             "date", "holiday", "opening", "prologue_text",
                             "first_arrival_media", "prologue_hud", "opening_audio")}
    if any(p.exists() or p.is_symlink() for p in work_dirs.values()):
        raise BundleError("fresh exporter work path already exists")
    stage = Path(tempfile.mkdtemp(prefix=".runtime-bundle-", dir=cache))
    generated_font = False
    additional_work_paths: list[Path] = []
    roles: dict[str, str] = {}
    gaps: dict[str, str] = {}
    successes: dict[str, Path] = {}
    py = sys.executable
    src = str(input_path)
    commands = [
        ("rules", "rule_tables", [py, "tools/port/school_day_tables.py", "--input", src,
                                     "--output", str(work_dirs["rules"])]),
        ("entry_rules", "name_entry_rules", [py, "tools/port/new_game_tables.py", "--input", src,
                                                "--output", str(work_dirs["entry_rules"])]),
        ("entry", "name_entry_content", [py, "tools/port/new_game_entry_content.py", "--input", src,
                                            "--output", str(work_dirs["entry"])]),
        ("weekday", "weekday56_media", [py, "tools/port/weekday_56_content.py", "--input", src,
                                           "--output", str(work_dirs["weekday"])]),
        ("club", "club_media", [py, "tools/port/club_command_content.py", "--input", src,
                                  "--output", str(work_dirs["club"]), "--month", "4"]),
        ("date", "date_preview", [py, "tools/port/date_scene_preview_content.py", "--disc", src,
                                   "--exe", str(executable), "--output", str(work_dirs["date"])]),
        ("holiday", "holiday_text", [py, "tools/port/holiday_hud_content.py", "--repository", str(ROOT),
                                       "--executable", str(executable), "--output", str(work_dirs["holiday"])]),
        ("opening", "opening_pack", [py, "tools/port/opening_runtime_content.py", "--input", src,
                                        "--output", str(work_dirs["opening"])]),
        ("prologue_text", "prologue_text", [py, "tools/port/prologue_runtime_content.py", "--input", src,
                                            "--repository", str(ROOT), "--output", str(work_dirs["prologue_text"])]),
        ("first_arrival_media", "first_arrival_media", [py, "tools/port/first_arrival_content.py", "--input", str(track or input_path),
                                                        "--source-root", str(ROOT), "--output", str(work_dirs["first_arrival_media"])]),
    ]
    if opening_only:
        needed = {"entry_rules", "entry", "opening", "prologue_text", "first_arrival_media"}
        for key, role, _command in commands:
            if key not in needed:
                gaps[role] = "outside the requested first-arrival resource scope"
        commands = [row for row in commands if row[0] in needed]
    try:
        for key, role, command in commands:
            ok, reason = _run(command, role)
            if ok and work_dirs[key].is_dir():
                successes[role] = work_dirs[key]
            else:
                gaps[role] = reason or role + ": expected output missing"

        if opening_only:
            gaps["geko_dialogue"] = "outside the requested first-arrival resource scope"
        else:
            try:
                _export_geko_sections(input_path, cache, token, stage, additional_work_paths)
                roles["geko_dialogue"] = "packs/geko_dialogue"
            except (OSError, ValueError, KeyError, TypeError):
                _discard_partial_role(stage, "packs/geko_dialogue")
                gaps["geko_dialogue"] = "fresh profiled GEKO sections could not be assembled"

        # The canonical font exporter has a fixed output root. Only run it when
        # its immutable destination is absent; copy the newly created package,
        # then remove exactly that package. Existing cache data is never read.
        fresh_font = cache / "fonts" / "native-font-v2"
        if fresh_font.exists() or fresh_font.is_symlink():
            gaps["font_root"] = "font_root: canonical immutable destination already exists; refusing cache reuse"
        else:
            # The fixed destination was absent above, so any partial tree that
            # the canonical writer leaves on failure belongs to this run.
            generated_font = True
            try:
                sys.path.insert(0, str(ROOT))
                from tools.port.font_content import write_external_font_content
                write_external_font_content(executable.read_bytes(), cache,
                                            repository=ROOT, files_root=original_root / "files")
                ok, reason = True, ""
            except (OSError, ValueError, RuntimeError):
                ok, reason = False, "font_root: source proof or canonical export failed"
            if ok and fresh_font.is_dir():
                try:
                    font_destination = stage / ".asset-cache" / "port" / "fonts" / "native-font-v2"
                    font_destination.parent.mkdir(parents=True, exist_ok=True)
                    _copy_pack(fresh_font, font_destination)
                    roles["font_root"] = ".asset-cache/port/fonts/native-font-v2"
                    shutil.rmtree(fresh_font)
                    if fresh_font.parent.exists() and not any(fresh_font.parent.iterdir()):
                        fresh_font.parent.rmdir()
                except Exception:
                    gaps["font_root"] = "font_root: fresh package failed bundle validation"
            else:
                gaps["font_root"] = reason or "font_root: expected output missing"

        if "font_root" in roles:
            # The portable HUD profile names final bundle-relative resources;
            # its fresh font dependency must already be present in this stage.
            hud_destination = stage / "packs" / "prologue_hud"
            hud_destination.parent.mkdir(parents=True, exist_ok=True)
            ok, reason = _run([py, "tools/port/prologue_hud_content.py", "--input", src,
                               "--source-root", str(ROOT), "--output", str(hud_destination)], "prologue_hud")
            if ok and hud_destination.is_dir():
                roles["prologue_hud"] = "packs/prologue_hud"
            else:
                _discard_partial_role(stage, "packs/prologue_hud")
                gaps["prologue_hud"] = reason or "prologue_hud: fresh original source export failed"
        else:
            gaps["prologue_hud"] = "prologue_hud: fresh original font dependency unavailable"

        bgm_destination = stage / "audio" / "first-arrival"
        ok, reason = _run([py, "tools/port/first_arrival_bgm_content.py", "--input", src,
                           "--output", str(bgm_destination)], "original_first_arrival_bgm")
        if ok and bgm_destination.is_dir():
            roles["original_first_arrival_bgm"] = "audio/first-arrival"
        else:
            _discard_partial_role(stage, "audio/first-arrival")
            gaps["original_first_arrival_bgm"] = reason or "fresh original BGM source export failed"

        # The original command-0/7 media and HUD room packs need publication
        # artifacts with no matching fresh producer. Do not reuse an old cache.
        gaps["school_day_media"] = "fresh command-0/7 route/profile publication inputs unavailable"
        gaps["spring_media"] = "fresh media/profile hashes for the spring exporter are not established"
        gaps["hud_manifest"] = "fresh room-manifest producer is unavailable"
        gaps["hud_context"] = "fresh room-manifest producer is unavailable"
        gaps["holiday_scene"] = "no separate holiday-scene pack producer is available"

        for role, source_dir in successes.items():
            # Rules are a manifest file; other products remain pack directories.
            if role in ("rule_tables", "name_entry_rules", "name_entry_content", "holiday_text"):
                filename = "entry.json" if role == "name_entry_content" else "manifest.json"
                payload = source_dir / filename
                if not payload.is_file():
                    gaps[role] = "exporter output has no expected role file"
                    continue
                role_path = "packs/" + role + "/" + filename
                dest = stage / role_path
                dest.parent.mkdir(parents=True, exist_ok=True)
                shutil.copy2(payload, dest)
                roles[role] = role_path
            else:
                role_path = "packs/" + role
                _copy_pack(source_dir, stage / role_path)
                roles[role] = role_path

        coverage = {"available_roles": sorted(roles),
                    "unavailable_roles": gaps,
                    "limitations": [
                        "rule and route packs are finite source-backed profiles",
                        "opening_pack contains source-bound bank/sequence bytes, but playback and presentation are unproven",
                        "capabilities must be computed by the receiving GDScript validator",
                        "this bundle does not establish complete runtime or dialogue coverage",
                    ]}
        manifest = _manifest(stage, executable, track, roles, coverage)
        (stage / "manifest.json").write_text(json.dumps(manifest, sort_keys=True,
                                       ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
        _safe_tree(stage)
        if output.exists() or output.is_symlink():
            raise BundleError("output appeared while generating bundle")
        os.replace(stage, output)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    finally:
        # Remove only work directories created by this invocation.
        for path in work_dirs.values():
            if path.exists() and not path.is_symlink():
                shutil.rmtree(path, ignore_errors=True)
        for path in additional_work_paths:
            if path.exists() and not path.is_symlink():
                path.unlink(missing_ok=True)
        if generated_font and fresh_font.exists() and not fresh_font.is_symlink():
            shutil.rmtree(fresh_font, ignore_errors=True)
            if fresh_font.parent.exists() and not any(fresh_font.parent.iterdir()):
                fresh_font.parent.rmdir()
    return output / "manifest.json"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", type=Path, required=True)
    parser.add_argument("--executable", type=Path, required=True)
    parser.add_argument("--track", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    parser.add_argument("--opening-only", action="store_true", help="generate only source-backed first-arrival prerequisites")
    args = parser.parse_args()
    try:
        result = build(args.input, args.executable, args.output, args.track, opening_only=args.opening_only)
    except (BundleError, OSError, ValueError) as exc:
        print("installed-runtime export failed: " + str(exc), file=sys.stderr)
        return 2
    print("installed-runtime manifest: " + str(result.relative_to(ROOT)))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
