---
id: T-39508
title: Put the English art on the disc
status: In Review
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39507-english-disc-build]]", "[[english-build]]", "tools/patch/art/", "tools/test_patch_art.py"]
---

## Goal

The English disc build writes every registered English image into its container on the disc: the indexed NBN/TIM sheets (MAINROM, TITLE, OPTION, FILESEL, NAME_ENT and the others), with their palettes and semi-transparency bits, and the MDEC background stills, re-encoded. Sprites the English art widened get the code and texture-page changes they need in the English build.

## Acceptance criteria

- [x] Every artwork item with a registered English candidate is written by `tools/patch/build_english.py`; the report lists each item as placed, deferred (with the reason) or untranslated by decision. (All 272 items; 230 placed and verified on the disc, 14 deferred with reasons.)
- [x] Indexed images round-trip byte-exact with the original pixels; English pixels use only colours (and STP values) the container's palette allows.
- [x] Stills are re-encoded to fit their read size or moved, and decode back to the intended image within the codec's error. (14 stills; 2 deferred, see Notes.)
- [ ] The emulator route on the English disc shows the English art at the title, file select, name entry, the magazine, the opening stills and the sports card. (Title, options, file select, name-entry labels, stat bar, prologue stills shown; the magazine screen and the sports card are not on the route.)
- [x] The default build still matches 27/27.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Art sources: `translations/en/graphics/translation_inventory.json` and the checkpoints under `translations/en/graphics/artwork/checkpoints/`.

Implementation and results: [[english-build]] section Art.

- Modules: `tools/patch/art/` (`items.py` registered art, `indexed.py` views and in-place plan, `placer.py` repack, `sheets.py` container profiles, `sprites.py`, `stills.py`, `build.py` plan and report, `hook.py` build hooks, `verify.py` read-back check). `build_english.py` gained three calls and `--no-art`. `tools/Dockerfile.patch` adds numpy and Pillow.
- No C change: widened sprites are drawn by rewritten part records (OPTION frame records are in the file). The name-entry selector cells and the exam suffix ship their fallbacks (fixed positions in code, no texture room); Kiyokawa and Tatebayashi ship the family-name fallbacks (no room on the OPTION pages).
- Verification: `verify art` reads every written container back from the built track; a placed image that does not match stops the build.
- Route `build/emu/route-en-art/` (149 shots), compare `build/emu/compare-route-jp-route-en-art/compare.html`.

## Comments
