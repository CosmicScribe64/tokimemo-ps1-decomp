---
id: T-20011
title: Match NAME_ENT pointer-coordinate hit test
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-22008-w4-p08]]", "[[name-ent-family-results]]"]
---

## Goal

Continue the completed T-22008 package's named leftovers in src/ovl/NAME_ENT/80132000.c: func_80134110 (152 bytes), func_80132320 (396 bytes), and func_80134248 (2,256 bytes). Claim #57: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/issues/57. This submission matches only the 152-byte coordinate helper; the fade and page-selection follow-up is [[tickets/T-20013-name-ent-fade-page-followup]]. Preserve neighboring functions. PR #55 (1,652-byte loader) and PR #59 (stable scoring) are now merged.

## Acceptance criteria

- [x] Recover and verify actual matching C for any promoted function.
- [x] Pass all 27 binary hashes and header/global/prototype guards.
- [x] Review C89, real field views and all necessary FAKE controls; synchronize ticket/kanban for submission.

## Notes

Start from main 567b5663, branch codex/name-ent-coordinate-selection. Consult T-22008's negative controls before source experiments. Large scene claim #50 remains In Progress on its independent branch; no new match is claimed there.

## Initial controls before the coordinate match

Full-unit resolved comparisons use consistent aligned instruction edits: coordinate baseline15, real actor-pointer view13, diagnostic bare failure exit10; no match. Fade baseline7 edits (14 printed rows); entry-state snapshots, reused induction locals, narrow copies and empty tests do not match. Character-page baseline227 aligned edits; initial byte views, signed casts, separate snapshots and a named divisor do not improve it. No production body or header changes. Ignored controls: build/T-20011. Bare exits and scalar-first pointer diagnostics are not eligible without a measured FAKE justification.


## Coordinate source recovery

A DisplayActor44 pointer to D_8011ECD0 gives the signed X/Y fields at offsets 0x26/0x2A. Read X once, test -119 <= X < 152, then read Y and test 57 <= Y < 104. Valid coordinates produce column (X + 120) / 16 and row (Y - 56) / 16; invalid coordinates set both outputs to -1.

Explicit return values forced extra return-copy ranges. An s32 function with a no-value success return and fall-through failure exit, using the real actor view, reaches zero full-unit instruction differences. The void version misses five instructions; direct fixed actor fields miss eight. The old dead y = x assignment and a temporary end label are removable with no byte differences and are removed. Only the integer exit-liveness constraint is marked FAKE with its measured reason. Callers ignore the result; one matching overlay prototype is added. Default Docker build passes (14 steps, headers/globals OK), all 27 independent SHA1 checks pass, and prototype guard passes (0 new / 15 known, branch OK). Actual default linked funcdiff reports MATCH (152 bytes); the function is plain C with no ASM fallback. Logs are ignored under build/T-20011.

## Review

Self-review against CODING_STANDARDS.md: C89 and fixed-width types; existing shared actor fields; one overlay prototype and no duplicate main declarations; no new buffers, dead initializers, forced registers or compiler changes. The sole FAKE is the integer no-value exit constraint, explained with the ticket and measured void control. All three callers use the function as a statement and discard its result. No neighboring function changes, generated ASM, game assets, binaries, permutation scripts or instruction listings submitted. Ticket/kanban In Review; maintainer review pending. Claim #57 remains open for T-20013.

- 2026-10-17 review (orchestrator, CODING_STANDARDS.md): plain C with one FAKE comment (implicit integer exit liveness; ticket and reason given), existing DisplayActor44 view, one overlay prototype, no inline asm. Merged as PR #60 in one clean build: 27/27 SHA1, headers, globals, prototypes, linked funcdiff MATCH (152 bytes). Done; the other two functions of claim #57 continue in T-20013.
