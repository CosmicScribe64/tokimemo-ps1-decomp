#!/usr/bin/env python3
"""Export source-bound first-arrival media and native sector transfers.

The source-proved MDEC preview and O_01 static records are delegated to the
frozen ``prologue_arrival_media.build_pack``. This adds exact CD transfer
payloads while keeping adjacent-file overread separate from decoder input.
It does not grant visible-actor, hardware-pixel, or full-scene capability.
"""
from __future__ import annotations

import argparse
import hashlib
import importlib.util
import json
import math
import os
from pathlib import Path, PurePosixPath
import shutil
import sys
import tempfile


class FirstArrivalError(ValueError):
    """Original input, source proof, or destination is outside this profile."""


PROJECT_ROOT = Path(__file__).resolve().parents[2]
SCHEMA = "tokimemo.first-arrival-media.v1"
TRACK_SHA256 = "28c3c501b0c67b87ca1e30515da73619497d509ff6adc28ff722aa02897dfb72"
BASE_BUILDER_RELATIVE = "tools/port/prologue_arrival_media.py"
BASE_BUILDER_SHA256 = "538d247fa75f31624ae956518e13ff74cebcd2ad81ca6a14523a88688e7b6f1a"
O01_PARSER_RELATIVE = "tools/assets/prologue_o01.py"
O01_PARSER_SHA256 = "464f84b172764b81629eac2514c73ce9cf0c03e3fb312be472c2375877c80dd4"
BACKGROUND_PATH = "CDROM/BS/P_01_01.BS"
BACKGROUND_LBA = 0x494B
BACKGROUND_SIZE = 18432
BACKGROUND_SHA256 = "280b440f4f005d53340afd5389cd755a5bbb574fa30c36013df484d039fd7aab"
BACKGROUND_SECTORS = 10
BACKGROUND_TRANSFER_SIZE = BACKGROUND_SECTORS * 2048
BACKGROUND_DESTINATION = 0x80162000
ADJACENT_PATH = "CDROM/BS/P_01_02.BS"
ADJACENT_LBA = 0x4954
ADJACENT_SIZE = 18432
ADJACENT_SHA256 = "61b0cd543a73c64ff9ad6d1e77d48217c02dbfbe7dab4941e90451b76f20f398"
ACTOR_PATH = "CDROM/PROLODIR/O_01.NBN"
ACTOR_LBA = 0xABAB
ACTOR_SIZE = 26624
ACTOR_SHA256 = "bb224a47fa1ac9c102dd4701d3d3e0819ba30cac1e5637183886b8be4f95340c"
ACTOR_SECTORS = 13
ACTOR_DESTINATION = 0x80180000
PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"
MAX_OUTPUT_FILE = 4 * 1024 * 1024
MAX_OUTPUT_TOTAL = 16 * 1024 * 1024
TOOL_SOURCE_PINS = {
    "tools/assets/input.py": "e876bd062eb367f636d9703935305993d6e6cc2f9a9800c84571015b351dcd52",
    "tools/assets/image_mdec.py": "6513b1e6434bdd1c65c804e5d637ae3db1f4fa11f69dd80b495cf05a2f8828ec",
    "tools/assets/image_mdec_model.py": "3b8e5611bf6f763171b8bf3d5a22269211048b0ec21dc342088e90ec043d2222",
    "tools/assets/images.py": "0198a5cf61d9683757b81b2fa4ca190d70f173ef59a47d3328b4d60ae8111033",
    "tools/assets/image_packs_prologue.py": "032beeac8daf78def2024a2e1ee8de1bf9ddd8ec2a380219eed58505581fed20",
    "tools/assets/media_background_context.py": "40441716507a1a407260caec428ad3aba706ee534cd05de67beded68a2d9d95e",
}

_BASE_MODULE = None


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _assert_no_symlink_path(path: Path) -> Path:
    absolute = Path(path).absolute()
    current = Path(absolute.anchor)
    for part in absolute.parts[1:]:
        if part in ("", ".", ".."):
            raise FirstArrivalError("path has a noncanonical component")
        current = current / part
        if current.is_symlink():
            raise FirstArrivalError("path contains a symlink")
    return absolute


