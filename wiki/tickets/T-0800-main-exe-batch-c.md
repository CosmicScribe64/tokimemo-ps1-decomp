---
id: T-0800
title: "Main exe batch C: 80041000-80059A20"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[source-files]]", "[[obin]]", "[[tickets/T-0018-ugen-temp-register-order]]"]
---

## Goal
Decompile functions in `src/main/80041000.c` through `src/main/80059A20.c` (about 300 remaining), smallest first, aiming for about 60 matches. IDO 5.3 with the frame pass; O.BIN names (hypotheses) are used as they are applied. Time-box each function; leave resistant ones as INCLUDE_ASM.

## Acceptance criteria
- [x] 53 functions matched (target was about 60); all 27 sha1 checks OK after every group
- [x] New failure patterns recorded in [[matching-notes]]
- [x] Commits carry `T-0800:`; no game data or generated asm committed
- [x] code-review gate passed (inline)

## Notes
Worktree branch `main-batch-c`. Other agents work on overlays in sibling worktrees. Shared declarations: append to `include/game.h`; names: append to `config/symbol_addrs*.txt`.

## Comments
2026-10-09: 53 functions matched in 7 groups (commits `T-0800: match ...`), `ninja progress` main game 75/834 -> 128/834 (8136 bytes), 27 of 27 sha1 OK. One group commit briefly carried a non-matching `func_800570B8` (caught by the sha1 check, reverted and amended). New idioms and failure patterns are in [[matching-notes]] (section Main exe batch C). Moved to In Review for the code-review gate.
