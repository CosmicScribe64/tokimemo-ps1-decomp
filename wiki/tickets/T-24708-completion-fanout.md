---
id: T-24708
title: Completion fan-out T-24708: overlays A: ENDING, VALEN, NAME_ENT, KANGEI
status: Done
assignee: worker
created: 2026-10-20
updated: 2026-10-26
links: ["[[status]]", "[[playbook]]", "[[T-24700-completion-fanout]]"]
---

## Goal

Match every remaining INCLUDE_ASM function in the owned files to literal bytes with the
[[playbook]] loop (funcdiff, viewsweep, autotweak, order_search, permute, regdiff).

## Owned files

- `src/ovl/ENDING/80133030.c`
- `src/ovl/ENDING/80133C10.c`
- `src/ovl/ENDING/80134D20.c`
- `src/ovl/ENDING/80137340.c`
- `src/ovl/ENDING/801396E0.c`
- `src/ovl/VALEN/80132760.c`
- `src/ovl/VALEN/80133C70.c`
- `src/ovl/NAME_ENT/80132000.c`
- `src/ovl/NAME_ENT/80139740.c`
- `src/ovl/NAME_ENT/80142A80.c`
- `src/ovl/KANGEI/80132090.c`
- `src/ovl/KANGEI/80134120.c`
- `src/ovl/KANGEI/80137B90.c`

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

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
