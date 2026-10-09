---
id: T-0400
title: Leaf function batch 1
status: In Progress
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[tickets/T-0014-find-exact-ucode-compiler]]"]
---

## Goal
Decompile about 40 leaf functions (no jal/jalr) of the `game` segment, smallest first, skipping 0x80080000-0x80086810 (SDK boundary rework). Non-leaf functions are blocked by T-0014.

## Acceptance criteria
- [ ] `tools/list_leaves.py` lists remaining leaf functions by size
- [ ] About 40 functions matched; sha1 OK after every group
- [ ] Failures recorded in [[matching-notes]]
- [ ] code-review gate passed

## Notes
Worktree branch `leaf-batch-1`. Tooling: `tools/list_leaves.py`.

## Comments
