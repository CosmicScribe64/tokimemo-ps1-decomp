---
id: T-27030
title: Phase 2 naming and port spec for overlay NAME_ENT (file screens, name entry, prologue, opening)
status: In Review
assignee: p2a-o-nameent (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/o-nameent]]"]
---

## Goal

Roadmap phase 2 (readable code), port-first scope, for the unit `src/ovl/NAME_ENT`: name the game logic, state and data tables of the game-start scenes (file screens, name and birthday entry, prologue, attract opening), name the hardware layer only at module level, and write the scene-flow spec the Godot port needs. This pass writes tables and wiki pages only; no source, header or symbol file is edited.

## Acceptance criteria

- [x] `config/renames/proposals/o-nameent.tsv`: one row per rename (kind scope old new note); all 257 functions of the overlay are covered (255 rows; `prologue_line_start` and `prologue_line_run` keep their names); 228 data rows (224 overlay, 4 main), 144 const, 5 file, 3 type, 4 field (639 rows in all); 20 rows start with HYPOTHESIS; the file starts with the same header comment as `core.tsv`.
- [x] `wiki/spec/o-nameent.md`: modes and entry points, state machine of each mode (stage and step tables), state and data with the new names, rules (formulas, thresholds, conditions), connections, open questions; linked from `wiki/spec/index.md`.
- [x] Emulator checks of the main claims (mode sequence 0x13, 0x14, 0x15, 0x34; the file screens on a blank card; the new game setup values; the SELECT chord preset); states under `build/emu/states/` stay ignored.
- [x] No game text, assets, asm or screenshots committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Code review against `CODING_STANDARDS.md` (docs-only change) and merge of the table by the phase 2 merge step.

## Notes

- Mode map (the core table's `MODE_NAME_ENT_3`/`_4` are settled here): 0x11 attract opening (`MODE_OPENING`), 0x13 file screens (`MODE_FILE_SELECT`), 0x14 name and birthday entry (`MODE_NAME_ENTRY`), 0x15 voiced prologue (`MODE_PROLOGUE`). The core names for 0x11 and 0x13 are overridden by const rows.
- Prefixes: the ADR gives `name_entry_` and `prologue_`; this table adds `file_select_` (mode 0x13, the largest part: 104 functions) and `opening_` (mode 0x11) and says so in the spec and in the open questions. The audit's `name_ent_` is superseded by the ADR. Folder `src/ovl/name_entry/`, four files: `name_entry.c`, `file_select.c`, `opening.c`, `prologue.c`.
- Method: read all four C files (9,900 lines) with the main names of the other phase 2 tables applied to a scratch copy; decoded the data tables from the overlay image; mapped every CD read constant to its disc file by content search (names only); ran the emulator for the title, file screens, name entry and the prologue (screenshots and states stay in `build/emu/`).
- Results of note: the mode 0x13 stage 2 step machine has 47 step values (list, albums without a system file, empty card, New, Copy, Clear); the player's profile and the heroine Shiori's birthday and blood type are asked in two screens; Shiori's club is `(day * 3 + month) % 11`, her start scores come from a 4 x 4 blood compatibility table (45/10/10 to 55/25/0), the paired clubs of Mio, Yuina, Ayako and Saki depend on it; four cheat names are checked; the prologue is 43 lines in 22 stages with two answers built from the entered Shiori data.
- Main-side data first named here (scope `main`): 0x800B3644 (blank character), 0x800B3A3C and 0x800B394C (per-slot voice clip pairs), 0x800E8BEF (an album record index byte nothing writes). `D_800B3DB0` (a flag the prologue sets for the tile upload) is left to h-graphics.
- Field renames touch other units' files: `Work800E7D10.flags` and `NameRecordF00.flags` become `girl_clear_count` (ETC, OPTION and EN_NICHI use the same field).
- Dead or odd code recorded as implemented: `file_select_scan` counts `slot_status[0..13]` only; the icon, menu and voice mode restores never take effect; `name_entry_birthday_buttons_update` reads two unassigned locals; steps 0xA6 and 0xA7 have identical bodies; some data words are never read (`file_select_unused_word_*`, an unused clip pair).

## Comments
