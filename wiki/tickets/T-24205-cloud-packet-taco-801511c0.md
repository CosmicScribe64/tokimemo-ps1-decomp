---
id: T-24205
title: "Cloud packet: TACO func_801511C0"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[cloud-matching]], [[matching-notes]], [[tickets/T-22025-w4-p25]], src/ovl/TACO/80149F90.c]
---

## Goal

Match TACO `func_801511C0` (164 B, `src/ovl/TACO/80149F90.c`) in a cloud session from a match packet ([[cloud-matching]]). Start: guarded draft at 5 rows (T-22025), only the `sw a1,0x3C(sp)` home store of `arg1` is late.

## Acceptance criteria

- [x] Scratch funcdiff: asm fallback MATCH, then the NON_MATCHING C judged by instructions.
- [x] MATCH -> plain C; else the guard note updated with T-24205 and the row count.
- [x] Note in [[matching-notes]]; branch `cloud/t24205-taco-80149f90` pushed (C and wiki only).
- [ ] Local linked build proves the result (local session).

## Notes

- 2026-10-20: toolchain per [[cloud-matching]] (image build about 8 minutes, worked first time). `funcdiff.py` against `/tmp/ref.o` cannot resolve without the disc (jal relocations against the local `.text` symbol and the 3 alignment nops show as DIFF), so rows were judged with a scratch objdump diff (relocations by symbol, `R_MIPS_26` equal, branch targets function-relative): asm fallback 0 lines.
- Result: no match. The guarded draft goes from 4 to 2 unified-diff lines (one instruction, the `sw a1,0x3C(sp)` store-back, is displaced; positions 9-13 still differ, so funcdiff still counts 5 rows). The change: the inits and the empty test on one source line (FAKE, as1 line scheduling). About 150 variants; details and rejected forms in [[matching-notes]] (T-24205).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. No match; guard note updated (one late home store). Done.
