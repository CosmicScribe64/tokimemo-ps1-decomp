"""Build source-bound opening bank files from the verified MODE2/2352 track.

This writes the four VAB header/body reads required by AudioService for the
two finite bank-0 roles, plus the source-bound sequence payloads. It does not
claim sequence playback or presentation of the original prologue.
"""
from __future__ import annotations

import hashlib
import json
import os
import shutil
import sys
import tempfile
from pathlib import Path


class OpeningPackError(ValueError):
    """The source input, bindings, or output destination is invalid."""


def _repository_root() -> Path:
    configured = os.environ.get("TOKIMEMO_ROOT")
    if configured:
        root = Path(configured).resolve()
        if not (root / "tools/assets/input.py").is_file():
            raise OpeningPackError("TOKIMEMO_ROOT does not identify a repository")
        return root
    for parent in Path(__file__).resolve().parents:
        if (parent / "tools/assets/input.py").is_file():
            return parent
    raise OpeningPackError("repository root unavailable; set TOKIMEMO_ROOT")


ROOT = _repository_root()
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools.assets.input import open_input
from tools.port import prologue_bank_content
from tools.port.school_day_tables import _assert_no_symlink_path, _check_export_path


SCHEMA = "tokimemo.opening-bank-pack.v1"
ROLE_FILES = {
    "se_header": "se_header",
    "se_body": "se_body",
    "music_header": "music_header",
    "music_body": "music_body",
    "sequence": "sequence",
}
REQUIRED_ROLES = ("prologue_arrival", "normal_gameplay")
REQUIRED_FIELDS = tuple(ROLE_FILES)


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _source_entry(entries: list[dict], expected: dict) -> dict:
    lba = expected.get("lba")
    path = expected.get("path")
    matches = [
        entry for entry in entries
        if entry.get("kind") == "file"
        and entry.get("location", {}).get("lba") == lba
        and entry.get("path") == path
    ]
    if len(matches) != 1:
        raise OpeningPackError("source asset is not the unique verified ISO entry")
    return matches[0]


