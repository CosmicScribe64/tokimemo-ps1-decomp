# main func_80062948 (src/main/80061710.c), 736 bytes: matched (T-25119)

Default C in `src/main/80061710.c`; `funcdiff --resolve --version default` prints MATCH (736 bytes),
no `INCLUDE_ASM` left for it. Commit `T-25119: match main:func_80062948`.

## Start state

Plain `INCLUDE_ASM`. Best bodies on record: T-25012 `body.c` (clean rewrite, frame 8 bytes off), and the
lost T-24600 packets (`matrices-entry/init-joined`, 40/184 words, frame 0xD8 instead of 0xD0).

## How it was measured

The machine ran at load 35, so a scratch harness compiled only this function: the body after
`#define NON_MATCHING` plus `common.h`/`game.h`, `tools/cc.py ... ido 5.3`, objdump of both objects,
relocations folded to absolute addresses (`D_800E6280+0x10F8` equals `D_800E7378`), branch targets
dropped, and a row diff against the `INCLUDE_ASM` object. One variant took about 8 seconds. Two shims
dumped the ucode entering uopt and entering ugen (`tools/cvt_pass.py` parser). The final check was
the whole-unit `funcdiff`.

## Steps, rows after each (scratch diff rows / 184 words)

1. T-25012 body with four scalars before the aggregates: 119 rows; frame 0xD0 but every aggregate home 8 low.
2. Only `i`, `j` before the SDK aggregates, `v` (and later `x`) after them, and no `n = j + 1` local:
   all homes and s0-s5 right, 89 rows. The original has two scalar slots above `rot`.
3. `D * (j + 2) * -2 - 2` instead of `-(D * (j + 2)) * 2 - 2`: as1 expands the multiply by -2 into
   `negu at; sll`, which spends one ring temporary instead of two. 40 rows (the T-24600 level).
4. Position update before the matrix copy: the copy temporary becomes t2. 35 rows.
5. Matrix copy through the record (`*(MATRIX *)D_801227A0[i].unk4 = m`, the DATE spelling; a
   GsCOORDINATE2 cast does the same): the copy walker is its own induction variable created after the
   flag walker, so s6/s7 come out right and the loop tail schedule matches. 22 rows.
6. `x = (&D_801230D0)[i + 1];` after the second rsin, used by t[1] and t[2]: 3 rows. Cause, read in the
   ucode entering uopt: cfe orders the operands of a commutative operation heavier first, and an
   indexed load (`ilod` of an `ixa`) is heavier than `v / 32`, so `-x / 2` was evaluated first. With a
   local, both sides weigh the same and source order holds. The free list then gives the original's
   t0 (negu), t1 (v / 32), t2 (/2), t3 (sum).
7. Two operand-order FAKEs for the same front-end rule: `(D | 0) * (j + 2)` (the counter must be the
   first `multu` operand; `*(u32 *)&D` also works) and `(v & 1) - (0 - X) + 2` (the and must be emitted
   before the reloaded position; uopt folds the double negation into `addu`). 0 rows, then MATCH.

## Negatives (measured, do not repeat)

- A pointer walker for D_801230D0 (`w[1]`): stores to `m.t[]` may alias it, so x is reloaded for
  t[2] (46-61 rows). A pointer for D_801227A0 alone: s6/s7 right but the loop tail order wrong.
- t[1] spellings: `-x / 2 + v / 32`, `(0 - x) / 2`, `x / -2`, `v / 32 - x / 2`, a `v / 32` local,
  the rsin call inline, `(s32)`/`(u32)` casts on either side, `s16 v`: no change or worse while x is an
  indexed load.
- Update spellings: `+=`, every order of `(v & 1)`, `X`, `2`, `(u32)v % 2`, `((v & 1) | 0)`, a second
  local for the reload (becomes a v1 range, 13-32 rows), `- -X` (an extra negu).
- rot.vz: `(u32)(j + 2)`, `u32 j`, `u32 i`, a `k = j + 2` local (adds a frame slot), a
  `GameState *gs` pointer (uopt keeps gs in a register), `(D * 1)`.
- Signed `-x * 4` in t[2] shares the negu with t[1] (183 words); `-(x * 4)`, `-400 - x * 4`,
  `(-x << 2)` do not give the original sequence.

## Reusable findings

- ugen allocates each temporary when it emits the instruction, from a FIFO free list. A different
  emission order (from the ucode operand order) is enough to permute t0..t3 with the same list.
- cfe puts the heavier operand of `+`/`*` first; an array element read is heavier than a scalar or
  local, and a `u32` conversion of the other side adds weight. A local copy of the element or
  `| 0` / `- (0 - x)` restores the source order.
