---
id: T-24712
title: Completion fan-out T-24712: wave 2: GYOZI close functions
status: In Review
assignee: worker
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]", "[[T-24700-completion-fanout]]"]
---

## Goal

Close the listed near-miss functions (best current row counts) to literal C matches.

## Owned files

- `src/ovl/GYOZI/80140C90.c`
- `src/ovl/GYOZI/80138B50.c`
- `src/ovl/GYOZI/80139920.c`

## Acceptance criteria

- [ ] Each listed function either MATCHes as C (funcdiff --resolve, no INCLUDE_ASM) and survives the 27-hash gate, or has a documented bounded negative.
- [ ] No edits outside the owned files and the headers they need; no git writes.
- [ ] Wiki notes and status updated by the orchestrator.

## Notes

Wave 2 of T-24700. The function cells in the list files are the current best row counts.

## Comments

- 2026-10-20: closed to In Review under the T-24900 plan. No promotion; guarded
  row-count improvements and evidence are recorded (see [[matching-notes]] and [[as1-scheduler-rule]]).
