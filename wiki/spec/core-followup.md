---
type: spec
unit: core-followup
updated: 2026-10-24
sources: ["config/renames/proposals/core.tsv", "config/renames/proposals/m-scheduler.tsv", "config/renames/proposals/m-weekday.tsv", "config/renames/core-followup-apply.tsv", "src/main/scheduler.c", "src/main/school_day.c", "include/game.h", "include/game_enums.h"]
---

# Port spec: essential shared-core follow-up (partial)

This is an early, source-checked slice for the next core/scheduler/school-day pass, not a completion claim. T-27067 reconciles the old core and module proposals against the current ledgers and code. This draft resolves one leftover scheduler entry point and records a precise club-season eligibility rule for the Godot port. The full row reconciliation and broader constant census remain open.

## Club-season tournament eligibility

In `day_event_queue_build_week` (`src/main/scheduler.c:411-474`), tomorrow's `month_calendar[0].marker[day] == 8` starts a club-season special week. The code first requires the player's club field (the 4-bit value in `PlayerFlagBits.club`) not equal to 11 (`CLUB_NONE`) and the selected command to be club (`(last_command & 0xF) == 4`). If club id is 6 (`CLUB_BASEBALL`) and `clubs[6].level` low nibble is at least 8, it appends six `DAY_EVENT_CLUB_MASTER_A` records, then a holiday record, with birthday and day-end records interleaved. The first three tournament records do not write their `girl` byte; the last three write 7. Every other non-none club in this branch gets six `DAY_EVENT_CLUB_CAMP` records instead, each with `girl = 4`, the command result in `scene`, and selected companion in `extra`.

The source proves the threshold comparison and its consequence. It does not establish a general “mastery score” meaning for the `level` nibble. For a separate year-97 New Year branch (`scheduler.c:535-550`), the same low nibble is compared with `>= 0xA` for non-baseball clubs when `func_8005352C()` is true; club ids below 6 instead use `practice_points >= 0x17C` to queue club-master-C. These are separate gates with distinct directions and values.

In the school-day SD selector (`src/main/school_day.c:162-315`), the record's `extra` byte is copied to `companion_girl`: 0 is `GIRL_SHIORI`, while 0xF is the no-companion sentinel (the actor-B setup checks `extra != 0xF` at line 398). The SD variant is 0 when the byte is 0, 2 only for athletics with girl 5, and 1 for every other nonzero value, including no companion. Club IDs extracted from `player_flags.h >> 12` choose club-specific rows in the seasonal SD tables.

`CLUB_BASEBALL = 6` is already declared in `include/game_enums.h`, and `PlayerFlagBits.club` remains a four-bit field. The `Club` enum supports the ID meaning of both the player flag and the `clubs[6]` record index. `GIRL_MIRA = 7` also exists, but it is not supported for the three tournament record `girl` writes: the queue reader dispatches type `0x25` by type alone, and the MASTER overlay does not read the current queue record. Leave those bytes unexplained; do not rename them as Mira.

The exact sites and evidence are in the ignored audit inventory `.claude/orchestrator/state/T-27067/club-sites.tsv`. Current `tools/rename.py const` refuses to plan `CLUB_BASEBALL` because that enumerator already exists in the header (`PROBLEM: CLUB_BASEBALL is already an identifier in the closure`). Its member site syntax cannot distinguish the `player_flags.h >> 12` club-ID expression from other literals associated with the whole `player_flags` member. Consequently this packet records the known enum mapping without proposing an unsafe broad constant substitution. A tool/API path that names this exact expression is needed before changing these sites through `rename.py const`.

## Remaining object entry point

`func_800737A0` is called from `schedule_week_end_mode_handler` in the stage `0xFF` case and in the default branch (`src/main/scheduler.c:2219-2221`). Its body advances the event queue with `day_event_next(0)`, then updates the parameter, message, date and icon HUD, mascot and decoded background (`src/main/school_day.c:11-20`). The proposal `schedule_week_end_exit` uses the scheduler topic prefix from ADR-0001, although the function remains physically in the `school_day.c` object. The six supported symbol rows in `config/renames/proposals/core-followup-apply.tsv` passed the full verification-enabled `rename.py plan` in the approved Docker image. The field rename emits a nonblocking note that other `unk_03` member tokens remain in the header closure; the four `ClubRecord` use sites were located, and the other visible uses are fields of different types.

## Unfinished reconciliation

The earlier core, scheduler and school-day proposal tables contain historical rows, O.BIN comment rows, constants, shared data, hypotheses and files. The accepted ledgers and current enum/header names supersede some proposal spellings. T-27067 must classify every row as applied, current under a later name, supported residue, allowed exception, or evidenced blocker before completion. This partial spec does not classify the remaining long tail.
