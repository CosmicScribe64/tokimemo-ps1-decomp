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

Finish the remaining game-code matches using the new research without repeating exhausted sweeps. The 2026-10-22 multi-chat plan in [[plan-remaining]] records the recovered-artifact audit, two causal pilots and three proposed file-disjoint worker lanes. Pilot checkpoints are awaiting integration; no new chats dispatched by this planning pass.

## Acceptance criteria

- [ ] Recover the remote queue, candidates, ledger and traces; reconcile unique unit/function identities against current source.
- [ ] Record reproducible whole-TU resolved baselines for the selected near set and reconstruction candidates.
- [ ] Execute two file-disjoint Sol 6.1 medium lanes, escalating only precise causal blockers to Astra.
- [ ] Promote only literal default-C matches passing all 27 executable checksums and declaration guards.
- [ ] Final census has zero unmatched game functions and all required verification passes.

## Notes

Fetched and fast-forwarded clean main to 7c87f223e (PRs #66/#67). Docker progress on that revision: 6833/6952 functions, 2104676/2276920 bytes, 92.4%; 119 functions and 172244 bytes remain. Status previously mixed this count with an older 129-function breakdown. Remote scratch QUEUE.md and trace/candidate artifacts referenced by T-25016 are not included in the pulled tracked files; obtain them before repeating work. Compiling salvage drafts with zeroed M2C_ERROR placeholders are not semantic reconstructions. See the current execution section of [[plan-remaining]].

### Artifact recovery

2026-10-22: user supplied archive; private extraction /private/tmp/t25016-recovered-20261022 contains queue, ledger, census and traces. Source-bed completeness still being checked by T-25019. TACO func_80138B2C selected for bounded whole-TU validation, not presumed near based on canonical11. KANGEI baseline reproduced with known local scheduler tie; no justified new source test yet.

### Three-chat dispatch, 2026-10-22

Reviewed the guarded BUNKA delta (explicit FAKE, retained fallback) and source-only MAIN checkpoint against standards and ticket scope; preserved on main883760224. Neither is a C promotion. Reused existing idle chats with Sol6.1medium and one active worker each: T-25022 MAIN src/main/8004F870.c in chat01a15aee-c8fd-7382-a23a-0da45f598e67; T-25023 BUNKA src/ovl/BUNKA_SD/801388D0.c in chat01a15a31-d97a-7802-804e-bdb52f0dcf1a; T-25024 TEL src/ovl/TEL/80132000.c in chat01a15a25-5486-7e23-83bc-44fc1fe73dd1. Workers create their tickets in isolated worktrees. These exact file claims supersede the chats' older broad assignments. Root owns shared headers/tools and integration. GEKO/OPTION remain contributor-reserved. Astra awaits a specific blocker packet.

### Scheduler lane rotation

2026-10-22: T-25023 Sol baseline reproduces four changed positions/379 words and tracer control matches stock relevant sections. Bounded Astra consultation found no justified new source test; packet being pinned. After preservation, that chat releases BUNKA and owns only src/ovl/GYOZI/80140C90.c under T-25025, tracing zero-carrier reaching definitions for func_80140E0C. T-24907 grids remain closed; no automatic second Astra consultation. MAIN and TEL ownership unchanged.
