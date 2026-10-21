---
id: T-24704
title: Completion fan-out T-24704: main high: record, CD and parameter functions
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

- `src/main/800674B0.c`
- `src/main/8006CB30.c`
- `src/main/800737A0.c`
- `src/main/80075320.c`

## Acceptance criteria

- [ ] Each owned function either compiles to a literal MATCH (funcdiff --resolve) and survives the full ninja 27-hash gate, or has a documented bounded negative result.
- [ ] No edits outside the owned files and the headers they need; no git writes (the orchestrator commits).
- [ ] Wiki notes and status updated by the orchestrator.

## Notes

Lane of the T-24700 completion fan-out. Draft archives and per-function history are in
`drafts/` and [[notes/functions]].

## Comments

### 2026-10-20 worker pass

No new literal matches; no source changes left in the lane (all four owned files are back at
`HEAD`). Notes for the orchestrator:

- `func_8006C988` (276 B, guard 11 rows): autotweak 323 edits on the guard (850 -> 850, exact
  20 -> 20) and 424 edits on the `s32` variant (715 -> 715, exact 20 -> 20), viewsweep 1 compile
  no gain. New probes: `s32` sel/cur turns the two per-arm `andi` into
  `move v1,t0` (target shape, still 11 rows); `g->unk_75E[n++]` is neutral; a separate value
  local (`u8`/`s32`) takes `$a0` and spills `first`; direct `D_800E6280.unk_75E[]` access (no
  `GameState *g`) uses `$a0` as base and spills. Remaining: `$t0`/`$t1` (pointer/value) and the
  arm/latch placement of `addiu` vs `move`; the original's pointer is in `$t1` and the value in
  `$t0`, i.e. one more high-priority local than the draft has, but every added local tried
  displaced an argument register instead.
- `func_8006D7EC` (4,692 B, guard 1 row): autotweak could not start while another lane's ETC
  header state was broken; the row is the known T-24001/T-24204 `idx++` -> `li v1,1` fold.
- `func_8006BA40` (488 B): the T-17000/T-14500 drafts replay at 49 resolved rows (scalar-alias
  form ~50); autotweak 435 edits, exact 94 -> 94. Above the guard limit, left `INCLUDE_ASM`.
- `parameter_up_down` (1,228 B): T-17016 draft replays at 218 rows. Left `INCLUDE_ASM`.
- `func_80076FDC` (852 B): T-17000 draft replays at ~172 rows (344 diff lines, 0x38 vs 0x30
  frame). Left `INCLUDE_ASM`.
- `func_8006A044` (648 B): T-17000 draft replays at ~105 rows. Left `INCLUDE_ASM`.
- Not re-probed: `func_80067870`, `func_80067F04`, `func_8006B0C8`,
  `func_8006D138`, `func_8007132C`, `func_80073CB0` were not re-probed because their only
  archived drafts use removed `D_` globals and would need retyping to the GameState structs
  first; `func_80075C84` was left at the documented T-24303/T-24501 alias blocker.

