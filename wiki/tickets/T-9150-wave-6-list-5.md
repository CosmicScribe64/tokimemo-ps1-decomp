---
id: T-9150
title: Wave 6 list 5
status: Done
assignee: wave-6 agent 5
created: 2026-10-10
updated: 2026-10-10
links: []
---

## Goal
Match as many functions as possible from wave-6 list 5 (37 files, 158 functions).

## Acceptance criteria
- [x] Matched functions build byte-identical, 27/27 OK
- [x] Inline review against CODING_STANDARDS.md

## Notes
24 functions matched (4,764 bytes): normal_date_bg_fadein/fadeout, TT func_8013A040, TACO func_80143E80/F40/80136C60/80137590/8014488C, main k_reset/func_8004E500/func_800626B0, TAIIKU func_801331D0 (FAKE pad), GEKO func_8013B7D0/D2C8/CF40/AA1C, EVENT func_801092C8/FAC00/F9CFC/80115714, RPG_BAT func_8013F4F0/F5C8/F694/F760. Idioms: [[matching-notes]] (Wave 6, list 5). 14 T-0018 rows added to [[data/t0018-cases]].

## Comments
Inline review against CODING_STANDARDS.md: C89, /* */ comments, fixed-width types, one FAKE (TAIIKU func_801331D0 pad) marked with T-9150, new structs documented with offsets (KWork, TacoCam), declarations in headers, no game data staged. No findings open.
