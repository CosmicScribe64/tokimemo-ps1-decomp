---
id: T-39506
title: Make dialogue, choices, names and name entry work in English
status: Done
assignee: claude
created: 2026-11-01
updated: 2026-11-02
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

Under `TOKIMEMO_ENGLISH`, the script and name code handles the English encoding from [[english-text-runtime]]: the dialogue parser, choices, speaker labels, name substitution and honorifics, and name entry with an English letter page (A-Z, a-z) shown by default and single-byte names of up to 8 letters in the existing buffers.

## Acceptance criteria

- [x] `talk_render`, `talk_speak_part`, `make_three_select`, `sndisp`/`sndi`, `gnsx`, `get_g_name`/`get_p_name` and the punctuation check handle English control bytes and single-byte text.
- [x] Name entry opens on the English letter page; kana pages still work; names are stored single-byte and drawn in the Latin font (list of the draw sites still to do in [[english-text-runtime]]).
- [x] A hand-encoded English dialogue page with a name, a choice and a page break shows correctly in the emulator.
- [x] The default build still matches 27/27.

## Notes

Part of [[tickets/T-39500-english-translation-patch]].

Done (2026-11-01), details in [[english-text-runtime]] ("Script parsers", "Names and name entry"):
- `src/english/script.c`: hooks for `talk_render`, `talk_speak_part`, `make_three_select`, `talk_names_set`, `gnsx`, `sndisp`, `sndi`, `get_g_name`, `get_p_name`; control bytes in `include/english/script.h` (adds inline labels 0x1C/0x1D ... 0x1F); pixel-width layout; 160-byte line buffers; honorifics from the translation records; long girl names kept in island buffers; strings still in the original format are converted on entry.
- `src/english/name_entry.c`: nine NAME_ENT hooks; letter page first and default; 7-letter names, 11-letter nickname; island imports from the default NAME_ENT build (`tools/english_imports.py`, `configure.py`).
- `src/english/text.c`: `english_text_width`, cell mode `english_text_cell`, empty entries get glyph count 0.
- `tools/emu/route.py`: `LATIN_ROWS` follows the real grid.
- Island: 20,160 bytes + bss to 0x80130730 (limit 0x80132000).

Verification: default `ninja` 27/27 OK; `ninja english` links. Emulator: the route (`tools/emu/run.sh route run --build en`) runs through to the load at the end on the English disc (test disc from `tools/patch/disc.py`: English exe, NAME_ENT, and a copy of `CDROM/SERIFU/G_BIRTH.BIN` with table-1 g0000 l0000-l0002 hand-encoded through `containers.build_containers` and `Image.write`, same file size).

Remaining: the draw sites listed in [[english-text-runtime]] that need overlay changes; the island imports must become NAME_ENT code once T-39505's free relink is in; the ABC label art (`name-label-abc`) at `english_tab_label_draw`; T-39507 must emit the control bytes (including labels and `nn`).

## Comments

2026-11-01 (Codex): integrated branch build with compact, independently allocated talk strings places all 134 data files and keeps zero whole files original. DATE dialogue fits its existing RAM buffer; no RAM relocation is needed. The eight JOHO label overflows still need the fixed-stride layout update. Visual checks and remaining NAME_ENT work are in progress.

2026-11-01 (Codex): JOHO labels and NAME_ENT panels/ABC are now placed; the fresh final-layout route verifies birthday digits and dialogue fixes through save/load. Nine overlong romanized presets still retain Japanese, so this ticket remains In Progress. Corrected frontmatter to match the board after wiki lint caught stale In Review status.

2026-11-02: pursuing full long-preset support without changing GameState/save field sizes or substituting shorter names. Proposed reversible encoding uses currently unused English-font single-byte codes for common two-letter sequences; renderer/width expand them, and name editing unpacks, edits one character, then repacks. Full two-byte Japanese characters must remain opaque so trail bytes cannot be mistaken for pairs. Persistent pair assignments will be explicit and stable. Validate all 45 presets, editing boundaries, script width/typewriter behavior, save/load, and original build matching before acceptance. This is implementation work under the existing ticket, not yet a completion claim.

2026-11-02: all 45 presets round-trip and fit with stable pair storage; largest family/given representation is seven bytes. C helpers and text load/width expansion are implemented; editor unpacks, edits one character and repacks, preserving full Shift-JIS pairs. Pixel caps are 56 for family/given and 88 for nickname, and build-time name width reservations were increased. Eight synthetic tests pass, including native execution of production C encoder/decoder/renderer loops and overflow sentinel checks. Full English build is running; default checksum, actual selection/editing/save/load and visual route checks remain required.

