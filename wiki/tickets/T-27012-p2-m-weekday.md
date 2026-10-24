---
id: T-27012
title: Phase 2 naming proposals and port spec for the school-day command loop (m-weekday)
status: Done
assignee: claude
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/m-weekday]]"]
---

## Goal

Roadmap phase 2 (readable code), unit `m-weekday`: `src/main/800737A0.c` (19 functions), the main-mode 0x40 school-day command loop, `parameter_up_down` (how a day's command changes the nine parameters), the vacation-day setup (`vacation_day_init` and its three steps) and the last stage of the weekly health check (`week_end_exit`). Name every function, global and constant the unit owns, write the port spec, and settle the open questions that a run of the game can settle. Port-first scope: game logic in full, hardware layer (CD read, MDEC background, SD actor setup) at module level.

## Acceptance criteria

- [x] `config/renames/proposals/m-weekday.tsv` has one row per rename (kinds func, data, const, file) and a comment row for each of the nine O.BIN-named functions; no `src/`, `include/` or symbol-file edits.
- [x] `wiki/spec/m-weekday.md` covers the record, the stages and steps, background and SD-pack selection, the parameter formula with worked examples, the vacation variant, the interfaces, port notes and open questions; linked from `wiki/spec/index.md`.
- [x] Uncertain game-meaning claims start with `HYPOTHESIS:` in the note column.
- [x] Review of the table and spec by the phase-2 reviewer.

## Notes

- 71 rows: 10 func, 19 data, 32 const, 1 file, 9 comment; 1 starts with HYPOTHESIS (`mascot_draw_flags`). No field or type rows (the records used are core's). Names reuse the core table (`game_state`, `day_events`, `main_stage_next`, `snd_command`, `Weekday`/`Club`/`Parameter` constants) and the sibling drafts of the same day (m-scheduler `WeekdayCommand`, `command_girl_select`, `command_roll_poor_result`; m-hud `season_index`, `in_summer_months`, `icon_bar_init`, `ActorSlot`; h-cdio `dec_bg_*`). Those names are drafts and may change at their review.
- Prefix choice: O.BIN stem `week_day_` for the school day and `vacation_day_` for the vacation setup (the O.BIN names are kept); new step functions are named by what they do (`_init_screen`, `_init_sd_load`, `_init_sd_actors`, `_bg_read`). `week_end_exit` is the last stage of mode 0x32. File: `src/main/school_day.c`; no dedicated header (its prototypes and table externs sit in `include/main_api.h`; they move to a header of their own when core splits `main_api.h`).
- Evidence beyond the C (emulator, a new game from the title menu through the name entry and prologue to the first school day, states under `build/emu/states/`, not committed): school-day timeline and the parameter change of a day match the formula in the spec exactly (command 0 on 1995-04-05, memory dumps before and after); the scenes of commands 0, 1, 2, 3, 5, 6 and 7; the SD pack id chosen per command; the vacation day on 1995-07-24 (mode 0x40 stage 10); `week_end_exit` on Saturday nights. A scratch copy of `tools/emu/emu.py` with a `--poke` option (build/scratch only, not part of the repo) wrote queue records and player flags into a loaded state to see the companion figure per command, the sick-rest bed scene and the club scene.
- Findings: the poor flag and the companion girl of a school-day record are fixed when the week's queue is built; the companion is a figure inside the SD pack (variant 0 shows girl 0, variant 1 the heroine of that command), selected by a nonzero byte, not by the girl id; gains are scaled, costs never; the 23, 28 and 33 day biorhythm decides poor days (owned by the scheduler units).
- Data facts: `D_800B6928` has 11 club rows (the 12th row read as `[0,0,0,0,0,-1,-1,-1,-1]` is padding plus the holiday key ring); `D_800B6890` and `D_800B68A8` have 11 rows plus 2 padding bytes; `D_800B67AC`, `D_800B6810` and `D_800B69E0` have 25, 32 and 13 entries (consecutive 41-sector files of `MAIN_SD`, `CLUB_SD` and `HOLI_SD`).
- Cross-unit: the sibling drafts name `D_800B6740` (week_picked_girl), `D_800B6748..58` (command_rank_bar_*), `ACTOR_SCENE_96`, `ACTOR_CALENDAR_MARK_FIRST` (actor 97); see the spec Open questions for the alias proposal (`ACTOR_SD_A`/`ACTOR_SD_B`).

## Comments

### Open for the orchestrator (2026-10-24)

This ticket has no card in `wiki/kanban.md` (the task forbids editing the board), and the parent `T-27000-phase-2` does not exist in this tree, so `tools/wiki.py lint` reports one status mismatch and one broken link for this ticket and its spec, the same as for T-27020 and T-27028. Review by the phase-2 reviewer is pending.

### Review (2026-10-24): approved

- Read at a5a3aad64. Approved with three non-blocking findings, none fixed on this branch.
- Duplicate constants: the 8 `WeekdayCommand` / `COMMAND_*` rows repeat m-scheduler's rows, which are already on w2-good (m-scheduler also has `COMMAND_NONE` 14). `rename.py const` refuses an enumerator that already exists in the header closure, so only one table may carry them. Decision for open question 1: m-scheduler owns group `WeekdayCommand`. At consolidation, drop these 8 rows here and keep the per-command scene facts in the spec; m-holiday should switch from group `Command` to `WeekdayCommand`.
- Spec open question 4 is stale: on w2-good, m-hud names actor 96 `ACTOR_EVENT_MOUTH` and actor 97 `ACTOR_EVENT_EYES`, and `ACTOR_CALENDAR_MARK_FIRST` no longer exists as a constant (the calendar page reuses slot 97). The `ACTOR_SD_A` / `ACTOR_SD_B` alias proposal stands. The spec should also say that its emulator results settle two HYPOTHESES in sibling specs: core's `sd_anim_fast` note (actors 96 and 97 are SD figures on the weekday and holiday screens) and m-scheduler's note (the companion girl appears in the command's picture).
- Cross-unit: p2a-m-dates-b (not yet on w2-good) names four of the same addresses differently (`D_800B67AC`, `D_800B6810`, `D_80120652`, `D_80120666`), so those names need one decision at consolidation.
- Scope: the diff against w2-good touches only `config/renames/proposals/m-weekday.tsv`, `wiki/spec/m-weekday.md`, `wiki/spec/index.md`, this ticket and `wiki/log.md`. No `src/`, `include/` or `config/symbol*` edits.
- Board: the card is added to the Done column by the kanban sync after the merge (this branch does not edit `wiki/kanban.md`).
