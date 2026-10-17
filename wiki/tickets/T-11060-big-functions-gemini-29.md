---
id: T-11060
title: Big functions (Gemini) 29
status: Done
assignee: g29
created: 2026-10-13
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "src/ovl/TAIIKU/80138950.c", "src/ovl/DATE/80132000.c", "src/main/8007C030.c", "src/main/800674B0.c"]
---

## Goal

Match big functions from `astra29-list.txt` across `src/ovl/TAIIKU/80138950.c`, `src/ovl/DATE/80132000.c`, `src/main/8007C030.c`, and `src/main/800674B0.c`.

## Acceptance criteria

- [ ] Work through `astra29-list.txt` (biggest first), matching functions or keeping near misses (<30 lines diff) under `#ifdef NON_MATCHING`.
- [ ] `tools/docker.sh ninja` passes 27/27 OK with `build/headers.ok` and `build/globals.ok`.
- [ ] Code review against `CODING_STANDARDS.md` completed and recorded.

## Notes

Owned files:
- `src/ovl/TAIIKU/80138950.c`
- `src/ovl/DATE/80132000.c`
- `src/main/8007C030.c`
- `src/main/800674B0.c`

## Comments

2026-10-17: Closing as superseded. Four functions were merged and are still C on main (DATE date_event_choose, formerly func_8013272C; main func_80082764, func_80068BE4, func_8006BDA8), but the ticket has no comments and the astra29 list was not worked through. The remaining functions in its four files (DATE/80132000 14, TAIIKU/80138950 12, 800674B0 6, 8007C030 2) are covered by T-14500, T-13600 and, for 800674B0, T-11100. Review: all FAKE comments give reason and T-11060; one trivial leftover, the stale func_8013272C prototype in include/ovl/DATE.h, goes to a followup.

Followup opened: [[tickets/T-21003-big-function-leftovers-cleanup|T-21003]].
