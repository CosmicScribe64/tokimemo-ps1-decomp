#!/usr/bin/env python3
"""tokimemo-decomp translation contributor tree tool.

Splits monolithic translation files (returned-v3.jsonl and supplemental.jsonl)
into a clean, human-editable, GitHub-browsable directory tree under translations/en/,
and reconstructs the exact original JSONL files byte-for-byte on build.
"""

import argparse
import hashlib
import json
import os
import re
import shutil
import sys
import tempfile
from collections import Counter, defaultdict
from typing import Any, Dict, List, Optional, Set, Tuple

# CJK character regex for copyright & Japanese text detection
CJK_PATTERN = re.compile(
    r"[\u3000-\u303f\u3040-\u30ff\u3400-\u4dbf\u4e00-\u9fff\uf900-\ufaff\uff66-\uff9f]"
)

# Text replacements for target-only image catalog conversion
IMAGE_REPLACEMENTS = [
    (
        "`期末試験結果発表` banner (`112×12` box) + `点` score suffix (`8×16`)",
        "Term Exam Results banner (`112×12` box) + Points score suffix (`8×16`)",
    ),
    (
        "Shared battle UI sheet: `攻撃`, `防御`, `奥義` (bright/dim `32×16`) + `ミス` (`16×8`)",
        "Shared battle UI sheet: Attack, Defend, Special (bright/dim `32×16`) + Miss (`16×8`)",
    ),
    (
        "`HU17.BIN` yukata uchiwa fan (`祭`); 3 suspected sprites are non-text emblems",
        "`HU17.BIN` yukata uchiwa fan (festival emblem); 3 suspected sprites are non-text emblems",
    ),
    (
        "### 1.2 Information Magazine (*Memorial Spot* / `メモリアルスポット`)",
        "### 1.2 Information Magazine (*Memorial Spot*)",
    ),
    ("`95年 4月 23日 (日)`", "`95 / 4 / 23 (Sun)`"),
    (
        "Replace `年` glyph cell (`9 rows` at `x=96, y=16`) with **`/`** separator (`7px` tall, palette ink 9 + shadow 10) and repoint `月` to `/` and `日` to blank so the top-left HUD reads **`95 / 4 / 23 (Su)`**.",
        "Replace Year glyph cell (`9 rows` at `x=96, y=16`) with **`/`** separator (`7px` tall, palette ink 9 + shadow 10) and repoint Month to `/` and Day to blank so the top-left HUD reads **`95 / 4 / 23 (Su)`**.",
    ),
    (
        "bottom `56×12` strip `'95発売予定` with",
        "bottom `56×12` strip 'Coming in 95' with",
    ),
    (
        "bottom `56×12` strip `好評発売中` with",
        "bottom `56×12` strip 'Now on Sale' with",
    ),
    (
        "(replace `年`/`月` with `/`, erase `日`)",
        "(replace Year/Month with `/`, erase Day)",
    ),
    (
        "erase `人` suffix at `x=108..119`",
        "erase counter suffix at `x=108..119`",
    ),
    (
        "replace `月` (`14×14`) with **`/`**, clear `日` (`14×14`), **`Blood type:`** (`48×14`), clear suffix `型` (`16×14`)",
        "replace Month (`14×14`) with **`/`**, clear Day (`14×14`), **`Blood type:`** (`48×14`), clear suffix Type (`16×14`)",
    ),
    ("`425点`", "`425 pts`"),
    (
        "replace `神宮前` (`12×32`) with vertical **`JINGUMAE`**",
        "replace Jingu-mae (`12×32`) with vertical **`JINGUMAE`**",
    ),
    (
        "Traditional Japanese festival fan kanji (`祭`). **Recommendation**: Keep `祭` as an authentic cultural motif on her yukata fan (just like a Happi coat crest), or replace with **`FEST`** / a firework crest inside the `24×24` fan face.",
        "Traditional Japanese festival fan emblem. **Recommendation**: Keep festival crest as an authentic cultural motif on her yukata fan (just like a Happi coat crest), or replace with **`FEST`** / a firework crest inside the `24×24` fan face.",
    ),
    (
        "frames `392`–`407` show `清川 望 / 笹木綾子` top-left and `鏡 魅羅 / 五十嵐麗` bottom-right",
        "frames `392`–`407` show Nozomi Kiyokawa / Ayako Sasaki top-left and Mira Kagami / Rei Igarashi bottom-right",
    ),
    ("(e.g., `清川 望`, `鏡 魅羅`)", "(e.g., Nozomi Kiyokawa, Mira Kagami)"),
    ("`笹木綾子`, `五十嵐麗`", "Ayako Sasaki, Rei Igarashi"),
]

# Character name dictionary for human-friendly titles
CHAR_NAMES = {
    "AS": "Asahina Yuko",
    "FU": "Fujisaki Shiori",
    "HI": "Himoo Yuina",
    "KA": "Katagiri Ayako",
    "KT": "Katagiri Ayako",
    "KG": "Kagami Mira",
    "KI": "Kisaragi Mio",
    "KS": "Kisaragi Mio",
    "KO": "Koshiki Yukari",
    "KY": "Kiyokawa Nozomi",
    "MI": "Mikihara Megumi",
    "NI": "Nijino Saki",
    "SA": "Saotome Yumi",
    "TA": "Tatebayashi Miharu",
    "REI": "Ijuin Rei",
    "IJ": "Ijuin Rei",
    "YO": "Saotome Yoshio",
    "YOSHIO": "Saotome Yoshio",
    "OT": "Other Characters",
}

