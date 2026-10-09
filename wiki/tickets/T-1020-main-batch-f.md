---
id: T-1020
title: "Batch F: main 80075320-80085E30"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-0800-main-exe-batch-c]]"]
---

## Goal
Match as many functions as possible (at least 40, smallest first) in `src/main/80075320.c`, `800789E0.c`, `80079B10.c`, `80085E30.c` (IDO 5.3 + frame pass). Skip quickly on known blockers (T-0017, T-0018, multu vs shifts).

## Acceptance criteria
- [x] At least 40 functions matched; all 27 sha1 OK after every group
- [x] New patterns recorded in [[matching-notes]]
- [x] Commits carry `T-1020:`; no game data or generated asm committed
- [x] code-review gate passed (inline)

## Notes
Worktree branch `batch-b-f`. Shared edits append-only (`include/game.h`, `config/symbol_addrs*.txt`, `wiki/matching-notes.md`).

## Comments
2026-10-09: 58 functions matched in 3 commits (`T-1020:`); `ninja progress` grand total 204 -> 262 functions, 17108 bytes; 27 of 27 sha1 OK after each group. Patterns in [[matching-notes]] (section Main exe batch F). Moved to In Review for the code-review gate.
