# T-25108: main func_80065B0C (1064 B, 266 words), guarded at 9 funcdiff lines

Start: `INCLUDE_ASM`. The only earlier bodies were the m2c census drafts (`drafts/T-14500`, `drafts/T-17000`,
313 rows; T-16930 agent A measured 353). No sibling. Best body: `final.c` (also the NON_MATCHING guard in
`src/main/80062CD0.c`). It needed two new externs in `main_api.h`: `s8 D_800B5A9C`, `u8 D_800B5AA0`.

## Reading of the target

- The selection index `D_800E7313` (GameState `unk_1093`) lives in `$s3`: loaded at entry, re-read after
  each call (`func_80065964`, `func_800654D0`, and in the else arm after `func_8004FC10`), stored to
  `D_800B5A9C` at the end. The entry value is saved to a byte home at sp+0x4B (`old`, u8, low priority).
- The y += statement recomputes `D_80125C10 + idx * 4` (second `sll`, `addu s0`) after the x store; the
  clamps after the call share one address.
- `n` (14/15) has a stack home (sp+0x4C) and is reloaded each iteration. Both homes come out by themselves.
- The constant 3 (`$s5`) is shared by the `% 3` divisor (with the div-by-zero check) and the loop's mode
  stores; 0x1000 in the loop (`$s1`) is not shared with the `case 0x1000` compare (`li at`).

## Measured (whole-unit compile, rows = my side-by-side count; funcdiff lines at the end)

| body | rows |
| --- | --- |
| m2c-shaped typed body, direct `D_800E6280.unk_1093` | 169 |
| + D_8011ECF6/ECFA written as record 0 fields `unk_26`/`unk_2A` | 169 (fixes the ECFA load order) |
| + every `unk_1093` read through `GameState *g` | 125 |
| + `switch ((u32)g->unk_F80 & 0x5000)` | 25 |
| + `unk_F6C` at the `n` test through `g` | 44 (temporaries one late in the loop) |
| + flag index `((GsRecF90 (*)[2])g->unk_F90)[i][0]` instead of `unk_F90[i * 2]` | 4 (2 real) |
| final: all GameState reads through `g` | 4, funcdiff 9 lines |

Why each lever works:
- `g->unk_1093` makes the index an ILOD: the x store (an ISTR) kills it for CSE, so the y address is
  recomputed as in the original; the extra use raises the range so it gets `$s3`, re-read after calls.
- The unsigned selector gives the case constant 0x1000 another constant than the loop's halfword 0x1000
  stores. With a signed selector one register range holds both (`li s2,4096` early) and i/0x1000 swap.
- `unk_F90[i * 2]` is two multiplies in ucode; ugen spends a temporary slot on the folded one, so every
  temporary of the loop's else arm is one late. The 2D view is one multiply.
- Through `g`, the n-test read of `unk_F6C` is not PRE'd into the clamp and else arms (it was in `$v0`).

## What is left (9 funcdiff lines, 2 instructions)

The else arm: the original has `li s5,3` after the two argument loads and before `jal menu_check`; the
draft has it after the reload of the index (`lb s3`), in the original's load-delay `nop`. The three branches
to the join are then 4 bytes off.

