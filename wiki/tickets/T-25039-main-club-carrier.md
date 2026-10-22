---
id: T-25039
title: Match MAIN club-number carrier
status: Done
assignee: Astra MAIN worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25004-class-c-main-8005b43c-parameter-up-down-col2sepia]]", "[[tickets/T-25008-closers-main-8005b43c-8006ba40-parameter-up-down]]", "[[playbook]]"]
---

## Goal

Match MAIN func_8005B43C, 860 bytes in src/main/8005A0B0.c. Exclusive source ownership is this one file; GYOZI ownership released. Work from integrated main 77eae26cf in /private/tmp/tokimemo-t25035, preserving earlier private evidence.

## Acceptance criteria

- Literal default-C match and affected MAIN checksum, declaration guards and independent standards/spec review.
- No inline assembly, patched instructions, per-function compiler exceptions or semantic shortcuts.
- Root owns integration and full 27-binary gate.

## Notes

T-25004 and T-25008 preserve a three-line near miss: shifted club selector allocated a0, target v1 with argument move. Prior scalar widths, argument casts/identities, empty tests and permuter grids are exhausted. First reproduce current baseline and inspect allocation/phase evidence before a distinct source experiment. Continue until match or concrete external blocker, with no historical time/probe cap.

## 2026-10-22 measured baseline and phase results

Corrected target size: 215 instructions / 860 bytes (historical 920-byte ticket figure is stale). Baseline is 215/215 words with three changed positions / six printed +/- lines: shift destination and comparison use a0 instead of v1, then the call delay slot is nop instead of move a0,v1.

Traced full TU confirms two ranges: shift-expression bit33 already receives v1, but local club bit28 is assigned a0 by the call precolor. ugen coalesces the copy into a0. A late-folded zero argument term breaks that precolor but a separate local produces a1/a2 instead. Repeating the shift expression with its late-zero identity removes the local range: club v1 and the call move now match; only halfword base/load/source uses a0 rather than a1 (three changed positions). Investigating the halfword carrier placement from this specific new boundary. All production C remains unchanged.

## Match and review handoff

Final source compares the raw halfword shift and defines temp_v1 only inside else, BEFORE the existing empty halfword read. The earlier expression remains v1; the local call carrier can be a0 without coalescing away the target move. Placing the assignment after the empty read instead gives cost 10, confirming the lifetime boundary. No late-zero expression or ignored argument is retained.

Docker default C: MATCH, linked function bytes equal target (860 bytes). MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 passes. Headers/global/prototype checks pass; zero new disagreements, 15 known. Whole MAIN checksum covers neighboring functions in the changed TU.

Root independent standards/spec source review passed 2026-10-22: nonvolatile halfword reread crosses no effects; branch-specific definition preserves the value; existing pad/empty-read FAKEs are annotated and no new FAKE is required for this ordinary lifetime change. Moved to In Review pending root's combined integration gate. Detailed control ledger: drafts/T-25039/README.md.

## Root acceptance

Independent standards/spec review passes. Integrated default-C literal proofs860/180bytes, fresh27/27binary checksums and declaration guards pass, prototypes0new/15known. No assembly fallback. Accepted and moved to Done.
