# T-25152: TACO func_80138B2C (80138890.c)

Result: not matched. Guard went from 30 rows to 3 rows (`funcdiff --resolve --version c`, whole unit).
Body: `func_80138B2C.c` in this folder (the same text as the guard in `src/ovl/TACO/80138890.c`).

## Start state

Guard from T-19015/T-16564, 30 rows, registers only. The original colours the stage
(`D_8015EDCC`) `$a0`, the time bonus `$a1` and the total `$a0`; IDO gave `$v0`, `$a0`, `$a1`.
regdiff: the stage was the promoted global (priority 1, cost 0 in `$v0`). Earlier passes tried
locals vs globals, dead inits, store orders, autotweak and the permuter without a cause.

## What uopt does (read from n64decomp/ido uoptreg2.c, revision d068e43)

- `cupcosts`: a live range of a **parameter** (`Pmt`, current block) that has a live unit in the
  entry block gets cost 1e20 for every register in `regscantpass` (v0, v1, f0, f2, ra). So a
  parameter live in block 0 can never take `$v0`/`$v1`; with nothing else in the way it gets `$a0`.
- Ranges interfere per basic block (`intfering` intersects `livebbs`), and a global is never kept
  in a register across a block that loads through a pointer (`indiracc`; for C `aliaswithptr`
  is always true). So the bonus words cannot be promoted globals across the `D_8015EDB4->unk80/7E/7C`
  tests; they are locals.
- Block-end register stores (`rstr`) of promoted globals are emitted at the branch, after every
  plain store of the block (`func_0042B09C`, register order). That is why the promoted total
  `D_8015F490[4]` was stored after the time bonus in every draft; the original stores the total first.
- Copy propagation (`find_replacements`) does not copy an assigned value that contains an indirect
  load (`has_ilod`). A plain global read is copied into its uses and the variable disappears.

## Changes, in order (rows after each)

1. Stage in the K&R parameter `arg0` (unused by the caller, which passes nothing): no effect while
   uopt copies `D_8015EDCC` into the uses (28 rows, range is the global or the cvt expression).
2. `arg0 = *(u8 *)&D_8015EDCC;` (FAKE byte view): the copy is blocked, arg0 is a parameter range
   in block 0, gets `$a0`; the time bonus then gets `$a1` and the total `$a0`. 22 rows.
3. `D_8015F490[0] = D_8015EDF0 += arg0 * 0xF4240;` puts the score load before the multiply
   (t6/t7 as in the original). 11 rows; every register matches, only the join block's store order
   and the two hoisted `lui` in the unk80 test block differ.
4. Total in `arg0` too, read with `*(s32 *)&D_8015EDF0` (FAKE word view, same `has_ilod` reason):
   arg0 is one parameter range (stage + total) in `$a0`, the total is stored in place. 7 rows.
5. Total computed before the bonus stores (`arg0 = ...; [2]; [1]; [4] = arg0; [3];`): the as1
   schedule of the join block and the hoisted `lui t9`/`lui at` now match. 3 rows.

## Rows now (3)

Clamp arm: original `lui a0,0x98; ori a0,a0,0x967f; sw a0,D_8015F4A0`; ours builds 9999999 in `$t2`.
uopt propagates the constant of `arg0 = 0x98967F` into the store and drops the def, so ugen
materialises it in its ring.

## Negatives added (all measured with this body)

- `D_8015F490[4] = arg0 = 0x98967F;` 3 rows, `$t2`.
- Keep arg0 alive after the if with `if ((u32) arg0 < 0) { D_8015F490[4] = arg0; }`: the constant
  becomes its own two-use range, `$v0` (3 rows). The same test inside the arm: `$t2`.
- `arg0 = ((u32) arg0 < 0) + 0x98967F;` 25 rows (allocation order changes).
- `arg0 -= arg0 - 0x98967F;` 28 rows; a word view on the arm store (`*(s32 *)&D_8015F490[4] = arg0;`) 3 rows, `$t2`.
- One store after the if (`if (...) arg0 = K; D_8015F490[4] = arg0;`): arg0 is live into the call
  block, whose `$a0` argument forbids it; 32 rows.
- Promoted total with the bonus words as globals (all, or `[3]` only): `[1]`/`[3]` are uncoloured
  (indiracc), direct stores, 40 to 68 rows.
- Line grouping of the four tail statements: no effect (as1 order comes from the `$at` chain).
- autotweak 300 s on the 11-row body: best order `[4],[3],[2],[1]` (9 rows), superseded by step 5.

## Next hypothesis

Find what keeps 9999999 in arg0's register in the arm. The def must survive copy propagation
(the store must read arg0, not the constant) while arg0 stays out of the call block. Candidates:
a spelling where the arm's value is an increment of arg0 (`is_incr` blocks the copy, e.g. a form
uopt sees as `arg0 = arg0 + c`), or a later read of arg0 that ugen removes but that does not reach
the `func_80044750` call block. Check each with regdiff: the constant must not get its own range.
Also check whether a natural spelling replaces the two FAKE views: any indirect load in the
assigned value works, for example the stage and score read through one struct pointer, but a new
local costs a frame slot (frame is 0x28 with the four scalars now).
