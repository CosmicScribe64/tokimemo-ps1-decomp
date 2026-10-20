---
id: T-24201
title: "Cloud packet: KANGEI func_80134724"
status: Done
assignee: cloud session
created: 2026-10-20
updated: 2026-10-20
links: [[[cloud-matching]], [[matching-notes]], [[tickets/T-24122-shugaku-final-3-and-small-overlays-sweep]], src/ovl/KANGEI/80134120.c]
---

## Goal

Match `KANGEI:func_80134724` (688 B, `src/ovl/KANGEI/80134120.c`) in a cloud session from a match packet
([[cloud-matching]]). Starting point: the guarded T-24122 draft at 2 rows (default-block delay-slot hoist).

## Acceptance criteria

- [x] Unchanged file reproduces sha1 `03b66d1474f31c0cd1a64f34a7829957c63d84b3` in the cloud toolchain
- [x] funcdiff against the packet reference shows no row differences (or an updated NON_MATCHING note): not matched, guard kept with `NON_MATCHING: T-24201, 2 rows`
- [ ] Local session proves the result with the full linked build

## Notes

Result: 2 rows left (the default-block delay-slot hoist), guard kept, findings in [[matching-notes]] (T-24201 section). Branch `cloud/t24201-kangei-80134120` changes only the NON_MATCHING comment, so the default build is unchanged (sha1 `03b66d1474f31c0cd1a64f34a7829957c63d84b3` in the cloud compile). Code review against `CODING_STANDARDS.md` and the local linked build check are still due.

## Comments

- 2026-10-20 review (orchestrator, CODING_STANDARDS.md): cloud branch changed only the guard note and wiki; no asm or game data; merged with a clean 27/27 build and header/global/prototype checks. No match; the guard stays. Done.
