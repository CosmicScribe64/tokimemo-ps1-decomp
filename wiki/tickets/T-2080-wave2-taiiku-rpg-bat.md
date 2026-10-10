---
id: T-2080
title: "Wave 2: overlays TAIIKU, RPG_BAT"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[overlays]]"]
---

## Goal

Decompile functions in `src/ovl/TAIIKU.c` and `src/ovl/RPG_BAT.c` (IDO 5.3, frame pass, `-Wo,-nokpicopt`), smallest first, aiming for at least 60 matches. First retry the three `NON_MATCHING` TAIIKU functions left from T-0017. Work list: `tools/queue.py --next 40 --files TAIIKU,RPG_BAT`.

## Acceptance criteria

- [ ] Functions matched (funcdiff MATCH), all 27 sha1 checks OK
- [ ] New failure patterns noted in [[matching-notes]]; T-0018 cases appended to [[data/t0018-cases]]
- [ ] Code-review gate run inline, findings resolved

## Notes

## Comments
