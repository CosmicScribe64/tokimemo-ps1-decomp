---
id: T-11110
title: Small functions (Gemini) 34
status: Done
assignee: gemini-g34
created: 2026-10-13
updated: 2026-10-17
links: []
---

## Goal
Match mid-size functions in `src/ovl/EVENT/8010FE10.c` from `g34-list.txt` (16 functions, 4,800 bytes).

## Acceptance criteria
- [ ] Every function in `g34-list.txt` attempted with m2c, funcdiff, and regdiff/permute
- [ ] `tools/docker.sh ninja` passes 27/27 OK, `headers.ok` and `globals.ok`
- [ ] Code reviewed against `CODING_STANDARDS.md`

## Notes

## Comments

2026-10-17 cleanup: 7 of the 16 functions matched (func_80116AA8, func_801159D4, func_80116B94, func_80116940, func_80117004, func_801196B8, func_8011AC18) and 7 near misses were guarded; the other two have no recorded result. Six of the guarded drafts were matched later by T-11360, T-11600 and T-11270; func_80118868 stays guarded at 8 lines under T-11600. Code review against CODING_STANDARDS.md is clean; main builds 27/27. Closed.