2026-11-02: corrected entry geometry uses original family x, given x six pixels farther right, and a 72-pixel nickname cap (supersedes earlier 88-pixel editor-cap note; placeholder reservation remains 88). Fresh controller route completes editing and save/load, 171 captures/38,703 frames, zero original-font calls/unknown/overflow. Actual preset chord and final-disc audit verify full names. Save-summary overflow discovered during review is now fixed with bounded complete copies and adaptive rows; focused production-helper tests pass. New build/GPU check pending.

2026-11-02: save-summary helper now GPU-verified in `route-en-name-save-card/019_full_name_load_summary.png`. Fresh boot of the updated executable successfully loads and reloads a previous build's test card; full family/given are preserved, all details fit, and trace records zero original-font calls/unknown/overflow. Confirmation screen needed six pixels of extra family/given separation; final overlay build is running.

2026-11-02: final confirmation GPU route passes (6,548 frames/146 s/27 captures), visible family/given separation, zero original-font trace calls/unknown/overflow. Full-name change set reviewed against CODING_STANDARDS with no unresolved findings; default build matches all 27 binaries, globals and 14,449 rename rows pass. Final review disc f3af6fdd9c30c47ad930503bccf56d7bb55f04d5 and browser package are verified. Ticket remains In Progress for the documented date ambush short-name overflow and remaining runtime coverage.

2026-11-02: repaired the date ambush's long speaker-name copy using the existing bounded runtime setter. Calling the canonical choice initializer is equivalent to the old interior entry (it skips only a NOP). Relinker repair additionally preserves selected input-section symbol definitions instead of pinning their absolute side-script aliases, and ignores discarded-section references. Full disc `build/date-name-proof` passes build/readback/xdelta gates (SHA-1 prefix c24fd36a5851); 25 focused regressions pass and original DATE matches. Fresh bootstrap/ambush GPU check is pending because main overlay entry addresses changed.

2026-11-02: ambush GPU replay now verifies the full label and actual battle screen, zero original-font uploads/unknown/overflow. Samples show the original short field is safely terminated while its 24-byte backing buffer holds the complete name; the neighboring girl's name is intact. All 27 original binaries still match. The runtime fix is ready for review alongside the broader text/audit work.

## Final standards review

Completed the remaining review of the date ambush change in `src/ovl/date/place_scenes.c` and its bounded setter in `src/english/script.c`. The English-only branch stores the full speaker name in the 24-byte backing buffer and a terminated prefix in the original eight-byte field. The adjacent name remains intact. The named choice-table entry replaces an alternate entry that skipped only a NOP; the default branch retains its original call. C89, SDK declarations and existing state/save layouts are preserved.

The prior full-name review already covers all 45 presets, editing, confirmation, save/load and the persistent pair table. Rechecked the production pair encoder/decoder and text expansion; the 31 text/name regressions pass. All 27 default binaries currently match their configured hashes (`build/runtime-closeout.log`). The 6,500-frame ambush run records zero original-font calls, unknown calls or overflow calls. Its final dumps retain the complete long label, a terminated short field and the neighboring Shiori name; capture 2450 displays the full speaker label and capture 5900 reaches battle. The current disc audit and later layout reviews cover the published runtime. No unresolved source-review findings remain; broader text quality stays in T-39511.

## Recovered branch notes

These notes were left uncommitted on November 1. They preserve the earlier work and then-open findings; the completion entries above supersede their status and remaining-work list.

Round 2 (2026-11-01, on the free relink of T-39505), details in [[english-text-runtime]] and [[english-build]]:
- Name entry moved into NAME_ENT (`#ifdef` block in `src/ovl/name_entry/name_entry.c`, still hooked); `tools/english_imports.py` and its link edge are gone. The tab column is 32 px.
- 266 `needs_relink` -> 0: `tools/patch/text/retarget.py` moves 196 strings and re-points their relocations; pointer views under the flag for the fortune, wish, camp club, ending song and magazine tables; 27 slots under English C literals counted as moved in the build; the kana pages, reading labels, misread ETC LBA tables and 9 over-long name presets kept original.
- Draw sites: file select (name spacing, one system-file mark), label x by pixel width (3 overlays), Yumi's honorific, final exam space, long names and New Year values through bounded setters, prologue Rei title.
- Dialogue reflow judged on the Japanese source (`tools/patch/text/reflow.py`), balanced rows, inline labels encoded, layout report.
- The 340 `empty_target` records are all empty sources (no translation gap).

Open: JOHO.NBN labels (8 `overflow`), the second NAME_ENT texture page and ABC label art (asked by the art agent), page splitting for the 79 pages longer than their window (needs a runtime page control), "Delinquents" overflow in DATE `place_scenes.c` (the relink stops on an alternate entry when that object's text changes), 576 lone-word last rows.
