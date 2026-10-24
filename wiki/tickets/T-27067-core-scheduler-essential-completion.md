---
id: T-27067
title: Complete shared core scheduler and school-day semantic residue
status: In Progress
assignee: core-followup-agent
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[tickets/T-27000-phase-2]]", "[[spec/core-followup]]"]
---

## Goal

Reconcile remaining shared core fields/types and essential scheduler/school-day constants and locals against applied ledgers. Prepare source-backed executable residue and exact literal site inventories. Root applies and owns shared tracking.

## Acceptance criteria

- [ ] Every original core/m-scheduler/m-weekday proposal row reconciled as applied, supported residue, evidenced exception or concrete unresolved blocker.
- [ ] New symbol-only table plans in Docker; no fake const rows. Constants have exact source sites and type/sign evidence for `rename.py const`.
- [ ] Independent Standards/Spec review resolved; root integration and matching proof before Done.

## Work notes

2026-10-24: Started with an early source-backed slice. `src/main/scheduler.c:421` proves player club id 6 is `CLUB_BASEBALL`, selects `clubs[6]` by that same id, and uses `level` low nibble `>= 8` as the club-season tournament gate. The code proves the gate's consequence, not a general mastery formula. Queue `girl = 7` is written only to the final three tournament records; neither type-0x25 dispatch nor the MASTER overlay reads it, so its meaning remains open. See [[spec/core-followup]] and ignored `.claude/orchestrator/state/T-27067/club-sites.tsv`.

The current six-row proposal includes `schedule_week_end_exit`, the supported `ClubRecord.flags` field, and four role-named school-day locals. The full verification-enabled Docker `rename.py plan` succeeded: 6 rows, 7 files to change in the dry run. It reported a nonblocking warning that 29 `unk_03` member tokens remain in the broad header closure; source inspection identified the four ClubRecord uses and confirmed other visible `unk_03` uses belong to other types. The 936-row machine-readable comparison is in ignored `.claude/orchestrator/state/T-27067/proposal-reconciliation.tsv`; it distinguishes exact applied-ledger rows, later ledger names and raw residue categories, but literal-site and semantic review of all residue is still open. No changes were applied; ticket stays In Progress.
