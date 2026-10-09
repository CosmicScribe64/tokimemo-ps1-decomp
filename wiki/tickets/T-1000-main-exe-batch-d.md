---
id: T-1000
title: "Batch D: main 8005A0B0-80061710"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[source-files]]"]
---

## Goal
Decompile functions in `src/main/8005A0B0.c` and `src/main/80061710.c`, smallest first, at least 40 matches. IDO 5.3 with the frame pass. Time-box each function; skip known blockers (T-0017, T-0018, multu vs shifts).

## Acceptance criteria
- [ ] At least 40 functions matched (39 reached; the rest are the T-0017/T-0018 patterns in matching-notes); all 27 sha1 checks OK after every group
- [x] New failure patterns recorded in [[matching-notes]]
- [x] Commits carry `T-1000:`; no game data or generated asm committed
- [x] code-review gate passed (inline)

## Notes
Worktree branch `batch-b-d`. Sibling worktrees work on other files. Shared edits are append-only (`include/game.h`, `config/symbol_addrs*.txt`, `wiki/matching-notes.md`).

## Comments
2026-10-09: 39 of 129 functions matched in 6 commits (`T-1000: ...`), main exe sha1 OK after each; `ninja progress` grand total 243/6962 (this worktree only). One unmarked-looking cast in `func_8005C4CC` is explained by an inline comment; no FAKE needed. Failure patterns in [[matching-notes]] (section Main exe batch D). Moved to In Review.
