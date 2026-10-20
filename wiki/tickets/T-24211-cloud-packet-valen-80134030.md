---
id: T-24211
title: Cloud packet: VALEN func_80134030
status: Done
assignee: claude (cloud)
created: 2026-10-20
updated: 2026-10-20
links: ["[[cloud-matching]]", "[[matching-notes]]", "[[tickets/T-19016-w3-d-p16]]", src/ovl/VALEN/80133C70.c]
---

## Goal

Match `func_80134030` (696 bytes) in `src/ovl/VALEN/80133C70.c` from a single-function match packet in a cloud container ([[cloud-matching]]). Guarded draft from [[tickets/T-19016-w3-d-p16]] sits at 27 lines: as1 hoists two `lui` of the default block above the range check, the original keeps them in the block and fills the `beqz` slot with the dispatch `sll`.

## Acceptance criteria

- [x] Scratch copy (other INCLUDE_ASM/INCLUDE_RODATA stripped) without NON_MATCHING matches the packet reference (setup proof).
- [x] C draft matches, or stays guarded with an updated row count and reason.
- [ ] Note in [[matching-notes]] (T-24211); local linked build proof.

## Notes

Packet asm lives only under the gitignored `asm/`; never committed.

## Comments

- 2026-10-20: setup proven (asm-fallback scratch copy equals the packet reference, 0 lines; the draft 6 instruction lines). Not matched: as1 hoists the default block's first two `lui` into the single predecessor (range-check block); only a jump-table reference to the default (`case 10:`, bound 11) stops it. About 30 variants and a scan of 174 matched files (OMIMAI `func_801340E4` hoists the same way in the original). Guard comment now T-24211. Details in [[matching-notes]] (T-24211). Local linked build proof (comment-only change in code) and review due.

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only its C file and wiki, no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. No match; guard down to 6 rows with an updated note. Done.
