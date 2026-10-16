---
id: T-12070
title: Codex cx7 TACO actor matching
status: In Review
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over the nine functions in ../codex/cx7-list.txt. Own only src/ovl/TACO/8015ABF0.c. Prioritize verified bytes from the 1–2.4 KB functions and reusable actor-family patterns; bound historical small compiler-gap retries.

## Acceptance criteria

- [x] Investigate all assigned functions and record discarded/guarded drafts.
- [x] Every promotion has literal linked-byte proof and overlay checksum verification.
- [x] Final 27 binary checksums, headers/globals and branch prototypes pass.
- [ ] Submit for independent standards review.

## Notes

Baseline Docker configure/ninja -j2 running; build/scratch/baseline.log. Traced uopt supplied by orchestrator at build/scratch/r9-uopt/variants/trace. Read existing compiler-gap notes; no new production compiler change. External research report suggests scratch-only AS1 diagnostics after fidelity controls; no such experiment has been run here.

## Comments

Review required before Done.

Baseline verified: Docker configure/ninja -j2 passes 27/27 binary checksums, headers and globals (build/scratch/baseline.log).

Initial full-file pass repaired saved drafts' missing by-value TcPos arguments and incorrect actor-pointer scaling. B59C (2,392 bytes) fell from 550 differing rows to 61 after unsigned flag writes, eliminating a decompiler-created backward goto, and moving the position local after scalar temporaries. Current draft remains scratch-only; C718 165, C2CC 307, ACCC 508 rows after compilation repairs. Saved symbol declarations and missing prototypes are scoped to scratch candidates pending actual promotion. Evidence: build/scratch/{initial-score,rescore-large,b59variants,b59v2}.log and B59C-v2.diff.

### func_8015B59C matched — 1,940 linked bytes

Production `ninja -j2 build/ovl/TACO.ok` passes SHA1; default resolved funcdiff reports literal linked MATCH, 1,940 bytes (worklist's 2,392-byte estimate is stale). Natural indexed loop, direct actor reloads, unsigned byte flag stores and inline switch expression eliminate saved draft artifacts; TcPos reproduces the by-value particle calls. Byte-offset actor views remain FAKE-labeled because replacing them with direct TcActor fields differs by 30 rows. No compiler change. Proof: build/scratch/B59C-production.log; progression b59v3/v4/v5/b59clean logs. One match, 1,940 bytes so far.

### func_8015C718 matched — 1,340 linked bytes

Production default resolved funcdiff reports literal MATCH (1,340 bytes, versus worklist estimate 1,364); TACO SHA1 passes. Repaired TcPos by-value calls, removed cached actor pointers across random calls, and reversed commutative position/random additions to recover original scheduling. The last six selector rows required a direct u16 load: casting an s16 load to u16 produces the same load instruction but a different uopt register web. Byte-offset views are FAKE-labeled. Evidence: build/scratch/{c718v2,c718v4,c718selector}.log, C718-v4.diff and C718-production.log. Total: two matches, 3,280 linked bytes.

### func_8015C2CC matched — 972 linked bytes

Production resolved funcdiff literal MATCH and TACO SHA1 pass (build/scratch/C2CC-production.log). Actual size 972 versus worklist estimate 1,040. Direct actor rereads and byte-offset views reduced 307 rows to 9; direct u8 switch read plus explicit case 0 restores the 17-entry jump-table range. The last pair of reversed loads matches when the case-13 call and following load share a source line. These source-shape requirements are FAKE-labeled. Table declarations and the C2CC prototype live in the overlay header; redundant array-address casts removed and reverified. Total: three matches, 4,252 bytes.

User steering through orchestrator now authorizes longer (~45 minute) focused passes on the largest remaining functions, including beyond-list functions in the owned object. Inventory confirms no extra larger targets: ACCC 1,260 bytes; BD30 576 bytes; all others <=360 bytes. Prioritize those two over tiny compiler-gap chases.

## Handoff

Three matches / 4,252 actual linked bytes: B59C 1,940; C718 1,340; C2CC 972. All three contain documented FAKE byte-offset actor views; C2CC also preserves case 0 and source-line grouping. No compiler changes or copyrighted data added.

Remaining full-file evidence: existing B1B8 guard 16 rows, C0A0 9 rows, BF70 32 rows, remeasured in build/scratch/final-guards.log. Existing guards are unchanged, including the pre-existing BF70 borderline 32-row draft. Scratch-only: ACCC 411 best (acccvariants/3, scalar lifetime/table reconstruction incomplete); BD30 128 best (bd30variants/3, loop induction and register allocation differ); CE64 73 initial rows. No larger new guards retained. All nine worklist functions received full-file scoring; tiny misses did not receive extended retries after user byte-priority steering.

Orchestrator directed wrapping this package promptly for an 8,672-byte dialogue target in a separate worktree. No main-object edits were made here. Follow-up opportunities: ACCC requires replacing split m2c loop variables and recovering shared table/part types; BD30 omits the original first-loop induction increment and generates multu instead of shift/add indexing. Native AS1 diagnostics/traced uopt were available but not needed for these matches.

Verification logs: build/scratch/final-configure.log, final-clean.log, final-build.log, final-prototypes.log, final-guards.log, plus each function's *-production.log. Branch prototype audit: 0 new disagreements, 15 known. Full baseline-to-HEAD diff check passes. Await independent standards review; do not mark Done.

Final clean validation completed: configure succeeds; forced sha1/osha1/header/global stamp rebuild runs 526 ninja tasks and passes 27/27 binary checksums, headers and globals. Both final build and prototype audit exit 0.