uopt analysis (IDO sources, n64decomp/ido d068e43, `uoptreg2.c`, `uoptemit.c`): uopt blocks end at calls.
The constant's live range is the active set: live backwards and "appeared" on some path from the entry
(`unk11C`), plus blocks that reference it. In the draft the else blocks (menu_check, func_8004FC10, and the
block uopt adds for the index reload) are not in the range; the join is a reaching block with `needreglod`,
and the rlod/rstr optimisation moves its reload to the end of the else predecessor (`rlodbb`, emitted at
the block end, after the `lb s3`). The original's `li` before the menu_check `cup` can only be an rlod at
the start of the menu_check block (rlods are emitted at the block start, rlodbb after a block's `cup`) or a
code-motion insertion (emitted just before the `cup`), so the `$s5` piece of the original's range covers the
menu_check block. A block joins the range only where the constant is live and has appeared on some path
from the entry, so the original must reference the constant 3 in the else arm before the call or in the
entry block, without an instruction of its own. Not found (see the mechanism check below).

## Negatives (no change unless noted)

- `% 3` instead of `% 3U`, `(u32)unk_F6C % 3`, `% 3 == 2U`; loop stores `= 3U` or `(u8)3`.
- `if (g->unk_1093) {}` before or after the else calls; `do { ... } while (0)` around them.
- A dead `n = 3` at the entry or in the else arm (removed before allocation).
- `func_8004FC10(3)` / `menu_check(3, ...)` as probes: argument constants go straight to `$a0`, no range.
- A local `k = 3` (s32, u8, s8, s16, u32) in both arms, used by the divisor and the loop stores: uopt
  propagates it (the divisor becomes an immediate without the check, 13 to 140 rows); with `k = 3` at the
  join or before the loop the same.
- Late-folded `((u32)x < 0) * 3` as the menu_check argument: folded before allocation.
- `D_8012059A[1] > 3`; `for (i = 0; i != n; i++)` (5); `menu_check(0, D_8011ECF6, D_8011ECFA)` (same 4,
  kept the record form for consistency); the += lines with the scalar symbols (8: ECFA load before the store).
- `s32 func_8004FC10();`, `void menu_check();`, `s32 rsin();` through `funcdiff --proto`: 9 lines each.
- `old` initialised after `dx`/`dy`; `% 3` computed into a local before the outer test.
- A late-false arm `if ((u32)g->unk_F5F < 0) { rec[64].unk_01 = 3; }` before menu_check: removed before
  allocation, no change. With `(u32)old < 0` the index register moves (7).
- Mechanism check, a real store `rec[64].unk_01 = 3` before menu_check (80, extra code): the store takes
  `li t4,3` (a split-off local piece of the constant) and `li s5,3` stays after the index reload. So a
  reference to 3 in that block is not enough by itself; the `$s5` piece has to cover the block.
- autotweak (300 s, 318 edits): nothing. permuter (900 s, -j 2, base score 400): nothing better.

## Next hypothesis

The mechanism check shows that a reference to 3 in the menu_check block is split off into its own
temporary, so the question is why the else blocks stay out of the `$s5` piece. Next: make the traced uopt
print the live units of the constant-3 range (block numbers 25, 26, 58 of the else arm, 27 the join) before
and after `split`, for the final body and for the M1 store probe. If the else blocks are cut by `split`, the
original's range had a higher priority or fewer interferences there (compare the `$s3` index range, which
is live through the same blocks); if they never enter the range, look for the reference in the entry block.

## T-25201 (closer): matched

The R9SAVE lines of the traced uopt (draft.log of a regdiff run) list each range's live units. The constant 3
range had units in block 1 (divisor, reload), 27 (join, reload) and 38/39/40 (loop stores), none in the else
blocks 25 (menu_check), 26 (func_8004FC10), 58 (index reload). The M1 probe added no unit either: the store
`rec[64].unk_01 = 3` has a fixed address, so it is a `str` to a variable whose value is scanned with optype 0
(`constinreg` is false for small constants), and ugen loads it with `li t4,3`. An indirect store's value is
optype 3, a register candidate.

Fix (FAKE, T-25201): `if ((u32)a < 0) { ((DisplayActor44 *)D_8011ECD0)[a + 64].unk_01 = 3; }` as the first
statement of the else arm. uopt keeps the test on the parameter, the arm block gets a unit, the else blocks
join the `$s5` range (reload at the start of block 25, which as1 moves after the argument loads), and ugen drops
the arm. 0 rows; default build MATCH.

| variant | rows |
| --- | --- |
| `(u32)a < 0` arm, store to fixed `rec[64].unk_01` | 9 (no unit) |
| arm with `*(u8 *)a = 3`, `((u8 *)D_8011ECD0)[a] = 3`, `rec[a].unk_01`, `rec[a + 64].unk_01` | 0 |
| arm between menu_check and func_8004FC10 | 6 |
| fixed-address arm (`rec[64].unk_01`) after func_8004FC10 | 17 |
| arm on `(u32)old < 0` | 3 |
| plain `*(u8 *)a = 3;` before menu_check (real store) | 9 |

