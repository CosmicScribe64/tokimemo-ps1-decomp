---
id: T-24207
title: "Cloud packet: ETC func_80140AE4"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], [[tickets/T-24129-taco-and-etc-remaining-closers]], src/ovl/ETC/8013FF80.c]
---

## Goal

Match `func_80140AE4` (224 bytes) in `src/ovl/ETC/8013FF80.c` from a match packet (the original
disassembly of that one function), in a cloud session per [[cloud-matching]]. Start: the
T-24129 guard at 20 rows (ugen temporary rotor starts at `$t7` instead of `$t6` after the
`func_80143240` call).

## Acceptance criteria

- [x] Asm fallback compiles to the packet's instructions (scratch single-function setup)
- [x] `func_80140AE4` matched in plain C, or the guard updated with a row count
- [x] T-24207 section in [[matching-notes]]
- [ ] Local linked proof (full build, funcdiff MATCH, SHA1) by the local session
- [ ] Code review against `CODING_STANDARDS.md`

## Notes

Instruction-level match (`funcdiff` against the packet reference: "identical once relocations
are applied", the only differing row is `D_800E6280+0xF80` against `D_800E7200`). Guard and
`INCLUDE_ASM` removed; two marked FAKEs. Details in [[matching-notes]] (T-24207).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Matches: linked funcdiff --resolve MATCH; FAKE comments ticketed. Done.
