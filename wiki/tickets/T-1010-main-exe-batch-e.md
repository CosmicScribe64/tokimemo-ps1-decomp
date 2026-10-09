---
id: T-1010
title: "Batch E: main 80062CD0-8006CB30"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-0800-main-exe-batch-c]]", "[[tickets/T-0017-const-in-reg-loop-hoisting]]", "[[tickets/T-0018-ugen-temp-register-order]]"]
---

## Goal
Decompile functions in `src/main/80062CD0.c` and `src/main/8006CB30.c`, smallest first, at least 40 matches. IDO 5.3 plus frame pass. Skip quickly on known blockers (T-0017, T-0018, multu vs shifts).

## Acceptance criteria
- [x] At least 40 functions matched (40 of 153); all 27 sha1 OK after every group
- [x] New patterns recorded in [[matching-notes]] (section Main exe batch E)
- [x] Commits carry `T-1010:`; no game data or generated asm committed
- [x] code-review gate passed (inline)

## Notes
Worktree branch `batch-b-e`.

## Comments
2026-10-09: 40 functions matched in 3 groups (commits `T-1010: match ...`), `ninja progress` grand total 244/6962 functions, 17900/2279368 bytes, 27 of 27 sha1 OK. Two fakematches marked `FAKE` (`bustup_wink`, `bustup_speech`: no-op `& 0xFF`). Two array-form accesses (`func_8006764C`, `func_80065900`) match in the linked executable but funcdiff reports relocation-name differences. New idioms and blockers in [[matching-notes]]. Moved to In Review for the code-review gate.
