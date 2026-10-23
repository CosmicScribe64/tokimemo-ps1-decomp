---
id: T-24056
title: Match three short SHOUGATU siblings
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[shougatu-family-results]]"]
---

## Goal
Recover func_8013D820/func_8013D92C in src/ovl/SHOUGATU/8013D660.c and func_8013DF14 in src/ovl/SHOUGATU/8013DD90.c from matched GYOZI siblings func_80140244/func_80140330/func_801408C4.

## Acceptance criteria
- [x] Read target negatives/current headers and matched sibling C; one serial Docker compiler.
- [x] Promote only actual unguarded default C with literal linked proof and SHOUGATU checksum; pin wins individually.
- [x] Preserve useful source/provenance and leave In Review for root review.

## Notes
Start 5d361e017, preserve prior T-24052 matches. Only these two TUs; no header/config changes without coordination. T-9370/T-10030 D820 threshold96/permuter negatives are exhausted and not repeated. New pass uses current sibling C and canonical f9/DisplayActor44 fields rather than old byte reconstruction. T-9510/T-17500 old scores are diagnostic, not current proofs. No push/merge.

## Results

No match. Preserved canonical source-only D820/D92C/DF14 drafts reproducing62/72/12 printable full-TU rows. D820 established threshold-allocation gap remains; no repeated96-variant/permuter sweep. D92C corrected >=1 predicate restores target length; named limit remains a marked FAKE diagnostic. DF14 canonical f9 form is two words short because the first byte-load address and clamp reload/store-base choices differ. Focused pointer/byte/reload/update-lifetime probes do not recover both; ledger in drafts/T-24056/README.md and [[shougatu-family-results]].

Root conditionally approved s16 D_80145FDC extern with LH/SH evidence, but it remains scratch-only since no promotion is viable. No production/header/config changes, zero new C bytes. Final annotated files reproduce through drafts/T-24028/probe.py; unchanged default SHOUGATU checksum control passes through ASM. In Review for root archive review. Prior T-24052 source/pins preserved; no push/merge.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