def _source_root(explicit: str | os.PathLike[str] | None = None) -> Path:
    configured = explicit or os.environ.get("TOKIMEMO_SOURCE_ROOT")
    if configured:
        root = _assert_no_symlink_path(Path(configured))
        if not (root / "src/main/background_images.c").is_file():
            raise FirstArrivalError("source root does not contain the pinned game C files")
        return root
    for parent in Path(__file__).resolve().parents:
        if (parent / "src/main/background_images.c").is_file():
            return _assert_no_symlink_path(parent)
    raise FirstArrivalError("source repository unavailable; set TOKIMEMO_SOURCE_ROOT")


def _verify_tool_source(root: Path) -> dict:
    checked = {}
    for relative, expected in TOOL_SOURCE_PINS.items():
        path = root / relative
        _assert_no_symlink_path(path)
        if not path.is_file() or _sha(path.read_bytes()) != expected:
            raise FirstArrivalError("pinned original-input or media reader changed: " + relative)
        checked[relative] = expected
    for relative, expected in ((BASE_BUILDER_RELATIVE, BASE_BUILDER_SHA256),
                               (O01_PARSER_RELATIVE, O01_PARSER_SHA256)):
        path = PROJECT_ROOT / relative
        _assert_no_symlink_path(path)
        if not path.is_file() or _sha(path.read_bytes()) != expected:
            raise FirstArrivalError("pinned candidate media component changed: " + relative)
        checked[relative] = expected
    return checked


def _load_base_builder():
    global _BASE_MODULE
    if _BASE_MODULE is not None:
        return _BASE_MODULE
    path = PROJECT_ROOT / BASE_BUILDER_RELATIVE
    _assert_no_symlink_path(path)
    if not path.is_file() or _sha(path.read_bytes()) != BASE_BUILDER_SHA256:
        raise FirstArrivalError("frozen prologue-arrival media builder changed")
    canonical = _source_root()
    if str(canonical) not in sys.path:
        sys.path.insert(0, str(canonical))
    # The immutable base builder imports this candidate-local NBN parser,
    # which is intentionally not part of the canonical repository's tools.
    parser_path = PROJECT_ROOT / O01_PARSER_RELATIVE
    _assert_no_symlink_path(parser_path)
    if not parser_path.is_file() or _sha(parser_path.read_bytes()) != O01_PARSER_SHA256:
        raise FirstArrivalError("frozen O_01 parser changed")
    import tools.assets as assets_package
    parser_spec = importlib.util.spec_from_file_location("tools.assets.prologue_o01", parser_path)
    if parser_spec is None or parser_spec.loader is None:
        raise FirstArrivalError("cannot load frozen O_01 parser")
    parser_module = importlib.util.module_from_spec(parser_spec)
    sys.modules[parser_spec.name] = parser_module
    parser_spec.loader.exec_module(parser_module)
    setattr(assets_package, "prologue_o01", parser_module)
    spec = importlib.util.spec_from_file_location("_frozen_prologue_arrival_media", path)
    if spec is None or spec.loader is None:
        raise FirstArrivalError("cannot load frozen prologue-arrival media builder")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    # build_pack uses this only for its temporary MDEC work directory. Keep
    # that transient output inside this candidate's ignored cache.
    module.PROJECT_ROOT = PROJECT_ROOT
    _BASE_MODULE = module
    return module


def _file_entry(entries: list[dict], path: str, lba: int, size: int, sha256: str) -> dict:
    found = [entry for entry in entries
             if entry.get("kind") == "file"
             and entry.get("path") == path
             and entry.get("location", {}).get("lba") == lba]
    if len(found) != 1:
        raise FirstArrivalError("original ISO path/LBA is missing or ambiguous")
    entry = found[0]
    if entry.get("size") != size or entry.get("sha256") != sha256:
        raise FirstArrivalError("original ISO entry size or hash differs from the pinned profile")
    return entry


