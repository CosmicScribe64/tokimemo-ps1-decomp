---
id: T-14200
title: Recover MAIN source families
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-breakthroughs]]", "[[main-family-results]]"]
---

## Goal
Recover verified game-code bytes in MAIN from 0f117230, excluding 8004F870.c and 8007C030.c. No overlay edits.

## Acceptance criteria
- Literal full-TU default linked proof for accepted functions and MAIN SHA1.
- Headers, globals, prototype guards; preserve existing C and generated assembly.
- Standards review before Done. Failed probes remain ignored.

## Notes
Existing image only, Docker analysis/build, one compiler. Prior broad default sweeps will not be repeated.

## Comments

### Initializer checkpoint
func_800412E0: 676 default full-TU literal linked bytes, MAIN SHA1 OK. Evidence and rejected focused probes in [[main-family-results]]. Only added shared prototype is the existing void func_80041840 definition. No generated assembly or existing C bodies modified.
Header/global guards pass; prototype guard: 0 new / 15 known disagreements, branch check OK (build/main14200/412-guards.log). MAIN SHA1 independently checked by ninja. Review pending; work continues.

### Character initializer checkpoint
func_80041C2C: 796 default linked bytes, MAIN SHA1, headers/globals pass; prior 676-byte initializer reverified. Existing GameState pointer and CharFlags suffice; no new type layout. Cumulative 1472 bytes. Evidence in [[main-family-results]].

### Review corrections
Initial standards/spec reviews identified an unguarded prototype, unexplained strncpy casts and match-directed literal masks. Header guard fixed, casts removed, final source uses actual GsDateBits/CharFlags.f20 fields. Both literal functions and MAIN SHA1 reverified (bitfield-proof.log, bitfield-build.log). Re-review pending. No initializer FAKE remains.

### Background checkpoint
Default literal matches 578F4 (1064), 57D28 (1648), 58398 (2440), MAIN SHA1 OK. Reuse existing SPRT plus paired size stores and one sprite pointer. The latter two explicitly carry FAKE unused-word frame/home workarounds (4088 bytes); 578F4 needs no FAKE. Cumulative 6624 literal bytes; 2536 without new FAKE. All 22 original C bodies preserved verbatim. Evidence in [[main-family-results]]; final guards/review pending.

### Final acceptance and review
- Five final full-TU default literal proofs pass: 676, 796, 1064, 1648 and 2440 bytes, total 6624 (build/main14200/final-proof.log).
- All 27 binary checksum checks freshly pass after the RGB cast cleanup (final-confirm-build.log); the preceding full shared-header rebuild also passes (final-build.log).
- Headers/globals pass on final source (final-source-guards.log). Prototype guard: 0 new / 15 known, branch check OK (final-guards.log); cleanup changes no declarations.
- All 22 original function bodies in touched files remain verbatim (body-audit.log). No overlay/reserved-file/ASM/compiler/coverage edits; no push or PR.
- Code-review skill, GPT-6.1 Sol medium: standards and spec axes pass. Initial findings were resolved with header guard correction, actual date/nibble bitfields, pointer argument cleanup and removal of signed RGB casts. Final verification satisfies the reviewers' build condition.
- Two FAKE frame-home locals remain honestly documented (4088 bytes); 2536 bytes have no new FAKE.
- Final census on this isolated baseline: 5887/6958 game functions, 1414152/2279368 game bytes (62.0%). This completes the bounded checkpoint, not the wider 80% objective or all MAIN functions.

Integration: local branch codex/main-14200, based on 0f117230. Commits d7c2e35d, 5991ac30, 86584044 and 6236f995 plus final review/validation commit must be integrated together so the reviewed source supersedes intermediate forms. Cross-chat status tool delivery was rejected by automatic approval review; no workaround attempted.
