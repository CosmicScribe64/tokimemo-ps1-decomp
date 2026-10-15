---
id: T-12020
title: Match cx2 TAIIKU functions
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: []
---

## Goal

Deep matching pass over fifteen cx2 functions in src/ovl/TAIIKU/80133C80.c.

## Acceptance criteria

- [ ] Matches verified by exact linked funcdiff and unit SHA1.
- [ ] Retained drafts guarded with measured differences.
- [ ] Clean build, prototype checks and standards review.

## Notes

Ownership: src/ovl/TAIIKU/80133C80.c plus include/config/wiki. Work cutoff 21:45 UTC.

## Comments

- 2026-10-15: Baseline configure plus ninja completed: 27/27 SHA1, headers OK, globals OK. Read wiki, workflow, cheat sheet, standards and local instructions. Known cases include alias hoisting, counter scheduling and decrement register allocation.

### func_80135DF8: MATCH, 324 bytes

A TaiRandomTable assignment and nested ten-by-three actor loops reproduce the target. Actor index row * 3 + column + 46 preserves the multiply sequence. No FAKE required. Docker ninja: TAIIKU.bin OK; funcdiff --resolve: exact linked 324-byte MATCH.

### Initial guarded-draft experiments

func_801368B8 remains four +/- diff rows (two instruction positions) after 145 source-line and increment-form variants, all full-file scored. func_801346FC remains eight rows (four positions) after return-type and decrement-local variants. Sixteen explicit hoist/local/member variants did not improve func_80137228; keep these compiler-gap timeboxes bounded.
