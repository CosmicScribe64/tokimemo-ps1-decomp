# Translation Contributing Guide

Thank you for contributing to the Tokimeki Memorial English translation project! This guide explains how translation files are structured, how to edit them directly on GitHub or locally, and how to maintain protected formatting tokens.

---

## 1. Directory Structure

The translations are organized into intuitive directories under `translations/en/`:

- `dialogue/bunka/` — Cultural Festival event dialogues (`CDROM/BUNKA/*.BIN`)
- `dialogue/serifu/` — Main date, holiday, school trip, and story dialogues (`CDROM/SERIFU/*.BIN`)
- `dialogue/master/` — Master system dialogues (`CDROM/MASTER/*.BIN`)
- `test_dialogue/` — Character test voice lines and dialogue scripts (`CDROM/SPEAKTST/*.BIN`)
- `system/exedir/` — Phone calls (`tel.md`), options (`option.md`), battle minigame (`rpg_bat.md`), name entry (`name_ent.md`)
- `system/c_strings/` — Decompiled C source strings and game logic messages
- `system/main_exe/` — Executable strings from `SLPM_86.053`
- `system/tables/` & `dictionaries/` — System names, gift lists, locations, and profile displays
- `supplemental/` — Supplemental sports festival dialogue, karaoke lyrics, and ending credits
- `graphics/` — Target-only catalog of UI, calendar, exam, and sprite image text

Canonical distribution files (`returned-v3.jsonl`, `supplemental.jsonl`, and `manifest.json`) are preserved at the root of `translations/en/`.

---

## 2. How to Edit on GitHub

1. Navigate to the file you want to edit (e.g. [`dialogue/serifu/date_sa.md`](dialogue/serifu/date_sa.md)).
2. Click the **Pencil icon** (Edit this file) in the top-right corner of the file viewer.
3. Locate the line you wish to translate or polish.
4. Edit the **Translation** column.
5. Set the **Status** column to `ready` or your reviewer tag.
6. Scroll down, add a short commit description (e.g., `Polish Saki date dialogue lines`), and open a Pull Request!

---

## 3. Formatting Rules & Protected Tokens

### Protected Control Tokens
The game engine requires specific control tokens for text rendering and formatting. These are marked with double brackets:

- `[[T001:row]]` — Row line break in dialogue textboxes.
- `[[T001:sub:player_family]]` — Runtime substitution for the player's last name.
- `[[T001:sub:current_girl_short_name]]` — Runtime substitution for the girl's given name.
- `[[T001:format_directive]]` / `[[T002:line_feed]]` — C sprintf format directives and newlines.

> [!IMPORTANT]
> **Always preserve protected tokens** (`[[T...]]`) in your translation. You can adjust the English words before, between, or after them, but do not delete or rename the tokens!

### Pipe Characters in Tables
Markdown tables use `|` to separate columns. If your translation text needs a pipe character, escape it as `\|`.

### Empty Slots
Unused dictionary slots are marked `_(empty slot)_`. Leave them unchanged unless providing a translation for that specific slot.

---

## 4. Copyright Discipline

In accordance with repository policies:
- **Never paste Japanese source text** into files in `translations/en/`.
- Only English target translations, stable IDs, notes, and layout instructions may be committed.

---

## 5. Building & Validating Locally

Run validation and build commands using the project Docker container:

```bash
# Validate zero Japanese characters and protected tokens
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py validate

# Reconstruct canonical JSONL files
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py build
```
