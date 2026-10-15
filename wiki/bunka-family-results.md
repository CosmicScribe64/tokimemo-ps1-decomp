---
type: research
updated: 2026-10-15
---

# BUNKAKEN and BUNKASAI family reconstruction

Scope: [[tickets/T-12700-bunka-families]], from 95c3fc9f. Existing broad experiment evidence was read from the integration checkout's wiki/matching-breakthroughs.md and wiki/broad-coverage-results.md. That prior 190-target default sweep returned five functions / 5,036 bytes; this work does not repeat it.

## Aggregate address form

The existing CharFlags bitfield view fixes BUNKAKEN flag tests/stores, but a direct global member expression still leaves aggregate-base register differences in several handlers. A local `GameState *g = &D_800E6280` used for all aggregate accesses follows already matched siblings in src/ovl/BUNKAKEN/8013BFE0.c and src/ovl/BUNKAKEN/8014A6A0.c. On the first two candidates this gives zero full-TU differences; replacing only the store with the pointer does not. This is source-form reuse, not a new compiler pass or independent type discovery.

The same form gives zero full-TU differences for func_80142A40, func_80142FD4 and func_801438F8. func_80141B80 remains nonmatching (134 rows). Literal proofs and accepted counts are recorded in [[tickets/T-12701-bunkaken-address-family]]. All unsuccessful scratch variants remain in build/bunka12701.

## Reload after calls

The BUNKASAI func_80146A78 archived draft saves a calendar state byte across calls. Reloading the existing GameState field for its final comparison removes the spill and reproduces the target's frame and instruction sequence. Local GameState pointer variants worsen this candidate. Literal proof is recorded in [[tickets/T-12702-bunkasai-family]]; scratch variants remain in build/b12702.

No compiler flags, generated assembly, or shared main types are changed by these experiments. A zero candidate score is not counted as an accepted match until linked bytes and the overlay SHA1 both pass.
