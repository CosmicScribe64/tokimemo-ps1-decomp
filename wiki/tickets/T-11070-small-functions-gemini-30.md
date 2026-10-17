---
id: T-11070
title: Small functions (Gemini) 30
status: Done
assignee: gemini-g30
created: 2026-10-13
updated: 2026-10-17
links: []
---

## Goal
Match mid-size functions in `src/ovl/EVENT/8010FE10.c` from `g30-list.txt` (12 functions, 2,780 bytes).

## Acceptance criteria
- [ ] Every function in `g30-list.txt` attempted with m2c, funcdiff, and regdiff/permute
- [ ] `tools/docker.sh ninja` passes 27/27 OK, `headers.ok` and `globals.ok`
- [ ] Code reviewed against `CODING_STANDARDS.md`

## Notes

## Comments

2026-10-17 cleanup: 6 of 12 functions matched (func_801138D8, func_80115890, func_80117390, func_80118A60, func_80119370, func_8011C588); merged as 1fde050ed and still C on main, which builds 27/27. The other six were never recorded; the rest of EVENT 8010FE10.c went to T-11110 and T-11600. Review found two unmarked tricks (first-symbol reach in func_80117390, pad-field bit-field cast in func_80119370); they are in the cleanup followup. Closed.
