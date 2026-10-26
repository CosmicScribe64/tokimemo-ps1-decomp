# Translation contributing guide

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
Markdown tables use `|` to separate columns. If your translation text needs a pipe character, escape it as `\|`.

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
