# T-25142: BUNKA_SD func_80134D94

Result: not matched. Best body: `func_80134D94.c` here, aligned cost 12 (`funcdiff.diff_score`,
Levenshtein over the resolved words), 243/243 words, but 45 positional `funcdiff --resolve
--version c` rows, because one load sits one slot early and shifts about 35 rows. That is over the
30-row guard limit, so the source is back to plain `INCLUDE_ASM` (the old T-24137 guard, with the
wrong selector test, coordinates and argument order of T-24516, is gone too).
Start state: the T-25053 archive `signed-selector.c`, cost 102, 384 positional rows.

The body is an implicit-int function: to use it, change `void func_80134D94(void);` in
include/ovl/BUNKA_SD.h to `s32 func_80134D94(void);`. Measured with that header: the caller
`func_80134800` still matches and BUNKA_SD.bin is sha1 OK (the change was reverted with the guard).

## How it was measured

`score.py` (copy it to `build/T-25142/`, run `tools/docker.sh python3 build/T-25142/score.py
BODY.c [--sbs]`) replaces the `#ifdef NON_MATCHING` guard of func_80134D94 with a body file, compiles
the unit with the build's pipeline and prints `rows` (positional) and `cost` (aligned); `--sbs`
writes a side-by-side listing. It needs a guard in the source (any body under `#ifdef NON_MATCHING`
around the `INCLUDE_ASM`) and either the s32 header or `--proto 's32 func_80134D94(void);'`.
About 25 s per body. regdiff with the s32 body needs a scratch copy of the file whose
`#include "ovl/BUNKA_SD.h"` is wrapped to hide the void declaration.

## Levers that closed rows (in order)

1. **Grey level is `d * 0x10201`, not `d * 0x10101` (semantic fix).** The original computes
   `v = 0x68E - frame; v += v << 8; v += v << 8` (in place in `v0`). The old draft had
   `* 0x10101`, which is the wrong colour value. 102 -> 80.
2. **`s32` function, `return w;` at the end of the last block.** The original keeps
   `move v0,v1` (w) before the last store: an implicit-int function whose last block returns
   the quotient. 80 -> 76.
3. **Last block as in func_80134850:** `w = *(u8 *)&D_800E6280.unk_110C / 14;`,
   `*(u8 *)&... = 0;` in the reset, `*(u8 *)&... += 1;` at the end. uopt promotes the counter
   to `a0` and w gets `v1`, as in the original. Before that, w and the local counter tied on
   priority 2 and the counter, mentioned first, took `v1`. 76 -> 64.
4. **Arm value copy:** `t = q * 1; if (t < 0) {}` and the arms use `t`. `q * 1` is not a plain
   copy, so copy propagation keeps a range (`cvt(mpy(q, 1))`, priority 1.4) that takes `v0`;
   ugen writes `move v0,a0` at the switch head, as the original does. `t = q` (with or without
   the empty test) folds back; without the empty test `q * 1` also folds (97). Same shape as
   `t = q * 2; if (t < 0) {}` in func_80132708. 62 -> 52 (with the col change below at 62).
5. **Frame counter read through a pointer:** `GsWord *fc = &D_800E6280.unk_1104;` and every read
   as `fc->u` (as in the matched ETC func_80144224, T-25146; `*(u32 *)&D_800E6280.unk_1104.u` in
   the colour block alone gives the same bytes). An expression with an `ilod` is not copy
   propagated (`has_ilod`), so col stays one variable in `v0` and the two `col += col << 8`
   are done in place (`sll t6,v0,8; addu v0,v0,t6`); the counter read itself still uses the
   promoted `t0`. With a plain read, uopt makes the inner values CSE ranges (`v0`, `v1`, `t0`)
   and the frame counter moves to `t1` (cost 206-224), or distributes a multiply
   (`negu v0,t0`). 52 -> 14.
6. **Return on the same line as the last store** (`*(u8 *)&...unk_110C += 1; return w;`): as1
   then puts `lui at` before the add, as in the original. 14 -> 12.

## What is left (first divergence first)

1. First animation block, counter load: the original has `lui a1` (in the `beqz` delay slot)
   and `lbu a1,%lo(a1)`; the draft has `lui v0; lbu a1,4365(v0)`. The draft's uopt output has
   a load expression range in `v0` (`cvt(unk_110D)`, uses 2) that as1 coalesces into the `lbu`
   but not into the `lui`. Removing it with an indirect view (`b = *(u8 *)&...unk_110D;`) gives
   the right load shape but then every range of the block is one colour low (b `a0`, q `v1`,
   selector `a1`): the original seems to have two high-priority ranges in this block (load and
   division expressions) and still loads straight into `a1`.
2. Same block: the selector is loaded before the division (the FAKE `if (D_80122EAC) {}` from
   T-25053). The original loads it at the switch head but still in `a2`. Without the empty test
   the selector takes `v1` (cost 40-42). This moves `lw a2` and the as1 filler of the first
   `beqz` (`addiu t3,a1,1`).
3. Last block: the original copies w to `v0` before `sb t8`; the draft copies it after the store.

Also run: `autotweak --time 300 --score-jobs 2` on the cost-12 guard: 457 edits, no gain.

## Negatives added by T-25142 (all measured on the bodies named)

- Return/reuse: `s32` with `return q;` at the very end (q shared by both blocks): spills, 226.
  q and b reused for the last block: 78-104. `return w` at the function end (w function scope): 79.
  `s32` without any return: 81-105.
- Colour: `* 0x10201`, `* 0x101 * 0x101`, `(s32)` casts, `.w` view, `register`, a loop over
  `col += col << 8` (kept as a loop, 264), `* 1` barriers inside the colour block (206),
  reusing t as the colour variable (neutral), `0x101 * (...)` (100).
- Arm copy: `(s32)q`/`(u16)q`/`(s16)q` in the arms (62/152/130), recomputing `b / 14` in the
  arms instead of q (272-278, no PRE).
- First block counter: `GameState *g` pointer or `((u8 *)&D_800E6280)[0x110D]` (121),
  `volatile` view (194), u32/u8 b (60/98), promoted-global form like the last block
  (`D_800E6280.unk_110D / 14`, reset and `+= 1` on the global): 64-111.
- Selector test moved to the reset branch (40), to an `else if` (52), right before the switch (40),
  or dropped (40-42): the selector then takes `v1`.
- Last block: declaration order of c/w (neutral), `w = 0` first (78), `c = 0` first on s1 (68),
  a comma return (12, same as the one-line form).

## Next hypothesis

Find the source of the first block's second high-priority range without the load expression.
Try the indirect counter view together with an expression that the original computes in that
block and that uopt keeps as a range (for example the reset written through the same view,
`*(u8 *)&D_800E6280.unk_110D = 0` plus a local copy for `b + 1`), and check with
`regdiff` (TOKIMEMO_SCRATCH with r9-uopt) that b gets `a1`, q `a0` and the selector `a2` with
the selector test removed.
