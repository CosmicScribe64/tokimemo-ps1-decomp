---
id: T-1010
title: "Batch E: main 80062CD0-8006CB30"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-0800-main-exe-batch-c]]", "[[tickets/T-0017-const-in-reg-loop-hoisting]]", "[[tickets/T-0018-ugen-temp-register-order]]"]
---

## Goal
Decompile functions in `src/main/80062CD0.c` and `src/main/8006CB30.c`, smallest first, at least 40 matches. IDO 5.3 plus frame pass. Skip quickly on known blockers (T-0017, T-0018, multu vs shifts).

## Acceptance criteria
- [ ] At least 40 functions matched; all 27 sha1 OK after every group
- [ ] New patterns recorded in [[matching-notes]]
- [ ] Commits carry `T-1010:`; no game data or generated asm committed
- [ ] code-review gate passed (inline)

## Notes
Worktree branch `batch-b-e`.

## Comments
