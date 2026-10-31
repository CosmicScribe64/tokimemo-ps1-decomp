"""Build the GEKO player-birthday content pack (native mode 0x56).

The pack holds two things read from the original disc:
  steps.json      the 29-entry step table encounter_player_birthday_steps
                  (GEKO.EXN, 0x80144E94), each word resolved to the function
                  name the project's symbol files give that address
  sections/<n>/   the 13 P_BIRTH.BIN talk sections that
                  encounter_player_birthday_scripts_bind installs
                  (src/ovl/girl_encounters/birthday_and_shared_scene_flow.c),
                  exported with tools/port/dialogue_content.py

Usage (inside the tokimemo-content image, repository at /repo):
  python -m tools.port.player_birthday_content --input /disc/files \
      /repo/.asset-cache/port/player-birthday-content-v1

The output must be a new directory under .asset-cache/port/. Nothing in it
may be committed. The pack records hashes of every input it read.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import struct
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SCHEMA = "tokimemo.geko-player-birthday-content.v1"
GEKO_PATH = "CDROM/EXEDIR/GEKO.EXN"
GEKO_SHA256 = "f7295203dc38417b8ff3c274833c06554dd072f5a6efcc74674cdaf38f06e5ad"
GEKO_LOAD = 0x80132000
STEP_TABLE = 0x80144E94
STEP_COUNT = 29
P_BIRTH_PATH = "CDROM/SERIFU/P_BIRTH.BIN"
P_BIRTH_PROFILE = "omimai-p-birth"
SECTION_COUNT = 13
SYMBOL_FILES = ("config/symbols_GEKO_hypotheses.txt", "config/symbol_addrs.txt",
                "config/symbol_addrs_obin.txt")

# Step indices that rules/girl_encounters/player_birthday.gd implements, with
# the function each must name. Any other table is refused.
RULE_STEPS = {
    0: "encounter_player_birthday_init",
    1: "encounter_player_birthday_girl_select",
    2: "encounter_player_birthday_audio_load",
    6: "encounter_player_birthday_closing_init",
    22: "encounter_player_birthday_finish",
    23: "encounter_player_birthday_scripts_load",
    25: "encounter_player_birthday_default_script_select",
    27: "encounter_player_birthday_finish",
}


class ContentError(ValueError):
    """An input or output that the pack refuses."""


def _sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def symbols(root: Path = ROOT) -> dict[int, str]:
    """Function names by address from the project's symbol files (first wins)."""
    out: dict[int, str] = {}
    for name in SYMBOL_FILES:
        text = (root / name).read_text(encoding="utf-8")
        for match in re.finditer(r"^(\w+)\s*=\s*0x([0-9A-Fa-f]+);", text, re.M):
            out.setdefault(int(match.group(2), 16), match.group(1))
    return out


def step_table(geko: bytes, names: dict[int, str], expected_sha256: str = GEKO_SHA256) -> list[dict]:
    """Decode and resolve the step table; refuse unknown or unexpected entries."""
    if _sha(geko) != expected_sha256:
        raise ContentError("GEKO.EXN hash differs from the pinned original")
    offset = STEP_TABLE - GEKO_LOAD
    if offset + 4 * STEP_COUNT > len(geko):
        raise ContentError("step table lies outside GEKO.EXN")
    rows = []
    for index in range(STEP_COUNT):
        address, = struct.unpack_from("<I", geko, offset + 4 * index)
        name = names.get(address)
        if name is None:
            raise ContentError("step %d address 0x%08X has no symbol" % (index, address))
        rows.append({"index": index, "address": "0x%08X" % address, "function": name})
    for index, name in RULE_STEPS.items():
        if rows[index]["function"] != name:
            raise ContentError("step %d is %s, expected %s" % (index, rows[index]["function"], name))
    return rows


def _read(source, path: str) -> bytes:
    entries = [item for item in source.entries() if item.get("path") == path]
    if len(entries) != 1:
        raise ContentError("original input does not contain one %s" % path)
    return source.read(entries[0])


def _new_output(output: Path) -> Path:
    raw = os.fspath(output)
    if any(part == ".." for part in raw.replace("\\", "/").split("/")):
        raise ContentError("output path cannot contain parent traversal")
    output = Path(os.path.abspath(output))
    allowed = (ROOT / ".asset-cache" / "port").resolve()
    if allowed not in output.resolve(strict=False).parents:
        raise ContentError("output must be a child of .asset-cache/port/")
    if output.exists() or output.is_symlink():
        raise ContentError("output already exists; packs are immutable")
    return output


