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

### 2026-11-01 worker agent: second fix round (reviewer pass 2 and owner decisions)

Checkpoint `T39410-menus-fixes-v1` updated in place (same candidate ids, regenerated PNGs, manifests, ledgers, `REGISTER.md`; the reproduction test passes). Status stays In Review.

- Scavenger Race (owner decision: one name everywhere). `translations/en/supplemental/tai_tx.md` row `0030` and its `translations/en/supplemental.jsonl` record said `Scavenger Hunt`; both now say `Scavenger Race`. No other instance in `translations/en`. The SLPM Markdown table is gitignored, so `tools/translation_tree.py build` cannot run; the two files were edited by hand, every supplemental row was checked against its Markdown table (508 rows, 0 mismatches) and `validate` passes. The roundtrip hash pin in `tools/test_translation_tree.py` was updated. `translations/en/manifest.json` `supplemental_sha256` was already stale before this change and was left alone.
- kasetu-0 title: outlined capitals `SCAVENGER RACE` (one-texel paper strokes, dark outline), rows 41-54 and letter-spaced like the outlined Japanese 借り物競走説明.
- Name entry: new `name-label-abc.unlit` / `.lit` (56x16, and 32x16 in `fallback-v1`) for the English letter page, outline and colours of frames 15/16. One extra texel between H and i (and A-B, B-C): the (+1,+1) shadow had filled the gap. Fallback `Symb` is now `Sym`. `candidate_key` in `tools/translation_inventory.py` maps `name-label-<page>.<state>`; the ABC cells make one candidate-only atom `name_ent-abc` (inventory pins 271 -> 272 items updated).
- FILESEL `system-tab` region is `[17, 88, 42, 11]`. `tools/translation_filesel_indexed.py` reserves the rect, and the region map, the `T39004-filesel-v4` manifest (`page_regions_sha256`, edit regions) and ledger `manifest_sha256` were regenerated from the tool; PNG outputs are byte-identical. `FileSelectPageTests` failed because they still pointed at the superseded `T39004-filesel-v3` checkpoint, built from an older region map; they now test v4 and also prove the shifted System page needs, and fits, the widened region.
- STP (G1): every output was checked against the source `.stp.png` mask. STP is clear on every texel of every menu source except the KASETU card, where it is set on every texel. No source or output has opaque black; every English colour has a single STP value in its source; transparent = word 0. The build now fails on any breach and records the result per item (`REGISTER.md` section 9). ALL_C keeps source indices only (none has STP set).
- Nits: the widened option sprites 0103/0104 (72) and 0113/0114 (83) keep one clear column after the shadow. The singular case (`1 points`) is noted for the code side in `REGISTER.md`; no art change.
- Catalog rows 92, 97, 109 and 123 edited in `image_text_catalog.jsonl` and `.md` (widths, `Sym`, outlined title). `translation_inventory.json` not regenerated (integrator).

### 2026-11-01 worker agent: round 3, birthday panel headers

The final reviewer found `Birth day` (and `Birth month`) as grid headers under the `Birthday:` field of the development-v1 name panel 0001. New candidate `name-panel-0001` (menus-fixes-v1) re-letters them `Month` / `Day` in the same native font, ink, shadow and origins; it supersedes `name-reviewed-development-v1/name-panel-0001`. Panels 0000, 0002 and 0014 have no such duplicate and keep their development-v1 drafts. Catalog row 106 edited to match. The superseded draft had a bounded in-game agent review, so the inventory pins moved (agent-reviewed revisions 4 -> 3, superseded records 239 -> 240). Status stays In Review.

### 2026-11-01 worker agent: code review fixes

Findings from the code review of `tools/translation_menus_fixes.py` and the checkpoint; all 50 PNGs are byte-identical after the rebuild.

- Original pixels (must-fix). `manifest.json` said `committed_files: English crops and cells only; no original pixel`, `REGISTER.md` said "no original pixel", and both ledgers said `original_images_included: false`. That was wrong: `kasetu-0.png` (320x240), `filesel-page-information-0.png`, `filesel-frame-19/20`, `name-panel-0001.png`, the title frames and the label cells keep original art outside their edit regions (`kasetu-0`: 71,928 of 76,800 opaque texels are the source's). Now both ledgers say `original_images_included: true` with `source_pixels_note`; each asset, fallback and ALL_C crop records `source_pixels`; the manifest `committed_files` and `REGISTER.md` section 10 state the truth with a per-file table. No file is re-masked.
- Basis: the owner asked for translated assets to be kept in the repository (`translations/en/graphics/artwork/checkpoints/T39004-2026-10-29/README.md`, [[tickets/T-39004-english-asset-completion]]), and the whole-frame FILESEL, sports and name-entry drafts this checkpoint supersedes were committed the same way (the T39004-filesel-v4 ledger already carries a `source_pixels_note` for them). See also the counter-example in [[tickets/T-39410-mainrom-art-fixes]].
- Tool: `publish` copies only the files the manifest lists, each checked against its hash, through a `.partial` folder (no `copytree`); ledgers use `encoded()`; function-local imports moved to the top; unused `sys` removed. Tests: `assertGreater`-style section checks instead of a line count of `REGISTER.md`, comments on the pinned 33 candidates, a publish test, and the reproduction test also compares `fallback-ledger.json` and both manifests.
- `test_committed_inventory_matches_inputs` pins the ledger hash and fails until `translation_inventory.json` is regenerated after the merge.
