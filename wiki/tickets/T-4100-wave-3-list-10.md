---
id: T-4100
title: Wave 3: list 10
status: In Progress
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Wave 3 batch agent 10: match as many of the 131 listed functions (26188 bytes) as possible in the 27 owned C files (main 8004F870, 80062CD0, 800737A0, 80075320; overlays BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, ETC, EVENT, GEKO, GYOZI, KANGEI, RPG_BAT, TACO, TAIIKU). Work list: scratchpad `wave3-list-10.txt`.

## Acceptance criteria

- [ ] Work list processed in order, time-boxed per function.
- [ ] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK.
- [ ] T-0018 cases appended to [[data/t0018-cases]]; new patterns in [[matching-notes]].
- [ ] Inline code review against CODING_STANDARDS recorded below.

## Notes

## Comments
