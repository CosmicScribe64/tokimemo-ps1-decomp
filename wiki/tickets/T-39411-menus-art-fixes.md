---
id: T-39411
title: Fix the menu and screen English art after the owner and reviewer pass
status: In Review
assignee: worker agent
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39400-finish-missing-english-art]]", "[[tickets/T-39004-english-asset-completion]]", "[[assets-media]]"]
---

## Goal

Redraw or correct the nine menu and screen art items the owner and the reviewer flagged (title screen, all-clear banner, exam suffix, option cast list, name-entry selector labels, save-screen buttons and System tab, sports card title), with the approved names, in one checkpoint the integrator can register.

## Acceptance criteria

- [x] title-0001..0004 and catalog-127 are re-lettered bold at the Japanese height with the original decoration.
- [x] examine-0001 reads `points`; the option cast list uses the approved spellings and full names; name-entry labels are full words, one revision per item, lit and unlit alike.
- [x] filesel-0020 names are undistorted with the kanji margin and nothing in the bevel rows; filesel-0019 uses the approved stat names; the System tab has a left margin; the sports card says `Scavenger Race`.
- [x] `checkpoints/T39410-menus-fixes-v1` holds the PNGs, `ledger.json` with `supersedes`, and `REGISTER.md` listing every integrator change (catalog wording and bindings, widths, region edits).
- [x] `tools/translation_menus_fixes.py` rebuilds every output byte for byte; `tools/test_translation_menus_fixes.py` passes.
- [ ] Code review against `CODING_STANDARDS.md`.

## Notes

Shared registration files (`tools/translation_inventory.py`, `translation_inventory.json`, the catalog, `inventory_decisions.json`, `filesel_page_regions.json`) were not edited; see `REGISTER.md`.

## Comments

### 2026-11-01 worker agent: drafts made, awaiting review

The ledger loads in the inventory with 39 supersedes and the inventory builds with it. Previews: original | English at 5-10x for every changed variant. Drafts are unreviewed and port-made; nothing is approved or integrated.
