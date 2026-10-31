---
id: T-39315
title: Port the engine mode-screen files (WP-D)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]", "[[tickets/T-39002-restore-port-runtime]]"]
---

## Goal

Port `scheduler.c`, `home_screens.c`, `holiday.c` and `school_day.c` onto the engine, following [[engine-porting-contract]]. These files are the main loop's calendar, command screen, holiday and school day, including the month rollover where the actual route stops today.

## Acceptance criteria

- [ ] Every function of these files is ported, with function-differential tests from emulator captures, or listed as untested with a reason.
- [ ] Month rollover and the full day-event queue across a year boundary are covered by captures from states near those dates.
- [ ] The core suite, `run_engine_diff.sh --all`, the signature check and the opening frame-timing gate pass.

## Comments
