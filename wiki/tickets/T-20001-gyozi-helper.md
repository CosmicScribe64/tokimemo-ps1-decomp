---
id: T-20001
title: Match the GYOZI dialogue helper from its SHOUGATU sibling
status: Done
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

The direct sibling port, with nested conditions and the post-increment tested in place, still had six counter-register rows different: old counter in v1 and boolean in v0, rather than a0/v1. Pointer, union and containing-record counter views did not fix this. The setup callee func_801343A4 needs an s32 return type: declaring it as s32 in the shared header and definition gives the original a0/v1 registers and also preserves the callee and all seven other callers. The return-type correction is sufficient; a file-local caller override is unnecessary.

The final implementation uses plain s32 func_801343A4(void) in include/ovl/GYOZI.h and src/ovl/GYOZI/80134000.c. Removed the GYOZI_OVERRIDE define, header guard and local prototype from the initial submission, as requested by maintainer review and CODING_STANDARDS.md section 8a. The callee remains a fall-through function without an explicit return value. No main_api.h changes, new aggregates, fakematches or compiler-pass changes.

Verification in Docker: tools/funcdiff.py GYOZI:func_8013506C --resolve reports MATCH (linked function bytes equal target, 172 bytes) on the actual default build, without --proto. The full ninja build passed GYOZI SHA1 and globals; explicit SHA1 checks of every built binary against config/*.sha1 and config/overlays/*.sha1 passed 27/27. Funcdiff's header gate also passed. Build and scratch logs remain under gitignored build/T-20001/.

## Comments

2026-10-16 review correction: the initial self-review missed the simpler shared-header return type. Maintainer review showed the plain s32 declaration and definition also match; the unnecessary local override is removed. C89, existing fixed-width types and symbols retained, no generated asm edits or game data staged. The s32 fall-through shape matches the sibling. The full-unit SHA1 verifies the literal's placement, callee and other callers as well as the matched callback. Historical T-0018 rows are retained and superseded by the update after the Rules block. Ticket remains In Review for maintainer acceptance.

Initial-submission history (before the shared-return-type review fix): integrated upstream main 9e74085b (DATE package merges); shared wiki conflicts were resolved by retaining both sides. Verification after integration: actual default funcdiff MATCH (172 linked bytes), 8-step incremental ninja build with GYOZI and DATE SHA1 OK, explicit 27/27 binary SHA1 checks, and sync_protos --check-branch OK (0 new / 15 known disagreements). Submitted PR #37; maintainer review pending.

Refreshed PR #37 against upstream main 81095a46. Shared index/log/matching-note conflicts preserve both sides; no production source conflicts. Actual default linked proof still matches all 172 bytes, the full 490-step ninja rebuild passes every binary SHA1 and build guard, and the explicit verifier reports 27/27 again (build/T-20001/refreshed-*.log, gitignored). No changes to the reviewed GYOZI implementation or caller view.

Maintainer review follow-up verification (2026-10-16): replaced the local override with plain shared s32 func_801343A4(void) in both header and definition. Actual default linked funcdiff proves callback func_8013506C MATCH (172 bytes) and setup callee func_801343A4 MATCH (124 bytes). The full GYOZI SHA1 preserves all seven other callers; ninja passes its 67 steps and globals, and explicit binary verification passes 27/27. Prototype branch guard: 0 new disagreements, 15 known, branch check OK. Evidence: gitignored build/T-20001/review-{proof,build,checksums,protos}.log. Final local standards review confirms one shared declaration, no override/local duplicate, C89 and intended source/header/wiki files only. Ticket remains In Review for maintainer acceptance. Per maintainer request, no further main merge; they will resolve wiki conflicts upstream.

2026-10-16, maintainer review (PR #37): review point addressed (plain s32 func_801343A4 in header and definition, override removed, wiki corrected). Verified on the integration base: 27/27, headers, globals, prototypes OK, funcdiff MATCH (172 bytes). Status Done.