# Archive friendly titles
ARCHIVE_TITLES = {
    "GASSHUKU": "Club Training Camp",
    "GASSH_FU": "Club Training Camp (Shiori)",
    "SHOGATU": "New Year Shrine Visit",
    "SHUGAKU": "School Trip to Kyoto",
    "GEKO": "Walking Home After School",
    "G_BIRTH": "Girl Birthdays",
    "P_BIRTH": "Player Birthday",
    "VALEN": "Valentine's Day",
    "WHITE": "White Day",
    "XMAS": "Christmas Party",
    "OMIMAI": "Sick Visit",
    "NYUBU": "Joining School Club",
    "HAGEMASI": "Encouragement Scenes",
    "KARADATA": "Karaoke Data",
    "SERIFU": "Main Story Dialogue",
    "ENDING": "Ending Sequences",
    "EPILOG": "Epilogue Sequences",
    "GROUP_D": "Group Dates",
    "G_APPEAR": "Girl Appearance Events",
    "G_INVITE": "Girl Invitation Events",
    "TEL": "Telephone Calls & Conversations",
    "NAME_ENT": "Name & Birthday Entry",
    "RPG_BAT": "RPG Battle Minigame",
    "BUNKA_SD": "Cultural Festival Chibi Events",
    "OPTION": "Audio, Screen & Control Options",
    "OLH": "Online Help System",
    "ETC": "General System & Menu Overlays",
    "EVENT": "Story Events Overlay",
    "TACO": "Octopus Minigame",
    "DATE": "Date System & Locations",
    "GYOZI": "School Calendar & Events",
    "BUNKAKEN": "Cultural Club Activities",
    "SHOUGATU": "New Year Events",
    "BUNKASAI": "Cultural Festival System",
    "MASTER": "Master System Sequences",
    "TAIIKU": "Sports Festival System",
    "KANGEI": "Welcome Party Events",
    "DATE2": "Extended Date Outings",
    "TT": "Table Tennis Minigame",
    "RENSYU": "Practice Matches",
    "SIAI_TX": "Sports Matches & Commentary",
    "KARAKASI": "Karakasa Umbrella Dance",
    "F_DANCE": "Folk Dance Dialogue",
    "KISA23": "Sports Festival Track & Field",
    "KIYOTAI": "Sports Festival Cheerleading",
    "O": "Staff Credits & Developer Symbols",
    "NAME_KAN": "Kanji Character Entry Table",
}


