# T-25143: SHOUGATU func_8013EA98 (wave 9 neighbor conveyor, T-25200)

## Start state

- `src/ovl/SHOUGATU/8013E330.c` held plain `INCLUDE_ASM` for this function.
- Best known body: T-25046 `unsigned-status-retry`
  (`/private/tmp/tokimemo-t25023/build/t25046/unsigned-status-retry/body.c`), copied here as
  `func_8013EA98.c`. Measured in this worktree: 230/230 words, frame 0x58, aligned cost 36,
  42 distinct `funcdiff --resolve` rows (84 printed lines). It uses the `D_800E71DF` alias at the
  post-status compare and the `func_800847B8` argument, which would need a keep line with a proof.
- 42 rows is over the 30-row guard limit, so the source stays `INCLUDE_ASM` and the body lives here.

## First divergence (36-cost body)

All rows are in the girl-selection loop and its exit:

1. `t0`/`t2` swap: `D_80146054` gets `t0`, constant 9 gets `t2`. regdiff calls it a priority tie
   (2.2 against 2.2) decided by bit order.
2. Selector range. The original has no `move t4,t8` after the selector store and no
   `move t4,t9` on the first retry edge. Its `>= 9` test reads the stored value through ugen
   forwarding (`andi t9,t8,0xff`). Its `lui t4` sits in the header, moved there by as1 from the
   `lbu t4` at `.L8013EBE8`. In the draft, the `>= 9` block is in the selector's register range, so
   its reload is forwarded into `t4`.
3. Latch. The original has one block, `.L8013EC44`, that all five rejection paths reach:
   `lui t4; lbu t4; b header`. The draft has no latch block. Its reload sits in the count block,
   and the retries branch straight to the header.
4. Exit: the original uses `lui v1` for the post-loop `lbu a0, F5F`, the draft `lui a0`.

## New tool: unit-level allocator trace

`/private/tmp/tokimemo-t25044/build/T-25044/uopt-trace` is the `tools/uopt_patch.py trace` variant
(arm64). Copy it to `build/ea98/tr/uopt-trace` and pass `--uopt /work/build/ea98/tr/uopt-trace` to
regdiff, or use `scripts/tr.py`, which also saves uopt's output ucode as `fn.after.txt` (rlod/rstr
placement). `scripts/units.sh LABEL` prints the flow graph and every `R9SAVE`/`R9UNIT` line for
the selector variable: live units per block, with loads, stores, `load` (needreglod) and `save`
(needregsave), before and after a split. `scripts/m.py LABEL BODY` gives the whole-TU score;
`scripts/mk.py` replaces the loop of a base body; `scripts/run2.sh` runs two at a time. All of
these run in Docker from the worktree root, with bodies under `build/ea98/`.

## What the trace shows (uopt reference: uoptreg1.c makelivranges, uoptreg2.c split/addadjacents/globalcolor)

- A block that holds an `ilod` through a computed address (`unk_1BC[F5F]`) is an alias hole
  (`indiracc`). Its selector accesses do not form live units, so its store and loads are plain
  memory operations that ugen forwards. Holes count as uses for liveness (uoptreg2.c:241).
  Every unit or reaching block with a hole as a predecessor gets a reload unit (`load=1`).
- The 36 body's selector units are bb5 (`>= 9`, reload), bb7 (`== 5`, reload), bb10, bb13, plus
  zero-reference reload units bb15 (count block) and bb17 (exit). These two exist because bb14,
  the final `!rec[F5F].b1` test, is a hole. Also bb21 (forced store, first access a store) and
  bb22.
- The split starts at the first unit whose first access is a store, here the forced
  `unk_F5F = D_80145FE4`. It grows along successors only (`addadjacents`), so the post-loop piece
  is split off and the whole loop, `>= 9` block included, stays in the `t4` piece.
