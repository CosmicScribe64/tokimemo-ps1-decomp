---
id: T-21003
title: "Remove leftovers from merged big-function work"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11060-big-functions-gemini-29]]", "[[tickets/T-11080-big-functions-opus-31]]"]
---

## Goal

Remove leftovers from merged big-function work. (1) include/ovl/DATE.h line 69 still declares s32 func_8013272C(void); next to s32 date_event_choose(void); after T-11090 renamed the symbol (T-11060 changed its return type before the rename merged); delete the old line. (2) src/ovl/NAME_ENT/80139740.c has 15 identical '/* Falls through without a return statement. */' m2c comment lines (several stacked above func_8013F208, from T-11010 commit fa8d0de51) and src/ovl/ENDING/80137340.c has one; delete them. Declaration and comment changes only; confirm tools/docker.sh ninja 27/27 and check_headers afterwards.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-11060-big-functions-gemini-29|T-11060]], [[tickets/T-11080-big-functions-opus-31|T-11080]].

## Comments