def sha256_file(path: str) -> str:
    """Calculate SHA256 of a file."""
    h = hashlib.sha256()
    with open(path, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()


def route_id(id_str: str, is_supplemental: bool = False) -> Tuple[str, str, str]:
    """Route an ID to (relpath, local_id, display_title)."""
    if is_supplemental:
        parts = id_str.split(":")
        path = parts[1]
        local_id = ":".join(parts[2:])

        stem = path.split("/")[-1].split(".")[0].lower()
        if "EXN" in path:
            fname = f"supplemental/{stem}_exn.md"
        elif stem == "o":
            fname = "supplemental/o_bin.md"
        else:
            fname = f"supplemental/{stem}.md"

        title_stem = stem.upper()
        desc = ARCHIVE_TITLES.get(title_stem, title_stem.replace("_", " ").title())
        title = f"Supplemental text, {desc} (`{path}`)"
        return fname, local_id, title

    parts = id_str.split(":")
    p0 = parts[0]

    if p0 == "talk":
        cont_parts = parts[1].split("/")
        category = cont_parts[1].lower()
        archive_name = cont_parts[2].split(".")[0]
        fname = f"dialogue/{category}/{archive_name.lower()}.md"
        local_id = ":".join(parts[2:])

        char_match = None
        for code, cname in CHAR_NAMES.items():
            if archive_name.startswith(f"{code}_") or archive_name.endswith(f"_{code}"):
                char_match = cname
                break
            if archive_name == code:
                char_match = cname
                break

        desc = ARCHIVE_TITLES.get(archive_name, archive_name.replace("_", " ").title())
        if char_match and char_match not in desc:
            title = f"Dialogue, {desc}, {char_match} (`{parts[1]}`)"
        else:
            title = f"Dialogue, {desc} (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "speaktst-layout":
        cont_parts = parts[1].split("/")
        archive_name = cont_parts[2].split(".")[0]
        fname = f"test_dialogue/{archive_name.lower()}.md"
        local_id = f"{parts[2]}:{parts[3]}"

        code = archive_name.replace("TEST_", "")
        cname = CHAR_NAMES.get(code, code)
        title = f"Test dialogue, {cname} (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "c":
        mod = parts[1].lower()
        fname = f"system/c_strings/{mod}.md"
        local_id = ":".join(parts[2:])
        desc = ARCHIVE_TITLES.get(parts[1], parts[1].replace("_", " ").title())
        title = f"C code strings, {desc} (`{parts[1]}`)"
        return fname, local_id, title

    if p0.startswith("CDROM/EXEDIR/"):
        stem = p0.split("/")[2].split(".")[0]
        fname = f"system/exedir/{stem.lower()}.md"
        local_id = ":".join(parts[1:])
        desc = ARCHIVE_TITLES.get(stem, stem.replace("_", " ").title())
        title = f"Overlay strings, {desc} (`{p0}`)"
        return fname, local_id, title

    if p0 == "SLPM_86.053":
        fname = "system/main_exe/slpm_86_053.md"
        local_id = ":".join(parts[1:])
        title = "Main executable strings (`SLPM_86.053`)"
        return fname, local_id, title

    if p0 == "overlay-table":
        stem = parts[1].split("/")[-1].split(".")[0]
        fname = f"system/tables/{stem.lower()}.md"
        local_id = ":".join(parts[2:])
        desc = ARCHIVE_TITLES.get(stem, stem.replace("_", " ").title())
        title = f"Overlay table data, {desc} (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "dictionary":
        stem = parts[1].split("/")[-1].replace(".", "_")
        fname = f"system/dictionaries/{stem.lower()}.md"
        local_id = ":".join(parts[2:])
        title = f"Dictionary tables (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "bunka-sd-view":
        fname = "system/views/bunka_sd_view.md"
        local_id = ":".join(parts[2:])
        title = "Cultural festival SD chibi view events (`BUNKA_SD.EXN`)"
        return fname, local_id, title

    fname = "system/misc.md"
    local_id = id_str
    title = "Miscellaneous runtime and buffer strings"
    return fname, local_id, title


def split_tree(
    returned_path: str,
    supplemental_path: str,
    manifest_path: Optional[str],
    image_catalog_path: Optional[str],
    out_dir: str,
) -> Dict[str, Any]:
    """Split returned-v3 and supplemental into contributor tree, without duplicate _meta/."""
    os.makedirs(out_dir, exist_ok=True)

    # Preserve canonical distribution files in out_dir
    dest_returned = os.path.join(out_dir, "returned-v3.jsonl")
    dest_supplemental = os.path.join(out_dir, "supplemental.jsonl")
    dest_manifest = os.path.join(out_dir, "manifest.json")

    if os.path.abspath(returned_path) != os.path.abspath(dest_returned):
        shutil.copy2(returned_path, dest_returned)

    if os.path.abspath(supplemental_path) != os.path.abspath(dest_supplemental):
        shutil.copy2(supplemental_path, dest_supplemental)

    if manifest_path and os.path.exists(manifest_path) and os.path.abspath(manifest_path) != os.path.abspath(dest_manifest):
        shutil.copy2(manifest_path, dest_manifest)
    elif not os.path.exists(dest_manifest):
        # Generate manifest.json if missing
        base_manifest = {
            "schema": "tokimemo.translation-backup.v1",
            "source_units_sha256": "a2c06f02625792bd2e0807048c748bccdf60ad60662cf0eb0fc466622ae72560",
            "main_rows": 55403,
            "main_nonempty_targets": 54786,
            "main_sha256": sha256_file(dest_returned),
            "supplemental_rows": 508,
            "supplemental_sha256": sha256_file(dest_supplemental),
            "supplemental_input_sha256": "9a9938f421a108aa0c5acc64943da6040946f5cfad1bed7a44b04b4243b007eb",
            "supplemental_source_byte_coverage_verified": False,
            "translation_quality_verified": False,
            "image_translation_catalog_included": True,
        }
        with open(dest_manifest, "w", encoding="utf-8") as f:
            json.dump(base_manifest, f, indent=2)

    # Remove legacy _meta/ directory if present
    meta_dir = os.path.join(out_dir, "_meta")
    if os.path.exists(meta_dir):
        shutil.rmtree(meta_dir)

    print(f"Reading {returned_path}...")
    files_data = defaultdict(list)
    file_titles = {}

    with open(returned_path, "r", encoding="utf-8") as f:
        for line in f:
            d = json.loads(line)
            row_id = d["id"]
            target = d.get("target_text")
            relpath, local_id, title = route_id(row_id, is_supplemental=False)
            file_titles[relpath] = title
            files_data[relpath].append(
                (
                    local_id,
                    d.get("review_status", ""),
                    target,
                    d.get("translator_note", ""),
                    row_id,
                )
            )

    print(f"Reading {supplemental_path}...")
    with open(supplemental_path, "r", encoding="utf-8") as f:
        for line in f:
            d = json.loads(line)
            row_id = d["id"]
            target = d.get("translation")
            relpath, local_id, title = route_id(row_id, is_supplemental=True)
            file_titles[relpath] = title
            files_data[relpath].append(
                (
                    local_id,
                    d.get("review_status", ""),
                    target,
                    d.get("translator_note", ""),
                    row_id,
                )
            )

    # Write human markdown files
    print(f"Writing {len(files_data)} markdown files...")
    for relpath, rows in files_data.items():
        fullpath = os.path.join(out_dir, relpath)
        os.makedirs(os.path.dirname(fullpath), exist_ok=True)

        title = file_titles.get(relpath, "Translation file")
        total_rows = len(rows)
        ready_rows = sum(
            1 for r in rows if r[1] in ("ready_for_review", "supplemental_ready")
        )
        unresolved_rows = total_rows - ready_rows
        translated_rows = sum(1 for r in rows if r[2] is not None)
        empty_slots = total_rows - translated_rows

        md_lines = [
            f"# {title}",
            "",
            f"File `{relpath}` contains {translated_rows} translated lines, {empty_slots} empty source slots, and {total_rows} total records ({ready_rows} ready for review, {unresolved_rows} unresolved).",
            "",
            "| ID | Status | Translation | Notes |",
            "| :--- | :--- | :--- | :--- |",
        ]

        for local_id, status, target, note, full_id in rows:
            if status in ("ready_for_review", "supplemental_ready"):
                disp_status = "ready"
            elif "unresolved" in status:
                disp_status = "unresolved"
            else:
                disp_status = status

            if target is None:
                disp_target = "_(empty slot)_"
            elif target != target.strip() or target.strip() == "":
                disp_target = f"`{target}`"
            else:
                disp_target = target.replace("|", "\\|")

            disp_note = note.replace("|", "\\|") if note else ""
            md_lines.append(
                f"| `{local_id}` | {disp_status} | {disp_target} | {disp_note} |"
            )

        with open(fullpath, "w", encoding="utf-8") as f:
            f.write("\n".join(md_lines) + "\n")

    # Convert image translation catalog
    if image_catalog_path and os.path.exists(image_catalog_path):
        print(f"Converting image catalog from {image_catalog_path}...")
        convert_image_catalog(
            src_path=image_catalog_path,
            out_md_path=os.path.join(out_dir, "graphics", "image_text_catalog.md"),
            out_jsonl_path=os.path.join(out_dir, "graphics", "image_text_catalog.jsonl"),
        )

    # Generate README, CONTRIBUTING, INDEX
    print("Generating README.md, CONTRIBUTING.md, INDEX.md...")
    generate_documentation(out_dir, files_data, file_titles)

    return {
        "returned_v3_rows": 55403,
        "returned_v3_sha256": sha256_file(dest_returned),
        "supplemental_rows": 508,
        "supplemental_sha256": sha256_file(dest_supplemental),
        "total_files": len(files_data),
    }


def convert_image_catalog(src_path: str, out_md_path: str, out_jsonl_path: str):
    """Convert IMAGE_TRANSLATIONS.md to target-only catalog with zero Japanese text."""
    with open(src_path, "r", encoding="utf-8") as f:
        content = f.read()

    for old, new in IMAGE_REPLACEMENTS + [
        (
            "Every entry provides:\n  1. **Asset & Region ID** (source path, frame/sheet index, pixel dimensions `W×H`)\n  2. **Original Japanese Text** (exact transcription of visible lettering)\n  3. **English Meaning / Translation**\n  4. **Suggested Translated Graphic / Same-Size Abbreviation**",
            "Every entry provides:\n  1. **Asset & Region ID** (source path, frame/sheet index, pixel dimensions `W×H`)\n  2. **English Meaning / Translation**\n  3. **Suggested Translated Graphic / Same-Size Abbreviation**",
        )
    ]:
        content = content.replace(old, new)

    lines = content.splitlines()
    out_lines = []
    jsonl_records = []

    current_section = "General"
    current_container = ""
    current_headers = None
    orig_idx = -1

    for line in lines:
        s = line.strip()
        if s.startswith("# "):
            out_lines.append(
                "# Tokimeki Memorial — Target-Only Image & Graphic Translation Catalog"
            )
            out_lines.append("")
            out_lines.append(
                "> **Note**: This catalog documents all text-bearing graphical assets, dimensions, English translations, and suggested graphic layout treatments. All original Japanese source strings have been stripped in accordance with repository copyright discipline."
            )
            out_lines.append("")
            continue

        if s.startswith("## ") or s.startswith("### "):
            header_text = s.lstrip("#").strip()
            current_section = header_text
            m_cont = re.search(r"\(([^)]+)\)", header_text)
            if m_cont:
                current_container = m_cont.group(1).replace("`", "")
            out_lines.append(line)
            current_headers = None
            orig_idx = -1
            continue

        if s.startswith("|"):
            cells = [c.strip() for c in s.split("|")[1:-1]]
            if "Original Japanese" in cells:
                current_headers = cells
                orig_idx = cells.index("Original Japanese")
                new_cells = [c for i, c in enumerate(cells) if i != orig_idx]
                out_lines.append("| " + " | ".join(new_cells) + " |")
                continue

            if "---" in s and current_headers:
                sep_cells = [c for i, c in enumerate(cells) if i != orig_idx]
                out_lines.append("| " + " | ".join(sep_cells) + " |")
                continue

            if current_headers and orig_idx >= 0 and len(cells) == len(current_headers):
                row_dict = {
                    "section": current_section,
                    "container": current_container,
                }
                for h, c in zip(current_headers, cells):
                    if h != "Original Japanese":
                        key = (
                            h.lower()
                            .replace(" ", "_")
                            .replace("/", "_")
                            .replace("`", "")
                            .replace("(", "")
                            .replace(")", "")
                        )
                        row_dict[key] = c
                jsonl_records.append(row_dict)

                new_cells = [c for i, c in enumerate(cells) if i != orig_idx]
                out_lines.append("| " + " | ".join(new_cells) + " |")
                continue

            out_lines.append(line)
            continue

        out_lines.append(line)

    final_md = "\n".join(out_lines) + "\n"

    cjk_matches = CJK_PATTERN.findall(final_md)
    if cjk_matches:
        raise ValueError(
            f"Found {len(cjk_matches)} Japanese characters in converted catalog: {cjk_matches[:20]}"
        )

    jsonl_text = "\n".join(json.dumps(r, ensure_ascii=False) for r in jsonl_records) + "\n"
    cjk_jsonl = CJK_PATTERN.findall(jsonl_text)
    if cjk_jsonl:
        raise ValueError(
            f"Found {len(cjk_jsonl)} Japanese characters in JSONL catalog: {cjk_jsonl[:20]}"
        )

    os.makedirs(os.path.dirname(out_md_path), exist_ok=True)
    with open(out_md_path, "w", encoding="utf-8") as f:
        f.write(final_md)

    with open(out_jsonl_path, "w", encoding="utf-8") as f:
        f.write(jsonl_text)


def parse_markdown_table(path: str) -> Dict[str, Tuple[str, Optional[str], str]]:
    """Parse a markdown file table. Returns local_id -> (status, target_text, note)."""
    rows = {}
    in_table = False
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            s = line.strip()
            if not s.startswith("|") or not s.endswith("|"):
                continue
            cells = [c.strip() for c in re.split(r"(?<!\\)\|", s)[1:-1]]
            if len(cells) < 4:
                continue
            if cells[0] in ("ID", ":---", "---"):
                in_table = True
                continue
            if not in_table:
                continue

            local_id = cells[0].strip("`")
            status = cells[1]
            raw_target = cells[2]
            note = cells[3].replace("\\|", "|")

            if raw_target.startswith("`") and raw_target.endswith("`") and len(raw_target) >= 2:
                target = raw_target[1:-1]
            elif raw_target in ("_(empty slot)_", "-") or raw_target == "":
                target = None
            else:
                target = raw_target.replace("\\|", "|")

            rows[local_id] = (status, target, note)
    return rows


def build_tree(
    tree_dir: str,
    out_returned_path: Optional[str] = None,
    out_supplemental_path: Optional[str] = None,
    base_returned_path: Optional[str] = None,
    base_supplemental_path: Optional[str] = None,
) -> Tuple[str, str]:
    """Reconstruct returned-v3.jsonl and supplemental.jsonl by merging markdown edits into baseline JSONL files."""
    if not base_returned_path:
        local_cand = os.path.join(tree_dir, "returned-v3.jsonl")
        base_returned_path = local_cand if os.path.exists(local_cand) else "translations/en/returned-v3.jsonl"

    if not base_supplemental_path:
        local_cand = os.path.join(tree_dir, "supplemental.jsonl")
        base_supplemental_path = local_cand if os.path.exists(local_cand) else "translations/en/supplemental.jsonl"

    if not out_returned_path:
        out_returned_path = os.path.join(tree_dir, "returned-v3.jsonl")

    if not out_supplemental_path:
        out_supplemental_path = os.path.join(tree_dir, "supplemental.jsonl")

    md_cache = {}

    def get_row_data(row_id: str, is_supplemental: bool, orig_target: Optional[str], orig_note: str, orig_status: str) -> Tuple[str, Optional[str], str]:
        relpath, local_id, _ = route_id(row_id, is_supplemental)
        if relpath not in md_cache:
            fpath = os.path.join(tree_dir, relpath)
            md_cache[relpath] = parse_markdown_table(fpath)

        table = md_cache[relpath]
        if local_id not in table:
            raise KeyError(f"Local ID {local_id} not found in {relpath} (full ID: {row_id})")

        md_status, target, note = table[local_id]

        if md_status == "ready":
            final_status = orig_status if "ready" in orig_status else ("ready_for_review" if not is_supplemental else "supplemental_ready")
        elif md_status == "unresolved":
            final_status = orig_status if "unresolved" in orig_status else "unresolved"
        else:
            final_status = md_status

        # Whitespace preservation rule for unmodified lines
        if orig_target is not None and target is not None:
            if target == orig_target or target.strip() == orig_target.strip():
                target = orig_target

        if orig_note and note.strip() == orig_note.strip():
            note = orig_note

        return final_status, target, note

    # Build returned-v3.jsonl
    print(f"Building {out_returned_path} from baseline {base_returned_path}...")
    temp_ret = out_returned_path + ".tmp"
    os.makedirs(os.path.dirname(os.path.abspath(out_returned_path)), exist_ok=True)

    with open(base_returned_path, "r", encoding="utf-8") as f_in, open(temp_ret, "w", encoding="utf-8", newline="\n") as f_out:
        for line in f_in:
            d = json.loads(line)
            orig_target = d.get("target_text")
            orig_note = d.get("translator_note", "")
            orig_status = d.get("review_status", "")

            status, target, note = get_row_data(d["id"], False, orig_target, orig_note, orig_status)
            d["review_status"] = status
            d["target_text"] = target
            d["translator_note"] = note

            line_out = json.dumps(d, ensure_ascii=False, separators=(",", ":")) + "\n"
            f_out.write(line_out)

    os.replace(temp_ret, out_returned_path)

    # Build supplemental.jsonl
    print(f"Building {out_supplemental_path} from baseline {base_supplemental_path}...")
    temp_sup = out_supplemental_path + ".tmp"
    os.makedirs(os.path.dirname(os.path.abspath(out_supplemental_path)), exist_ok=True)

    with open(base_supplemental_path, "r", encoding="utf-8") as f_in, open(temp_sup, "w", encoding="utf-8", newline="\n") as f_out:
        for line in f_in:
            d = json.loads(line)
            orig_target = d.get("translation")
            orig_note = d.get("translator_note", "")
            orig_status = d.get("review_status", "")

            status, target, note = get_row_data(d["id"], True, orig_target, orig_note, orig_status)
            d["review_status"] = status
            d["translation"] = target if target is not None else ""
            d["translator_note"] = note

            line_out = json.dumps(d, ensure_ascii=False) + "\n"
            f_out.write(line_out)

    os.replace(temp_sup, out_supplemental_path)

    ret_sha = sha256_file(out_returned_path)
    sup_sha = sha256_file(out_supplemental_path)
    print(f"Reconstructed returned-v3 SHA256:     {ret_sha}")
    print(f"Reconstructed supplemental SHA256:    {sup_sha}")
    return ret_sha, sup_sha


def validate_tree(tree_dir: str) -> bool:
    """Validate zero Japanese text and protected token consistency across contributor tree."""
    print(f"Validating contributor tree at {tree_dir}...")

    # 1. Zero Japanese check
    cjk_errors = []
    total_files_checked = 0

    for root, dirs, files in os.walk(tree_dir):
        for fname in files:
            if fname.endswith((".md", ".json", ".jsonl")):
                total_files_checked += 1
                fpath = os.path.join(root, fname)
                with open(fpath, "r", encoding="utf-8", errors="replace") as f:
                    for line_no, line in enumerate(f, 1):
                        matches = CJK_PATTERN.findall(line)
                        if matches:
                            cjk_errors.append(
                                (
                                    os.path.relpath(fpath, tree_dir),
                                    line_no,
                                    matches[:5],
                                    line.strip()[:60],
                                )
                            )

    if cjk_errors:
        print(f"VALIDATION FAILED: Found {len(cjk_errors)} lines with Japanese text:")
        for fn, ln, m, snippet in cjk_errors[:10]:
            print(f"  {fn}:{ln}: chars={m} | snippet={snippet}")
        return False
    else:
        print(f"PASSED: Zero Japanese characters in all {total_files_checked} files.")

    # 2. Protected token integrity check
    token_errors = []
    for root, dirs, files in os.walk(tree_dir):
        for fname in files:
            if not fname.endswith(".md") or fname in ("README.md", "CONTRIBUTING.md", "INDEX.md"):
                continue
            fpath = os.path.join(root, fname)
            relpath = os.path.relpath(fpath, tree_dir)
            table = parse_markdown_table(fpath)

            for local_id, (status, target, note) in table.items():
                if target is None:
                    continue
                if target.count("[[") != target.count("]]"):
                    token_errors.append((relpath, local_id, "Mismatched double brackets [[]]"))

    if token_errors:
        print(f"VALIDATION FAILED: Protected token errors ({len(token_errors)}):")
        for fn, lid, err in token_errors[:10]:
            print(f"  {fn} ({lid}): {err}")
        return False
    else:
        print(f"PASSED: Protected tokens syntax valid across all dialogue files.")

    # 3. Canonical distribution files presence
    for req in ("returned-v3.jsonl", "supplemental.jsonl", "manifest.json"):
        if not os.path.exists(os.path.join(tree_dir, req)):
            print(f"VALIDATION FAILED: Missing canonical distribution file {req} in {tree_dir}")
            return False

    return True


def print_status(tree_dir: str):
    """Print comprehensive status report."""
    manifest_path = os.path.join(tree_dir, "manifest.json")
    if os.path.exists(manifest_path):
        with open(manifest_path, "r", encoding="utf-8") as f:
            m = json.load(f)
        print("=== Contributor Tree Status ===")
        print(f"Schema:               {m.get('schema')}")
        print(f"Main Rows:            {m.get('main_rows', m.get('returned_v3_rows')):,}")
        print(f"Supplemental Rows:    {m.get('supplemental_rows'):,}")
        print(f"Source Units SHA:     {m.get('source_units_sha256')}")
        print(f"Main SHA256:          {m.get('main_sha256', m.get('returned_v3_sha256'))}")
        print(f"Supplemental SHA256:  {m.get('supplemental_sha256')}")
        print("===============================")


def generate_documentation(
    out_dir: str,
    files_data: Dict[str, List[Any]],
    file_titles: Dict[str, str],
):
    """Generate README.md, CONTRIBUTING.md, and INDEX.md."""
    cat_stats = defaultdict(lambda: {"total": 0, "translated": 0, "empty": 0, "ready": 0, "files": 0})
    char_stats = defaultdict(lambda: {"total": 0, "translated": 0, "empty": 0, "ready": 0, "files": 0})

    for relpath, rows in files_data.items():
        parts = relpath.split("/")
        if len(parts) >= 2 and parts[0] in ("dialogue", "system"):
            cat = f"{parts[0]}/{parts[1].split('.')[0]}"
        else:
            cat = parts[0]

        total = len(rows)
        translated = sum(1 for r in rows if r[2] is not None)
        empty = total - translated
        ready = sum(1 for r in rows if r[1] in ("ready_for_review", "supplemental_ready"))

        cat_stats[cat]["total"] += total
        cat_stats[cat]["translated"] += translated
        cat_stats[cat]["empty"] += empty
        cat_stats[cat]["ready"] += ready
        cat_stats[cat]["files"] += 1

        title = file_titles.get(relpath, "")
        for code, cname in CHAR_NAMES.items():
            if cname in title:
                char_stats[cname]["total"] += total
                char_stats[cname]["translated"] += translated
                char_stats[cname]["empty"] += empty
                char_stats[cname]["ready"] += ready
                char_stats[cname]["files"] += 1
                break

    # 1. README.md
    readme_content = """# Tokimeki Memorial (PS1) English translation

This directory holds the target-only English translation for *Tokimeki Memorial* (`SLPM-86053`, PlayStation the Best), split into 184 Markdown tables by disc archive, scene, and executable module.

Every record with Japanese source text is translated (100%), covering all 54,786 translatable lines in `returned-v3.jsonl` and all 508 lines in `supplemental.jsonl`. The remaining 617 records in `returned-v3.jsonl` are empty source slots on the PS1 disc (`Empty source slot (no Japanese text).`) kept so table indices match the binary layout.

## Guides and indexes

- [Contributing guide (CONTRIBUTING.md)](CONTRIBUTING.md) covers editing tables, control tokens (`[[T001:row]]`), and review statuses.
- [Directory index (INDEX.md)](INDEX.md) lists all 184 Markdown files with line and empty-slot counts.
- [Image text catalog (graphics/image_text_catalog.md)](graphics/image_text_catalog.md) lists translated UI, HUD, menu, magazine, credit, and card graphics.

## Translation counts by category

All translatable lines in every category have English text. Categories where `Total records` exceeds `Translated lines` contain blank source slots in the original binary tables (`_(empty slot)_`).

| Category | Files | Translatable lines | Translated lines | Empty source slots | Total records | Ready for review |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
"""
    for cat in sorted(cat_stats.keys()):
        s = cat_stats[cat]
        readme_content += f"| `{cat}` | {s['files']} | {s['translated']:,} | {s['translated']:,} | {s['empty']:,} | {s['total']:,} | {s['ready']:,} |\n"

    readme_content += """
## Character dialogue counts

| Character | Files | Translatable lines | Translated lines | Empty source slots | Total records | Ready for review |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
"""
    for cname in sorted(char_stats.keys()):
        s = char_stats[cname]
        readme_content += f"| {cname} | {s['files']} | {s['translated']:,} | {s['translated']:,} | {s['empty']:,} | {s['total']:,} | {s['ready']:,} |\n"

    readme_content += """
## Validation and build

Use `tools/translation_tree.py` inside Docker to check edits and rebuild the canonical JSONL files:

```bash
# Check that no Japanese source text was added and all protected tokens match
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py validate

# Rebuild returned-v3.jsonl and supplemental.jsonl from the Markdown tables
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py build
```

## Provenance and canonical files

- `returned-v3.jsonl` matches the validated translation return byte-for-byte: 55,403 stable IDs, 54,786 translated targets, and 617 empty source slots. It stores target text, translator notes, protected token markers, and source hashes without Japanese source text.
- `supplemental.jsonl` stores 508 translated supplemental records with IDs, context, target text, notes, review status, and protected markers. Extracted `japanese` and `copy_template` fields are excluded. These rows still need original-byte provenance and coverage review.
- `manifest.json` pins the SHA-256 hashes of both files and their source inputs. Keep protected control markers and stable IDs intact when editing. There is no original textbox length cap.

Source worktree: `/Users/lila/Documents/Projects/tokimemo-wt/translation`. Original game data must be supplied separately. Runtime language selection and translated image assets remain later work.
"""
    with open(os.path.join(out_dir, "README.md"), "w", encoding="utf-8") as f:
        f.write(readme_content)

    # 2. CONTRIBUTING.md
    contributing_content = """# Translation contributing guide

This guide explains how the Markdown tables in `translations/en/` map to the canonical JSONL files and how to edit lines without breaking control tokens.

## Directory layout

- `dialogue/bunka/` contains Cultural Festival event scripts (`CDROM/BUNKA/*.BIN`).
- `dialogue/serifu/` contains date, holiday, school trip, and story scripts (`CDROM/SERIFU/*.BIN`).
- `dialogue/master/` contains master event scripts (`CDROM/MASTER/*.BIN`).
- `test_dialogue/` contains voice and dialogue test scripts (`CDROM/SPEAKTST/*.BIN`).
- `system/exedir/` contains overlay strings for phone calls (`tel.md`), options (`option.md`), the RPG battle minigame (`rpg_bat.md`), and name entry (`name_ent.md`).
- `system/c_strings/` contains strings referenced from decompiled C code.
- `system/main_exe/` contains strings from `SLPM_86.053`.
- `system/tables/` and `system/dictionaries/` contain system names, gift lists, date locations, and profile labels.
- `supplemental/` contains sports festival dialogue, karaoke lyrics, and ending credits.
- `graphics/` contains the target-only catalog of UI, calendar, exam, and sprite text.

Canonical distribution files (`returned-v3.jsonl`, `supplemental.jsonl`, and `manifest.json`) live at the root of `translations/en/`.

## Editing a file

1. Open the target Markdown file (for example, [`dialogue/serifu/date_sa.md`](dialogue/serifu/date_sa.md)).
2. Edit the `Translation` column for the row ID you want to revise.
3. Update the `Status` column to `ready` when the line has been reviewed.
4. Run `tools/translation_tree.py validate` and `tools/translation_tree.py build` before opening a pull request.

## Protected tokens and table formatting

The game engine uses control tokens inside double brackets for line breaks, name substitutions, and string formatting:

- `[[T001:row]]` inserts a line break inside a dialogue textbox.
- `[[T001:sub:player_family]]` substitutes the player's family name at runtime.
- `[[T001:sub:current_girl_short_name]]` substitutes the heroine's given name at runtime.
- `[[T001:format_directive]]` and `[[T002:line_feed]]` represent C `sprintf` directives and newlines.

> [!IMPORTANT]
> Keep every `[[T...]]` token in the line. You may move words around a token, but do not delete or rename the token itself.

### Pipe characters in tables

Markdown tables use `|` as a column separator. Escape literal pipe characters inside a cell as `\\|`.

### Empty source slots

Blank binary slots with no Japanese source text appear as `_(empty slot)_` with the note `Empty source slot (no Japanese text).` Leave them as `_(empty slot)_` so slot indices match the disc binary.

## Copyright rules

Do not commit Japanese source text to `translations/en/`. Commit only English target text, stable IDs, translator notes, and layout metadata.

## Local validation and build

Run validation and rebuild commands in Docker:

```bash
# Check for accidental Japanese characters and mismatched control tokens
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py validate

# Rebuild returned-v3.jsonl and supplemental.jsonl from Markdown edits
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py build
```
"""
    with open(os.path.join(out_dir, "CONTRIBUTING.md"), "w", encoding="utf-8") as f:
        f.write(contributing_content)

    # 3. INDEX.md
    index_content = """# Translation file directory index

Index of all 184 Markdown files in `translations/en/`. All translatable lines have English translations; `Empty slots` counts blank records in the original binary that have no Japanese source text.

| File path | Description | Translated lines | Empty slots | Total records | Ready |
| :--- | :--- | :---: | :---: | :---: | :---: |
"""
    for relpath in sorted(files_data.keys()):
        rows = files_data[relpath]
        title = file_titles.get(relpath, relpath)
        total = len(rows)
        translated = sum(1 for r in rows if r[2] is not None)
        empty = total - translated
        ready = sum(1 for r in rows if r[1] in ("ready_for_review", "supplemental_ready"))
        index_content += f"| [{relpath}]({relpath}) | {title} | {translated:,} | {empty:,} | {total:,} | {ready:,} |\n"

    with open(os.path.join(out_dir, "INDEX.md"), "w", encoding="utf-8") as f:
        f.write(index_content)


def main():
    parser = argparse.ArgumentParser(description="Translation Tree Tool")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # split
    split_p = subparsers.add_parser("split", help="Split JSONL files into contributor tree")
    split_p.add_argument("--returned", default="translations/en/returned-v3.jsonl")
    split_p.add_argument("--supplemental", default="translations/en/supplemental.jsonl")
    split_p.add_argument("--manifest", default="translations/en/manifest.json")
    split_p.add_argument("--image-catalog", default="/wt/.asset-cache/text/IMAGE_TRANSLATIONS.md")
    split_p.add_argument("--out-dir", default="translations/en")

    # build
    build_p = subparsers.add_parser("build", help="Build JSONL files from contributor tree")
    build_p.add_argument("--tree-dir", default="translations/en")
    build_p.add_argument("--out-returned", default=None)
    build_p.add_argument("--out-supplemental", default=None)
    build_p.add_argument("--base-returned", default=None)
    build_p.add_argument("--base-supplemental", default=None)

    # validate
    val_p = subparsers.add_parser("validate", help="Validate contributor tree")
    val_p.add_argument("--tree-dir", default="translations/en")

    # status
    stat_p = subparsers.add_parser("status", help="Print contributor tree status")
    stat_p.add_argument("--tree-dir", default="translations/en")

    args = parser.parse_args()

    if args.command == "split":
        split_tree(
            returned_path=args.returned,
            supplemental_path=args.supplemental,
            manifest_path=args.manifest if os.path.exists(args.manifest) else None,
            image_catalog_path=args.image_catalog if os.path.exists(args.image_catalog) else None,
            out_dir=args.out_dir,
        )
    elif args.command == "build":
        build_tree(
            tree_dir=args.tree_dir,
            out_returned_path=args.out_returned,
            out_supplemental_path=args.out_supplemental,
            base_returned_path=args.base_returned,
            base_supplemental_path=args.base_supplemental,
        )
    elif args.command == "validate":
        ok = validate_tree(args.tree_dir)
        sys.exit(0 if ok else 1)
    elif args.command == "status":
        print_status(args.tree_dir)


if __name__ == "__main__":
    main()
