---
id: T-39506
title: Make dialogue, choices, names and name entry work in English
status: In Progress
assignee: claude
created: 2026-11-01
updated: 2026-11-01
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
