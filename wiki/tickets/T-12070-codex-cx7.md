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
