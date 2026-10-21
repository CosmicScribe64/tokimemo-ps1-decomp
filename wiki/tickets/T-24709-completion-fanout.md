---
id: T-24709
title: Completion fan-out T-24709: overlays B: GEKO, MASTER, SHOUGATU, RPG_BAT, TEL, EN_NICHI, BUNKA_SD, OPTION
status: In Progress
assignee: worker
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]", "[[T-24700-completion-fanout]]"]
---

## Goal

Match every remaining INCLUDE_ASM function in the owned files to literal bytes with the
[[playbook]] loop (funcdiff, viewsweep, autotweak, order_search, permute, regdiff).

## Owned files

- `src/ovl/GEKO/801323E0.c`
- `src/ovl/GEKO/801338F0.c`
- `src/ovl/GEKO/80134D10.c`
- `src/ovl/GEKO/80142760.c`
- `src/ovl/MASTER/80132140.c`
- `src/ovl/MASTER/80133680.c`
- `src/ovl/SHOUGATU/80134890.c`
- `src/ovl/SHOUGATU/8013E330.c`
- `src/ovl/RPG_BAT/801447E0.c`
- `src/ovl/RPG_BAT/8014ADD0.c`
- `src/ovl/RPG_BAT/80154430.c`
- `src/ovl/TEL/80132000.c`
- `src/ovl/TEL/8013CCD0.c`
- `src/ovl/EN_NICHI/80136B10.c`
- `src/ovl/BUNKA_SD/80134540.c`
- `src/ovl/BUNKA_SD/801388D0.c`
- `src/ovl/OPTION/80132000.c`

## Acceptance criteria

- [ ] Each owned function either compiles to a literal MATCH (funcdiff --resolve) and survives the full ninja 27-hash gate, or has a documented bounded negative result.
- [ ] No edits outside the owned files and the headers they need; no git writes (the orchestrator commits).
- [ ] Wiki notes and status updated by the orchestrator.

## Notes

Lane of the T-24700 completion fan-out. Draft archives and per-function history are in
`drafts/` and [[notes/functions]].

## Comments
