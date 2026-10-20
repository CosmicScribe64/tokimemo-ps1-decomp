---
id: T-24210
title: "Cloud packet: main func_800591D8"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[tickets/T-24206-cloud-packet-main-800590cc]], [[tickets/T-17017-w3-main-p17]], src/main/80058D20.c]
---

## Goal

Match `func_800591D8` (304 bytes) in `src/main/80058D20.c` from a match packet (the original
disassembly of that one function), in a cloud session per [[cloud-matching]]. Start: the
T-17017 guard at 32 differing lines (aliased `w`/`h`, frame 0x38).

## Acceptance criteria

- [x] Asm fallback compiles to the packet's instructions (scratch single-function setup)
- [x] `func_800591D8` matched in plain C, or the guard updated with a row count
- [x] T-24210 section in [[matching-notes]]
- [ ] Local linked proof (full build, funcdiff MATCH, SHA1) by the local session
- [ ] Code review against `CODING_STANDARDS.md`

## Notes

Instruction-level match (0 differing lines, scratch `objdump` row diff with relocations mapped
to absolute addresses; `tools/funcdiff.py --built ... --expected /tmp/ref.o` reports the words
identical once relocations are applied, literal-byte proof needs the disc). Plain C with one
FAKE (the T-24206 `u16 *` re-read view plus store order); `INCLUDE_ASM` and the `NON_MATCHING`
guard removed. Details in [[matching-notes]] (T-24210).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Matches: linked funcdiff --resolve MATCH; FAKE comments ticketed. Done.
