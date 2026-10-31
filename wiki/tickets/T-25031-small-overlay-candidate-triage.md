---
id: T-25031
title: Qualify unreviewed small-overlay remaining functions
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["`plan-remaining`", "[[tickets/T-25028-read-only-candidate-triage]]", "[[tickets/T-25030-overlay-candidate-triage]]"]
---

## Goal

Read-only selection from NAME_ENT, SHOUGATU, VALEN, KANGEI/GYOZI/BUNKA_SD, at most twelve actual remaining functions excluding prior24 and exhausted near tails. Stop upon a concrete still-unrepaired control/ABI/type defect with target evidence and minimal proving experiment.

## Acceptance criteria

- [x] Verify latest-main remaining membership and scope exclusions before selection.
- [x] Inspect newest useful body, callers/target and provenance without obsolete-hole claims.
- [x] Pin actionable evidence or compact scoped negative table.

## Notes

Isolated documentation tree /private/tmp/tokimemo-t25023. One worker, no agents/Astra/builds/source sweeps/root edits/push. MAIN belongs to another worker. T-25030 preserved at708f1c4f6. No source ownership claimed by this survey.

## Actionable result: SHOUGATU func_8013E50C

Stopped after four reviewed actual remaining functions. Current main44a495b88 still119. NAME_ENT3AF78/38C1C/398EC newest bodies already address historical defects; no new nomination there.

Newest corrected SHOUGATU archive still passes GameState-derived address and &sp40 to func_80084D3C, creating a fictitious table escape. Actual alias is matched check_para_limit(void), address80084D3C. Canonical declaration already exists; matched C at src/main/8007C030.c:2815 has no parameters. Original callee defines a1 at80084D3C/40 and a0 at80084D6C before any read/branch; neither incoming value is consumed. Caller a0 is the real stat-store address in the helper call delay slot, not an argument contract. This answers the previously open actual-callee question; the earlier call-multiset audit did not establish arguments.

Nominate one proving variant: preserve corrected unk_10F8 body and replace only FAKE helper arguments with check_para_limit(); measure current whole-TU baseline/variant, sp40 call escape and frame/table homes. No shared declarations needed, no predicted match or guaranteed frame reduction. Unsupported extra args are ABI/escape-model defect, not a claimed runtime behavior bug. Full exact evidence and experiment: drafts/T-25031/README.md. No compiler/source experiment in triage; source unchanged. Root assigns matching owner. Ticket to In Review.

## Checks

Read-only current membership and matched-callee/target inspection; no compiler invoked. Wiki lint zero broken links/status mismatches, one existing large-page warning. Diff check clean; production/header/tools/config unchanged. Root authorizes the subsequent T-25033 single canonical-call control separately.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
