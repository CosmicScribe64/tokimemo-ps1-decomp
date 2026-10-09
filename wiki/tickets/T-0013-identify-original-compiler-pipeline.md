---
id: T-0013
title: Identify the original compiler pipeline
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[tickets/T-0011-game-code-file-boundaries-and-compiler]]"]
---

## Goal

Find a compiler/assembler pipeline that reproduces the original codegen. gcc 2.6 to 2.95 plus maspsx cannot match even trivial setters, see [[matching-notes]].

## Acceptance criteria

- [ ] Hypothesis tested: original used something other than gcc+ASPSX as modelled by maspsx (`$t6`-first allocation, `$at` store expansion with delay-slot fill, `lh` copies, `or` for move).
- [ ] A pipeline (compiler, flags, assembler step) found that byte-matches the unmatched examples in [[matching-notes]], or the evidence that none is available.
- [ ] [[toolchain]] updated with the result.

## Notes

Look at more complex functions (loops, switch, saved registers, stack frames) for further compiler signatures before choosing candidates.

## Comments
