---
id: T-39508
title: Put the English art on the disc
status: Done
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]", "[[tickets/T-39507-english-disc-build]]", "[[english-build]]", "tools/patch/art/", "tools/test_patch_art.py"]
---

## Goal

The English disc build writes every registered English image into its container on the disc: the indexed NBN/TIM sheets (MAINROM, TITLE, OPTION, FILESEL, NAME_ENT and the others), with their palettes and semi-transparency bits, and the MDEC background stills, re-encoded. Sprites the English art widened get the code and texture-page changes they need in the English build.

## Acceptance criteria

- [x] Every artwork item with a registered English candidate is written by `tools/patch/build_english.py`; the report lists each item as placed, deferred (with the reason) or untranslated by decision. (Published build: 252 verified items, 28 explicit exceptions, zero deferred or missing.)
- [x] Indexed images round-trip byte-exact with the original pixels; English pixels use only colours (and STP values) the container's palette allows.
- [x] Stills are re-encoded to fit their read size or moved, and decode back to the intended image within the codec's error. (All 16 translated stills verified; zero deferred.)
- [x] Emulator coverage shows the English art at the title, file select, name entry, the magazine, opening stills and sports cards. The magazine and sports cards have separate controlled runs, documented below.
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

2026-11-02: inspected all five sports instruction cards on `build/phone-width-proof`, each in a controlled 1,800-frame GPU run with zero original-font activity. The English instructions fit. Both three-legged-race variants showed an untranslated speech bubble beside the animated character. T-39521 tracks this newly found sprite omission; the art ticket remains open. Captures are under `build/emu/sports-card-runner`, `sports-card-spoon`, `sports-card-ball`, `sports-card-pair-0` and `sports-card-pair-1`.

- 2026-11-02 Codex: D_03_08 also passes the unchanged production gates with the analytic basis (lambda 32, ring weight 50): ring maximum 3, float disagreement 2, English mean absolute error 1.0286, 16,734 bytes within its original nine sectors. The alternative chroma-weight experiment reduced letter fidelity and was not adopted. Full rebuild is recomputing all stills. A new `tools/emu/routes/holiday-magazine.route` opens the magazine normally from `holiday_1`; `build/emu/route-en-magazine-v2` completes with a full-page capture and clean return (11 s). The first issue's English labels and dialogue are legible; the Japanese masthead is an approved exception. This proves the first issue only, not every later magazine issue.

- 2026-11-02 Codex: corrected the MDEC least-squares basis to the unclipped orthonormal response; the previous level-40 hardware-model probe saturates at coarse quantizers. Synthetic small-impulse regression passes at quantizers 6 and 12; 16 related tests pass. P_01_02 probe now fits 18,432 bytes with exact unedited macroblocks, maximum ring change 3 RGB555 steps and float/fixed disagreement 2, inside unchanged existing gates. Its English 1-A/B/C headings look legible in the decoded candidate. D_03_08's first new attempt misses the ring gate at only pixel (237,71), six steps versus the allowed four; further settings and chroma-cell weighting are being tested. No gate relaxed.

- 2026-11-02 Codex: narrowed-banner replay completed at frame 23,116 in 383 s through the end of the prologue. Output has 77 captures including copied pre-resume checkpoints; final banner proof is `build/emu/route-en-banner-fit/046_prologue_18.png`. Code saved in 7d54bc082.

- 2026-11-02 Codex: GPU review caught the portrait covering the end of the first banner draft. Narrowed IJUIN to 124 pixels; `build/emu/route-en-banner-fit/preview-046.png` now shows all five letters clear of the hair. The original-width build completed the full 33,649-frame route through save/load (146 captures, 743 s). The revised banner build has byte-identical main and NAME_ENT binaries and is replaying from the compatible prologue checkpoint. Final target SHA-1 `a0098b5c0568ebfe46f67cb3324ebb236d08b047`; all readback and xdelta gates pass. Source review against CODING_STANDARDS found no C/default-build changes, committed game data, unchecked palette additions, or writes outside the declared banner regions. Ticket remains In Review with compressed stills and off-route coverage unresolved.

- 2026-11-01 Codex: added a source-hash-bound, code-generated Ijuin Rei banner for `CDROM/PROLODIR/CUT_10.NBN`, omitted from the original inventory. The writer preserves file size, palette, and pixels outside two lettering regions, using original gold bands and port-made block capitals. Final-disc readback passed in `build/banner-proof` (track-one SHA-1 prefix `51ef02b48683`); fresh GPU review is running in `build/emu/route-en-banner`. The movie reconciles into the art report only after raw-extent readback. Catalog-142 now records the explicit off-frame-sign exception in T-39500 instead of calling it missing. Focused art/movie suite: 44 tests pass, one opt-in full-build skip. Two compressed stills and off-route screen coverage remain open.

