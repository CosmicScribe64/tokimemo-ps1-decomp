---
id: T-24203
title: Cloud packet: GEKO func_80136C20
status: Done
assignee: claude (cloud)
created: 2026-10-20
updated: 2026-10-20
links: ["[[cloud-matching]]", "[[matching-notes]]", "[[tickets/T-22010-w4-p10]]", src/ovl/GEKO/80134D10.c]
---

## Goal

Match `func_80136C20` (604 bytes) in `src/ovl/GEKO/80134D10.c` from a single-function match packet in a cloud container ([[cloud-matching]]). Guarded draft from [[tickets/T-22010-w4-p10]] sits at 4 rows: the original fills the delay slot of the `D_80145088` branch with the `lui` of `unk_F5F`.

## Acceptance criteria

- [x] Scratch copy (other INCLUDE_ASM/INCLUDE_RODATA stripped) without NON_MATCHING matches the packet reference (setup proof).
- [x] C draft matches, or stays guarded with an updated row count and reason.
- [ ] Note in [[matching-notes]] (T-24203); local linked build proof.

## Notes

Packet asm lives only under the gitignored `asm/`; never committed.

## Comments

- 2026-10-20: setup proven (asm fallback scratch copy equals the packet reference). Not matched: still 4 rows, the `lui` of `unk_F5F` is placed by as1 in the load delay of `D_80145090` instead of the `D_80145088` branch slot; ugen output already has the selector load in the switch block. 28 more variants, none moves it. Guard comment updated to T-24203. Details in [[matching-notes]] (T-24203). Local linked build proof (guarded file unchanged in code) and review due.

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only the guard note and wiki; no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. No match; the guard stays. Done.
