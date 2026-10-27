---
id: T-24702
title: Completion fan-out T-24702: main GPU/sprite: mod, srn and menu
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

- `src/main/8004C3F0.c`
- `src/main/8004F870.c`

## Acceptance criteria

- [ ] Each owned function either compiles to a literal MATCH (funcdiff --resolve) and survives the full ninja 27-hash gate, or has a documented bounded negative result.
- [ ] No edits outside the owned files and the headers they need; no git writes (the orchestrator commits).
- [ ] Wiki notes and status updated by the orchestrator.

## Notes

Lane of the T-24700 completion fan-out. Draft archives and per-function history are in
`drafts/` and [[notes/functions]].

## Results

0 of 12 matched; `src/main/8004C3F0.c` and `src/main/8004F870.c` are unchanged (all targets back to
`INCLUDE_ASM`), no header edits left behind.

**Metric caveat.** `funcdiff --resolve` on a function that is still `INCLUDE_ASM` compares the original
asm with itself and prints `MATCH` with "linked function bytes equal target". All 12 functions in this
lane do that today; it is preservation evidence only, not a C match. Every number below is from a
`#ifdef NON_MATCHING` C body compiled by the normal `--resolve` path, and every body was reverted
because all are far over the 30-row guard rule.

Per function, best C body available and the fresh row count in this checkout:

| function | C body used | rows | what is left |
|---|---|---|---|
| `srn_tpage_show` (3224) | `drafts/T-24046/srn_tpage_show.c` | 592 | repeated POLY_FT4/RECT packet allocation and scheduling, 812 vs 806 instructions (T-24046) |
| `mod_trans_vram` (752) | `drafts/T-24501/main/mod_trans_vram.c` | 226 | buffer 0x88 vs 0x9C, row/cell spills, divisor/selector allocation (T-24501) |
| `mod_set_gpu` (804) | `drafts/T-24301/main/mod_set_gpu.c` | 195 | frame 0xA0 vs 0xA8, mode/record/divisor spill offsets (T-24301, T-24006) |
| `mod_set` (780) | `drafts/T-24301/main/mod_set.c` | 294 | frame 0xA8 vs 0xE8, request aggregate and selector allocation (T-24301, T-24006) |
| `srn_set` (596) | `drafts/T-24301/main/srn_set.c` | **113** | best of the lane: y initial load into `$s3` instead of `$t1`+copy, `$fp` to cols vs wid, spill homes; `autotweak --time 300` tried 669 edits with no gain. T-17015 body 216 rows, a separate-`yoff` variant 166 |
| `sndisp` (388) | hand reconstruction from the asm | n/a | the 5-byte prologue copy is `u8 idx[5] = {0}`'s local initializer and needs the `8004F870` data island so it lands at `D_800B40F8`; without it the copy shape is unreachable (tested memcpy with a prototype, 5 byte stores, 5-byte struct copy, pointer/temp forms). With the island T-17018 records 69 rows, including `&idx` kept in `$s0` across `strncpy` (IDO recomputes it) and `$s4`/`$s5` swapped |
| `sndi` (1052) | `drafts/T-24501/main/sndi.c` | 150 | frozen induction/cursor priority blocker (T-24501) |
| `get_h_tokimeki` (1600) | m2c draft only | (742 archived) | m2c draft has 58 unknown-type tokens; needs a typed reconstruction first |
| `get_near_menu` (1784) | m2c draft only | (1424 archived) | 92 unknown-type tokens; unrolled 4-way s16 vector subtractions |
| `func_8004FC10` (1812) | m2c draft only | (752 archived) | 91 unknown-type tokens |
| `func_8005215C` (1260) | m2c draft only | (318 archived) | state machine; 61 unknown-type tokens |
| `func_80052F68` (1200) | m2c draft only | (327 archived) | m2c draft does not compile after mechanical cleanup (`void *` walks, `s16 *`/`s32 *` mix) |

No duplicate/near-duplicate leverage: `neardupes.py --unit main` over all 12 names reports 0 groups
with a matched source and 0 unmatched members without a matched twin.

## Comments

- 2026-10-20: closed to In Review under the T-24900 plan. No promotion; guarded
  row-count improvements and evidence are recorded (see [[matching-notes]] and [[as1-scheduler-rule]]).

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
