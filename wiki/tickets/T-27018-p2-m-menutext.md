---
id: T-27018
title: Phase 2 naming proposals and port spec for main text, menus and girl queries (m-menutext)
status: In Review
assignee: claude
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/m-menutext]]"]
---

## Goal

Roadmap phase 2 (readable code), unit `m-menutext`: name every function, owned global and relied-on field of `src/main/8004E500.c` (the kanji text system, 25 functions), `src/main/8004F6B0.c` (the mode 0xFE VRAM viewer, 1 function) and `src/main/8004F870.c` (menus, choices, speaker labels, name builders, girl score and state queries, 35 functions), and write the port spec. Output only: a rename table and wiki pages. No edits to `src/`, `include/` or symbol files.

## Acceptance criteria

- [x] `config/renames/proposals/m-menutext.tsv` has one row per rename (kinds func, data, field, file, const, comment); core names (`p2a-core`) are used and not duplicated.
- [x] Uncertain game-meaning claims are marked `HYPOTHESIS:` first in the note column.
- [x] O.BIN-named functions kept, each with an English comment row (core covers 22 of the 45, this table the other 23).
- [x] `wiki/spec/m-menutext.md` covers the text list and typewriter, speaker plates and dialogue blocks, the menu engine and cursor modes, girl scores, ranks, closeness and points, name builders, birthday and club helpers, connections, quirks, evidence and open questions; linked from `wiki/spec/index.md`.
- [x] Emulator probes confirm layouts and rules where the code alone left doubt (spec section 15).
- [ ] Review of the table and spec by the phase-2 reviewer.

## Notes

- 150 rows: 10 func, 53 data (10 of them alias labels that splat made inside tables), 3 field, 5 file (3 sources, 2 new headers), 56 const, 23 comment. 1 note starts with `HYPOTHESIS:` (`closeness_grid`).
- Prefixes follow the ADR main map: `text_` for the text module (O.BIN `k_*` and `set_kanji_string` kept), `menu_`, `speaker_` (O.BIN `gnsx`, `sndisp`, `sndi` kept) and `girl_` for the menu and girl module. File proposals: `src/main/text_system.c`, `src/main/debug_vram_view.c`, `src/main/menu_speaker_girl.c`; headers `include/main/text.h`, `include/main/menu.h`.
- Names adopted from the m-scheduler spec (section 16): `girl_scores`, `girl_points_shiori`, `girl_points_heroines`, `player_in_sports_club`.
- Existing non-O.BIN names changed: field `pos` to `revealed` (abbreviation), `glyph_expand_1bpp` to `text_glyph_expand`, `font_glyphs` and `font_range_table` to `text_font_glyphs` and `text_font_range_table` (module prefix), `text_list_count` to `text_state` (it is the whole TextState object).
- Findings worth knowing: a text screen is a 52-entry list with consecutive glyph slots, so `k_sub_reset` is a rewind of the glyph allocator; a dialogue block is a run of entries (`text_first_line` .. `text_lines_total` in core's names, really first index and one-past-last index) and scrolls only when it has four or more lines; `sndi` expands name codes (the player form always comes from the current girl, not the speaker); `get_g_zyotai_s` and `_h` disagree by design; girls 13 and 14 score as girls 0 and 1; `girl_scores` fills 16 closeness slots from 11 inputs; slot 15 of the short-name table is girl 10's given name; the closeness grid is 5 x min(affection bucket, max(3, tokimeki bucket)) with small deviations.
- Emulator evidence (`build/emu` is not committed): title-menu save state; probes of the menu items at the title, the text list in NAME_ENT, a dialogue block (mode byte poked to 0x44), the six-line scroll, the settings menu navigation (mode 0x34 stage 4, menu modes 1 and 3, disabled item skipping, wrap, SELECT), voice gating (club-joining message, voice modes 1 and 2), the name builders (TEL lines through the overlay loader: all 16 ids, both affection states) and the score pipeline (mode 0x30 with girl records and parameters poked). A Python model of the tokimeki, affection, closeness and points formulas reproduced the real code's 11 + 11 values in two scenarios.
- The kanban card is not added by this pass (the orchestrator does it); `tools/wiki.py lint` reports the status mismatch until then. `rename.py plan` on the 66 func, data and field rows: 0 problems, 45 files would change (the tool does not read comment, const or file rows of this table format).
- Disagreements with core are in the spec section 16 (names of `+0xF60`/`+0xF61`, `TextEntry.extra` and `attr`, three comment rows), not as second rows.

## Comments
