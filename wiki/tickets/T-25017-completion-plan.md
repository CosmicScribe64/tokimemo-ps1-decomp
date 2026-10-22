---
id: T-25017
title: Reconcile the census and execute the evidence-driven completion plan
status: In Progress
assignee: orchestrator
created: 2026-10-22
updated: 2026-10-22
links: [[[plan-remaining]], [[status]], [[tickets/T-25016-candidate-reconstruction]]]
---

## Goal

Finish the remaining game-code matches using the new research without repeating exhausted sweeps. Planning is complete; one Sol medium lane is reproducing the KANGEI near match while the orchestrator recovers remote artifacts.

## Acceptance criteria

- [ ] Recover the remote queue, candidates, ledger and traces; reconcile unique unit/function identities against current source.
- [ ] Record reproducible whole-TU resolved baselines for the selected near set and reconstruction candidates.
- [ ] Execute two file-disjoint Sol 6.1 medium lanes, escalating only precise causal blockers to Astra.
- [ ] Promote only literal default-C matches passing all 27 executable checksums and declaration guards.
- [ ] Final census has zero unmatched game functions and all required verification passes.

## Notes

Fetched and fast-forwarded clean main to 7c87f223e (PRs #66/#67). Docker progress on that revision: 6833/6952 functions, 2104676/2276920 bytes, 92.4%; 119 functions and 172244 bytes remain. Status previously mixed this count with an older 129-function breakdown. Remote scratch QUEUE.md and trace/candidate artifacts referenced by T-25016 are not included in the pulled tracked files; obtain them before repeating work. Compiling salvage drafts with zeroed M2C_ERROR placeholders are not semantic reconstructions. See the current execution section of [[plan-remaining]].
