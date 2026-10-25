# Tokimeki Memorial (PS1) — English Translation Corpus

Welcome to the contributor-friendly translation workspace for the matching decompilation of **Tokimeki Memorial: Forever with You** (PlayStation 1, `SLPM-86053`).

This directory contains the entire English translation split into intuitive, human-editable, GitHub-browsable Markdown files organized by game disc archive, scene, and system module.

- **Total Spoken & System Lines**: 55,403 lines
- **Supplemental Disc Text**: 508 lines
- **Target-Only Image Catalog**: Complete HUD, menus, magazines, credits & cards
- **Format**: Clean GitHub Markdown tables with stable IDs and protected control tokens
- **Lossless Build**: Reproduces canonical JSONL distribution byte-for-byte

---

## Quick Links

- [**Contributing Guide (CONTRIBUTING.md)**](CONTRIBUTING.md) — How to edit lines on GitHub or locally, control token syntax (`[[T001:row]]`), and review statuses.
- [**Master Directory Index (INDEX.md)**](INDEX.md) — Complete clickable directory of all 184 translation files with line counts and status.
- [**Image Text Catalog (graphics/image_text_catalog.md)**](graphics/image_text_catalog.md) — Comprehensive target-only catalog of all UI and graphic text.

---

## Translation Progress by Category

| Category | Files | Total Lines | Translated | % Done | Ready for Review |
| :--- | :---: | :---: | :---: | :---: | :---: |
| `dialogue/bunka` | 46 | 12,914 | 12,914 | 100.0% | 11,795 |
| `dialogue/master` | 8 | 815 | 815 | 100.0% | 785 |
| `dialogue/serifu` | 44 | 17,122 | 16,819 | 98.2% | 16,819 |
| `supplemental` | 9 | 508 | 508 | 100.0% | 508 |
| `system/c_strings` | 26 | 2,200 | 2,200 | 100.0% | 994 |
| `system/dictionaries` | 4 | 482 | 475 | 98.5% | 475 |
| `system/exedir` | 25 | 3,591 | 3,591 | 100.0% | 124 |
| `system/main_exe` | 1 | 673 | 673 | 100.0% | 72 |
| `system/misc` | 1 | 31 | 31 | 100.0% | 0 |
| `system/tables` | 7 | 587 | 557 | 94.9% | 0 |
| `system/views` | 1 | 286 | 286 | 100.0% | 282 |
| `test_dialogue` | 12 | 16,702 | 16,425 | 98.3% | 0 |

---

## Character Dialogue Breakdown

| Heroine / Character | Files | Total Lines | Translated | Ready for Review |
| :--- | :---: | :---: | :---: | :---: |
| **Asahina Yuko** | 6 | 2,667 | 2,640 | 1,132 |
| **Fujisaki Shiori** | 11 | 6,641 | 6,627 | 4,148 |
| **Himoo Yuina** | 7 | 3,244 | 3,211 | 1,065 |
| **Kagami Mira** | 6 | 2,689 | 2,662 | 1,524 |
| **Katagiri Ayako** | 9 | 4,244 | 4,225 | 2,797 |
| **Kisaragi Mio** | 8 | 3,489 | 3,446 | 1,990 |
| **Kiyokawa Nozomi** | 6 | 3,241 | 3,200 | 1,464 |
| **Koshiki Yukari** | 6 | 2,678 | 2,656 | 1,364 |
| **Mikihara Megumi** | 6 | 2,391 | 2,347 | 1,193 |
| **Nijino Saki** | 6 | 2,798 | 2,782 | 1,262 |
| **Other Characters** | 2 | 742 | 739 | 190 |
| **Saotome Yumi** | 6 | 2,682 | 2,654 | 1,516 |

---

## Tooling & Verification

The contributor tree is managed and verified by `tools/translation_tree.py`:

```bash
# Validate zero Japanese source text and protected token integrity
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py validate

# Reconstruct canonical JSONL files from markdown edits
docker run --rm -v $(pwd):/repo -w /repo tokimemo-decomp-arm64:latest python3 tools/translation_tree.py build
```

---

## Provenance & Canonical JSONL Files

- `returned-v3.jsonl` is byte-identical to the independently validated return: 55,403 stable IDs, 54,786 nonempty targets and 617 empty targets. It contains targets, notes, markers and source identity; no extracted Japanese source-text field.
- `supplemental.jsonl` preserves all 508 supplemental rows' IDs, context, targets, notes, review status and markers. The extracted `japanese` and `copy_template` fields are excluded. These rows still need original-byte provenance and coverage review.
- `manifest.json` pins both files and their original input identity. Protected control markers and stable IDs must remain intact. There is no original textbox length cap.

Source worktree: `/Users/lila/Documents/Projects/tokimemo-wt/translation`. Original game data must be supplied separately. Runtime language selection and translated image creation remain later integration work.
