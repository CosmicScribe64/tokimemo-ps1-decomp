---
id: T-27022
title: Phase 2 naming and port spec for overlay DATE (dates)
status: In Review
assignee: p2a-o-date (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: [[[tickets/T-27000-phase-2]], [[spec/o-date]]]
---

## Goal

Roadmap phase 2 (readable code), port-first scope, for the unit `src/ovl/DATE` (668 functions, 19 source files): name the dating logic (stages, steps, places, answers, scores, birthdays, special date events), the state and the data tables, name the hardware layer only at module level, and write the scene-flow spec the Godot port needs. This pass writes tables and wiki pages only; no source, header or symbol file is edited.

## Acceptance criteria

- [x] `config/renames/proposals/o-date.tsv`: one row per rename (kind scope old new note); all 655 `func_` functions of the overlay named (13 patch-note names kept with comment rows, `date_shiori_dispatch` renamed), data labels, types, fields, files and constants (1,120 rows); rows with a game-meaning guess start with HYPOTHESIS; the file starts with the same header comment as `core.tsv`.
- [x] `wiki/spec/o-date.md`: stage machine, state and data with the new names, rules (formulas, thresholds, transitions), connections, verification log, open questions; linked from `wiki/spec/index.md`.
- [x] No game text, assets, asm or screenshots committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Code review against `CODING_STANDARDS.md` (docs-only change) and merge of the table by the phase 2 merge step.

## Notes

- Prefix: `date_` for functions and data (the overlay's established prefix: `date_event_trigger`, `date_fill_sections_<girl>`, `date_place_names`), `Date*` for types, `DATE_*` for constants; folder proposal `src/ovl/date/`. The special date events are `date_event_<girl>_<season>[_part]` (46 scenes), the places `date_place_setup_<place>` and `date_<place>_enter_spot`.
- Method: read the matched C of all 19 files and the 72 step tables in `asm/ovl/DATE/data`; per-function summaries (callers, table slots, calls) were generated to name the 300 mechanical step functions by pattern (background read, sound, data load, data init, setup, speak with face). About twenty emulator runs from a boot-to-title state with a scratch `--poke` option (kept in `build/emu`, not in the repo) confirmed the stage and step order, the meeting-place question, the score formula, the end-of-date bookkeeping, the birthday stages, the repeat-spot penalty, the ambush trigger, a special event reward and an amusement park trace (spec section 13).
- Names of main variables that only DATE uses are proposed with scope `main` (`date_place`, `date_choice_class`, ...); `D_80122CE0` is named `rpg_launch_code` (shared with RPG_BAT, whose table does not name it).
- Disagreement with `core`: `unk_724..727` are the chosen place, the true appointment place and two decoys, not "recent appointment places" (spec section 14).

## Comments