def build_pack(input_path: str | os.PathLike[str]) -> tuple[dict, dict[str, bytes]]:
    """Re-derive source bindings, then read and verify every bound original file."""
    try:
        banks_payload, bindings_manifest = prologue_bank_content.build_bundle(str(input_path))
    except Exception as exc:
        raise OpeningPackError("verified opening bank bindings failed") from exc

    request_proof = bindings_manifest.get("requests")
    if not isinstance(request_proof, dict) or set(request_proof) != set(REQUIRED_ROLES):
        raise OpeningPackError("opening role set differs from the source profile")
    banks = json.loads(banks_payload.decode("utf-8"))
    if not isinstance(banks, dict) or set(banks) != set(REQUIRED_ROLES):
        raise OpeningPackError("verified bank payload has an unexpected role set")

    files: dict[str, bytes] = {}
    bank_roles: dict[str, dict] = {}
    with open_input(str(input_path)) as source:
        source_description = source.describe()
        expected_track = getattr(prologue_bank_content.original, "TRACK_SHA256", None)
        if expected_track is None:
            from tools.port.school_day_tables import TRACK_SHA256
            expected_track = TRACK_SHA256
        if source_description.get("source_sha256") != expected_track:
            raise OpeningPackError("original input track hash differs from the verified release")
        entries = source.entries()

        for role in REQUIRED_ROLES:
            request = request_proof[role]
            function = request.get("function")
            expected_function = prologue_bank_content.ROLES.get(role)
            if function != expected_function:
                raise OpeningPackError("opening role source function differs from the profile")

            # The request argument table is re-derived from current pinned C and
            # original EXE/overlay evidence by build_bundle above. Re-read each
            # exact LBA/path entry and compare its complete bytes to that proof.
            source_text = (ROOT / "src/ovl/name_entry/prologue.c").read_text(encoding="utf-8")
            binding = prologue_bank_content._arguments(source_text, function)
            for field in REQUIRED_FIELDS:
                lba = binding.get(field)
                if type(lba) is not int or lba <= 0:
                    raise OpeningPackError("required opening bank read is absent from source")
            expected_assets = banks.get(role, {}).get("assets")
            if not isinstance(expected_assets, dict) or set(expected_assets) != set(REQUIRED_FIELDS):
                raise OpeningPackError("source-bound opening role has an incomplete asset set")

            assets_for_role: dict[str, dict] = {}
            for field, filename_suffix in ROLE_FILES.items():
                proof = expected_assets[field]
                if (type(proof.get("lba")) is not int or proof["lba"] != binding[field]
                        or not isinstance(proof.get("path"), str)
                        or type(proof.get("size")) is not int
                        or not isinstance(proof.get("sha256"), str)):
                    raise OpeningPackError("source asset metadata disagrees with the original request")
                entry = _source_entry(entries, proof)
                data = source.read(entry)
                if len(data) != proof["size"] or _sha(data) != proof["sha256"]:
                    raise OpeningPackError("original bank asset bytes differ from the source proof")
                relative = f"reads/{role}-{filename_suffix}.bin"
                if relative in files:
                    raise OpeningPackError("duplicate opening asset path")
                files[relative] = data
                assets_for_role[field] = {
                    "path": relative,
                    "size": len(data),
                    "sha256": _sha(data),
                    "source_path": proof["path"],
                    "source_lba": proof["lba"],
                }
            bank_roles[role] = {
                "assets": assets_for_role,
                "source_request": function,
                "bank": 0,
                "playback_capability": False,
            }

        # prologue_line_11_run loads the alternate music bank while arrival
        # music still owns the first bank. Authenticate all three real files;
        # an arrival-memory copy is not a Rei bank.
        source_text = (ROOT / "src/ovl/name_entry/prologue.c").read_text(encoding="utf-8")
        if "audio_bank_load_start(1, 0, 0, 0xCCCB, 0xCC86, 0xCC82);" not in source_text:
            raise OpeningPackError("Rei bank source request changed")
        rei_assets = {}
        for field, lba, size, digest in (
            ("music_header", 0xcccb, 10272, "f5ae3b750f10fec1a1cd0698ab1b2540de93bde3f5b4bb62dfa8da23848e489c"),
            ("music_body", 0xcc86, 139680, "be8281abca7901556494ded265dd7f686cd5e58179db2f0d7dcb0ae95f90bb21"),
            ("sequence", 0xcc82, 6416, "18dd4e4a9050cdf2fea53c4ab799c301a64c0bc36e4a059d9d5f3717b0628574"),
        ):
            matches = [entry for entry in entries if entry.get("location", {}).get("lba") == lba]
            if len(matches) != 1:
                raise OpeningPackError("Rei original bank file is missing or ambiguous")
            data = source.read(matches[0])
            if len(data) != size or _sha(data) != digest:
                raise OpeningPackError("Rei original bank bytes differ")
            relative = f"reads/prologue_rei-{field}.bin"
            files[relative] = data
            rei_assets[field] = {"path": relative, "size": size, "sha256": digest,
                                 "source_path": matches[0]["path"], "source_lba": lba}
        bank_roles["prologue_rei"] = {"assets": rei_assets, "source_request": "prologue_line_11_run",
                                      "bank": 1, "playback_capability": False}

    opening = {
        "schema": SCHEMA,
        "bank_roles": bank_roles,
        "provenance": {
            "source_sha256": source_description["source_sha256"],
            "source_format": source_description.get("format"),
            "bindings_manifest_sha256": _sha(json.dumps(
                bindings_manifest, sort_keys=True, separators=(",", ":")
            ).encode("utf-8")),
            "source_pins": bindings_manifest["source_pins"],
            "requests": bindings_manifest["requests"],
        },
        "capabilities": {
            "vab_bank_bytes": True,
            "sequence_payload_bytes": True,
            "sequence_playback": False,
            "prologue_presentation": False,
            "runtime_execution_proven": False,
        },
    }
    return opening, files


def _write_pack(destination: Path, opening: dict, files: dict[str, bytes]) -> None:
    destination.mkdir(parents=True)
    (destination / "reads").mkdir()
    for relative, data in sorted(files.items()):
        path = destination / relative
        if path.parent != destination / "reads" or path.name in ("", ".", ".."):
            raise OpeningPackError("unsafe generated asset path")
        path.write_bytes(data)
    (destination / "opening.json").write_text(
        json.dumps(opening, sort_keys=True, indent=2) + "\n", encoding="utf-8"
    )


def export(input_path: str | os.PathLike[str], output_path: str | os.PathLike[str]) -> Path:
    """Write a complete immutable pack under ignored `.asset-cache/port/`."""
    try:
        destination, _cache = _check_export_path(output_path)
    except Exception as exc:
        raise OpeningPackError("output must be a new safe pack directory under .asset-cache/port") from exc
    opening, files = build_pack(input_path)
    destination.parent.mkdir(parents=True, exist_ok=True)
    _assert_no_symlink_path(destination.parent)
    stage = Path(tempfile.mkdtemp(prefix=".opening-runtime-", dir=destination.parent))
    try:
        stage.rmdir()
        _write_pack(stage, opening, files)
        _assert_no_symlink_path(destination.parent)
        # Recheck immediately before the atomic publication; never overwrite.
        _check_export_path(destination)
        os.rename(stage, destination)
    except Exception:
        shutil.rmtree(stage, ignore_errors=True)
        raise
    return destination


def main() -> int:
    import argparse

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--input", required=True, help="verified original MODE2/2352 track image")
    parser.add_argument("--output", required=True, help="new ignored directory under .asset-cache/port")
    args = parser.parse_args()
    result = export(args.input, args.output)
    print(result / "opening.json")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
