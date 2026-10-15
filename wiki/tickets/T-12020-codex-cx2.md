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

### func_80136EC0: MATCH, 380 bytes

Recovered a three-entry u16 table (TaiWait3, size 6) at D_801497F4. The signed switch selector preserves separate case-1 and byte-store constants, and random-result-first addition gives the original temporary ordering. Exact linked funcdiff: 380-byte MATCH. Full header-triggered rebuild: 27/27 checksums, headers/globals OK. No FAKE required.

Traced regdiff uses the existing trace compiler copied by the orchestrator into build/scratch/r9-uopt/variants/trace (source /private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/b1bfb9f2-ead9-439a-9858-e4d32a33c9ae/scratchpad/r9-uopt/variants/trace). It runs successfully: func_801346FC has 421/421 instructions and four register-only positions; local M123-212 outranks the selector range. Selector-width/lifetime variants did not improve the eight listing rows.

### func_80135AD0: MATCH, 596 bytes

Recovered eleven signed character IDs at D_80149558 and an eleven-word eligible-character list. A named row+9 byte-store value gives the original frame homes without unused locals. Declare the eligible array, used temporary, then the copied table to recover exact stack offsets. Store the sprite-resource ID before scale fields and the y coordinate last: the full-file order search reaches zero. No FAKE required. Docker targeted ninja: TAIIKU.bin SHA1, headers and globals pass; funcdiff: exact linked 596-byte MATCH.

Other scratch results: func_80136BB4 reaches 32 +/- rows after table/local declaration ordering; typed func_80137544 remains 103+ rows, func_80135480 160 rows, func_80135830 135 rows, func_80137EA8 254 rows and func_801365C8 228 rows. These broad drafts remain scratch-only pending deeper passes.
