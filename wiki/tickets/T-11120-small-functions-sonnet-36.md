---
id: T-11120
title: Small functions (Sonnet via Antigravity) 36
status: Done
assignee: Sonnet via Antigravity
created: 2026-10-14
updated: 2026-10-17
links: []
---

## Goal

Match the 16 smallest-first functions (5,168 bytes) of `src/ovl/NAME_ENT/80139740.c`: func_8013C934, func_8013C580, func_8013DAC4, func_80140574, func_8013AF78, func_8013F0DC, func_8013FB8C, func_80140430, func_8013BE18, func_8013C9F0, func_8013F454, func_80142880, func_8013D4CC, func_8013E430, func_8013B994, func_8013CEC8.

## Acceptance criteria

- [ ] Every function attempted (m2c draft, build, funcdiff); near misses guarded with NON_MATCHING
- [ ] `tools/docker.sh ninja` 27/27 OK, headers and globals OK
- [ ] Inline review against CODING_STANDARDS.md

## Notes

Branch s36. Only `src/ovl/NAME_ENT/80139740.c` (plus append-only overlay header/symbol files) is touched.

## Comments

2026-10-17 cleanup: merged via cb7b3529c. 6 of 16 functions matched (func_8013C934, func_8013C580, func_80142880, func_80140430, func_80140574, func_8013C9F0) and 9 were guarded as near misses. Seven of those guarded functions are plain C on main now; func_8013B994 and func_8013BE18 are still guarded, and func_8013AF78 was never attempted. Code review is clean and main builds 27/27. Closed.