def _native_extent(source, entry: dict, track_sha256: str, count: int,
                   expected_lba: int) -> tuple[bytes, dict]:
    if not hasattr(source, "iter_raw_sectors"):
        raise FirstArrivalError("original raw-sector reader is unavailable")
    if type(count) is not int or count <= 0 or type(expected_lba) is not int or expected_lba < 0:
        raise FirstArrivalError("native sector extent has invalid numeric bounds")
    synthetic = {
        "path": entry["path"],
        "kind": "file",
        "size": count * 2048,
        "location": {"lba": expected_lba, "extent_bytes": count * 2048},
    }
    payloads: list[bytes] = []
    sectors: list[dict] = []
    raw_hash = hashlib.sha256()
    payload_hash = hashlib.sha256()
    try:
        for index, row in enumerate(source.iter_raw_sectors(synthetic, track_sha256)):
            if (row.get("lba") != expected_lba + index
                    or row.get("form") != 1
                    or row.get("form_name") != "Form1"
                    or row.get("subheader_valid") is not True
                    or row.get("payload_size") != 2048
                    or row.get("raw_size") != 2352
                    or not isinstance(row.get("payload"), bytes)
                    or not isinstance(row.get("raw"), bytes)
                    or len(row["payload"]) != 2048
                    or len(row["raw"]) != 2352
                    or row.get("payload_sha256") != _sha(row["payload"])
                    or row.get("raw_sha256") != _sha(row["raw"])):
                raise FirstArrivalError("native transfer contains an unverified or unexpected sector")
            payloads.append(row["payload"])
            raw_hash.update(row["raw"])
            payload_hash.update(row["payload"])
            sectors.append({
                "lba": row["lba"],
                "form": row["form_name"],
                "subheader_valid": True,
                "raw_sha256": row["raw_sha256"],
                "payload_sha256": row["payload_sha256"],
            })
    except FirstArrivalError:
        raise
    except Exception as exc:
        raise FirstArrivalError("original native-sector transfer could not be read") from exc
    if len(payloads) != count:
        raise FirstArrivalError("native transfer sector count differs from source")
    payload = b"".join(payloads)
    proof = {
        "first_lba": expected_lba,
        "sector_count": count,
        "payload_bytes": len(payload),
        "raw_sector_bytes": count * 2352,
        "payload_sha256": payload_hash.hexdigest(),
        "raw_extent_sha256": raw_hash.hexdigest(),
        "sectors": sectors,
    }
    return payload, proof


