---
id: T-24035
title: Match small BUNKA_SD and GEKO siblings
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[bunka-sd-family-results]]", "[[scenes-family-results]]"]
---

## Goal
Recover BUNKA_SD func_80134418/func_801344A8 and GEKO func_80134228 from matched siblings func_8013987C/func_80135AC8.

## Acceptance criteria
- [ ] Read prior negative evidence and port sibling source forms with one Docker compiler.
- [ ] Any promoted C is actually unguarded, with target fallback removed, literal default linked proof and unit checksum.
- [ ] Keep useful source-only negatives and wiki evidence; In Review pending root review.

## Notes
Start dfbf3080d, existing complete-bunkaken worktree. Only BUNKA_SD/80134260.c and GEKO/80134040.c assigned. T-12090/T-9530 fade negatives predate T-22021 promoted byte/clear match; GEKO T-17700 guard predates T-22010 sibling byte-view/dead-test match. No headers without root notice, no push/merge.

## Comments

### Verified first two matches
GEKO func_80134228: copied matched func_80135AC8 with the flag mask 0x40 and without that sibling's D_80122D4C clear. Both existing source-shaping FAKE sites are retained with T-24035: u8 read view and dead byte-result test. Actual normal C, no guard/fallback. Docker literal default linked funcdiff MATCH 244 bytes.

BUNKA_SD func_801344A8: copied matched func_8013987C with decrement 2 rather than 1. Promoted u8 views and *= 0 clear retain the sibling's disclosed FAKE reason. Actual normal C, no guard/fallback. Literal default linked funcdiff MATCH 152 bytes including original padding.

Docker ninja -j1 rebuilt both units, headers OK, BUNKA_SD and GEKO checksums OK. Existing GEKO pointer-assignment warnings at D_800CA134/D_800CA138 are outside these changed functions. New verified C: 396 bytes. Third target remains in progress.
