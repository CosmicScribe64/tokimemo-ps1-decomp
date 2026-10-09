---
id: T-0400
title: Leaf function batch 1
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-0014-find-exact-ucode-compiler]]"]
---

## Goal
Decompile about 40 leaf functions (no jal/jalr) of the `game` segment, smallest first, skipping 0x80080000-0x80086810 (SDK boundary rework). Non-leaf functions are blocked by T-0014.

## Acceptance criteria
- [x] `tools/list_leaves.py` lists remaining leaf functions by size
- [x] About 40 functions matched; sha1 OK after every group
- [x] Failures recorded in [[matching-notes]]
- [x] code-review gate passed

## Notes
Worktree branch `leaf-batch-1`. Tooling: `tools/list_leaves.py`.

## Comments
2026-10-09: 40 leaf functions matched (see commits `T-0400`); sha1 OK after each group. 1 FAKE (func_8004DAC4, arg spill). 16 attempted functions left as INCLUDE_ASM, reasons in [[matching-notes]] (mostly T-0014 address-CSE).
Code review (code review, since 159adc9): Standards - C89, `/* */` comments, no game data tracked, FAKE comment on func_8004DAC4, struct offsets documented, commits carry T-0400. Findings fixed: documented the D_800B5938 array/scalar overlap and the game.h trailing newline. Spec - all criteria met (40 matched, sha1 OK, failures in matching-notes). No open findings.
