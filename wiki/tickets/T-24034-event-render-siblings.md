---
id: T-24034
title: Port EVENT rendering siblings
status: In Review
assignee: complete_render_family
created: 2026-10-17
updated: 2026-10-17
links: ['[[event-family-results]]']
---

## Goal

Port func_800FEF74 (940 B), func_800FFA4C (404 B), and if time func_800F9730 (464 B), using matched DATE siblings. Bounded twenty-five minute pass, one compiler.

## Acceptance criteria

- Unguarded C source gate, normal default linked literal proof and EVENT checksum for any claimed win.
- Shared-header changes only after root notice of exact types.
- Independent standards review before Done.

## Notes

Exclusive sources src/ovl/EVENT/800FDC20.c and src/ovl/EVENT/800F9680.c. Read T-9530/T-9710/T-20600 negatives for func_800F9730, plus matched DATE siblings. Earlier drafts failed before recovered SDK polygon types.

## Results

All three are unguarded C literal matches (1,808 B): func_800FEF74 940 B, func_800FFA4C 404 B, func_800F9730 464 B. The full EVENT checksum passes.

The large sibling uses DATE func_80152268/T-17204 with legacy frame counter D_800B1AE4, byte selector table D_801218DC, random API func_800789A0, draw callback func_800FF320, and page helper func_8001A0CC. Argument-site s16 casts from DATE had to be removed: the target passes promoted ints and the casts otherwise generate extra narrowing. The inherited FAKE j=v dead copies remain explicitly documented; no new trick was needed.

Both renderers reuse SDK POLY_G3/POLY_G4 and the existing EventStateView coordinate arrays, legacy viewport index and ordering-table base. The quad replaces its sibling's unused stack word with a meaningful state pointer. Explicit colour masks/source-line scheduling are marked FAKE with T-20600/T-24034 provenance. Main_api.h adds only void func_80067628(); after root type notice: target passes the polygon in a0, ignores the result, and the matched sibling uses a void initializer. No other declaration or definition of that unknown legacy name was found. EVENT.h unchanged.

Self-review against CODING_STANDARDS.md: C89 and SDK/recovered types, target INCLUDE_ASM absent for all three, normal toolchain and flags, explicit FAKE comments, no raw data/assembly, no renamed legacy semantics. Awaiting independent root standards review and all-overlay integration.

Final validation: source gates pass for all three targets; default literal proofs match 940/404/464 B. EVENT SHA1 `ff7184d7e92b7aff372450ac3b8c2bdd42e55a5e`. Headers, globals, main_api and whitespace checks pass. Root reviewed the production diff as consistent with standards before pinning; integration remains with root.
