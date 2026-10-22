---
id: T-25040
title: Match MAIN flag dispatcher
status: Done
assignee: Astra counter worker
created: 2026-10-22
updated: 2026-10-22
links: ["[[tickets/T-25036-main-counter-late-fold]]", "[[tickets/T-23003-w5-big-p03]]"]
---

## Goal

Match only `func_8006D138` in `src/main/8006CB30.c`, preserving the committed T-25036 match. Its 872 text bytes remain assembly; investigate the flag-word snapshot and persistent GameState base using current whole-TU evidence.

## Acceptance criteria

- [x] Literal guarded/default C match and affected MAIN checksum.
- [x] Declaration checks and independent standards/spec review.
- [x] Source-only changes, no compiler exceptions or extra observable operations.

## Notes

Selected from the two assigned functions before source edits. D138's latest ledger has 219 candidate / 218 target instructions, initial flag load recovered but base/address allocation incorrect. The alternative `func_8007132C` has 984/998 words and cost 892 in T-24024; it remains outside this ticket's scope.

## Comments

- 2026-10-22: claimed in the existing isolated tree after pinning T-25036. Read prior T-23003/T-24301/T-24600 evidence. Replaying the latest recoverable typed snapshot precedes new source experiments.
- Reconstructed 219/218-word full-TU shape with explicit snapshot, preserved empty date tests and combined flag/base entry test. Direct bit-field snapshot keeps target temporary numbering but base and flag registers differ; an extra initial flag-address load remains. Late-zero pointer identity keeps base-relative flag loads, but splits the base into separate allocator ranges and grows the candidate to 229 words. Production source is unchanged; allocator tracing follows this causal distinction.
- The empty entry switch triggers the existing unsigned-switch copy boundary and restores 218/218 words. Best `direct-entry-switch-bits` costs 130 aligned edits: year/base are constrained and allocated first, while the initial flags expression is unconstrained. The resulting register cycle is year v0→v1, base v1→a0, flags a0→v0. One b7 reload is also absolute instead of base-relative. Late-zero output indices recover that register cycle but introduce alias reloads and 255 words. Absolute date/store spellings and volatile output stores do not solve it. Current tests extend the snapshot's live range through late-folded store-value identities, without changing output addresses.
- Constraint-count instrumentation gives the decisive boundary: year/base each have22 edges, the leaf's total-color threshold; flags only5. Replacing the empty year98 comparison constant with `0x62 + ((u32)gs->unk_03F < 0)` removes one unprofitable constant range and recovers every target register. `drafts/T-25040/near.c` preserves 218/218 words,215 exactly equal; only the first b7 load's address and its adjacent branch/delay slot differ (3 instructions,6 printed rows). This is still not a literal match. Reproduction and negatives are in `drafts/T-25040/README.md`; continue the b7 boundary.

- Root independent replay on current main (2026-10-22): copied immutable direct-98-latezero body into current full translation unit and rebuilt in Docker. Reproduces218 target/218 candidate words, six diagnostic lines at rows72–74 (three changed words), all other215 exact. Production remains assembly; worker continues final address/scheduling residue.

- Root independently replayed newer snapshot-98-b7-late-address on current main:218/218words,cost4, only rows116–117 differ (nop/base-relative lbu versus lui/absolute lbu). Saved snapshot-near.c and replay-snapshot.py in drafts/T-25040. All216otherwords exact; no promotion. Worker continues final byte-address representation.

- Closure: explicit initial flags snapshot plus a late-zero first b7 address reduces the residue to the first b8 byte load. Giving the real b8 read a late-zero address alone moves the static representation to another live word read. The added `if ((u32)gs->unk_F64[1] < 0) {}` holds a discarded static view at the exact byte width and offset; ugen removes it, while the real byte load remains base-relative. All218 words now agree. Every unsigned comparison is false, every pointer offset is zero, and no extra runtime operation is emitted. Existing empty date controls are collectively FAKE-annotated with the new controls.
- Final annotated source: guarded and default whole-TU C each pass872 literal linked bytes at0x8006D138 and the entire MAIN SHA1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26`. Headers/global migration pass; prototype branch0new/15known. Production is default C. Root reviewed the unannotated matching body with no semantic issue and is checking final annotations/standards. Ticket moves to In Review for independent final review and full27 integration, both owned by root.
- Root reviewed the final annotated source against standards/spec: no findings. Checked fixed-width C89, existing aligned flag view, defined unsigned comparisons and zero offsets, exact b8 byte mapping, and complete FAKE coverage. Awaiting only root's full27 integration gate; keep In Review until that passes.

Root acceptance2026-10-22: independent literal default-C872bytes and fresh27/27 binary checksums pass. Headers/globals and prototypes0new/15known pass; final standards/spec review has no findings. Moved Done. Prior snapshot replay now supports the promoted source. Worker continues reservedT-25052.

### Compiler-option explanation correction (T-25052)

The accepted source still passes its literal proof. Actual input ucode captured
in the T-25052 whole-TU replay has `OPTN 405 1` before this function: zcopy is on.
The earlier README explanation that the empty switch enables zcopy off was
incorrect. The empty switch changes entry control flow; its selector is removed
by cfe. No source or compiler changes are needed for this documentation correction.
