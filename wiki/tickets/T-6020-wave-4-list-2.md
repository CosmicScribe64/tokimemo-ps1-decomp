---
id: T-6020
title: Wave 4: list 2
status: In Progress
assignee: claude
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Wave 4 batch agent 2: match as many of the 151 listed functions (41284 bytes) as possible in the 34 owned C files (main 8004E500, 80053650, 80061710; overlays BUNKASAI, BUNKA_SD, DATE, DATE2, ENDING, ETC, EVENT, GEKO, GYOZI, KANGEI, NAME_ENT, RPG_BAT, SHOUGATU, SHUGAKU, TACO, TAIIKU, TEL, TT). Work list: scratchpad `wave4-list-2.txt`.

## Acceptance criteria

- [ ] Work list processed in order, time-boxed per function.
- [ ] Clean build (`rm -rf asm build; configure.py; ninja`): 27 of 27 OK, `build/headers.ok`.
- [ ] T-0018 cases appended to [[data/t0018-cases]]; new patterns in [[matching-notes]].
- [ ] Inline code review against CODING_STANDARDS recorded below.

## Notes

## Comments
