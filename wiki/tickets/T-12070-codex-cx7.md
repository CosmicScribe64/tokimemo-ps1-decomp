---
id: T-12070
title: Codex cx7 TACO actor matching
status: In Progress
assignee: Codex cx1
created: 2026-10-15
updated: 2026-10-15
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Deep matching pass over the nine functions in ../codex/cx7-list.txt. Own only src/ovl/TACO/8015ABF0.c. Prioritize verified bytes from the 1–2.4 KB functions and reusable actor-family patterns; bound historical small compiler-gap retries.

## Acceptance criteria

- [ ] Investigate all assigned functions and record discarded/guarded drafts.
- [ ] Every promotion has literal linked-byte proof and overlay checksum verification.
- [ ] Final 27 binary checksums, headers/globals and branch prototypes pass.
- [ ] Submit for independent standards review.

## Notes

Baseline Docker configure/ninja -j2 running; build/scratch/baseline.log. Traced uopt supplied by orchestrator at build/scratch/r9-uopt/variants/trace. Read existing compiler-gap notes; no new production compiler change. External research report suggests scratch-only AS1 diagnostics after fidelity controls; no such experiment has been run here.

## Comments

Review required before Done.

Baseline verified: Docker configure/ninja -j2 passes 27/27 binary checksums, headers and globals (build/scratch/baseline.log).

Initial full-file pass repaired saved drafts' missing by-value TcPos arguments and incorrect actor-pointer scaling. B59C (2,392 bytes) fell from 550 differing rows to 61 after unsigned flag writes, eliminating a decompiler-created backward goto, and moving the position local after scalar temporaries. Current draft remains scratch-only; C718 165, C2CC 307, ACCC 508 rows after compilation repairs. Saved symbol declarations and missing prototypes are scoped to scratch candidates pending actual promotion. Evidence: build/scratch/{initial-score,rescore-large,b59variants,b59v2}.log and B59C-v2.diff.

### func_8015B59C matched — 1,940 linked bytes

Production `ninja -j2 build/ovl/TACO.ok` passes SHA1; default resolved funcdiff reports literal linked MATCH, 1,940 bytes (worklist's 2,392-byte estimate is stale). Natural indexed loop, direct actor reloads, unsigned byte flag stores and inline switch expression eliminate saved draft artifacts; TcPos reproduces the by-value particle calls. Byte-offset actor views remain FAKE-labeled because replacing them with direct TcActor fields differs by 30 rows. No compiler change. Proof: build/scratch/B59C-production.log; progression b59v3/v4/v5/b59clean logs. One match, 1,940 bytes so far.
