"""Stage a code-only desktop export inside Docker; never copy game assets."""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil

SOURCE_SUFFIXES = {".gd", ".tscn", ".tres", ".gdshader", ".json"}
SCENES = {
    "week": "res://app/playable.tscn",
    "new-game": "res://app/new_game_entry.tscn",
}


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(repository: Path, destination: Path, scene: str) -> dict:
    """Copy only source files into a fresh build directory, recording every byte."""
    if ".." in os.fspath(destination).split("/"):
        raise ValueError("desktop output cannot contain parent traversal")
    repository = repository.resolve(strict=True)
    destination = Path(os.path.abspath(destination))
    allowed = repository / ".asset-cache/port"
    if allowed not in destination.parents:
        raise ValueError("desktop output must be below the ignored port cache")
    for component in [destination, *destination.parents]:
        if component.is_symlink():
            raise ValueError("desktop output cannot traverse symlinks")
    if destination.parent.exists():
        raise ValueError("desktop build directory must be new")
    project = repository / "port/godot"
    for component in [project, *project.parents]:
        if component.is_symlink():
            raise ValueError("project source ancestry cannot traverse symlinks")
    selected = []
    for path in sorted(project.rglob("*")):
        relative = path.relative_to(project)
        if any(part.startswith(".") for part in relative.parts) or "tests" in relative.parts:
            continue
        if path.is_symlink():
            raise ValueError(f"project symlink is not exportable: {relative}")
        if not path.is_file():
            continue
        if path.name == "PORTING.md":
            continue
        if path.name != "project.godot" and path.suffix not in SOURCE_SUFFIXES:
            raise ValueError(f"non-source project file requires explicit review: {relative}")
        selected.append((path, relative))
    if not any(str(relative) == SCENES[scene].removeprefix("res://") for _, relative in selected):
        raise ValueError("selected development scene is missing")
    config = (project / "project.godot").read_text()
    original_scene = 'run/main_scene="res://shell.tscn"'
    if config.count(original_scene) != 1:
        raise ValueError("project main-scene contract changed; review desktop staging")
    # Only the staged copy changes; the canonical project remains untouched.
    config = config.replace(original_scene, f'run/main_scene="{SCENES[scene]}"')
    # The pinned engine requires this import format for universal macOS exports.
    texture_setting = "textures/vram_compression/import_s3tc_bptc=true"
    if "textures/vram_compression/import_s3tc_bptc=" in config:
        raise ValueError("project texture-import setting changed; review desktop staging")
    if "[rendering]" in config:
        config = config.replace("[rendering]", "[rendering]\n" + texture_setting, 1)
    else:
        config += "\n[rendering]\n" + texture_setting + "\n"
    destination.mkdir(parents=True)
    source_hashes = {}
    for source, relative in selected:
        target = destination / relative
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)
        source_hashes[str(relative)] = digest(target)
        if source_hashes[str(relative)] != digest(source):
            raise ValueError("project source changed during staging")
    (destination / "project.godot").write_text(config)
    preset = repository / "tools/port/desktop_export.cfg"
    shutil.copyfile(preset, destination / "export_presets.cfg")
    return {
        "schema": "tokimemo.desktop-stage.v1",
        "scene": scene,
        "staged_s3tc_bptc_import": True,
        "source_files": source_hashes,
        "staged_project_sha256": digest(destination / "project.godot"),
        "preset_sha256": digest(preset),
        "engine_lock_sha256": digest(repository / "tools/port/godot.lock.json"),
        "source_only_allowlist_applied": True,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository", type=Path, required=True)
    parser.add_argument("--destination", type=Path, required=True)
    parser.add_argument("--scene", choices=SCENES, default="week")
    args = parser.parse_args()
    receipt = prepare(args.repository, args.destination, args.scene)
    (Path(os.path.abspath(args.destination)).parent / "source-receipt.json").write_text(
        json.dumps(receipt, indent=2) + "\n"
    )
    print(f"Staged {len(receipt['source_files'])} source files for {args.scene}")


if __name__ == "__main__":
    main()
