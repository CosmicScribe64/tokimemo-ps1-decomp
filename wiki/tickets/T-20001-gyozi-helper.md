---
id: T-20001
title: Match the GYOZI dialogue helper from its SHOUGATU sibling
status: In Review
assignee: Codex
created: 2026-10-16
updated: 2026-10-16
links: ["[[matching-notes]]", "[[data/t0018-cases]]", "[[shougatu-family-results]]"]
---

## Goal

Match GYOZI func_8013506C (172 bytes) in src/ovl/GYOZI/80134FD0.c using the already matched SHOUGATU func_8013315C in src/ovl/SHOUGATU/801330C0.c as the source model. The whole-tree duplicate report identifies this remaining pair, including the embedded string; the existing GYOZI draft is guarded under T-10850.

## Acceptance criteria

- [x] Resolved function diff proves exact linked bytes and GYOZI checksum passes.
- [x] All 27 binary checksums pass; header/global checks pass.
- [x] Prototype branch guard: 0 new disagreements, 15 known.
- [x] Local review against CODING_STANDARDS.md; awaiting upstream review.
- [x] Submit a focused upstream PR without generated files or game data: https://github.com/CosmicScribe64/tokimemo-ps1-decomp/pull/37.

## Notes

Branch codex/match-gyozi-helper starts from upstream main 639693d2. This contribution is independent of the scoped-queue fix in PR #36 (T-20000), whose description includes the requested communication-platform question. TT is excluded. All decompilation/build tools run in the existing pinned Docker image via WSL; the supplied CHD is gitignored.

The direct sibling port, with nested conditions and the post-increment tested in place, still had six counter-register rows different: old counter in v1 and boolean in v0, rather than a0/v1. Pointer, union and containing-record counter views did not fix this. The sibling's first callee is declared s32, while GYOZI's func_801343A4 is declared void. An in-memory s32 func_801343A4(void) prototype made all 172 linked bytes match; changing the sound callee's return type alone did not.

Kept the required caller view through GYOZI_OVERRIDE_func_801343A4 in this C file and a guard around the normal void prototype in include/ovl/GYOZI.h, following CODING_STANDARDS.md section 8a. The actual void definition in src/ovl/GYOZI/80134000.c and all other callers keep their existing view. No main_api.h changes, new types, fakematches or compiler-pass changes.

Verification in Docker: tools/funcdiff.py GYOZI:func_8013506C --resolve reports MATCH (linked function bytes equal target, 172 bytes) on the actual default build, without --proto. The full ninja build passed GYOZI SHA1 and globals; explicit SHA1 checks of every built binary against config/*.sha1 and config/overlays/*.sha1 passed 27/27. The header guard also passed funcdiff's header gate. Build and scratch logs remain under gitignored build/T-20001/.

## Comments

2026-10-16 local review against CODING_STANDARDS.md: no findings. C89, existing fixed-width types and symbols retained, no generated asm edits, no game data staged. The local overlay prototype override uses the documented section 8a shape and explains its evidence. The existing s32 fall-through function shape matches the sibling. The full-unit SHA1 verifies the literal's placement and the other functions in the translation unit as well as the matched callback. Historical T-0018 rows are retained and superseded by a new update. Ticket remains In Review for upstream review.

Integrated upstream main 9e74085b (DATE package merges); shared wiki conflicts were resolved by retaining both sides. Final verification after integration: actual default funcdiff MATCH (172 linked bytes), 8-step incremental ninja build with GYOZI and DATE SHA1 OK, explicit 27/27 binary SHA1 checks, and sync_protos --check-branch OK (0 new / 15 known disagreements). Submitted PR #37; maintainer review pending.

Refreshed PR #37 against upstream main 81095a46. Shared index/log/matching-note conflicts preserve both sides; no production source conflicts. Actual default linked proof still matches all 172 bytes, the full 490-step ninja rebuild passes every binary SHA1 and build guard, and the explicit verifier reports 27/27 again (build/T-20001/refreshed-*.log, gitignored). No changes to the reviewed GYOZI implementation or caller view.
