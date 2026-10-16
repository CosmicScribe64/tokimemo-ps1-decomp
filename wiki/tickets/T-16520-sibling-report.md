---
id: T-16520
title: Matched-sibling report for work packages
status: In Progress   # Backlog | Ready | In Progress | In Review | Done  (must match column in wiki/kanban.md)
assignee: claude-tooling
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16200-matching-wave-2]], [[decompile-workflow]]
---

## Goal

For every unmatched function (INCLUDE_ASM, guarded drafts included), find the closest matched functions anywhere in
the tree (all overlays and main; TT may be read) by register-erased opcode-sequence similarity, and cluster
unmatched functions that look alike. A matching agent then starts from a matched sibling's C instead of from nothing.
New tool `tools/siblings.py`; branch `t16520-siblings`. No file under `src/` is edited.

## Acceptance criteria

- [ ] `tools/siblings.py` writes JSON (target -> top 3 siblings with ratio, size, C file, function) and a text summary by tier (>= 0.90, 0.80-0.90, 0.70-0.80) with counts and bytes, plus unmatched-unmatched clusters at ratio >= 0.90
- [ ] `--package` writes a short note file per target: the sibling's C excerpt and both asm listings side by side
- [ ] Whole tree in a few minutes (opcode n-gram index before difflib)
- [ ] Unit tests in `tools/test_siblings.py`; full tool suite passes
- [ ] Short section in `wiki/decompile-workflow.md`: what it does, how to run it, limits
- [ ] Tier counts on the current tree recorded; 5 pairs spot-checked by eye

## Notes

## Comments
