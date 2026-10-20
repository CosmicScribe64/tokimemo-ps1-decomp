---
id: T-24206
title: "Cloud packet: main func_800590CC"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], [[tickets/T-17017-w3-main-p17]], src/main/80058D20.c]
---

## Goal

Match `func_800590CC` (268 bytes) in `src/main/80058D20.c` from a match packet (the original
disassembly of that one function), in a cloud session per [[cloud-matching]]. Start: the
T-17017 guard at 16 differing lines (three K&R parameters, aliased `w`/`h`).

## Acceptance criteria

- [x] Asm fallback compiles to the packet's instructions (scratch single-function setup)
- [x] `func_800590CC` matched in plain C, or the guard updated with a row count
- [x] T-24206 section in [[matching-notes]]
- [ ] Local linked proof (full build, funcdiff MATCH, SHA1) by the local session
- [ ] Code review against `CODING_STANDARDS.md`

## Notes

Instruction-level match (0 differing lines, scratch `objdump` diff with relocations mapped to
absolute addresses) with a marked FAKE: plain C, `INCLUDE_ASM` and the `NON_MATCHING` guard
removed. One parameter as in the asm and `main_api.h` (no extra K&R parameters, no locals).
Details in [[matching-notes]] (T-24206).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. func_800590CC matches: linked funcdiff --resolve MATCH (268 bytes); FAKE comments ticketed. Done.
