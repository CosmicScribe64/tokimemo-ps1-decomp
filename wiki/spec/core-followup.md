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

The exact sites and evidence are in the ignored audit inventory `.claude/orchestrator/state/T-27067/club-sites.tsv`. The former constant-reuse and caller-site limitations are closed by the reviewed T-27068/T-27072/T-27073 tooling changes. Current source uses `CLUB_BASEBALL` for the player-club comparison and club-record index in `src/main/scheduler.c:425`; the corresponding literal sites are applied. This enum mapping does not establish a meaning for the three unexplained tournament `girl` writes.

The broader source-checked literal slice is in ignored `.claude/orchestrator/state/T-27067/essential-literals.tsv`; it records exact paths, line numbers, comparison direction, payload meaning and fixed-width/signedness notes for club-season gates, schedule thresholds, health checks, command effects and SD selection. The 29 old scheduler EventFlag candidates have a separate exact-index site scan in `event-flag-sites.tsv` (28 direct-index matches; value 61 uses a computed pointer in the scheduler counter path). This is site discovery, not blanket constant approval. The remaining flag-field candidates and their actual current type views are in ignored `.claude/orchestrator/state/T-27067/field-residue.tsv`. The 936 original proposal rows and their current status categories are in ignored `.claude/orchestrator/state/T-27067/proposal-reconciliation.tsv`; the residue interpretation is summarized in `proposal-census.md`. These inventories narrow the remaining review but do not close every historical row.

## Remaining object entry point

`schedule_week_end_exit` is called from `schedule_week_end_mode_handler` in the stage `0xFF` case and default branch (`src/main/scheduler.c:2219-2221`). Its body advances the event queue with `day_event_next(0)`, then updates the parameter, message, date and icon HUD, mascot and decoded background (`src/main/school_day.c:11-20`). The current name follows the scheduler topic prefix in ADR-0001 although the definition remains in the school-day object. The `ClubRecord.unk_03` rename emits a nonblocking note for same-spelled members in other types or excluded preprocessor branches; the four `ClubRecord` use sites were located.

Existing core ownership stays as recorded in the applied ledgers: `talk_work` is the separately declared scalar `s32` scene work value at 0x80122CD0. The separately declared `s32 D_80122CD4` is the girl selector at the +0x04 offset described by `TalkWork.girl`; do not treat it as a struct lvalue or as part of a `talk_work` object. `schedule_last_picked_girl` is set at the start of a queue build and rejects only the most recently returned girl across the walk-home and invitation selectors. It blocks consecutive repeats but does not prevent an A,B,A selection within one build.

## Unfinished reconciliation

The earlier core, scheduler and school-day proposal tables contain historical rows, O.BIN comment rows, constants, shared data, hypotheses and files. The accepted ledgers and current enum/header names supersede some proposal spellings. T-27067 must classify every row as applied, current under a later name, supported residue, allowed exception, or evidenced blocker before completion. This partial spec does not classify the remaining long tail.
