---
id: T-24701
title: Completion fan-out T-24701: main low: graphics and sprite tail
status: In Review
assignee: worker
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]", "[[T-24700-completion-fanout]]"]
---

## Goal

Match every remaining INCLUDE_ASM function in the owned files to literal bytes with the
[[playbook]] loop (funcdiff, viewsweep, autotweak, order_search, permute, regdiff).

## Owned files

- `src/main/80043510.c`
- `src/main/800451D0.c`
- `src/main/80046500.c`
- `src/main/80047550.c`
- `src/main/800490C0.c`
- `src/main/80049FF0.c`
- `src/main/8004AD60.c`

## Acceptance criteria

- [ ] Each owned function either compiles to a literal MATCH (funcdiff --resolve) and survives the full ninja 27-hash gate, or has a documented bounded negative result.
- [ ] No edits outside the owned files and the headers they need; no git writes (the orchestrator commits).
- [ ] Wiki notes and status updated by the orchestrator.

## Notes

Lane of the T-24700 completion fan-out. Draft archives and per-function history are in
`drafts/` and [[notes/functions]].

## Comments

- 2026-10-20: closed to In Review under the T-24900 plan. No promotion; guarded
  row-count improvements and evidence are recorded (see [[matching-notes]] and [[as1-scheduler-rule]]).
