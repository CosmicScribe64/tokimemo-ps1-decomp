---
id: T-39502
title: Add the English text runtime behind a build flag
status: In Review
assignee: claude
created: 2026-11-01
updated: 2026-11-01
links: ["[[tickets/T-39500-english-translation-patch]]"]
---

## Goal

An English build of the decomp, selected by a flag, with a proportional Latin font, a text encoding for English and pixel-width layout, while the default build still matches 27/27.

## Acceptance criteria

- [x] `configure.py` English variant builds the executable and overlays into a separate output folder without the SHA-1 gate; the default build still prints OK for all 27.
- [x] English encoding and proportional font designed, documented and implemented in `text_system.c`; size and memory limits checked at build time.
- [x] One English string shown correctly in the emulator (the title menu labels are sprite art, so the first string is the file select memory-card message, one screen after Game Start).

## Notes

Part of [[tickets/T-39500-english-translation-patch]]. Design, function list and open risks: [[english-text-runtime]].

## Comments

- 2026-11-01 (claude): Implemented the English variant. `ninja english` builds the main exe and all 26 overlays into `build/en/`:
  - Original addresses are pinned. The island after the main bss is loaded by `src/english/boot.s`. `load_string`, `set_kanji_string` and `disp_string` are hooked to `src/english/text.c` (single-byte ASCII plus Shift-JIS, per-glyph pen positions).
  - The proportional font `translations/en/font/latin12.txt` has 105 glyphs and goes through `tools/english_font.py`. Post-link checks (`tools/english_link.py`): no original symbol moves, island ≤ 0x80132000, exe ≤ 0xBC000, overlays ≤ 0x30000 with the checksum rewritten.
  - First English text: the NAME_ENT file select memory-card message. In the emulator it renders correctly (screenshot `build/en/shots/file_select_memory_card_check.png`, local only).
  - The default build prints OK for all 27 binaries, plus headers, symbols, globals and renames.
  - Awaiting the CODING_STANDARDS review.
