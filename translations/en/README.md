# Tokimeki Memorial (PS1) English translation

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
| `dialogue/bunka` | 46 | 12,914 | 12,914 | 0 | 12,914 | 11,795 |
| `dialogue/master` | 8 | 815 | 815 | 0 | 815 | 785 |
| `dialogue/serifu` | 44 | 16,819 | 16,819 | 303 | 17,122 | 16,819 |
| `supplemental` | 9 | 508 | 508 | 0 | 508 | 508 |
| `system/c_strings` | 26 | 2,200 | 2,200 | 0 | 2,200 | 994 |
| `system/dictionaries` | 4 | 475 | 475 | 7 | 482 | 475 |
| `system/exedir` | 25 | 3,591 | 3,591 | 0 | 3,591 | 124 |
| `system/main_exe` | 1 | 673 | 673 | 0 | 673 | 72 |
| `system/misc` | 1 | 31 | 31 | 0 | 31 | 0 |
| `system/tables` | 7 | 557 | 557 | 30 | 587 | 0 |
| `system/views` | 1 | 286 | 286 | 0 | 286 | 282 |
| `test_dialogue` | 12 | 16,425 | 16,425 | 277 | 16,702 | 0 |

## Character dialogue counts

| Character | Files | Translatable lines | Translated lines | Empty source slots | Total records | Ready for review |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: |
| Asahina Yuko | 6 | 2,640 | 2,640 | 27 | 2,667 | 1,132 |
| Fujisaki Shiori | 11 | 6,627 | 6,627 | 14 | 6,641 | 4,148 |
| Himoo Yuina | 7 | 3,211 | 3,211 | 33 | 3,244 | 1,065 |
| Kagami Mira | 6 | 2,662 | 2,662 | 27 | 2,689 | 1,524 |
| Katagiri Ayako | 9 | 4,225 | 4,225 | 19 | 4,244 | 2,797 |
| Kisaragi Mio | 8 | 3,446 | 3,446 | 43 | 3,489 | 1,990 |
| Kiyokawa Nozomi | 6 | 3,200 | 3,200 | 41 | 3,241 | 1,464 |
| Koshiki Yukari | 6 | 2,656 | 2,656 | 22 | 2,678 | 1,364 |
| Mikihara Megumi | 6 | 2,347 | 2,347 | 44 | 2,391 | 1,193 |
| Nijino Saki | 6 | 2,782 | 2,782 | 16 | 2,798 | 1,262 |
| Other Characters | 2 | 739 | 739 | 3 | 742 | 190 |
| Saotome Yumi | 6 | 2,654 | 2,654 | 28 | 2,682 | 1,516 |

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
