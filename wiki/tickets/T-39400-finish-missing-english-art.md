---
id: T-39400
title: Finish the missing English art
status: Done
assignee: worker agent
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39004-english-asset-completion]]", "[[assets-media]]"]
---

## Goal

Make English drafts for the 21 art items the inventory lists as missing, so the English patch can replace every Japanese sign and label the owner wants translated.

## Acceptance criteria

- [x] The nine MAINROM genre tags (frames 0x134, 0x135, 0x138, 0x139, 0x141-0x145) are registered in `tools/translation_inventory.py` from the existing port-made checkpoint, and the inventory counts them as drafts.
- [x] The twelve BS sign stills have corrected file bindings in the image text catalog, a size proven from the decoded stream or the calling code, and an English draft drawn in each sign's style (eleven of twelve; catalog-142, the ride sign, is cut off by the frame edge and stays missing, see the comment).
- [x] The drafts are recorded in a checkpoint ledger with source and output hashes, and the inventory and translation tests pass.
- [x] No original game image is committed; only the English drafts, as with the existing artwork.

## Comments

### 2026-11-01 worker agent: drafts made, awaiting review

**MAINROM tags.** `T39004-mainrom-port-made-v1` is in `LEDGERS`. The nine tag overrides in `inventory_decisions.json` now read candidate-only with the reasons of `decisions/mainrom-port-made.json`; the old "new glyphs not allowed" wording is gone. The ledger also holds six "Apr 4" month-name composites of frame 60, a form the owner did not choose, so `UNREGISTERED` in `tools/translation_inventory.py` leaves them out (the date panel captions stay the only frame-60 candidates).

**BS stills.** `tools/translation_bs_signs.py` and the skeleton font `tools/translation_bs_font.py` draw the lettering; checkpoint `T39400-bs-signs-v1` has 17 crops. Sizes: `dec_bg_cd_read(lba, 1)` reads ten sectors and `dec_bg_decode_to_vram(1)` decodes 320x160, so every still read that way has 200 macroblocks, and each stream confirms it (end marker, zero bytes after it). LBAs come from the back-to-back layout from 0x3F03, which reproduces eleven LBAs the game code uses (P_01_02 at 0x4954 included). P_01_02 is 640x160 from `prologue_board_load`. Proof per file is in the manifest `geometry_proofs`.

**Catalog bindings fixed** (jsonl and md, record in `decisions/bs-signs.json`): 135 D_01_67 to D_01_69, 136 to D_01_70, 137 to D_01_72 (art club), 141 to D_29_09, 142 to D_11_11 (Suspected), 143 to D_15_03, 144 to D_19_02, 145 to D_24_03 (TICKET plaque 20x10, not the planned TICKETS), 149 to I_17_01. Container hashes refreshed with the new `containers` op.

**Left out.** catalog-142: the ride sign runs off the right edge of D_11_11 and only a few katakana are inside, which do not match the catalog wording. Not changed: catalog-135 (science-club plaque, deliberately untranslated by default D-3, now bound to its real file D_01_69; a one-line follow-up if the owner wants it), the gate pillar plates, P_01_02 re-encode (the T-39012 ring and white-wrap findings still apply to the patch step).

**Inventory** (before to after): artwork missing 21 to 1, candidate-only 169 to 189, english-reviewed 52 and deliberately-untranslated 29 unchanged; candidate records 360 to 424 (distinct 351 to 415), human-approved 53 unchanged.

**Gates** (Docker, `tokimemo-recovery-splat`): `test_translation_*` discovery 254 run, same two errors as main (`test_translation_tree` `test_edit_propagation` and `test_lossless_roundtrip`, missing `slpm_86_053.md`); main runs 231 tests with the same two. New: 21 `test_translation_bs_signs` tests and 2 inventory tests, all pass. `translation_inventory.py check` passes; `wiki.py lint` 0 broken links, 0 mismatches (the 2 large pages exist on main).

Drafts are unreviewed and port-made; nothing is approved or integrated. Code review against `CODING_STANDARDS.md` is still to do before Done.

### 2026-11-01 code-review fixes (T-39412 pass)

`tools/translation_bs_signs.py` shares its helpers with the stills tool through `tools/translation_bs_common.py`. Caller
citations name the C function and the call text; line numbers stay in the manifest for a human reader only. The
`original_pixels: 0` literal is replaced by the computed `crop.source_equal_pixels` (0 to 4126 per crop: a fitted plane
or diffusion face reproduces flat original colours; nothing is copied) and a `crop.fill` statement; the class-mark
removals now say "Laplace (diffusion) fill". `fit_plane` reports the sample count of its final coefficients. The 17
crops kept their bytes; manifest and ledger of `T39400-bs-signs-v1` were republished (Pillow / numpy versions recorded).

- 2026-11-01: Code review against CODING_STANDARDS.md passed on the second look (all ten findings resolved; remaining nits recorded in the log). Three independent side-by-side art reviews ended with no must-fix or should-fix left. Done.
