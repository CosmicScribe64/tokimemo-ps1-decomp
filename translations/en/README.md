# Tokimeki Memorial (PS1) English translation

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
| `dialogue/bunka` | 46 | 12,914 | 12,914 | 0 | 12,914 |
| `dialogue/master` | 8 | 815 | 815 | 0 | 815 |
| `dialogue/serifu` | 44 | 16,819 | 16,819 | 303 | 17,122 |
| `supplemental` | 9 | 508 | 508 | 0 | 508 |
| `system/c_strings` | 26 | 2,200 | 2,200 | 0 | 2,200 |
| `system/dictionaries` | 4 | 475 | 475 | 7 | 482 |
| `system/exedir` | 25 | 3,591 | 3,591 | 0 | 3,591 |
| `system/main_exe` | 1 | 673 | 673 | 0 | 673 |
| `system/misc` | 1 | 31 | 31 | 0 | 31 |
| `system/tables` | 7 | 557 | 557 | 30 | 587 |
| `system/views` | 1 | 286 | 286 | 0 | 286 |
| `test_dialogue` | 12 | 16,425 | 16,425 | 277 | 16,702 |

## Character dialogue counts

| Character | Files | Translatable lines | Translated lines | Empty source slots | Total records |
| :--- | :---: | :---: | :---: | :---: | :---: |
| Asahina Yuko | 6 | 2,640 | 2,640 | 27 | 2,667 |
| Fujisaki Shiori | 11 | 6,627 | 6,627 | 14 | 6,641 |
| Himoo Yuina | 7 | 3,211 | 3,211 | 33 | 3,244 |
| Kagami Mira | 6 | 2,662 | 2,662 | 27 | 2,689 |
| Katagiri Ayako | 9 | 4,225 | 4,225 | 19 | 4,244 |
| Kisaragi Mio | 8 | 3,446 | 3,446 | 43 | 3,489 |
| Kiyokawa Nozomi | 6 | 3,200 | 3,200 | 41 | 3,241 |
| Koshiki Yukari | 6 | 2,656 | 2,656 | 22 | 2,678 |
| Mikihara Megumi | 6 | 2,347 | 2,347 | 44 | 2,391 |
| Nijino Saki | 6 | 2,782 | 2,782 | 16 | 2,798 |
| Other Characters | 2 | 739 | 739 | 3 | 742 |
| Saotome Yumi | 6 | 2,654 | 2,654 | 28 | 2,682 |

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
