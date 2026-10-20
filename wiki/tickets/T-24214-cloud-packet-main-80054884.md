---
id: T-24214
title: "Cloud packet: main func_80054884"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], [[tickets/T-24123-main-remaining-sweep-b]], src/main/80053650.c]
---

## Goal

Match main `func_80054884` (624 B, the memory-card save writer, `src/main/80053650.c`) from a cloud match packet, starting from the T-24123 guard (score 36, 18 instructions, register-only: ugen temporaries one lower from the `idx` block on).

## Acceptance criteria

- [x] Toolchain per [[cloud-matching]]; scratch single-function setup, asm fallback equals the packet reference (funcdiff: identical once relocations are applied).
- [x] Instruction-level match in the scratch diff (all instructions equal; remaining rows are `D_80123320+0xF00` = `D_80124220` and section-relative string offsets), guard and `INCLUDE_ASM` removed, plain C.
- [ ] Local session: linked build, funcdiff MATCH, SHA1.
- [ ] Code review against CODING_STANDARDS.md.

## Notes

Match: `idx = sp47 & 0xFF;` (no-op mask on the `u8` out-parameter) spends the one ugen temporary the draft was short, so `$t2/$t3/$t4` and the copy loop's `$t5/$t6/$t9` land on the original registers. Marked FAKE; the existing pad locals now carry a FAKE comment too. Details: [[matching-notes]] (T-24214).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. Matches: linked funcdiff --resolve MATCH; FAKE comments ticketed. Done.
