---
id: T-39508
title: Put the English art on the disc
status: In Review
assignee: claude
created: 2026-11-01
updated: 2026-11-02
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

Round 2 (2026-11-01): merged `patch/english` (free relink, T-39505); the art step and the disc read-back pass on the relinked executable. Grown MAINROM frames in `english_hud_frames` (`src/english/hud_frames.c`) place the Save labels, the date-panel captions and every weekday: 235 placed and verified, 9 deferred. The still encoder softens strokes where the fixed-point IDCT overflows (P_01_02 and D_03_08 still fail). NAME_ENT panels and the 56-texel selector labels need a second name-entry texture page in `name_entry.c` (T-39506); the facts are in [[english-build]] section Art.

## Comments

- 2026-11-02 Codex: narrowed-banner replay completed at frame 23,116 in 383 s through the end of the prologue. Output has 77 captures including copied pre-resume checkpoints; final banner proof is `build/emu/route-en-banner-fit/046_prologue_18.png`. Code saved in 7d54bc082.

- 2026-11-02 Codex: GPU review caught the portrait covering the end of the first banner draft. Narrowed IJUIN to 124 pixels; `build/emu/route-en-banner-fit/preview-046.png` now shows all five letters clear of the hair. The original-width build completed the full 33,649-frame route through save/load (146 captures, 743 s). The revised banner build has byte-identical main and NAME_ENT binaries and is replaying from the compatible prologue checkpoint. Final target SHA-1 `a0098b5c0568ebfe46f67cb3324ebb236d08b047`; all readback and xdelta gates pass. Source review against CODING_STANDARDS found no C/default-build changes, committed game data, unchecked palette additions, or writes outside the declared banner regions. Ticket remains In Review with compressed stills and off-route coverage unresolved.

- 2026-11-01 Codex: added a source-hash-bound, code-generated Ijuin Rei banner for `CDROM/PROLODIR/CUT_10.NBN`, omitted from the original inventory. The writer preserves file size, palette, and pixels outside two lettering regions, using original gold bands and port-made block capitals. Final-disc readback passed in `build/banner-proof` (track-one SHA-1 prefix `51ef02b48683`); fresh GPU review is running in `build/emu/route-en-banner`. The movie reconciles into the art report only after raw-extent readback. Catalog-142 now records the explicit off-frame-sign exception in T-39500 instead of calling it missing. Focused art/movie suite: 44 tests pass, one opt-in full-build skip. Two compressed stills and off-route screen coverage remain open.
