---
id: T-2070
title: "Wave 2: TT"
status: In Progress
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal
Match as many remaining `INCLUDE_ASM` functions of the TT overlay (`src/ovl/TT.c`, `include/ovl/TT.h`) as possible.

## Acceptance criteria

- [ ] At least 60 functions matched (or the queue is exhausted)
- [ ] 27 of 27 sha1 OK after a clean rebuild
- [ ] Inline code review against CODING_STANDARDS.md, findings resolved

## Notes
Queue: `tools/queue.py --next 40 --files TT`.

## Comments
