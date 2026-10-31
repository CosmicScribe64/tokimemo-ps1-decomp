---
id: T-27022
title: Phase 2 naming and port spec for overlay DATE (dates)
status: Done
assignee: p2a-o-date (Sonnet)
created: 2026-10-24
updated: 2026-10-24
links: `[tickets/T-27000-phase-2`, `spec/o-date`]
---

## Goal

Roadmap phase 2 (readable code), port-first scope, for the unit `src/ovl/DATE` (668 functions, 19 source files): name the dating logic (stages, steps, places, answers, scores, birthdays, special date scenes), the state and the data tables, name the hardware layer only at module level, and write the scene-flow spec the Godot port needs. This pass writes tables and wiki pages only; no source, header or symbol file is edited.

## Acceptance criteria

- [x] `config/renames/proposals/o-date.tsv`: one row per rename (kind scope old new note); all 655 `func_` functions of the overlay named (11 patch-note names kept with comment rows; `date_event_trigger`, `date_event_choose` and `date_shiori_dispatch` renamed; the alias label `func_80132004` has a row), data labels, types, fields, files and constants (1,103 rows); the main-exe variables DATE names are 18 request rows in `config/renames/proposals/o-date-main-requests.tsv`; rows with a game-meaning guess start with HYPOTHESIS; the file starts with the same header comment as `core.tsv`.
- [x] `wiki/spec/o-date.md`: stage machine, state and data with the new names, rules (formulas, thresholds, transitions), connections, verification log, open questions; linked from `wiki/spec/index.md`.
- [x] No game text, assets, asm or screenshots committed; no edits under `src/`, `include/` or `config/symbol*` files.
- [ ] Code review against `CODING_STANDARDS.md` (docs-only change) and merge of the table by the phase 2 merge step.

## Notes

- Prefix: `date_` for functions and data (the overlay's established prefix: `date_fill_sections_<girl>`, `date_place_names`), `Date*` for types, `DATE_*` for constants; folder proposal `src/ovl/date/`. The special date scenes are `date_scene_<girl>_<season>[_part]` (48 scenes; the bare word `event` is a GLOSSARY trap for a scripted scene, review round 1), the places `date_place_setup_<place>` and `date_<place>_enter_spot`.
- Method: read the matched C of all 19 files and the 72 step tables in `asm/ovl/DATE/data`; per-function summaries (callers, table slots, calls) were generated to name the 300 mechanical step functions by pattern (background read, sound, data load, data init, setup, speak with face). About twenty emulator runs from a boot-to-title state with a scratch `--poke` option (kept in `build/emu`, not in the repo; the addresses and values of every run are in spec section 13) confirmed the stage and step order, the meeting-place question, the score formula, the end-of-date bookkeeping, the birthday stages, the repeat-spot penalty, the ambush trigger, a special scene reward and an amusement park trace (spec section 13).
- Names of main variables that only DATE uses (`date_place`, `date_choice_class`, ...) are requests in `o-date-main-requests.tsv`, not rows of the unit table (an overlay table may not rename main symbols; `D_80122D44` is `scene_effect_flags` in `m-flow`). `D_80122CE0` is requested as the `DialogueWork` field `launch_code` (shared with RPG_BAT, whose table does not name it).
- Disagreement with `core`: `unk_724..727` are the chosen place, the true appointment place and two decoys, not "recent appointment places" (spec section 14).

## Comments

### Review round 1 (2026-10-24, fixes)

The reviewer returned the table with three blocking findings and a list of smaller items. All are fixed in this pass (tables and wiki pages only):

- Main-symbol collision: the `D_80122D44` row is gone (`scene_effect_flags` from `m-flow`; bit 1 is the title banner); the other 17 main rows moved to `o-date-main-requests.tsv` (18 rows with the `DialogueWork` field row for the launch code).
- Glossary trap word: every `date_event_*`, `DATE_EVENT_*`, `DateEventScene*` name and the files `event_*.c` use `scene`; `date_event_trigger` and `date_event_choose` became rename rows (`date_scene_trigger`, `date_scene_choose`).
- HYPOTHESIS marks: all five names with `narration` carry the mark, the other notes say what the code shows (speaker 14, core `GIRL_SOTOI`).
- Content errors: skating rink reaction line 6 (not 5), the reaction step has no sea, the looks thresholds of `date_scene_end` (30, 35 for Mira); `date_load_festival_portrait` is `date_load_special_portrait`; `DateScreenCoords8` and the park talk cursors (`date_park_parade_*`) got neutral, consistent notes.
- Smaller items: alias row for `func_80132004`; `noop`, `gte`, `addr`, `arg`, `Alt` spelled out; literal names replaced (`date_shrine_wait_long`, `date_scene_speak_unshuffled`, `date_scene_sequence_start_unused`); spec index says 48 scenes; spec section 14 notes mode 0x5C and DATE2; spec frontmatter has `unit` and `ticket`; poke addresses recorded in spec section 13. The board card for this ticket is not touched here (the kanban sync belongs to the orchestrator).

### Review round 2 (2026-10-24, Opus 5.5, approved, no findings)

All three blocking findings of round 1 are resolved.

- Main-symbol collision: the `D_80122D44` row is gone from `config/renames/proposals/o-date.tsv`, and the other 17 main rows moved to `config/renames/proposals/o-date-main-requests.tsv`. The spec uses `scene_effect_flags` with bit 1 described as "title banner on", and its writer list matches the C: the trigger does `|= 2`, the place 0xB karaoke hand-off sets the word to 2 before mode 0x5C, Miharu's spring intrusion does `|= 2`, Miharu's summer sea intrusion does `|= 3`, `date_end` and `date_scene_end` clear the word, and the main loader screen reads bits 0 and 1.
- Glossary trap word: no new-name column entry contains `event`. `date_event_trigger` and `date_event_choose` are rename rows to `date_scene_trigger` and `date_scene_choose`, and `DateEventChoice6` became `DateOpeningCursorView`. The remaining `event` tokens are established terms (`event_flags`, `event_counters`, `day_event_next`, `event_still_*`) or old-name citations.
- HYPOTHESIS marks: the five names containing `narration` carry the mark. Spec section 5.6 marks the claim and states what the code shows; the other two rows' notes no longer assert narration.
- The non-blocking content errors (skating rink line 6, special reaction places, Mira 35 against 30) are fixed and match the C.

`rename.py plan` was not run by the reviewer (Docker only per project rules); the author's run over the func, data, type and field rows reported 0 problems. The reviewer found no new clear errors in the touched rows or spec lines. The branch changes only the two tables, the spec, the spec index, the ticket and the log: nothing under `src/`, `include/` or `config/symbol*` (checked against the merge base). Set Done. The last acceptance item (merging the table into the rename tool input) stays open and is not part of this branch.
