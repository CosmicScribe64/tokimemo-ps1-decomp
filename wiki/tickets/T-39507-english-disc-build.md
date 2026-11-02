---
id: T-39507
title: Build the English disc and patch in one command
status: In Review
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

One command builds the English game and disc: the English exe and overlays, every text container written with the English encoder and build-time word wrap, CD read sizes and LBAs updated in the English build, files relocated on the disc, then the patched track, cue and xdelta patch.

## Acceptance criteria

- [x] English encoder over the T-39503 writers, emitting the control bytes of [[english-text-runtime]], with word wrap from the font metrics and each window's pixel width.
- [x] C literals and CD read sizes and LBAs change only in the English build; the default build still matches 27/27.
- [x] The disc boots in the emulator and shows English on the file select and opening screens. The title menu labels stay Japanese (see Notes).
- [x] A report lists every record that could not be placed or wrapped.

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Design, steps and results: [[english-build]].

Done (2026-11-01), branch `patch/build`:

- Command: `IMG=tokimemo-patch tools/docker.sh python3 -m tools.patch.build_english`, 222 s. Outputs go to `build/en/disc/`: track 1, cue, xdelta (752,337 bytes) and `report.json`. Track 1 SHA-1 c140f667cc7c..., the same in two runs.
- Encoder and wrap: `tools/patch/text/english.py`, `windows.py`, `wrap.py` and `layout.py`. Windows proven from the code: message speech 196 px, narration 252 px, choice 224 px, telephone 196 px. The message and telephone windows scroll, so no page break is needed; the telephone box holds at most 6 rows. 0 unmapped characters, 0 unwrappable records, and 1 record got an added break.
- C literals: generated into `build/en/src` and compiled by the new `cctext` rule (`tools/cc.py --source-text`). A fit loop keeps the pinned link working: 1,467 literals in 169 files written, 734 in 25 files left for the relink.
- CD: `cd_read_sectors`/`_nowait` are hooked to island versions (`src/english/cd.c`) that remap reads through a table generated from the disc layout (`tools/patch/cd_layout.py`). No call site changes, and the BUNKA tour tables and variable LBAs are covered. 22 talk files moved (+41 sectors). The 10 `DATE_*.BIN` files need 26-31 sectors against the 24-sector DATE buffer and keep their original bytes (4,718 records) until [[tickets/T-39505-english-memory-and-relink]] maps the RAM.
- Records: placed 30,540; file_kept_original 4,718; needs_relink 1,018 (734 C literals, plus 49 main and 235 overlay in-place records over capacity); kept_original 340 (empty target); moved_in_build 201; overflow 8 (JOHO.NBN).
- `tools/english_link.py ld` now also pins the section labels. Without it, BUNKAKEN failed the overlay check when its last rodata object shrank.
- `c_literals.locate_sites` finds a literal that moved since the export by its bytes. `file_select.c` changed under T-39502.
- Tests: `tools/test_patch_english.py`, 24 synthetic tests plus the disc-gated end-to-end test (`TOKIMEMO_ENGLISH_DISC=1`, 229 s). All pass, and so does `tools/test_patch_text.py`. Default build 27/27 OK.
- Emulator: `tools/emu/run.sh route run --label en --build jp --cue build/en/disc/tokimemo-en.cue` ran to the end (reason `done`, 30,061 frames, 145 screenshots, 9 min 16 s). The game boots, loads the moved files and plays through the prologue, week 1, the phone call, save, the date and load.
  - File select shows the English message, and the prologue and week 1 dialogue show English words.
  - Dialogue is garbled as expected: the old parser treats the spaces as row ends, and the control bytes are not read yet. This waits on [[tickets/T-39506-english-script-runtime]].
  - The title menu and the command screen labels stay Japanese. They come from relink records or art.
  - The date dialogue is Japanese because its DATE file was kept.
  - Screenshots: `build/emu/route-en/` (e.g. `016_file_select_slot_1.png`, `030_prologue_02.png`, `112_phone_answer_01.png`). The Japanese baseline is in the patch-emu worktree's `build/emu/route-jp/`.

## Comments
