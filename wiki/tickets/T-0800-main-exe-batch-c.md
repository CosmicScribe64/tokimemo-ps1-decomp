---
id: T-0800
title: "Main exe batch C: 80041000-80059A20"
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[source-files]]", "[[obin]]", "[[tickets/T-0018-ugen-temp-register-order]]"]
---

## Goal
Decompile functions in `src/main/80041000.c` through `src/main/80059A20.c` (about 300 remaining), smallest first, aiming for about 60 matches. IDO 5.3 with the frame pass; O.BIN names (hypotheses) are used as they are applied. Time-box each function; leave resistant ones as INCLUDE_ASM.

## Acceptance criteria
- [ ] About 60 functions matched; all 27 sha1 checks OK after every group
- [ ] New failure patterns recorded in [[matching-notes]]
- [ ] Commits carry `T-0800:`; no game data or generated asm committed
- [ ] code-review gate passed (inline)

## Notes
Worktree branch `main-batch-c`. Other agents work on overlays in sibling worktrees. Shared declarations: append to `include/game.h`; names: append to `config/symbol_addrs*.txt`.

## Comments
