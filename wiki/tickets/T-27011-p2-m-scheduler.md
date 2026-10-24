---
id: T-27011
title: Phase 2 naming proposals and port spec for the main scheduler (m-scheduler)
status: In Review
assignee: claude
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[spec/m-scheduler]]"]
---

## Goal

Roadmap phase 2 (readable code), unit `m-scheduler`: name every function, owned global and relied-on field of `src/main/8006CB30.c` (56 functions: the weekly schedule steps, the week queue builder, encounter, meeting and special-event selection, the queue reader and its type-to-mode switch, the calendar day advance, day notice, month start and the weekly health check) and write the port spec. Game logic: rules, thresholds and flow are written out in full.

## Acceptance criteria

- [x] `config/renames/proposals/m-scheduler.tsv` has one row per rename (kinds func, data, field, const, file, comment); no src/, include/ or symbol-file edits.
- [x] Core names (`p2a-core`) are used and not duplicated: the 14 functions core already names in this file get no row.
- [x] Uncertain game-meaning claims are marked `HYPOTHESIS:` first in the note column.
- [x] O.BIN-named functions kept, each with an English comment row.
- [x] `wiki/spec/m-scheduler.md` covers state, schedule steps, queue builder, encounter and meeting rules, special events, queue reader, notices, month start, calendar, health check and open questions; linked from `wiki/spec/index.md`.
- [ ] Review of the table and spec by the phase-2 reviewer.

## Notes

- 116 rows: 33 func, 11 comment, 8 data, 9 field, 54 const, 1 file. 15 notes start with `HYPOTHESIS:`.
- Prefix choice: main code has no unit prefix; module stems `schedule_` (O.BIN style), `day_events_`, `meeting_`, `command_`, `health_`, `month_start_`, `vacation_`. File proposal `src/main/scheduler.c`.
- Emulator evidence (`build/emu` is not committed): from the title-menu save state the main mode byte was poked to 0x30, 0x32 or 0x39 with chosen calendar and parameter bytes and the run stopped at a known step. Results: the first week queue (Wednesday to Saturday, `fc fb`, Sunday `10`), a Sunday build with a national holiday on Saturday, girl 0 affection 0x32 to 0x31 by `yuukou_down` on day 16, a meeting record for girl 1 from command 0 with liberal arts 0x40, the injury step with condition 0, and the `vacation` transitions 0 to 1, 2, 3 and back to 0. A first attempt through the GDB stub failed because writes to memory made the target run; the probes used a small Lua job on the existing PCSX-Redux image instead (not added to `tools/`).
- Quirks kept in the spec: the `health_check_roll` sick-visit loop tests the random start girl instead of the loop girl; the marker 11 and 8 tests read this month's table even when tomorrow is in the next month; `date_sasoi_judge` stores a girl in `week_picked_girl` even when the invitation is discarded.
- The command check (`func_8006BDA8`, owned by the HUD unit) is a three-wave biorhythm (23, 28, 33 days); its result is the `half` flag of `parameter_up_down`, not a success flag. The spec section 6 describes it for the port.
- Cross-unit: suggested names for 13 callees owned by other main units are in the spec (section 16), not in the table.

## Comments
