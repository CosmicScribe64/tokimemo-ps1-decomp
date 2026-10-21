# T-24905 TACO func_8013B460: y3 store position

Status: not matched, 4 rows (2 per quad), guard unchanged in `src/ovl/TACO/8013B170.c`.
Source file untouched by this run (all experiments are scratch copies under `drafts/T-24905/`).

## Classification (plan-remaining section A)

**Same-block order, not the as1 global filler.**

- The whole region (last division guard's `break 6` .. the `jal AddPrim`) is one basic
  block. `funcdiff --resolve` shows exactly one moved instruction per quad: `sh t3,0x22(s0)`
  (the `p->y3` store) sits after the eight `sb` UV stores and before `lw t4,%lo` in the
  target; the draft has it directly after the y3 chain, before the first `sb`.
- The global delay-slot filler only moves instructions that are **not** loads, stores,
  calls or branches ([[as1-scheduler-rule]] rule 4). The moved instruction is a store, so it
  cannot be a filler pick. It also moves only *into an earlier* block; here the target has
  the instruction *later* than the draft.
- Target order: `... sb 0x25, sb 0x24, sb 0x1D, sb 0x1C, sb 0x15, sb 0x14, sb 0xD, sb 0xC,
  lui t4,%hi(D_8011ECA0), sh t3,0x22, lw t4,%lo(D_8011ECA0)`. The `sh` is wedged inside the
  `lui`/`lo` pair of the AddPrim argument. That is a per-block list-scheduling choice, so
  the `volatile` lever does not apply (there is no plain-global `lui` candidate).
- Consequence: the remaining source lever is statement order / source-line grouping of the
  y3 statement against the UV comma statement.

## What was tried this run (scratch compile + `objdump` of the guarded TU, `dump.py`, `gen.py`)

Reproduced the baseline first: `base/dis.txt` has the documented 2-row diff.

| variant | shape | result |
| --- | --- | --- |
| v1 | `s16 y3;` assigned before the UV comma line, `p->y3 = y3;` after it | **right statement order**: chain stays before the eight `sb`, `sh` moves past all eight. But the `sh` is still one slot early (before `lui t4`, target has it after), the y3 chain ends in `$v0` instead of `$t2`, and `0xF0`/`0xFF` recolour to `$a2`/`$v1` (target `$v1`/`$v0`). ~7 differing rows per quad, worse than the baseline. |
| v3 | y3 store as the last operand of the UV comma statement (no temp) | the whole block reorders: the `sb` group is emitted before the y3 chain (`lui t4; lw t4` split), 391 rows. Not it. |
| v5 | y3 statement moved after the UV line (no temp) | chain moves below the stores (`lui t4`, chain, `sh`, `lw t4`), 401 rows / 10 extra rows, frame spill changes (`sw a2,76(sp)`). |
| v6 | temp assigned in one comma chain with the UV stores | `sb` group emitted before the chain, 367 rows, constants recoloured. |
| v7 | `s32` temp instead of `s16` | identical to v1. |
| v8 | temp stored after `AddPrim` | store spills to `86(sp)`, 399 rows. |
| v2 / v4 | y3 first in the comma statement / y3+UV+AddPrim on one line | identical to the baseline (2-row diff) -- confirms T-24200. |

`v4.c` did not compile (one line, no newline inside the comma chain); not investigated further.

## Next concrete hypothesis

The temp shape (v1) is the only one that reproduces the target *statement order*. It is off
by (a) one scheduling slot for the `sh` and (b) the ugen ring colouring of the two UV
constants and of the y3 chain's last `addu`. Next run should treat v1 as the starting point:

1. The `sh` must be emitted after `lui t4`, i.e. one slot later. Try the temp store on the
   same source line as the AddPrim argument (textually after the call, ugen order
   `... sh, args, jal`, letting as1 sink it) or a second statement between the store and the
   call that consumes a slot (e.g. `p->y3 = y3, p = p;`-style no-op, FAKE) to see whether
   as1 then places the `sh` inside the `lui`/`lo` pair.
2. Fix the ring colouring without a named 0x80/0xFF/0xF0 local (all folded, see T-19030):
   spend one ugen temporary before the UV statement (the playbook's ring lever) so that
   0xF0 returns to `$v1` and 0xFF to `$v0`, and check whether that also restores `addu $t2`.
3. Do not repeat plain statement permutations -- T-24200, T-24502 and this run have covered
   them; only the temp shape moves the `sh`.

If a run cannot get below the baseline's 4 rows, restore `INCLUDE_ASM`-equivalent guard as
it is now (the guard is 4 rows, well under the 30-row limit).