- A reload at a block with several predecessors moves onto the edges of the non-member
  predecessors (`dorlodrstropt`, `somepremember`/`allpreppout`) as soon as one predecessor holds
  the same variable in that register. The original keeps its latch reload at the block start, so
  no latch predecessor holds the selector variable in `t4`. This is not yet explained.

## Structure the original needs (measured pieces)

| piece | how to get it | evidence |
|---|---|---|
| a real latch shared by all rejections | `do { pick } while ((A \|\| B \|\| C \|\| D \|\| E) && (i++, 1));` (diag `r1.c`) | final test exactly the original's (`bnez v1,exit; slti; bnez at,exit`, latch falls through), all retries branch to the latch; 180 rows from the register cascade |
| `>= 9` read outside the range | alias hole in that block: diag `F5F >= 9 + rec[F5F].b1 * 0` (`m0b36.c`) or a dead local read (`j2.c`) | that block becomes exactly `andi t9,t8,0xff; sltiu 9`, and as1 moves the `lui` of the `== 5` reload into the header, as in the original |
| no count/exit reload units | final test reads b1 from a local set in the header (`flag`) | bb15/bb17 units disappear (`r3.c`) |
| latch reload | combination `r3.c`: loop piece = {latch reload, bb6 reload, `== 5` reload, `== 6`, `== 54`} | priority exactly 0, so not coloured. Without the bb6 unit it would be +10 and coloured with the original's reload placement |

bb6 is the empty block uopt puts on the fall-through edge from the `>= 9` block to the `== 5`
label (a critical edge). With a hole in the `>= 9` block it needs its own reload, which the
original does not have.

## Negatives added (whole-TU aligned cost; base 36)

- E6E4-style `if (...) continue; ... break; } while (1);` (`c1`/`c2`): 70, loop code identical,
  no latch block survives. `goto next;` gives the same CFG as `continue`.
- Latch conditions `while ((u32)x >= 0)`, `!((u32)x < 0)`, `((u32)x < 0) == 0` for status, count,
  10F8 or D_80146054: identical 70 (folded early). With the selector, `(u32)F5F >= 0`: 254
  (per-edge `move v0,a0` copies). Latch `if ((u32)count < 0)`: 103.
- For-latch `for (i = 0; ; i++)` with continues (`f1`): 182, sum order `addu t9,v0,t7` reversed;
  only `addiu` moves into the header. With `status` u32 (`f2`): 186.
- `if (!(A||...||E)) break;` in a for-loop (`g1` 176/`g2` 172), nested and combined
  accept/break forms (`n1` 172, `n3` 172): retries reach the latch directly, but the count test
  is laid out `beqz at,latch; b exit`, the opposite of the original.
- `&` instead of `&&` in the first test: 125 (branchless). `(s32)` casts sharing the
  conversion: 199 (CSE into `v0`). `(u32)` casts: identical 36.
- Local copies of the pick (`sel` u8/s32, index by `sel`): 276 to 297.
- Alias `D_800E71DF` for the loop reads: 194 (hoisted, loop-invariant). Alias for the forced
  store: 172 (no first-store unit, so no split: the `>= 9` block stays in range).
- Hole diagnostics alone leave the loop range uncoloured (`m0b36` 198, `h1` 99, `j1` 188,
  `k1` 230, `r2` 209, `r3` 246). They are evidence, not candidates.

## Rows now

Unchanged: best is still the 36-cost body (`func_8013EA98.c`), 42 rows. The production source is
`INCLUDE_ASM`.

## Next hypothesis

Find source that gives the `>= 9` block an alias hole (a record access through `unk_1BC[F5F]`
that PRE later removes) without leaving the fall-through block bb6 needing its own reload.
Combine it with the condition latch from `r1.c` and a final test that does not re-read the
record. Then explain why the original's latch reload stays at the block start: no predecessor may
hold the selector variable in `t4`. One candidate is that the `== 5`/`== 6`/`== 54` compares use
a different ichain than the latch reload. Check each step with `scripts/units.sh` before scoring.
