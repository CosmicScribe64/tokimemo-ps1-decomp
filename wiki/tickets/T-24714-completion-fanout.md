---
id: T-24714
title: Completion fan-out T-24714: ETC func_8013CCE4 byte-view switch
status: In Review
assignee: worker
created: 2026-10-20
updated: 2026-10-20
links: ["[[status]]", "[[playbook]]", "[[T-24700-completion-fanout]]"]
---

## Goal

Close ETC func_8013CCE4 (584 B, 18 rows) from the T-24706 lead: `switch (*(u8 *)&G)` fixes the three
switch compares (`$v0` vs `$v1` plus the dead `or v0,v1,zero`) but makes uopt fully unroll the
following 4-trip buf[] loop (18 -> 89 rows). Keep the loop from unrolling while the byte-view
selector stays. Secondary: ETC func_8014790C (57 rows).

## Owned files

- `src/ovl/ETC/8013BEF0.c`
- `src/ovl/ETC/80146400.c`

## Acceptance criteria

- [ ] func_8013CCE4 MATCHes as C (funcdiff --resolve, no INCLUDE_ASM) and survives the 27-hash gate, or a documented bounded negative.
- [ ] No git writes; header edits only additive and reported.

## Comments

- 2026-10-20: closed to In Review under the T-24900 plan. No promotion; guarded
  row-count improvements and evidence are recorded (see [[matching-notes]] and [[as1-scheduler-rule]]).
