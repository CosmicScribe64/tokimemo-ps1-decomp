---
id: T-24209
title: "Cloud packet: ETC func_80144DF0"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], [[tickets/T-24129-taco-and-etc-remaining-closers]], src/ovl/ETC/80143240.c]
---

## Goal

Match `func_80144DF0` (388 bytes) in `src/ovl/ETC/80143240.c` from a match packet (the original
disassembly of that one function), in a cloud session per [[cloud-matching]]. Start: the
T-24129 guard at 26 rows (uopt promotes `D_800E6280.unk_1118` to `$v0` across the slot check).

## Acceptance criteria

- [x] Asm fallback compiles to the packet's instructions (scratch single-function setup)
- [x] `func_80144DF0` matched in plain C, or the guard updated with a row count
- [x] T-24209 section in [[matching-notes]]
- [ ] Local linked proof (full build, funcdiff MATCH, SHA1) by the local session
- [ ] Code review against `CODING_STANDARDS.md`

## Notes

Instruction-level match (scratch `objdump` row diff 47 -> 0 lines; `funcdiff --resolve` against the
packet reference differs only by the reference object's 3 trailing alignment nops). Plain C,
`INCLUDE_ASM` and the `NON_MATCHING` guard removed, three marked FAKEs: a byte-view store of
`unk_1118` re-read as a plain field (arm 1), a repeated byte-view store in the `D_800E8BEE` arm
(arm 3), and `s32 pad[3]` for the `sp+0x28` spill of `slot`. Details in [[matching-notes]] (T-24209).
The full file was not compiled in the cloud (the other functions' asm is not in the packet); the
local session runs the linked build.

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Matches: linked funcdiff --resolve MATCH; FAKE comments ticketed. Done.
