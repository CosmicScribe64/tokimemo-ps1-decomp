"""Stage a desktop export inside Docker; never copy game assets.

Only project source is staged, plus, for Linux arm64, the reviewed native
original-audio library and its descriptor at the hashes pinned by the
production BGM bridge. Other targets are code-only: the bridge rejects their
platform at runtime, so no unreviewed native code is shipped.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
from pathlib import Path
import re
import shutil

SOURCE_SUFFIXES = {".gd", ".tscn", ".tres", ".gdshader", ".json"}
SCENES = {
    # The production boot shell: installed-content checks, then setup or entry.
    "player": "res://shell.tscn",
    "week": "res://app/playable.tscn",
    "new-game": "res://app/new_game_entry.tscn",
}
# Export preset per target. Only linux-arm64 has a reviewed native audio library.
TARGETS = {
    "linux-arm64": {"preset": "Linux arm64", "native": True},
    "macos": {"preset": "macOS Development", "native": False},
}
NATIVE_BRIDGE = "services/first_arrival_bgm_bridge.gd"
NATIVE_DESCRIPTOR_UID = "original_audio.gdextension.uid"


def native_pins(project: Path) -> dict:
    """Read the reviewed library/descriptor identities from the runtime bridge."""
    text = (project / NATIVE_BRIDGE).read_text()
    pins = {}
    for name in ("LIBRARY", "LIBRARY_SHA", "DESCRIPTOR", "DESCRIPTOR_SHA"):
        found = re.findall(rf'^const {name} := "([^"]+)"$', text, re.MULTILINE)
        if len(found) != 1:
            raise ValueError(f"native bridge pin {name} is missing or ambiguous")
        pins[name] = found[0]
    for name in ("LIBRARY", "DESCRIPTOR"):
        if not pins[name].startswith("res://") or ".." in pins[name].split("/"):
            raise ValueError(f"native bridge path {name} is not a project path")
        pins[name] = pins[name].removeprefix("res://")
    for name in ("LIBRARY_SHA", "DESCRIPTOR_SHA"):
        if not re.fullmatch(r"[0-9a-f]{64}", pins[name]):
            raise ValueError(f"native bridge pin {name} is not a SHA-256")
    return pins


def native_files(project: Path) -> dict[str, str]:
    """Return {relative path: SHA-256} for the reviewed native files, verifying pins."""
    pins = native_pins(project)
    library = project / pins["LIBRARY"]
    descriptor = project / pins["DESCRIPTOR"]
    for path in (library, descriptor):
        if path.is_symlink() or not path.is_file():
            raise ValueError(f"reviewed native file is missing: {path.relative_to(project)}")
    if digest(library) != pins["LIBRARY_SHA"]:
        raise ValueError("native library differs from the hash pinned by the BGM bridge")
    if digest(descriptor) != pins["DESCRIPTOR_SHA"]:
        raise ValueError("native descriptor differs from the hash pinned by the BGM bridge")
    entries = re.findall(r'^(linux\.(?:debug|release)\.arm64) = "res://([^"]+)"$',
                         descriptor.read_text(), re.MULTILINE)
    libraries_section = descriptor.read_text().split("[libraries]", 1)[-1]
    declared = [line for line in libraries_section.splitlines() if "=" in line]
    if len(entries) != 2 or len(declared) != 2 or {path for _, path in entries} != {pins["LIBRARY"]}:
        raise ValueError("native descriptor must declare only the reviewed Linux arm64 library")
    files = {pins["LIBRARY"]: pins["LIBRARY_SHA"], pins["DESCRIPTOR"]: pins["DESCRIPTOR_SHA"]}
    uid = project / NATIVE_DESCRIPTOR_UID
    if uid.is_file() and not uid.is_symlink():
        _check_uid(uid)
        files[NATIVE_DESCRIPTOR_UID] = digest(uid)
    return files


def _check_uid(path: Path) -> None:
    uid = path.read_text().strip()
    if not uid.startswith("uid://") or not uid[6:].isalnum():
        raise ValueError(f"invalid UID: {path.name}")


def _check_scene_ids(path: Path, relative: Path) -> None:
    """Reject a scene node without a stored unique_id.

    Godot 4.7 assigns a random unique_id to every such node when it converts the
    scene to binary at export, so the .pck would differ between identical builds.
    """
    for line in path.read_text().splitlines():
        if line.startswith("[node ") and not re.search(r" unique_id=\d+\]$", line):
            raise ValueError(f"scene node has no stored unique_id (export would not be reproducible); "
                             f"re-save it in the pinned Godot editor: {relative}")


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def prepare(repository: Path, destination: Path, scene: str, target: str = "macos") -> dict:
    """Copy only source files into a fresh build directory, recording every byte."""
    if target not in TARGETS:
        raise ValueError(f"unsupported desktop target: {target}")
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
    native = native_files(project) if TARGETS[target]["native"] else {}
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
        if str(relative) in native:
            selected.append((path, relative))
            continue
        # Godot writes these stable script identifiers on the first editor import.
        # Retain them only alongside a selected script, not as arbitrary payloads.
        script_uid = path.name.endswith(".gd.uid") and path.with_suffix("").is_file()
        if script_uid:
            _check_uid(path)
        if not TARGETS[target]["native"] and (
                path.suffix in {".so", ".gdextension"} or path.name.endswith(".gdextension.uid")):
            # Code-only target: the bridge rejects this platform before loading.
            continue
        if path.name != "project.godot" and path.suffix not in SOURCE_SUFFIXES and not script_uid:
            raise ValueError(f"non-source project file requires explicit review: {relative}")
        if path.suffix == ".tscn":
            _check_scene_ids(path, relative)
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
        staged = destination / relative
        staged.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, staged)
        source_hashes[str(relative)] = digest(staged)
        if source_hashes[str(relative)] != native.get(str(relative), digest(source)):
            raise ValueError("project source changed during staging")
    (destination / "project.godot").write_text(config)
    preset = repository / "tools/port/desktop_export.cfg"
    shutil.copyfile(preset, destination / "export_presets.cfg")
    return {
        "schema": "tokimemo.desktop-stage.v2",
        "scene": scene,
        "target": target,
        "export_preset": TARGETS[target]["preset"],
        "native_files": dict(sorted(native.items())),
        "staged_s3tc_bptc_import": True,
        "source_files": source_hashes,
        "staged_project_sha256": digest(destination / "project.godot"),
        "preset_sha256": digest(preset),
        "engine_lock_sha256": digest(repository / "tools/port/godot.lock.json"),
        "source_only_allowlist_applied": True,
        "original_game_data_included": False,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--repository", type=Path, required=True)
    parser.add_argument("--destination", type=Path, required=True)
    parser.add_argument("--scene", choices=SCENES, default="week")
    parser.add_argument("--target", choices=TARGETS, default="macos")
    args = parser.parse_args()
    receipt = prepare(args.repository, args.destination, args.scene, args.target)
    (Path(os.path.abspath(args.destination)).parent / "source-receipt.json").write_text(
        json.dumps(receipt, indent=2) + "\n"
    )
    print(f"Staged {len(receipt['source_files'])} files for {args.target} {args.scene}")


if __name__ == "__main__":
    main()