def _candidate_pack(input_path: str | os.PathLike[str], source_root: Path) -> tuple[dict, dict[str, bytes]]:
    tool_source_pins = _verify_tool_source(source_root)
    base = _load_base_builder()
    try:
        base_manifest, base_files = base.build_pack(input_path, source_root=source_root)
    except Exception as exc:
        if isinstance(exc, FirstArrivalError):
            raise
        raise FirstArrivalError("frozen source-bound arrival media build failed") from exc
    try:
        with base.open_input(str(input_path)) as source:
            desc = source.describe()
            if desc.get("format") != "mode2-2352" or desc.get("source_sha256") != TRACK_SHA256:
                raise FirstArrivalError("input is not the exact verified original MODE2 track")
            entries = source.entries()
            background_entry = _file_entry(entries, BACKGROUND_PATH, BACKGROUND_LBA,
                                           BACKGROUND_SIZE, BACKGROUND_SHA256)
            adjacent_entry = _file_entry(entries, ADJACENT_PATH, ADJACENT_LBA,
                                         ADJACENT_SIZE, ADJACENT_SHA256)
            actor_entry = _file_entry(entries, ACTOR_PATH, ACTOR_LBA,
                                      ACTOR_SIZE, ACTOR_SHA256)
            background = source.read(background_entry)
            adjacent = source.read(adjacent_entry)
            actor = source.read(actor_entry)
            if (_sha(background) != BACKGROUND_SHA256 or len(background) != BACKGROUND_SIZE
                    or _sha(actor) != ACTOR_SHA256 or len(actor) != ACTOR_SIZE):
                raise FirstArrivalError("original file bytes differ from pinned ISO entries")
            background_transfer, background_sector_proof = _native_extent(
                source, background_entry, TRACK_SHA256, BACKGROUND_SECTORS, BACKGROUND_LBA)
            actor_transfer, actor_sector_proof = _native_extent(
                source, actor_entry, TRACK_SHA256, ACTOR_SECTORS, ACTOR_LBA)
            if background_transfer[:BACKGROUND_SIZE] != background:
                raise FirstArrivalError("background transfer prefix differs from the exact ISO file")
            adjacent_first = adjacent[:2048]
            if (len(adjacent_first) != 2048
                    or background_transfer[BACKGROUND_SIZE:] != adjacent_first):
                raise FirstArrivalError("background overread is not the next file's first sector")
            if actor_transfer != actor:
                raise FirstArrivalError("native O_01 transfer differs from its exact ISO file")
    except FirstArrivalError:
        raise
    except Exception as exc:
        raise FirstArrivalError("verified original native media reads failed") from exc

    files = dict(base_files)
    files["background/P_01_01.BS"] = background
    files["native/arrival-bg-transfer.bin"] = background_transfer
    files["actor/O_01.NBN"] = actor_transfer
    for relative, payload in files.items():
        _validate_generated_file(relative, payload)
    if sum(len(payload) for payload in files.values()) > MAX_OUTPUT_TOTAL:
        raise FirstArrivalError("generated first-arrival pack exceeds its bounded output size")

    manifest = dict(base_manifest)
    manifest["schema"] = SCHEMA
    manifest["native_transfers"] = {
        "background": {
            "file": "native/arrival-bg-transfer.bin",
            "size": len(background_transfer),
            "sha256": _sha(background_transfer),
            "lba": BACKGROUND_LBA,
            "sectors": BACKGROUND_SECTORS,
            "destination": BACKGROUND_DESTINATION,
            "decoder_file": "background/P_01_01.BS",
            "decoder_size": len(background),
            "decoder_sha256": _sha(background),
            "sector_proof": background_sector_proof,
            "adjacent_extent": {
                "lba": ADJACENT_LBA,
                "path": ADJACENT_PATH,
                "size": 2048,
                "sha256": _sha(adjacent_first),
                "iso_file_size": ADJACENT_SIZE,
                "iso_file_sha256": ADJACENT_SHA256,
                "role": "first sector of next ISO file; preserved overread, not padding",
            },
            "overread_used_for_decoder": False,
        },
        "actor": {
            "file": "actor/O_01.NBN",
            "size": len(actor_transfer),
            "sha256": _sha(actor_transfer),
            "lba": ACTOR_LBA,
            "sectors": ACTOR_SECTORS,
            "destination": ACTOR_DESTINATION,
            "sector_proof": actor_sector_proof,
        },
    }
    manifest["runtime_profile"] = {
        "id": "stage0_hidden_actor.v1",
        "stage": 0,
        "background_slot": 1,
        "background_brightness": 0x80,
        "actor96_initial_draw_flags": 0x24,
        "actor96_visible_rendering": False,
        "actor96_stage0_support": "hidden actor load and tpage/palette setup only",
        "native_visible_composition": False,
        "hardware_idct_color_parity": False,
        "complete_arrival_scene": False,
    }
    manifest["capabilities"] = {
        "stage0_background_preview": True,
        "native_background_transfer": True,
        "actor96_source_bytes": True,
        "actor96_hidden_load_profile": True,
        "actor96_visible_rendering": False,
        "native_visible_composition": False,
        "hardware_pixel_parity": False,
        "complete_arrival_frame": False,
        "runtime_stage_progression": False,
    }
    manifest["limitations"] = list(base_manifest.get("limitations", []))
    manifest["limitations"].extend([
        "the ten-sector P_01_01.BS transfer includes the first sector of adjacent P_01_02.BS; that sector is preserved separately from the nine-sector decoder input",
        "actor 96 is supported only as an initially hidden stage-0 source load with tpage/palette setup; visible actor composition is not a runtime capability",
        "audio bank role 0/BGM, title/date/message HUD, and full-scene integration remain external prerequisites",
    ])
    manifest["files"] = {
        relative: {"size": len(payload), "sha256": _sha(payload),
                   "kind": "png" if relative.lower().endswith(".png") else "original_bytes"}
        for relative, payload in sorted(files.items())
    }
    manifest["producer"] = {
        "path": "tools/port/first_arrival_content.py",
        "source_sha256": _sha(Path(__file__).read_bytes()),
        "base_builder_path": BASE_BUILDER_RELATIVE,
        "base_builder_sha256": BASE_BUILDER_SHA256,
        "o01_parser_path": O01_PARSER_RELATIVE,
        "o01_parser_sha256": O01_PARSER_SHA256,
        "reader": "tools.assets.input.Input.iter_raw_sectors",
        "tool_source_pins": tool_source_pins,
    }
    return manifest, files


