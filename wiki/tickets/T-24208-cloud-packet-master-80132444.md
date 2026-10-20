---
id: T-24208
title: "Cloud packet: MASTER func_80132444"
status: Done
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], [[tickets/T-22015-w4-p15]], src/ovl/MASTER/80132140.c]
---

## Goal

Match MASTER `func_80132444` (624 B, `src/ovl/MASTER/80132140.c`) from a cloud match packet, starting from the T-22015 guard (22 rows, all in case 4).

## Acceptance criteria

- [x] Toolchain per [[cloud-matching]]; scratch single-function setup, asm fallback equals the packet reference (0 rows).
- [x] Instruction-level match in the scratch row diff (0 rows, size 0x270), guard removed, plain C.
- [ ] Local session: linked build, funcdiff MATCH, SHA1.
- [ ] Code review against CODING_STANDARDS.md.

## Notes

Match: in case 4, `*(u8 *)&D_800E7D50 |= 1; |= 2; |= 1;` written before the three copies. uopt promotes the byte to `$v1` and folds the ORs into one `ori 3`; the six ugen temporaries of the three ORs put the copies in `$t1..$t3` and the OR chain in `$t8`/`$t0`. One or two ORs, or the ORs after the copies, stay at 20 rows. Marked FAKE (the third OR). Details: [[matching-notes]] (T-24208).

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. func_80132444 matches: linked funcdiff --resolve MATCH; one ticketed FAKE (repeated byte OR). Done.