- 2026-11-02 Codex: first full analytic-basis build fixes both deferred signs but makes D_19_02 four bytes too large. Added a fallback search seed using the previous level-40 response only after all analytic attempts fail, with unchanged size/pixel/decode checks. Fresh museum result is 16,370 bytes in 16,384 allocated, ring 3, float disagreement 1. Fourteen unchanged successful analytic results were revalidated and reused with cache-key provenance in `build/still-cache-migration.json`; final disc assembly/readback is running. Focused suite: 58 tests pass, one opt-in movie skip. Controlled tournament capture `build/emu/club-placard-english/shot_800.png` shows the English placard clearly; zero original-font uploads (including zero unknown/overflow). Full main route with font instrumentation remains in progress. Source review against CODING_STANDARDS found no C/default-build changes, raw-source writes, gate relaxation or staged game data; final visual coverage remains open.

- 2026-11-02 Codex: final `build/still-fallback-proof` passes disc readback and xdelta validation in 161.7 s. Target SHA-1 e00ffdf0b44b94ad97168b425c1ce564dad04b9f, patch 6,533,072 bytes: 245 verified items, 28 approved exceptions, zero deferred/missing. The museum decode at its manifest-proven 320x160 size is legible; GPU coverage remains outstanding. Historical deferred counts above are superseded by this result.

- 2026-11-02 Codex: fresh full route with font tracing completes through save/load (33,649 frames, 750 s, 146 captures). Reviewed `031_prologue_03.png`: English class heading visible, original tiny roster retained by approved exception. Tournament placard is separately GPU-verified. All 32 art containers compare identical between the route disc and final fallback disc except the off-route museum D_19_02. Museum and remaining off-route visual checks are still open. Final focused rerun: 69 tests, four skips (original-draft fixtures unavailable and full-movie opt-in).

- 2026-11-02 Codex correction: D_19_02 is the skating-rink entrance, not a museum: source LBA 0x456C is loaded by `date_place_setup_skating_rink`, `date_scene_yuko_winter_background_2`, and `date_scene_megumi_winter_background_1`. A controlled winter date fixture is running for GPU proof. Earlier museum wording describes this same asset and is superseded.

2026-11-02: T-39516 repairs two omitted school-name plates beside the existing festival banners. `build/festival-plates-proof` verifies 248 art placements and 28 exceptions; actual-disc comparison changes only D_01_47.BS and D_01_48.BS. Both festival GPU captures show English banners and plates, with zero original-font activity over 1,000 frames each. Inventory retention prose was stale and is now corrected. Broader decoded-pixel audit remains open; clean trace counts do not prove sprite text coverage.

T-39521 repairs all four omitted sports bubbles. The latest disc verifies 252 placed images with 28 approved exceptions. Both partner variants were inspected in the emulator; a longer cycle confirms the readiness label and both direction labels. The start-call bubble has decoded-pixel and disc-readback evidence only. No clipping, overlap or damage to neighboring artwork was found.

## Published artwork quality pass

Inspected all 254 shipped sprite target images, using the actual placement and fallback selections, and all 16 translated still containers decoded directly from the current disc. Container hashes match the published build report. No new clipping, overlap or damaged neighboring artwork was found. Texture-page fragments and highlight components were assessed with their assembled runtime examples; they are not standalone screens. `translations/en/reviews/shipped-art-quality-2026-11-02.json` records image hashes, current container identity and the 31 review sheets. Movie, generated prologue banner and retained-pixel evidence remain separate from this pass.

2026-11-02 reconciliation: re-opened the first magazine page and prologue banner captures; the English remains legible, with the approved magazine masthead retained. Current-disc comparison preserves the banner and movie bytes. The 25 art tests pass. The published report records 252 verified items, 28 explicit exceptions and zero deferred or missing items, replacing the historical totals above. Source-review closeout remains separate from these completed visual checks.

## Completed source review

2026-11-02: reviewed art planning, sprite selection and fallback handling, executable frame relocation, still encoding and the final-disc verifier against CODING_STANDARDS. Grown frames stay within the reserved block and their on-disc pointers are checked. Still candidates must fit their original sectors and pass surrounding-pixel and decoder checks; sprite targets and unaffected views are compared after placement. Generated game-derived outputs remain ignored. The 25-test art suite passes, and the published report contains no deferred or missing item. Existing generator-specific reviews cover the later signs, banner and sports additions. No unresolved code finding remains for the registered assets. This technical/artwork sign-off does not claim every possible scene composition was played. Ticket and board move to Done.
