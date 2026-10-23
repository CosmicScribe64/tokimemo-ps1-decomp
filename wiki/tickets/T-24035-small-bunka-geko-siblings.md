---
id: T-24035
title: Match small BUNKA_SD and GEKO siblings
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[bunka-sd-family-results]]", "[[scenes-family-results]]"]
---

## Goal
Recover BUNKA_SD func_80134418/func_801344A8 and GEKO func_80134228 from matched siblings func_8013987C/func_80135AC8.

## Acceptance criteria
- [x] Read prior negative evidence and port sibling source forms with one Docker compiler.
- [x] Any promoted C is actually unguarded, with target fallback removed, literal default linked proof and unit checksum.
- [x] Keep useful source-only negatives and wiki evidence; In Review pending root review.

## Notes
Start dfbf3080d, existing complete-bunkaken worktree. Only BUNKA_SD/80134260.c and GEKO/80134040.c assigned. T-12090/T-9530 fade negatives predate T-22021 promoted byte/clear match; GEKO T-17700 guard predates T-22010 sibling byte-view/dead-test match. No headers without root notice, no push/merge.

## Comments

### Verified first two matches
GEKO func_80134228: copied matched func_80135AC8 with the flag mask 0x40 and without that sibling's D_80122D4C clear. Both existing source-shaping FAKE sites are retained with T-24035: u8 read view and dead byte-result test. Actual normal C, no guard/fallback. Docker literal default linked funcdiff MATCH 244 bytes.

BUNKA_SD func_801344A8: copied matched func_8013987C with decrement 2 rather than 1. Promoted u8 views and *= 0 clear retain the sibling's disclosed FAKE reason. Actual normal C, no guard/fallback. Literal default linked funcdiff MATCH 152 bytes including original padding.

Docker ninja -j1 rebuilt both units, headers OK, BUNKA_SD and GEKO checksums OK. Existing GEKO pointer-assignment warnings at D_800CA134/D_800CA138 are outside these changed functions. New verified C: 396 bytes. Third target remains in progress.

## Independent root verification

Reviewed production changes against CODING_STANDARDS.md. SDK/aggregate types and explicit FAKE annotations retained; no unresolved findings. All27 default hashes, header/global/prototype guards and actual unguarded C literal proofs passed before publication. Only the proven targets count as coverage.
### Fade-in near miss and final checkpoint
func_80134418 remains guarded, now at four printable diff rows: target 36 instructions, candidate 38. Promoted byte views plus a marked FAKE clear/add cap keep the target's counter and pointer registers; only the cap's redundant andi/move and changed branch offset remain. Plain/signed cap, unsigned constants, local cap, logical cap and algebraic forms failed; details and source-only near draft in drafts/T-24035/.

Reproduction of the final annotated guard gives four rows. Docker ninja -j1 rebuilt BUNKA_SD with headers/checksum OK; GEKO remains exact. Both actual C wins are rechecked with literal default linked proof commands. Neither matched target has a guard/fallback. Total new verified C remains 396 bytes; guarded fade-in contributes zero.

In Review pending root standards review/integration. No push/merge; all exploration preserved under ignored build/T-24035.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
