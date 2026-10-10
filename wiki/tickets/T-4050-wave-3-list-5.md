---
id: T-4050
title: "Wave 3: list 5"
status: In Progress
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Match the functions of wave 3 work list 5 (134 functions, 26044 bytes, 27 C files: main 80041000 and the overlays BUNKASAI, BUNKA_SD, DATE, DATE2, ETC, EVENT, GYOZI, MASTER, OPTION, TACO, TEL, TT, VALEN), best first, then continue with `queue.py --by bytes` inside those files.

## Acceptance criteria

- [ ] Every match verified by `funcdiff.py --resolve` and a full `ninja` (27 of 27 OK, `build/headers.ok`).
- [ ] Failures that are T-0018 shapes have one row each in `wiki/data/t0018-cases.md`.
- [ ] New patterns recorded in `wiki/matching-notes.md` (own section).
- [ ] Inline review against CODING_STANDARDS.md recorded below.

## Notes

Branch `w3-5`, worktree `tokimemo-wt/w3-5`. Not merged or pushed.

## Comments
