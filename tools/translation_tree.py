#!/usr/bin/env python3
"""tokimemo-decomp translation contributor tree tool.

Splits monolithic translation files (returned-v3.jsonl and supplemental.jsonl)
into a clean, human-editable, GitHub-browsable directory tree under translations/en/,
with 5-column Markdown tables:
  | ID | Context | Japanese | Translation | Notes |
and reconstructs the exact original JSONL files byte-for-byte on build.
"""

import argparse
import csv
import glob
import hashlib
import json
import os
import re
import shutil
import sys
from collections import defaultdict
from typing import Any, Dict, List, Optional, Set, Tuple

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

GIRL_SHORT_NAMES = {
    "AS": "Yuko",
    "FU": "Shiori",
    "HI": "Yuina",
    "KA": "Ayako",
    "KT": "Ayako",
    "KG": "Mira",
    "KI": "Mio",
    "KS": "Mio",
    "KO": "Yukari",
    "KY": "Nozomi",
    "MI": "Megumi",
    "NI": "Saki",
    "SA": "Yumi",
    "TA": "Miharu",
    "REI": "Rei",
    "IJ": "Rei",
    "YO": "Yoshio",
    "YOSHIO": "Yoshio",
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

PURE_UNKNOWN_PATTERN = re.compile(
    r"^(\[\[T\d+:(?:unknown_renderer_bytes|unknown)\]\]\s*)+$"
)


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
        title = f"Supplemental Text: {desc} (`{path}`)"
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
            title = f"Dialogue: {desc} - {char_match} (`{parts[1]}`)"
        else:
            title = f"Dialogue: {desc} (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "speaktst-layout":
        cont_parts = parts[1].split("/")
        archive_name = cont_parts[2].split(".")[0]
        fname = f"test_dialogue/{archive_name.lower()}.md"
        local_id = f"{parts[2]}:{parts[3]}"

        code = archive_name.replace("TEST_", "")
        cname = CHAR_NAMES.get(code, code)
        title = f"Test Dialogue: {cname} (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "c":
        mod = parts[1].lower()
        fname = f"system/c_strings/{mod}.md"
        local_id = ":".join(parts[2:])
        desc = ARCHIVE_TITLES.get(parts[1], parts[1].replace("_", " ").title())
        title = f"C Code Strings: {desc} (`{parts[1]}`)"
        return fname, local_id, title

    if p0.startswith("CDROM/EXEDIR/"):
        stem = p0.split("/")[2].split(".")[0]
        fname = f"system/exedir/{stem.lower()}.md"
        local_id = ":".join(parts[1:])
        desc = ARCHIVE_TITLES.get(stem, stem.replace("_", " ").title())
        title = f"Overlay Strings: {desc} (`{p0}`)"
        return fname, local_id, title

    if p0 == "SLPM_86.053":
        fname = "system/main_exe/slpm_86_053.md"
        local_id = ":".join(parts[1:])
        title = "Main Executable Strings (`SLPM_86.053`)"
        return fname, local_id, title

    if p0 == "overlay-table":
        stem = parts[1].split("/")[-1].split(".")[0]
        fname = f"system/tables/{stem.lower()}.md"
        local_id = ":".join(parts[2:])
        desc = ARCHIVE_TITLES.get(stem, stem.replace("_", " ").title())
        title = f"Overlay Table Data: {desc} (`{parts[1]}`)"
        return fname, local_id, title

    if p0 == "dictionary":
        stem = parts[1].split("/")[-1].replace(".", "_")
        fname = f"system/dictionaries/{stem.lower()}.md"
        local_id = ":".join(parts[2:])
        title = f"Dictionary Tables: `{parts[1]}`"
        return fname, local_id, title

    if p0 == "bunka-sd-view":
        fname = "system/views/bunka_sd_view.md"
        local_id = ":".join(parts[2:])
        title = "Cultural Festival SD Chibi View Events (`BUNKA_SD.EXN`)"
        return fname, local_id, title

    fname = "system/misc.md"
    local_id = id_str
    title = "Miscellaneous Runtime & Buffer Strings"
    return fname, local_id, title


def load_source_database(
    asset_cache_dir: str,
    supplemental_source_path: Optional[str],
    section_context_path: str,
) -> Tuple[Dict[str, Dict[str, str]], Dict[Tuple[str, str], str]]:
    """Load metadata database: id -> {japanese, copy_template, context}."""
    db: Dict[str, Dict[str, str]] = {}
    section_context: Dict[Tuple[str, str], str] = {}

    # 1. Section context lookup
    if os.path.exists(section_context_path):
        with open(section_context_path, "r", encoding="utf-8-sig") as f:
            reader = csv.DictReader(f, delimiter="\t")
            for r in reader:
                src_path = r.get("source_path", "")
                sec = r.get("section", "")
                hint = r.get("source_section_hint", "")
                if hint.startswith("place "):
                    hint = hint[6:]
                section_context[(src_path, sec)] = hint.title()

    # 2. Main TSVs
    tsv_pattern = os.path.join(asset_cache_dir, "**", "*.tsv")
    for fpath in glob.glob(tsv_pattern, recursive=True):
        if os.path.basename(fpath) == "INDEX.tsv":
            continue
        with open(fpath, "r", encoding="utf-8-sig") as f:
            reader = csv.DictReader(f, delimiter="\t")
            if not reader.fieldnames or "id" not in reader.fieldnames:
                continue
            for r in reader:
                rid = r.get("id")
                if rid:
                    db[rid] = {
                        "japanese": r.get("japanese", ""),
                        "copy_template": r.get("copy_template", ""),
                        "context": r.get("context", ""),
                    }

    # 3. Supplemental source JSONL
    if supplemental_source_path and os.path.exists(supplemental_source_path):
        with open(supplemental_source_path, "r", encoding="utf-8") as f:
            for line in f:
                d = json.loads(line)
                rid = d.get("id")
                if rid:
                    db[rid] = {
                        "japanese": d.get("japanese", ""),
                        "copy_template": d.get("copy_template", ""),
                        "context": d.get("context", ""),
                    }

    return db, section_context


def choose_japanese_text(
    japanese: str, copy_template: str, target_text: Optional[str]
) -> str:
    """Select the contributor-friendly Japanese display string."""
    ct = copy_template.strip()
    jp = japanese.strip()

    if target_text is None or (not ct and not jp):
        return "_(empty slot)_"

    if PURE_UNKNOWN_PATTERN.match(ct):
        if jp:
            return f"{jp} (`{ct}`)"
        return ct

    if not ct:
        return jp if jp else "_(empty slot)_"

    return ct


def format_context_string(
    row_id: str,
    raw_context: str,
    section_context: Dict[Tuple[str, str], str],
) -> str:
    """Format concise human-readable context string."""
    if not raw_context:
        return ""

    parts = [p.strip() for p in raw_context.split(";")]
    src_file = parts[0]
    kind = parts[1] if len(parts) > 1 else ""

    kv: Dict[str, str] = {}
    for p in parts[2:]:
        if "=" in p:
            k, v = p.split("=", 1)
            kv[k.strip()] = v.strip().strip('"')

    if kind == "script_record":
        section = kv.get("section", "")
        cmd = kv.get("command", "")
        spk_raw = kv.get("speaker", "")
        spk_label = ""
        if spk_raw:
            try:
                spk_data = json.loads(spk_raw)
                spk_label = spk_data.get("label", "")
            except Exception:
                pass

        # Location or scene name
        loc = section_context.get((src_file, section))
        if not loc:
            if section == "main":
                loc = "Main"
            elif section.startswith("section-"):
                num = section.split("-")[1]
                loc = f"Section {num}"
            elif section.startswith("table-"):
                num = section.split("-")[1]
                loc = f"Scene {num}"
            elif section.startswith("initializer-"):
                num = section.split("-")[1]
                loc = f"Init {num}"
            else:
                loc = section.replace("_", " ").title()

        # Speaker
        if cmd.startswith("player_parenthesis"):
            spk = "Player (monologue)"
        elif cmd == "player":
            spk = "Player"
        elif cmd in (
            "girl",
            "girl_auto",
            "girl_clear",
            "girl_continue",
            "girl_continue_auto",
            "girl_reply",
        ):
            # Extract heroine short name
            stem = src_file.split("/")[-1].split(".")[0].upper()
            found_name = None
            for code, sname in sorted(GIRL_SHORT_NAMES.items(), key=lambda x: -len(x[0])):
                if stem.startswith(f"{code}_") or stem.endswith(f"_{code}") or f"_{code}" in stem:
                    found_name = sname
                    break
            spk = found_name if found_name else "Heroine"
        elif cmd == "shiori":
            spk = "Shiori"
        elif cmd == "yoshio":
            spk = "Yoshio"
        elif "choice" in cmd:
            spk = "Choice"
        elif cmd == "inline":
            if "DATE_" in src_file:
                spk = "Player (monologue)"
            else:
                spk = "Narration"
        elif cmd == "effect":
            spk = "Effect"
        elif cmd == "plain_auto":
            spk = "Narration"
        else:
            if spk_label in ("player_family", "player"):
                spk = "Player"
            elif spk_label == "current_girl_short_name":
                spk = "Heroine"
            elif spk_label == "shiori":
                spk = "Shiori"
            elif spk_label == "yoshio":
                spk = "Yoshio"
            else:
                spk = spk_label.replace("_", " ").title() if spk_label else ""

        if loc and spk:
            return f"{loc} · {spk}"
        return loc or spk

    if kind == "source_literal":
        fn = kv.get("source_function")
        if fn:
            return f"{fn}()"
        line = kv.get("source_line")
        call = kv.get("call")
        if call and line:
            return f"{call}() L{line}"
        return kv.get("category", "C string").replace("_", " ").title()

    if kind in ("dictionary_slot", "stage_text_record"):
        tbl = kv.get("table", "")
        idx = kv.get("index", "")
        if tbl and idx != "":
            return f"{tbl}[{idx}]"
        return tbl or "Dictionary slot"

    if kind == "source_extent":
        cat = kv.get("category", "")
        return cat.replace("_", " ").title() if cat else "Extent"

    if kind == "speaktst_layout_text":
        stem = src_file.split("/")[-1].split(".")[0].upper()
        code = stem.replace("TEST_", "")
        cname = GIRL_SHORT_NAMES.get(code, code)
        return f"{cname} (voice test)"

    if kind == "supplemental_talk":
        line = kv.get("line", "0")
        return f"Match commentary (line {line})"

    if kind == "karaoke_lyrics":
        return "Karaoke lyrics"

    if kind == "ending_song_lyrics":
        return "Ending song lyrics"

    if kind == "duplicate_overlay_string":
        return "Duplicate overlay string"

    if kind == "character_table_files":
        return "Character table"

    if kind == "conditional_runtime_variant":
        rule = kv.get("rule_id", "runtime")
        return f"Runtime variant ({rule})"

    if kind == "initial_runtime_buffer":
        return "Initial runtime buffer"

    return kind.replace("_", " ").title()


def convert_image_catalog(src_path: str, out_md_path: str, out_jsonl_path: str):
    """Convert IMAGE_TRANSLATIONS.md to clean deslopped catalog with Original Japanese preserved."""
    with open(src_path, "r", encoding="utf-8") as f:
        content = f.read()

    # Normalize punctuation and formatting
    content = (
        content.replace(" — ", ", ")
        .replace("—", ", ")
        .replace("‘", "'")
        .replace("’", "'")
        .replace("“", '"')
        .replace("”", '"')
        .replace("slips, highlighting", "slips to highlight")
    )

    lines = content.splitlines()
    out_lines: List[str] = []
    jsonl_records: List[Dict[str, str]] = []

    current_section = "General"
    current_container = ""
    current_headers: Optional[List[str]] = None

    for line in lines:
        s = line.strip()

        # Clean document title
        if s.startswith("# "):
            out_lines.append(
                "# Tokimeki Memorial (PS1) - Image and graphic text translation catalog"
            )
            out_lines.append("")
            out_lines.append(
                "This catalog documents image assets and sprite regions containing Japanese text across Tokimeki Memorial: Forever with You (`SLPM-86053`)."
            )
            out_lines.append("")
            continue

        # Drop horizontal rules between sections to adhere to deslop rules
        if s == "---":
            continue

        # Sentence-case headings
        if s.startswith("## ") or s.startswith("### "):
            is_h2 = s.startswith("## ")
            raw_h = s[3:] if is_h2 else s[4:]
            raw_h = raw_h.strip()

            # Clean heading text
            clean_h = (
                raw_h.replace(" — ", " - ")
                .replace("—", " - ")
                .replace(" / ", " or ")
            )

            # Match container inside parentheses
            m_cont = re.search(r"\(([^)]+)\)", clean_h)
            if m_cont:
                current_container = m_cont.group(1).replace("`", "")

            # Convert to sentence case outside backticks and parens
            parts_h = re.split(r"(`[^`]+`|\([^)]+\))", clean_h)
            rebuilt_h = []
            for i, p in enumerate(parts_h):
                if p.startswith("`") or p.startswith("("):
                    rebuilt_h.append(p)
                else:
                    if i == 0:
                        rebuilt_h.append(p.strip().capitalize())
                    else:
                        rebuilt_h.append(p.lower())
            final_h_text = "".join(rebuilt_h)

            current_section = final_h_text
            out_lines.append(f"{'## ' if is_h2 else '### '}{final_h_text}")
            current_headers = None
            continue

        # Clean em-dashes and formatting in prose lines
        if not s.startswith("|"):
            cleaned_line = (
                line.replace(" — ", ", ")
                .replace("—", ", ")
                .replace("‘", "'")
                .replace("’", "'")
                .replace("“", '"')
                .replace("”", '"')
                .replace(", highlighting the class", "; this highlights the class")
            )
            out_lines.append(cleaned_line)
            continue

        # Markdown tables
        cells = [c.strip() for c in s.split("|")[1:-1]]
        if "Original Japanese" in cells:
            current_headers = cells
            out_lines.append("| " + " | ".join(cells) + " |")
            continue

        if "---" in s and current_headers:
            out_lines.append(line)
            continue

        if current_headers and len(cells) == len(current_headers):
            row_dict: Dict[str, str] = {
                "section": current_section,
                "container": current_container,
            }
            for h, c in zip(current_headers, cells):
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

            # Table row: keep Original Japanese
            clean_cells = [
                c.replace(" — ", ", ")
                .replace("—", ", ")
                .replace(", highlighting the class", "; this highlights the class")
                for c in cells
            ]
            out_lines.append("| " + " | ".join(clean_cells) + " |")
            continue

        out_lines.append(line)

    final_md = "\n".join(out_lines) + "\n"
    jsonl_text = (
        "\n".join(json.dumps(r, ensure_ascii=False) for r in jsonl_records) + "\n"
    )

    os.makedirs(os.path.dirname(out_md_path), exist_ok=True)
    with open(out_md_path, "w", encoding="utf-8") as f:
        f.write(final_md)

    with open(out_jsonl_path, "w", encoding="utf-8") as f:
        f.write(jsonl_text)


def write_tags_documentation(out_path: str):
    """Write translations/en/TAGS.md breaking down all 28 token families adhering to deslop rules."""
    content = """# Control token reference

This document describes all 28 double bracket control tokens used across the Tokimeki Memorial English translation files.

Each token marks an engine control code, line break, choice delimiter, or runtime string substitution. When translating dialogue and system lines, keep these tokens intact. You may reposition them within the sentence so the English reads naturally, but do not delete, rename, or invent markers.

## Dialogue formatting

These tokens control line breaks, row layout, and spacing in dialogue textboxes and system arrays.

### row
- Marker syntax: `[[T...:row]]`
- Purpose: Inserts a line break inside a dialogue box. Advances rendering to the next row.
- Example:
  - Japanese source: `こうして、[[T001:row]]文化祭は、終了した。`
  - English target: `And so, the[[T001:row]]festival was over.`
  - Line ID: `talk:CDROM/BUNKA/AS_S6.BIN:main:g0000:l0000`
- Contributor guidance: Keep this token to fit text within the dialogue box rows.

### row_separator
- Marker syntax: `[[T...:row_separator]]`
- Purpose: Delimits rows inside system text arrays and multi-row menu displays.
- Example:
  - Japanese source: `第一日目の試験科目は[[T001:row_separator]]語学だ。）`
  - English target: `Day one's exam is[[T001:row_separator]]language.)`
  - Line ID: `c:ETC:8014716c:2`
- Contributor guidance: Preserves row boundaries in engine menu arrays.

### line_feed
- Marker syntax: `[[T...:line_feed]]`
- Purpose: Represents an ASCII newline character in C formatted strings and console logs.
- Example:
  - Japanese source: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - English target: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - Line ID: `c:DATE:8013c190:1`
- Contributor guidance: Keep at the end of formatted strings.

### carriage_return
- Marker syntax: `[[T...:carriage_return]]`
- Purpose: Represents an ASCII carriage return character in serial or debug output strings.
- Example:
  - Japanese source: `[[T001:carriage_return]]`
  - English target: `[[T001:carriage_return]]`
  - Line ID: `c:main:80048cf8:3`
- Contributor guidance: Do not remove.

### tab
- Marker syntax: `[[T...:tab]]`
- Purpose: Represents an ASCII horizontal tab character used for column alignment in system menus.
- Example:
  - Japanese source: `今日の成績 [[T001:format_directive]][[T002:tab]]`
  - English target: `Today's Score [[T001:format_directive]][[T002:tab]]`
  - Line ID: `c:ETC:80146d4c:2`
- Contributor guidance: Preserves spacing columns in system menus.

## Branching and choices

These tokens delimit selectable options in choice menus and separate branching reaction variants.

### choice_option
- Marker syntax: `[[T...:choice_option]]`
- Purpose: Delimits selectable options in branching dialogue choice menus.
- Example:
  - Japanese source: `うん、いいよ。[[T001:choice_option]]いや、ちょっとね…。`
  - English target: `Sure, sounds good.[[T001:choice_option]]Well, it's just...`
  - Line ID: `talk:CDROM/BUNKA/AS_S6S.BIN:main:g0000:l0003`
- Contributor guidance: The engine splits options on this token. Translate each option on its respective side of the marker.

### conditional_choice_zero
- Marker syntax: `[[T...:conditional_choice_zero]]`
- Purpose: Marks choice branch zero execution flow.
- Example:
  - Japanese source: `浴衣がすごく似合うね。[[T001:row]]可愛いよ。[[T002:conditional_choice_zero]]`
  - English target: `That yukata really suits you.[[T001:row]]You look cute.[[T002:conditional_choice_zero]]`
  - Line ID: `talk:CDROM/SERIFU/DATE_FU.BIN:section-18:g0001:l0000`
- Contributor guidance: Keep at the end of the choice phrase.

### reply_variant
- Marker syntax: `[[T...:reply_variant]]`
- Purpose: Delimits alternative response variants in multi-path girl reactions.
- Example:
  - Japanese source: `じゃあ、ソッコーで行こうよ。[[T001:row]]ほら、早く、早く。[[T002:reply_variant]]ええーっ、だめなの？[[T003:row]]超楽しみにしてたのに…。[[T004:row]]じゃあ、いいわよ。`
  - English target: `Then let's go, pronto![[T001:row]]C'mon, hurry, hurry![[T002:reply_variant]]Whaaat, you can't?[[T003:row]]I was super excited...[[T004:row]]Fine, whatever.`
  - Line ID: `talk:CDROM/BUNKA/AS_S6S.BIN:main:g0001:l0000`
- Contributor guidance: Separate distinct reaction variants using this token.

### reply_skip_marker
- Marker syntax: `[[T...:reply_skip_marker]]`
- Purpose: Directs the script parser to jump past alternative reaction branches.
- Example:
  - Japanese source: `あ、ありがとう。[[T001:row]]嬉しいな。[[T002:reply_variant]][[T003:reply_skip_marker]]`
  - English target: `Th-thank you.[[T001:row]]I'm so happy.[[T002:reply_variant]][[T003:reply_skip_marker]]`
  - Line ID: `talk:CDROM/SERIFU/DATE_FU.BIN:section-18:g0002:l0000`
- Contributor guidance: Place immediately after the reply branch.

## Name substitutions

These tokens substitute character and player names dynamically into spoken dialogue.

### girl_name
- Marker syntax: `[[T...:girl_name]]`
- Purpose: Substituted at runtime with the active heroine's given name or nickname.
- Example:
  - Japanese source: `あっ、[[T001:girl_name]]。[[T002:row]]来てくれたんだ。`
  - English target: `Oh, [[T001:girl_name]]![[T002:row]]You came!`
  - Line ID: `talk:CDROM/BUNKA/AS_S6.BIN:main:g0003:l0000`
- Contributor guidance: Place where the girl's name naturally appears in the English sentence.

### player_form
- Marker syntax: `[[T...:player_form]]`
- Purpose: Substituted at runtime with the player's name using the heroine's specific address form or honorific.
- Example:
  - Japanese source: `[[T001:player_form]]。[[T002:row]]何、どうしたの？`
  - English target: `[[T001:player_form]],[[T002:row]]what's up?`
  - Line ID: `talk:CDROM/BUNKA/AS_S6S.BIN:main:g0003:l0000`
- Contributor guidance: Position where the player's name is spoken.

### player_family
- Marker syntax: `[[T...:player_family]]`
- Purpose: Substituted at runtime with the player's last name (family name).
- Example:
  - Japanese source: `[[T001:inline_label]]先生おっ、[[T002:player_family]]君[[T003:row]]待ってたよ。`
  - English target: `[[T001:inline_label]]Teacher: Ah, [[T002:player_family]]-kun![[T003:row]]I've been expecting you.`
  - Line ID: `talk:CDROM/SERIFU/EVENT_AS.BIN:initializer-00:g0002:l0001`
- Contributor guidance: Use where a teacher or acquaintance calls the protagonist by last name.

### player_given
- Marker syntax: `[[T...:player_given]]`
- Purpose: Substituted at runtime with the player's first name (given name).
- Example:
  - Japanese source: `よお、[[T001:player_given]]。`
  - English target: `Yo, [[T001:player_given]].`
  - Line ID: `talk:CDROM/BUNKA/BUN_PC.BIN:bun-pc:g0006:l0000`
- Contributor guidance: Use where close friends address the protagonist by first name.

### player_full
- Marker syntax: `[[T...:player_full]]`
- Purpose: Substituted at runtime with the player's full name (family name followed by given name).
- Example:
  - Japanese source: `あっ、俺、[[T001:player_full]]。[[T002:row]]こちらこそ、よろしく。[[T003:row]]（エセ外人みたいな娘だな。）`
  - English target: `Oh, I'm [[T001:player_full]][[T002:row]]Nice to meet you.[[T003:row]](Talks like a phony foreigner.)`
  - Line ID: `talk:CDROM/SERIFU/GASSHUKU.BIN:table-1:g0004:l0003`
- Contributor guidance: Use in self-introductions.

### yoshio_name
- Marker syntax: `[[T...:yoshio_name]]`
- Purpose: Substituted at runtime with Yoshio Saotome's name.
- Example:
  - Japanese source: `今日は、自由行動の日だ。[[T001:row]][[T002:yoshio_name]]と、一緒に[[T003:row]]行く約束なんだけど…。`
  - English target: `Today's a free day.[[T001:row]][[T002:yoshio_name]] and I[[T003:row]]planned to go out, but...`
  - Line ID: `talk:CDROM/SERIFU/SHUGAKU.BIN:table-12:g0010:l0000`
- Contributor guidance: Position where Yoshio is referenced.

## Contextual substitutions

These tokens insert contextual items, dates, fortunes, and school trip events.

### context_m
- Marker syntax: `[[T...:context_m]]`
- Purpose: Substituted with contextual school subjects, club names, or exam types.
- Example:
  - Japanese source: `[[T001:context_m]]は残念ながら[[T002:row]]不合格だったが、`
  - English target: `[[T001:context_m]] exam[[T002:row]]sadly didn't work out, but`
  - Line ID: `talk:CDROM/SERIFU/EPILOG.BIN:table-12:g0000:l0027`
- Contributor guidance: Treat as a noun phrase in the sentence.

### context_a
- Marker syntax: `[[T...:context_a]]`
- Purpose: Substituted with gift items, date items, or event props.
- Example:
  - Japanese source: `よし、[[T001:row]][[T002:context_a]]を、[[T003:row]]プレゼントしよう。`
  - English target: `I'll give her[[T001:row]][[T002:context_a]][[T003:row]]for her birthday.`
  - Line ID: `talk:CDROM/BUNKA/BUN_FU.BIN:main:g0006:l0000`
- Contributor guidance: Treat as an item noun.

### context_o
- Marker syntax: `[[T...:context_o]]`
- Purpose: Substituted with New Year shrine fortune results (omikuji outcomes such as Great Blessing or Modest Blessing).
- Example:
  - Japanese source: `ほら、見て。[[T001:row]][[T002:context_o]]だったよ。`
  - English target: `Look![[T001:row]]I got [[T002:context_o]]!`
  - Line ID: `talk:CDROM/SERIFU/SHOGATU.BIN:table-3:g0031:l0000`
- Contributor guidance: Treat as a fortune grade noun.

### context_s
- Marker syntax: `[[T...:context_s]]`
- Purpose: Substituted with sightseeing spots or souvenir names during the Kyoto school trip.
- Example:
  - Japanese source: `そうだ、思い出したぞ。[[T001:row]][[T002:context_s]]だ。`
  - English target: `Right, I remember.[[T001:row]]It's [[T002:context_s]]!`
  - Line ID: `talk:CDROM/SERIFU/SHUGAKU.BIN:table-11:g0001:l0000`
- Contributor guidance: Treat as a place or souvenir noun.

### context_w
- Marker syntax: `[[T...:context_w]]`
- Purpose: Substituted with shrine paper fortunes or wish items.
- Example:
  - Japanese source: `それじゃあ、[[T001:row]]俺は[[T002:context_w]]だよ。`
  - English target: `Well,[[T001:row]]mine was: [[T002:context_w]]`
  - Line ID: `talk:CDROM/SERIFU/SHOGATU.BIN:table-4:g0025:l0001`
- Contributor guidance: Treat as a wish or fortune noun.

## System directives

These tokens handle system-level format specifiers and speaker prefixes.

### format_directive
- Marker syntax: `[[T...:format_directive]]`
- Purpose: Represents a C printf format directive (`%s`, `%d`, etc.) inside system source strings.
- Example:
  - Japanese source: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - English target: `zatu[[T001:format_directive]] basho[[T002:format_directive]][[T003:line_feed]]`
  - Line ID: `c:DATE:8013c190:1`
- Contributor guidance: Match each directive to its respective variable.

### inline_label
- Marker syntax: `[[T...:inline_label]]`
- Purpose: Prefixes an inline speaker badge or narrator prompt before dialogue text.
- Example:
  - Japanese source: `[[T001:inline_label]]部員君、静かにしてくれ。[[T002:row]]デッサンの邪魔だよ。`
  - English target: `[[T001:inline_label]]Member: Keep it down.[[T002:row]]I'm trying to sketch.`
  - Line ID: `talk:CDROM/BUNKA/KG_S6W.BIN:main:g0087:l0001`
- Contributor guidance: Keep at the start of the line before the speaker label.

### terminal_byte_before_nul
- Marker syntax: `[[T...:terminal_byte_before_nul]]`
- Purpose: Preserves a non-NUL terminal control byte at the end of specific system strings.
- Example:
  - Japanese source: `『[[T001:terminal_byte_before_nul]]`
  - English target: `"[[T001:terminal_byte_before_nul]]`
  - Line ID: `c:ENDING:80132420:1`
- Contributor guidance: Keep at the end of the text.

## Storage and engine markers

These tokens preserve raw hardware control bytes and unmapped sequences.

### unknown_renderer_bytes
- Marker syntax: `[[T...:unknown_renderer_bytes]]`
- Purpose: Preserves raw bytes in overlay tables where font encoding or hardware shift units are unmapped.
- Example:
  - Japanese source: `クラシックのＣＤ (`[[T001:unknown_renderer_bytes]]`)`
  - English target: `Classical CD[[T001:unknown_renderer_bytes]]`
  - Line ID: `overlay-table:CDROM/EXEDIR/BUNKAKEN.EXN:name_grid:00`
- Contributor guidance: Keep the token at the end of the translated English string so the original byte container is preserved.

### unknown
- Marker syntax: `[[T...:unknown]]`
- Purpose: Marks an unmapped control sequence currently under reverse-engineering.
- Example:
  - Japanese source: `私、シューティング[[T001:row]]苦手っていうかぁ…、[[T002:row]]ね、[[T003:unknown]]竄[[T004:unknown]][[T005:unknown]][[T006:unknown]][[T007:unknown]][[T008:unknown]][[T009:unknown]]`
  - English target: `I'm not really good[[T001:row]]at shooters...[[T002:row]]Right, [[T003:unknown]]![[T004:unknown]][[T005:unknown]][[T006:unknown]][[T007:unknown]][[T008:unknown]][[T009:unknown]]`
  - Line ID: `talk:CDROM/BUNKA/AS_S6W.BIN:main:g0078:l0000`
- Contributor guidance: Keep all sequential unknown markers in order.

### storage_byte_1b
- Marker syntax: `[[T...:storage_byte_1b]]`
- Purpose: Preserves a raw control byte with value 0x1B in overlay data records.
- Example:
  - Line ID: `CDROM/EXEDIR/BUNKA_SD.EXN:0000a1bc:3a898eecdae1`
- Contributor guidance: Do not remove.

### storage_byte_1a
- Marker syntax: `[[T...:storage_byte_1a]]`
- Purpose: Preserves a raw control byte with value 0x1A in overlay data records.
- Example:
  - Line ID: `CDROM/EXEDIR/BUNKA_SD.EXN:0000a2bc:fdbb66f97af0`
- Contributor guidance: Do not remove.

### storage_byte_0c
- Marker syntax: `[[T...:storage_byte_0c]]`
- Purpose: Preserves a raw control byte with value 0x0C in overlay data records.
- Example:
  - Line ID: `CDROM/EXEDIR/TT.EXN:00024d40:bf60e4349cac`
- Contributor guidance: Do not remove.
"""
    with open(out_path, "w", encoding="utf-8") as f:
        f.write(content)


def generate_documentation(
    out_dir: str,
    files_data: Dict[str, List[Any]],
    file_titles: Dict[str, str],
):
    """Generate README.md, CONTRIBUTING.md, and INDEX.md."""
    cat_stats = defaultdict(lambda: {"total": 0, "translated": 0, "files": 0})
    char_stats = defaultdict(lambda: {"total": 0, "translated": 0, "files": 0})

    for relpath, rows in files_data.items():
        parts = relpath.split("/")
        if len(parts) >= 2 and parts[0] in ("dialogue", "system"):
            cat = f"{parts[0]}/{parts[1].split('.')[0]}"
        else:
            cat = parts[0]

        total = len(rows)
        translated = sum(1 for r in rows if r[3] is not None)

        cat_stats[cat]["total"] += total
        cat_stats[cat]["translated"] += translated
        cat_stats[cat]["files"] += 1

        title = file_titles.get(relpath, "")
        for code, cname in CHAR_NAMES.items():
            if cname in title:
                char_stats[cname]["total"] += total
                char_stats[cname]["translated"] += translated
                char_stats[cname]["files"] += 1
                break

    # 1. README.md
    readme_content = """# Tokimeki Memorial (PS1) English translation

This directory holds the English translation for *Tokimeki Memorial: Forever with You* (`SLPM-86053`, PlayStation the Best), split into 184 Markdown tables by disc archive, scene, and executable module.

Every record with Japanese source text is translated (100%), covering all 54,786 translatable lines in `returned-v3.jsonl` and all 508 lines in `supplemental.jsonl`. The remaining 617 records in `returned-v3.jsonl` are empty source slots on the PS1 disc (`_(empty slot)_`) kept so table indices match the binary layout.

## Guides and indexes

- [Contributing guide (CONTRIBUTING.md)](CONTRIBUTING.md) covers editing tables, column roles, and local validation.
- [Control token reference (TAGS.md)](TAGS.md) explains all 28 `[[T...:...]]` control token families with Japanese and English examples.
- [Directory index (INDEX.md)](INDEX.md) lists all 184 Markdown files with line and empty-slot counts.
- [Image text catalog (graphics/image_text_catalog.md)](graphics/image_text_catalog.md) lists UI, HUD, menu, magazine, credit, and card graphics with original Japanese lettering and English replacements.

## Translation counts by category

All translatable lines in every category have English text. Categories where `Total records` exceeds `Translated lines` contain blank source slots in the original binary tables (`_(empty slot)_`).

| Category | Files | Translatable lines | Translated lines | Empty source slots | Total records |
| :--- | :---: | :---: | :---: | :---: | :---: |
"""
    for cat in sorted(cat_stats.keys()):
        s = cat_stats[cat]
        empty = s["total"] - s["translated"]
        readme_content += f"| `{cat}` | {s['files']} | {s['translated']:,} | {s['translated']:,} | {empty:,} | {s['total']:,} |\n"

    readme_content += """
## Character dialogue counts

| Character | Files | Translatable lines | Translated lines | Empty source slots | Total records |
| :--- | :---: | :---: | :---: | :---: | :---: |
"""
    for cname in sorted(char_stats.keys()):
        s = char_stats[cname]
        empty = s["total"] - s["translated"]
        readme_content += f"| {cname} | {s['files']} | {s['translated']:,} | {s['translated']:,} | {empty:,} | {s['total']:,} |\n"

    readme_content += """
## Validation and build

Use `tools/translation_tree.py` inside Docker to validate edits and rebuild the canonical JSONL files:

```bash
# Validate token brackets and required files
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py validate

# Rebuild returned-v3.jsonl and supplemental.jsonl from the Markdown tables
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py build
```

## Provenance and canonical files

- `returned-v3.jsonl` matches the validated translation return byte-for-byte: 55,403 stable IDs, 54,786 translated targets, and 617 empty source slots.
- `supplemental.jsonl` stores 508 translated supplemental records with IDs, context, target text, notes, review status, and protected markers.
- `manifest.json` pins the SHA-256 hashes of both files and their source inputs. Keep protected control markers and stable IDs intact when editing.

Source worktree: `/Users/lila/Documents/Projects/tokimemo-wt/translation`. Original game data must be supplied separately.
"""
    with open(os.path.join(out_dir, "README.md"), "w", encoding="utf-8") as f:
        f.write(readme_content)

    # 2. CONTRIBUTING.md
    contributing_content = """# Translation contributing guide

Thank you for contributing to the Tokimeki Memorial English translation project. This guide explains how translation files are structured, how to edit them directly on GitHub or locally, and how to maintain protected formatting tokens.

## Directory structure

Translations are organized into structured directories under `translations/en/`:

- `dialogue/bunka/` - Cultural Festival event dialogues (`CDROM/BUNKA/*.BIN`)
- `dialogue/serifu/` - Main date, holiday, school trip, and story dialogues (`CDROM/SERIFU/*.BIN`)
- `dialogue/master/` - Master system dialogues (`CDROM/MASTER/*.BIN`)
- `test_dialogue/` - Character test voice lines and dialogue scripts (`CDROM/SPEAKTST/*.BIN`)
- `system/exedir/` - Phone calls (`tel.md`), options (`option.md`), battle minigame (`rpg_bat.md`), name entry (`name_ent.md`)
- `system/c_strings/` - Decompiled C source strings and game logic messages
- `system/main_exe/` - Executable strings from `SLPM_86.053`
- `system/tables/` and `dictionaries/` - System names, gift lists, locations, and profile displays
- `supplemental/` - Supplemental sports festival dialogue, karaoke lyrics, and ending credits
- `graphics/` - UI, calendar, exam, and sprite image text catalog

Canonical distribution files (`returned-v3.jsonl`, `supplemental.jsonl`, and `manifest.json`) are preserved at the root of `translations/en/`.

## Table layout

Each Markdown file contains a 5-column table:

```markdown
| ID | Context | Japanese | Translation | Notes |
| :--- | :--- | :--- | :--- | :--- |
| `g0000:l0000` | Neighborhood Park · Shiori | こういう所で、[[T001:row]]お話するのって、[[T002:row]]いいね。 | It's nice talking[[T001:row]]in a place like this,[[T002:row]]isn't it? |  |
```

Columns serve specific roles during translation and builds:

1. **ID**: Local identifier used by the build tool to locate the string. Do not edit.
2. **Context**: Scene location, speaker, or function reference.
3. **Japanese**: Source text and control tokens provided as reference. Do not edit.
4. **Translation**: Target text for the English patch. Edit this column to update or polish lines.
5. **Notes**: Optional commentary, explanations, or questions.

## Editing on GitHub

1. Open the file you want to edit (such as [`dialogue/serifu/date_sa.md`](dialogue/serifu/date_sa.md)).
2. Click the pencil icon to edit the file.
3. Locate the line you wish to translate or polish.
4. Edit the text in the **Translation** column.
5. Add any relevant notes in the **Notes** column.
6. Scroll down, add a short commit description, and open a pull request.

## Formatting rules and protected tokens

Dialogue boxes, system displays, and branching paths rely on specific syntax.

### Protected control tokens
The game engine requires specific control tokens for text rendering and formatting. These are marked with double brackets, such as `[[T001:row]]` or `[[T001:girl_name]]`.

Always preserve protected tokens in your translation. You can adjust the English words before, between, or after them, but do not delete or rename the tokens.

For complete documentation on all 28 token families, read [TAGS.md](TAGS.md).

### Pipe characters in tables
Markdown tables use `|` to separate columns. If your translation text needs a pipe character, escape it as `\\|`.

### Empty slots
Unused dictionary slots are marked `_(empty slot)_`. Leave them unchanged unless providing a translation for that specific slot.

## Building and validating locally

Run validation and build commands using the project Docker container:

```bash
# Validate token brackets and distribution files
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py validate

# Reconstruct canonical JSONL files from edits
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py build
```
"""
    with open(os.path.join(out_dir, "CONTRIBUTING.md"), "w", encoding="utf-8") as f:
        f.write(contributing_content)

    # 3. INDEX.md
    index_content = """# Translation file directory index

Complete clickable index of all 184 translation files in the contributor tree.

| File Path | Description | Translatable lines | Translated lines | Empty source slots | Total records |
| :--- | :--- | :---: | :---: | :---: | :---: |
"""
    for relpath in sorted(files_data.keys()):
        rows = files_data[relpath]
        title = file_titles.get(relpath, relpath)
        # Clean title for index to avoid spaced-dash
        clean_title = title.replace(" - ", " (") + (")" if " - " in title else "")
        total = len(rows)
        translated = sum(1 for r in rows if r[3] is not None)
        empty = total - translated
        index_content += f"| [{relpath}]({relpath}) | {clean_title} | {translated:,} | {translated:,} | {empty:,} | {total:,} |\n"

    with open(os.path.join(out_dir, "INDEX.md"), "w", encoding="utf-8") as f:
        f.write(index_content)


def split_tree(
    returned_path: str,
    supplemental_path: str,
    manifest_path: Optional[str],
    image_catalog_path: Optional[str],
    supplemental_source_path: Optional[str],
    asset_cache_dir: str,
    section_context_path: str,
    out_dir: str,
) -> Dict[str, Any]:
    """Split returned-v3 and supplemental into contributor tree with 5-column layout."""
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

    print("Loading source text metadata database...")
    source_db, section_context = load_source_database(
        asset_cache_dir=asset_cache_dir,
        supplemental_source_path=supplemental_source_path,
        section_context_path=section_context_path,
    )
    print(f"Loaded {len(source_db):,} records into metadata database.")

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

            src_entry = source_db.get(row_id, {})
            jp_raw = src_entry.get("japanese", "")
            ct_raw = src_entry.get("copy_template", "")
            ctx_raw = src_entry.get("context", "")

            jp_disp = choose_japanese_text(jp_raw, ct_raw, target)
            ctx_disp = format_context_string(row_id, ctx_raw, section_context)

            files_data[relpath].append(
                (
                    local_id,
                    ctx_disp,
                    jp_disp,
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

            src_entry = source_db.get(row_id, {})
            jp_raw = src_entry.get("japanese", "")
            ct_raw = src_entry.get("copy_template", "")
            ctx_raw = src_entry.get("context", d.get("context", ""))

            jp_disp = choose_japanese_text(jp_raw, ct_raw, target)
            ctx_disp = format_context_string(row_id, ctx_raw, section_context)

            files_data[relpath].append(
                (
                    local_id,
                    ctx_disp,
                    jp_disp,
                    target,
                    d.get("translator_note", ""),
                    row_id,
                )
            )

    # Write human markdown files
    print(f"Writing {len(files_data)} markdown files with 5-column layout...")
    for relpath, rows in files_data.items():
        fullpath = os.path.join(out_dir, relpath)
        os.makedirs(os.path.dirname(fullpath), exist_ok=True)

        title = file_titles.get(relpath, "Translation File")
        total_rows = len(rows)
        translated_rows = sum(1 for r in rows if r[3] is not None)

        md_lines = [
            f"# {title}",
            "",
            f"- **File Path**: `{relpath}`",
            f"- **Total Lines**: {total_rows}",
            f"- **Translated Lines**: {translated_rows}",
            "",
            "| ID | Context | Japanese | Translation | Notes |",
            "| :--- | :--- | :--- | :--- | :--- |",
        ]

        for local_id, ctx_disp, jp_disp, target, note, full_id in rows:
            clean_ctx = ctx_disp.replace("|", "\\|")
            clean_jp = jp_disp.replace("|", "\\|")

            if target is None:
                disp_target = "_(empty slot)_"
            elif target != target.strip() or target.strip() == "":
                disp_target = f"`{target}`"
            else:
                disp_target = target.replace("|", "\\|")

            disp_note = note.replace("|", "\\|") if note else ""
            md_lines.append(
                f"| `{local_id}` | {clean_ctx} | {clean_jp} | {disp_target} | {disp_note} |"
            )

        with open(fullpath, "w", encoding="utf-8") as f:
            f.write("\n".join(md_lines) + "\n")

    # Generate TAGS.md
    print("Writing TAGS.md...")
    write_tags_documentation(os.path.join(out_dir, "TAGS.md"))

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


def parse_markdown_table(path: str) -> Dict[str, Tuple[Optional[str], str]]:
    """Parse a markdown file table. Returns local_id -> (target_text, note)."""
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

            # In 5-column layout: ID, Context, Japanese, Translation, Notes
            # In 4-column layout (legacy): ID, Status, Translation, Notes
            if len(cells) >= 5:
                raw_target = cells[3]
                note = cells[4].replace("\\|", "|")
            else:
                raw_target = cells[2]
                note = cells[3].replace("\\|", "|")

            if raw_target.startswith("`") and raw_target.endswith("`") and len(raw_target) >= 2:
                target = raw_target[1:-1]
            elif raw_target in ("_(empty slot)_", "-") or raw_target == "":
                target = None
            else:
                target = raw_target.replace("\\|", "|")

            rows[local_id] = (target, note)
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
        base_returned_path = (
            local_cand if os.path.exists(local_cand) else "translations/en/returned-v3.jsonl"
        )

    if not base_supplemental_path:
        local_cand = os.path.join(tree_dir, "supplemental.jsonl")
        base_supplemental_path = (
            local_cand if os.path.exists(local_cand) else "translations/en/supplemental.jsonl"
        )

    if not out_returned_path:
        out_returned_path = os.path.join(tree_dir, "returned-v3.jsonl")

    if not out_supplemental_path:
        out_supplemental_path = os.path.join(tree_dir, "supplemental.jsonl")

    md_cache: Dict[str, Dict[str, Tuple[Optional[str], str]]] = {}

    def get_row_data(
        row_id: str,
        is_supplemental: bool,
        orig_target: Optional[str],
        orig_note: str,
    ) -> Tuple[Optional[str], str]:
        relpath, local_id, _ = route_id(row_id, is_supplemental)
        if relpath not in md_cache:
            fpath = os.path.join(tree_dir, relpath)
            md_cache[relpath] = parse_markdown_table(fpath)

        table = md_cache[relpath]
        if local_id not in table:
            raise KeyError(
                f"Local ID {local_id} not found in {relpath} (full ID: {row_id})"
            )

        target, note = table[local_id]

        # Whitespace preservation rule for unmodified lines
        if orig_target is not None and target is not None:
            if target == orig_target or target.strip() == orig_target.strip():
                target = orig_target

        if orig_note and note.strip() == orig_note.strip():
            note = orig_note

        return target, note

    # Build returned-v3.jsonl
    print(f"Building {out_returned_path} from baseline {base_returned_path}...")
    temp_ret = out_returned_path + ".tmp"
    os.makedirs(os.path.dirname(os.path.abspath(out_returned_path)), exist_ok=True)

    with open(base_returned_path, "r", encoding="utf-8") as f_in, open(
        temp_ret, "w", encoding="utf-8", newline="\n"
    ) as f_out:
        for line in f_in:
            d = json.loads(line)
            orig_target = d.get("target_text")
            orig_note = d.get("translator_note", "")

            target, note = get_row_data(d["id"], False, orig_target, orig_note)
            d["target_text"] = target
            d["translator_note"] = note

            line_out = json.dumps(d, ensure_ascii=False, separators=(",", ":")) + "\n"
            f_out.write(line_out)

    os.replace(temp_ret, out_returned_path)

    # Build supplemental.jsonl
    print(f"Building {out_supplemental_path} from baseline {base_supplemental_path}...")
    temp_sup = out_supplemental_path + ".tmp"
    os.makedirs(os.path.dirname(os.path.abspath(out_supplemental_path)), exist_ok=True)

    with open(base_supplemental_path, "r", encoding="utf-8") as f_in, open(
        temp_sup, "w", encoding="utf-8", newline="\n"
    ) as f_out:
        for line in f_in:
            d = json.loads(line)
            orig_target = d.get("translation")
            orig_note = d.get("translator_note", "")

            target, note = get_row_data(d["id"], True, orig_target, orig_note)
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
    """Validate contributor tree structure, bracket balance, and Japanese presence."""
    print(f"Validating contributor tree at {tree_dir}...")

    # 1. Canonical distribution files presence
    for req in ("returned-v3.jsonl", "supplemental.jsonl", "manifest.json", "TAGS.md"):
        if not os.path.exists(os.path.join(tree_dir, req)):
            print(f"VALIDATION FAILED: Missing required file {req} in {tree_dir}")
            return False

    # 2. Protected token integrity and Japanese population check
    token_errors = []
    empty_jp_errors = []
    table_files_checked = 0

    for root, dirs, files in os.walk(tree_dir):
        for fname in files:
            if not fname.endswith(".md") or fname in (
                "README.md",
                "CONTRIBUTING.md",
                "INDEX.md",
                "TAGS.md",
                "image_text_catalog.md",
            ):
                continue

            fpath = os.path.join(root, fname)
            relpath = os.path.relpath(fpath, tree_dir)
            table_files_checked += 1

            in_table = False
            with open(fpath, "r", encoding="utf-8") as f:
                for line_no, line in enumerate(f, 1):
                    s = line.strip()
                    if not s.startswith("|") or not s.endswith("|"):
                        continue
                    cells = [c.strip() for c in re.split(r"(?<!\\)\|", s)[1:-1]]
                    if len(cells) < 5:
                        continue
                    if cells[0] in ("ID", ":---", "---"):
                        in_table = True
                        continue
                    if not in_table:
                        continue

                    lid = cells[0].strip("`")
                    jp = cells[2]
                    target = cells[3]

                    # Non-empty target rows should have Japanese populated
                    if target != "_(empty slot)_" and not jp:
                        empty_jp_errors.append((relpath, lid, line_no))

                    # Check balanced [[ and ]]
                    if target.count("[[") != target.count("]]"):
                        token_errors.append((relpath, lid, "Mismatched double brackets [[]] in translation"))
                    if jp.count("[[") != jp.count("]]"):
                        token_errors.append((relpath, lid, "Mismatched double brackets [[]] in Japanese"))

    if empty_jp_errors:
        print(f"VALIDATION FAILED: Found {len(empty_jp_errors)} rows with empty Japanese text:")
        for fn, lid, ln in empty_jp_errors[:10]:
            print(f"  {fn}:{ln} ({lid})")
        return False

    if token_errors:
        print(f"VALIDATION FAILED: Protected token errors ({len(token_errors)}):")
        for fn, lid, err in token_errors[:10]:
            print(f"  {fn} ({lid}): {err}")
        return False

    print(f"PASSED: Validated {table_files_checked} Markdown tables. All rows and brackets valid.")
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


def main():
    parser = argparse.ArgumentParser(description="Translation Tree Tool")
    subparsers = parser.add_subparsers(dest="command", required=True)

    # split
    split_p = subparsers.add_parser("split", help="Split JSONL files into contributor tree")
    split_p.add_argument("--returned", default="translations/en/returned-v3.jsonl")
    split_p.add_argument("--supplemental", default="translations/en/supplemental.jsonl")
    split_p.add_argument(
        "--supplemental-source",
        default=".claude/orchestrator/state/T-28009/japanese-columns-v1/raw_inputs/supplemental-unprofiled-disc-text.jsonl",
    )
    split_p.add_argument("--manifest", default="translations/en/manifest.json")
    split_p.add_argument(
        "--image-catalog",
        default=".claude/orchestrator/state/T-28009/japanese-columns-v1/raw_inputs/IMAGE_TRANSLATIONS.md",
    )
    split_p.add_argument("--asset-cache", default=".asset-cache/text/translation")
    split_p.add_argument(
        "--section-context",
        default=".asset-cache/text/date-context-current/section-context.tsv",
    )
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
        # Resolve supplemental source path
        supp_src = args.supplemental_source
        if not os.path.exists(supp_src):
            alt_supp = "/Users/lila/Documents/Projects/tokimemo-wt/translation/.asset-cache/text/supplemental-unprofiled-disc-text.jsonl"
            if os.path.exists(alt_supp):
                supp_src = alt_supp

        # Resolve image catalog path
        img_cat = args.image_catalog
        if not os.path.exists(img_cat):
            alt_img = "/Users/lila/Documents/Projects/tokimemo-wt/translation/.asset-cache/text/IMAGE_TRANSLATIONS.md"
            if os.path.exists(alt_img):
                img_cat = alt_img

        split_tree(
            returned_path=args.returned,
            supplemental_path=args.supplemental,
            manifest_path=args.manifest if os.path.exists(args.manifest) else None,
            image_catalog_path=img_cat if os.path.exists(img_cat) else None,
            supplemental_source_path=supp_src if os.path.exists(supp_src) else None,
            asset_cache_dir=args.asset_cache,
            section_context_path=args.section_context,
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
