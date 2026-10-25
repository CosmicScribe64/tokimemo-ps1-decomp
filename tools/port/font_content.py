"""Package only source-proved font extents through the canonical asset reader.

The font lookup/metadata implementation remains tools/assets/text_font.py.
This helper separates its verified metadata from the raw glyph-bank bytes so a
dedicated port font pack can keep those bytes outside res://. Only the explicit
`--export-original` entry point reads the verified executable, and it calls the
existing source-proof readers before writing. This module does not decode glyphs.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import sys


def _repository_root() -> Path:
    for parent in Path(__file__).resolve().parents:
        if (parent / "tools" / "assets" / "text_font.py").is_file():
            return parent
    raise RuntimeError("cannot locate canonical tools/assets/text_font.py")


ROOT = _repository_root()
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
from tools.assets.text_font import (  # noqa: E402
    FONT_PROFILE,
    font_metadata,
    verify_font_source_proof,
)

FONT_PALETTE_EXTENTS = (
    ("text_palette_0", 0x800B3FAC),
    ("text_palette_1", 0x800B3FCC),
    ("text_shadow_palette_0", 0x800B3FEC),
    ("text_shadow_palette_1", 0x800B400C),
)
FONT_SOURCE_PROOF_ASSEMBLY = "asm/data/main/text_system.data.s"
FONT_PAYLOAD_RELATIVE = "glyph_bank.bin"
FONT_DESCRIPTOR_NAME = "font.json"
FONT_PACKAGE_RELATIVE = "fonts/native-font-v2"


def font_content(source_bytes: bytes, profile: dict | None = None) -> dict:
    """Return canonical metadata and the exact opaque bank extent separately."""
    selected = FONT_PROFILE if profile is None else profile
    metadata = font_metadata(source_bytes, selected)
    start = int(metadata["glyph_offset"])
    end = start + int(metadata["glyph_count"]) * int(metadata["glyph_stride"])
    bank = source_bytes[start:end]
    if len(bank) != end - start:
        raise ValueError("canonical font reader returned an incomplete glyph bank")
    return {
        "source_format": "tokimemo.native-font-source.v1",
        "metadata": metadata,
        "glyph_bank": bank,
        "payload_sha256": metadata["glyph_bank_sha256"],
    }


def _font_palette_tables(source_bytes: bytes, repository: Path) -> list[dict]:
    """Read the four source-proved two-color palette groups from existing MIPS spans."""
    from tools.assets.text_binary import _spans

    if hashlib.sha256(source_bytes).hexdigest() != FONT_PROFILE["sha256"]:
        raise ValueError("font palettes require the verified original executable")
    source_assembly = repository / FONT_SOURCE_PROOF_ASSEMBLY
    binary = (source_bytes, FONT_PROFILE["load_address"], FONT_PROFILE["header_size"],
              FONT_PROFILE["sha256"], FONT_PROFILE["path"])
    by_key = {}
    for span in _spans(source_assembly, repository, binary):
        by_key.setdefault((span["label"], span["address"]), []).append(bytes(span["raw"]))
    tables = []
    for label, address in FONT_PALETTE_EXTENTS:
        parts = by_key.get((label, address), [])
        raw = b"".join(parts)
        if len(parts) != 1 or len(raw) != 0x20:
            raise ValueError(f"source palette extent is missing or incomplete: {label}")
        words = [int.from_bytes(raw[offset:offset + 2], "little")
                 for offset in range(0, len(raw), 2)]
        if len(words) != 16:
            raise ValueError(f"source palette word count is invalid: {label}")
        tables.append({
            "label": label,
            "address": address,
            "length": len(raw),
            "raw_hex": raw.hex(),
            "words": words,
            "sha256": hashlib.sha256(raw).hexdigest(),
            "rows": 8,
            "colors_per_row": 2,
        })
    return tables


def _source_provenance(source_bytes: bytes, repository: Path, files_root: Path) -> dict:
    proof = verify_font_source_proof(source_bytes, repository, files_root)
    required_labels = {"text_font_range_table", "text_font_glyphs"}
    if {item.get("label") for item in proof.get("data_labels", [])} != required_labels:
        raise ValueError("canonical font reader did not verify both range and glyph extents")
    return {
        "source_path": FONT_PROFILE["path"],
        "source_sha256": hashlib.sha256(source_bytes).hexdigest(),
        "reader": "tools.assets.text_font.font_metadata",
        "proof_reader": "tools.assets.text_font.verify_font_source_proof",
        "assembly_palette_reader": "tools.assets.text_binary._spans",
        "verified_data_labels": proof["data_labels"],
        "palette_assembly_path": FONT_SOURCE_PROOF_ASSEMBLY,
    }


def write_external_font_content(source_bytes: bytes, cache_root: str | Path,
                                profile: dict | None = None, repository: str | Path | None = None,
                                files_root: str | Path | None = None) -> dict:
    """Write an immutable external payload beneath a caller-selected asset cache."""
    repo = Path(repository or ROOT).resolve(strict=True)
    requested = Path(cache_root)
    if ".." in requested.parts:
        raise ValueError("font cache path cannot contain parent traversal")
    cache = requested.absolute()
    if cache != repo / ".asset-cache" / "port":
        raise ValueError("font output root must be the repository .asset-cache/port")
    current = Path(cache.anchor)
    for part in cache.parts[1:]:
        current = current / part
        if current.is_symlink():
            raise ValueError("font cache path cannot contain symlinks")
    package = font_content(source_bytes, profile)
    if profile is None:
        repo = Path(repository or ROOT).resolve(strict=True)
        inputs = Path(files_root or repo / "disc" / "files").resolve(strict=True)
        package["source_proof"] = _source_provenance(source_bytes, repo, inputs)
        package["palettes"] = _font_palette_tables(source_bytes, repo)
    directory = cache / FONT_PACKAGE_RELATIVE
    parent = directory.parent
    if parent.exists() and parent.is_symlink():
        raise ValueError("font package parent cannot be a symlink")
    parent.mkdir(parents=True, exist_ok=True)
    if parent.is_symlink():
        raise ValueError("font package parent cannot be a symlink")
    if directory.exists() and directory.is_symlink():
        raise ValueError("font package directory cannot be a symlink")
    if directory.exists():
        raise FileExistsError("font package is immutable and already exists")
    directory.mkdir()
    if directory.is_symlink():
        raise ValueError("font package directory cannot be a symlink")
    output = directory / FONT_PAYLOAD_RELATIVE
    output.write_bytes(package["glyph_bank"])
    if output.is_symlink() or output.stat().st_size != len(package["glyph_bank"]):
        raise ValueError("external font payload did not persist exactly")
    descriptor = {key: value for key, value in package.items()
                  if key not in ("glyph_bank", "source_format")} | {
        "schema": "tokimemo.native-font.v1",
        "schema_version": 1,
        "payload": FONT_PAYLOAD_RELATIVE,
        "payload_size_bytes": len(package["glyph_bank"]),
    }
    descriptor_path = directory / FONT_DESCRIPTOR_NAME
    descriptor_bytes = (json.dumps(descriptor, sort_keys=True, separators=(",", ":")) + "\n").encode("utf-8")
    descriptor_path.write_bytes(descriptor_bytes)
    if descriptor_path.is_symlink():
        raise ValueError("font descriptor cannot be a symlink")
    return {
        "descriptor_path": descriptor_path.relative_to(cache).as_posix(),
        "payload_path": output.relative_to(cache).as_posix(),
        "payload_size_bytes": len(package["glyph_bank"]),
        "payload_sha256": package["payload_sha256"],
        "source_sha256": package["metadata"]["source_sha256"],
        "descriptor_sha256": hashlib.sha256(descriptor_bytes).hexdigest(),
        "source_proof": package.get("source_proof", {}),
        "palette_count": len(package.get("palettes", [])),
    }


def export_original_font(repository: str | Path = ROOT, cache_root: str | Path | None = None) -> dict:
    """Read the original executable read-only and publish only to .asset-cache/port."""
    repo = Path(repository).resolve(strict=True)
    inputs = repo / "disc" / "files"
    source = inputs / FONT_PROFILE["path"]
    if source.is_symlink() or not source.is_file():
        raise ValueError("verified original executable is missing or symlinked")
    data = source.read_bytes()
    cache = Path(cache_root or repo / ".asset-cache" / "port")
    return write_external_font_content(data, cache, repository=repo, files_root=inputs)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--export-original", action="store_true",
                        help="verify and export the original font into ignored .asset-cache/port")
    parser.add_argument("--repository", type=Path, default=ROOT)
    parser.add_argument("--cache-root", type=Path)
    arguments = parser.parse_args()
    if not arguments.export_original:
        parser.error("select --export-original")
    print(json.dumps(export_original_font(arguments.repository, arguments.cache_root), sort_keys=True))
