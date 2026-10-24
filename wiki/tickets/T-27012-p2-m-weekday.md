---
id: T-27012
title: Phase 2 naming proposals and port spec for the school-day command loop (m-weekday)
status: In Review
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
- [ ] Review of the table and spec by the phase-2 reviewer.

## Notes

- 71 rows: 10 func, 19 data, 32 const, 1 file, 9 comment; 2 start with HYPOTHESIS (`room_bg_sectors_holiday`, `mascot_draw_flags`). No field or type rows (the records used are core's). Names reuse the core table (`game_state`, `day_events`, `main_stage_next`, `snd_command`, `Weekday`/`Club`/`Parameter` constants) and the sibling drafts of the same day (m-scheduler `WeekdayCommand`, `command_girl_select`, `command_roll_poor_result`; m-hud `season_index`, `in_summer_months`, `icon_bar_init`, `ActorSlot`; h-cdio `dec_bg_*`). Those names are drafts and may change at their review.
- Prefix choice: O.BIN stem `week_day_` for the school day and `vacation_day_` for the vacation setup (the O.BIN names are kept); new step functions are named by what they do (`_init_screen`, `_init_sd_load`, `_init_sd_actors`, `_bg_read`). `week_end_exit` is the last stage of mode 0x32. File: `src/main/school_day.c`; no dedicated header (its prototypes and table externs sit in `include/main_api.h`; they move to a header of their own when core splits `main_api.h`).
- Evidence beyond the C (emulator, a new game from the title menu through the name entry and prologue to the first school day, states under `build/emu/states/`, not committed): school-day timeline and the parameter change of a day match the formula in the spec exactly (command 0 on 1995-04-05, memory dumps before and after); the scenes of commands 0, 1, 2, 3, 5, 6 and 7; the SD pack id chosen per command; the vacation day on 1995-07-24 (mode 0x40 stage 10); `week_end_exit` on Saturday nights. A scratch copy of `tools/emu/emu.py` with a `--poke` option (build/scratch only, not part of the repo) wrote queue records and player flags into a loaded state to see the companion figure per command, the sick-rest bed scene and the club scene.
- Findings: the poor flag and the companion girl of a school-day record are fixed when the week's queue is built; the companion is a figure inside the SD pack (variant 0 shows girl 0, variant 1 the heroine of that command), selected by a nonzero byte, not by the girl id; gains are scaled, costs never; the 23, 28 and 33 day biorhythm decides poor days (owned by the scheduler units).
- Data facts: `D_800B6928` has 11 club rows (the 12th row read as `[0,0,0,0,0,-1,-1,-1,-1]` is padding plus the holiday key ring); `D_800B6890` and `D_800B68A8` have 11 rows plus 2 padding bytes; `D_800B67AC`, `D_800B6810` and `D_800B69E0` have 25, 32 and 13 entries (consecutive 41-sector files of `MAIN_SD`, `CLUB_SD` and `HOLI_SD`).
- Cross-unit: the sibling drafts name `D_800B6740` (week_picked_girl), `D_800B6748..58` (command_rank_bar_*), `ACTOR_SCENE_96`, `ACTOR_CALENDAR_MARK_FIRST` (actor 97); see the spec Open questions for the alias proposal (`ACTOR_SD_A`/`ACTOR_SD_B`).

## Comments
