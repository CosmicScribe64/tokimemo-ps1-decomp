# Translation contributing guide

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

Markdown tables use `|` as a column separator. Escape literal pipe characters inside a cell as `\|`.

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