def _validate_generated_file(relative: str, payload: bytes) -> None:
    path = PurePosixPath(relative)
    if ("\\" in relative or "\x00" in relative or path.is_absolute() or not path.parts
            or any(part in ("", ".", "..") for part in path.parts)):
        raise FirstArrivalError("generated file path is not a safe relative path")
    if not isinstance(payload, bytes) or not payload or len(payload) > MAX_OUTPUT_FILE:
        raise FirstArrivalError("generated output is empty or not bytes")
    if path.suffix.lower() == ".png" and not payload.startswith(PNG_SIGNATURE):
        raise FirstArrivalError("generated PNG has no original PNG signature")


def build_pack(input_path: str | os.PathLike[str], *, source_root: str | os.PathLike[str] | None = None) -> tuple[dict, dict[str, bytes]]:
    source = Path(input_path)
    _assert_no_symlink_path(source)
    if source.is_symlink() or not source.is_file():
        raise FirstArrivalError("input must be a regular, non-symlink original track file")
    root = _source_root(source_root)
    _assert_no_symlink_path(root)
    return _candidate_pack(source, root)


def _publish(output_path: str | os.PathLike[str], manifest: dict, files: dict[str, bytes]) -> Path:
    cache = PROJECT_ROOT / ".asset-cache" / "port"
    _assert_no_symlink_path(PROJECT_ROOT)
    _assert_no_symlink_path(cache)
    destination = _assert_no_symlink_path(Path(output_path))
    try:
        relative = destination.relative_to(cache.absolute())
    except ValueError as exc:
        raise FirstArrivalError("output must stay beneath the candidate .asset-cache/port directory") from exc
    if not relative.parts or any(part in ("", ".", "..") for part in relative.parts):
        raise FirstArrivalError("output must name a safe new pack directory")
    if destination.exists() or destination.is_symlink():
        raise FirstArrivalError("refusing to overwrite an existing runtime pack")
    cache.mkdir(parents=True, exist_ok=True)
    _assert_no_symlink_path(cache)
    stage = Path(tempfile.mkdtemp(prefix=".first-arrival-runtime-", dir=cache))
    try:
        for relative_name, payload in sorted(files.items()):
            _validate_generated_file(relative_name, payload)
            target = stage.joinpath(*PurePosixPath(relative_name).parts)
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(payload)
        data = json.dumps(manifest, sort_keys=True, indent=2, ensure_ascii=False).encode("utf-8") + b"\n"
        (stage / "manifest.json").write_bytes(data)
        _assert_no_symlink_path(cache)
        if destination.exists() or destination.is_symlink():
            raise FirstArrivalError("runtime pack destination appeared during publication")
        os.rename(stage, destination)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return destination / "manifest.json"


def export(input_path: str | os.PathLike[str], output_path: str | os.PathLike[str] | None = None,
           *, source_root: str | os.PathLike[str] | None = None) -> Path:
    destination = output_path or PROJECT_ROOT / ".asset-cache" / "port" / "first-arrival-runtime-v1"
    manifest, files = build_pack(input_path, source_root=source_root)
    return _publish(destination, manifest, files)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="verified original MODE2/2352 track image")
    parser.add_argument("--output", help="new pack under candidate .asset-cache/port (default: first-arrival-runtime-v1)")
    parser.add_argument("--source-root", help="repository containing the pinned source/config/assembly files")
    args = parser.parse_args(argv)
    try:
        manifest_path = export(args.input, args.output, source_root=args.source_root)
    except (FirstArrivalError, OSError, ValueError) as exc:
        print("first-arrival content export failed: " + str(exc), file=sys.stderr)
        return 2
    print(manifest_path)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
