---
id: T-0014
title: Find the exact MIPS ucode compiler (frame +16, address CSE)
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[tickets/T-0013-identify-original-compiler-pipeline]]"]
---

## Goal

IDO 5.3 matches leaf game functions, but the original compiler allocates 16 more stack bytes in every non-leaf frame and CSEs global addresses less (`func_80042400`). Find the compiler (or option) that reproduces both, so non-leaf functions become matchable.

## Acceptance criteria

- [ ] Candidates tested (older MIPS/SGI ucode compilers such as IRIX 4 IDO, Ultrix 4.x or NEWS-OS `cc`; IDO 5.3 ugen/uopt options) with results recorded in [[matching-notes]].
- [ ] Either `func_80042400` and one non-leaf function (e.g. `func_80041584`) byte-match, or the evidence that no available compiler does.

## Notes

Evidence and hypotheses: [[matching-notes]] ("Not reproduced"). Do not fake the frame size.

## Comments