def build(input_path: Path, output: Path) -> dict:
    """Write the pack atomically and return its index."""
    sys.path.insert(0, str(ROOT))
    from tools.assets.input import open_input
    from tools.assets.script_profiles import PROFILES
    from tools.assets.text import parse_script
    from tools.assets.script_validation import validate_script
    from tools.port.dialogue_content import write_pack

    output = _new_output(output)
    profiles = [item for item in PROFILES if item.get("id") == P_BIRTH_PROFILE]
    if len(profiles) != 1 or profiles[0].get("path") != P_BIRTH_PATH \
            or len(profiles[0].get("sections", [])) != SECTION_COUNT:
        raise ContentError("P_BIRTH profile is missing or has changed")
    profile = profiles[0]
    with open_input(str(input_path)) as source:
        geko = _read(source, GEKO_PATH)
        p_birth = _read(source, P_BIRTH_PATH)
    if _sha(p_birth) != profile["sha256"]:
        raise ContentError("P_BIRTH.BIN hash differs from the profiled original")
    steps = step_table(geko, symbols())
    parsed = parse_script(p_birth, P_BIRTH_PATH, profile)
    validation = validate_script(p_birth, P_BIRTH_PATH, profile, parsed)
    catalog = {"schema": "tokimemo.text.catalog.v1",
               "files": [{"path": P_BIRTH_PATH, "status": "exported",
                          "script": parsed, "validation": validation}]}
    work = Path(tempfile.mkdtemp(prefix=".player-birthday-", dir=output.parent))
    try:
        catalog_path = work / "catalog.json"
        catalog_path.write_text(json.dumps(catalog, ensure_ascii=False, sort_keys=True), encoding="utf-8")
        sections = []
        for index, section in enumerate(profile["sections"]):
            if section["id"] != "girl-%d" % index:
                raise ContentError("P_BIRTH section order differs from scripts_bind")
            component = output.parent / (".pb-%s-%s.json" % (work.name, section["id"]))
            try:
                write_pack(catalog_path, P_BIRTH_PATH, section["id"], input_path, component)
                target = work / "sections" / str(index)
                target.mkdir(parents=True)
                shutil.copy2(component, target / "content.json")
            finally:
                component.unlink(missing_ok=True)
            sections.append({"index": index, "profile_section": section["id"],
                             "group_count": section["group_count"],
                             "path": "sections/%d/content.json" % index,
                             "component_sha256": _sha((target / "content.json").read_bytes())})
        catalog_path.unlink()
        (work / "steps.json").write_text(json.dumps(
            {"table": "encounter_player_birthday_steps", "address": "0x%08X" % STEP_TABLE,
             "steps": steps}, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        index = {"schema": SCHEMA,
                 "sources": {GEKO_PATH: _sha(geko), P_BIRTH_PATH: _sha(p_birth)},
                 "steps_path": "steps.json",
                 "steps_sha256": _sha((work / "steps.json").read_bytes()),
                 "section_count": len(sections), "sections": sections,
                 "girl_sections": {"0": 1, "1": 6, "2": 3, "3": 4, "4": 10, "5": 7,
                                   "6": 8, "7": 5, "8": 0, "9": 9, "10": 12},
                 "default_section": 11,
                 "scene_services_ported": False,
                 "semantic_coverage_complete": False}
        (work / "index.json").write_text(json.dumps(index, indent=2, sort_keys=True) + "\n", encoding="utf-8")
        if output.exists():
            raise ContentError("output appeared during generation")
        os.replace(work, output)
    finally:
        if work.exists():
            shutil.rmtree(work, ignore_errors=True)
    return index


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description="Build the GEKO player-birthday content pack.")
    parser.add_argument("--input", type=Path, required=True,
                        help="read-only original directory, MODE2 track, or supported archive")
    parser.add_argument("output", type=Path)
    args = parser.parse_args(argv)
    try:
        index = build(args.input, args.output)
    except ContentError as error:
        print("player birthday content: %s" % error, file=sys.stderr)
        return 1
    print("wrote %s: %d steps, %d sections" % (args.output, STEP_COUNT, index["section_count"]))
    return 0


if __name__ == "__main__":
    sys.exit(main())
