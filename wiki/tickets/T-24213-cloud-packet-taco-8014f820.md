---
id: T-24213
title: "Cloud packet: TACO func_8014F820"
status: In Review
assignee: claude (cloud session)
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[cloud-matching]], [[register-cheatsheet]], [[tickets/T-24136-taco-and-etc-wave-3-closers]], [[tickets/T-24120-taco-remaining-sweep]], src/ovl/TACO/80149F90.c]
---

## Goal

Match TACO `func_8014F820` (400 B, `src/ovl/TACO/80149F90.c`) from a cloud match packet, starting from the T-24136 guard (7 rows, s32 return view).

## Acceptance criteria

- [x] Toolchain per [[cloud-matching]]; scratch single-function setup, asm fallback equals the packet reference (0 rows, funcdiff words identical once relocations are applied).
- [x] Instruction-level match in the scratch row diff (0 rows, 100 instructions, size 0x190), guard removed, plain C.
- [ ] Local session: linked build, funcdiff MATCH, SHA1.
- [ ] Code review against CODING_STANDARDS.md.

## Notes

Match: the `unk_038` decrement as an element chain over the three selector bytes, `(&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = --(&D_800E6280.unk_036)[2];` (FAKE, T-24213). The pre-decrement stores the raw `addiu t9` to 038; the chain value is its `u8` result (`andi v0,t9,0xFF`) for 037 and 036, stored in the original order 037, 036, 038. The T-24136 s32 return view is still needed (void: 5 rows). Details: [[matching-notes]] (T-24213).

## Comments
