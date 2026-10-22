---
id: T-25040
title: Match MAIN flag dispatcher
status: In Progress
assignee: Astra counter worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25036-main-counter-late-fold]]", "[[tickets/T-23003-w5-big-p03]]"]
---

## Goal

Match only `func_8006D138` in `src/main/8006CB30.c`, preserving the committed T-25036 match. Its 872 text bytes remain assembly; investigate the flag-word snapshot and persistent GameState base using current whole-TU evidence.

## Acceptance criteria

- [ ] Literal guarded/default C match and affected MAIN checksum.
- [ ] Declaration checks and independent standards/spec review.
- [ ] Source-only changes, no compiler exceptions or extra observable operations.

## Notes

Selected from the two assigned functions before source edits. D138's latest ledger has 219 candidate / 218 target instructions, initial flag load recovered but base/address allocation incorrect. The alternative `func_8007132C` has 984/998 words and cost 892 in T-24024; it remains outside this ticket's scope.

## Comments

- 2026-10-22: claimed in the existing isolated tree after pinning T-25036. Read prior T-23003/T-24301/T-24600 evidence. Replaying the latest recoverable typed snapshot precedes new source experiments.
- Reconstructed 219/218-word full-TU shape with explicit snapshot, preserved empty date tests and combined flag/base entry test. Direct bit-field snapshot keeps target temporary numbering but base and flag registers differ; an extra initial flag-address load remains. Late-zero pointer identity keeps base-relative flag loads, but splits the base into separate allocator ranges and grows the candidate to 229 words. Production source is unchanged; allocator tracing follows this causal distinction.
- The empty entry switch triggers the existing unsigned-switch copy boundary and restores 218/218 words. Best `direct-entry-switch-bits` costs 130 aligned edits: year/base are constrained and allocated first, while the initial flags expression is unconstrained. The resulting register cycle is year v0→v1, base v1→a0, flags a0→v0. One b7 reload is also absolute instead of base-relative. Late-zero output indices recover that register cycle but introduce alias reloads and 255 words. Absolute date/store spellings and volatile output stores do not solve it. Current tests extend the snapshot's live range through late-folded store-value identities, without changing output addresses.
- Constraint-count instrumentation gives the decisive boundary: year/base each have22 edges, the leaf's total-color threshold; flags only5. Replacing the empty year98 comparison constant with `0x62 + ((u32)gs->unk_03F < 0)` removes one unprofitable constant range and recovers every target register. `drafts/T-25040/near.c` preserves 218/218 words,215 exactly equal; only the first b7 load's address and its adjacent branch/delay slot differ (3 instructions,6 printed rows). This is still not a literal match. Reproduction and negatives are in `drafts/T-25040/README.md`; continue the b7 boundary.

- Root independent replay on current main (2026-10-22): copied immutable direct-98-latezero body into current full translation unit and rebuilt in Docker. Reproduces218 target/218 candidate words, six diagnostic lines at rows72–74 (three changed words), all other215 exact. Production remains assembly; worker continues final address/scheduling residue.
