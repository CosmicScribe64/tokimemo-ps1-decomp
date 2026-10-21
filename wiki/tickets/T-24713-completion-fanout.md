---
id: T-24713
title: Completion fan-out T-24713: wave 2: as1 hoist family
status: In Review
assignee: worker
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]", "[[T-24700-completion-fanout]]"]
---

## Goal

Close the listed near-miss functions (best current row counts) to literal C matches.

## Owned files

- `src/ovl/KANGEI/80137B90.c`
- `src/ovl/KANGEI/80134120.c`
- `src/ovl/VALEN/80132760.c`
- `src/ovl/VALEN/80133C70.c`
- `src/ovl/ENDING/801396E0.c`
- `src/ovl/ENDING/80134D20.c`

## Acceptance criteria

- [ ] Each listed function either MATCHes as C (funcdiff --resolve, no INCLUDE_ASM) and survives the 27-hash gate, or has a documented bounded negative.
- [ ] No edits outside the owned files and the headers they need; no git writes.
- [ ] Wiki notes and status updated by the orchestrator.

## Notes

Wave 2 of T-24700. The function cells in the list files are the current best row counts.

## Comments

- 2026-10-20: closed to In Review under the T-24900 plan. No promotion; guarded
  row-count improvements and evidence are recorded (see [[matching-notes]] and [[as1-scheduler-rule]]).
