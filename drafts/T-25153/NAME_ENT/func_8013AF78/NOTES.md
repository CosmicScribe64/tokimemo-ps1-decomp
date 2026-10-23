# T-25153: NAME_ENT func_8013AF78 (74 words), not matched

Wave 9 neighbor conveyor (T-25200), worktree w9-NAME_ENT-80139740, about 75 minutes.

## Start state

`src/ovl/NAME_ENT/80139740.c` held the T-24132 guard (all-`u8` locals, `s32 pad[2]`, bare fallthrough):
99 rows in `funcdiff --resolve --version c`, 75 words against 74. That is over the 30-row guard limit, so
the source is now plain `INCLUDE_ASM` again and the bodies live here.

## Result

- `body.c`: 57 funcdiff rows, 75 words. Best measured body.
- `body_fallthrough.c`: 61 rows, 73 words. Same recovery with the original exit shape (`return 0` in the
  zero arm, bare fallthrough). `b`, `c` and `a` sit in the target registers (`$a1`, `$v1`, `$a2`).

Both bodies contain FAKE constructs (see below). Neither can be promoted as it is.

## What was measured (causal, one change at a time)

A scratch harness compiled only this function through `tools/cc.py` (IDO 5.3 with the cvt, frame and
ugen passes) and diffed the objdump against `expected/`: 2 s per variant. The row counts agree with
funcdiff within 2. The traced uopt (`regdiff -v`, `R9UNIT` lines in `build/scratch/regdiff/.../draft.log`)
explained each register move.

1. **Loop shape, closed.** `sltu` (not `bne`) comes from reusing the loop counter later in the function:
   a counter that is only the loop index is turned into an exact-count `bne` loop. This is the same
   mechanism as RPG_BAT `func_8014E994`. Reusing `i` as the selector (`i = D_8014D0D4 % 3; sel[i] = 1;`)
   or using one of the value locals as the counter both give the target loop.
2. **Raw loads, closed.** The original's `lui X; lbu X; andi tN,X,0x7F; or X,tN,zero` is the int-typed
   assignment `x = D & 0x7F` (tree root AND, no u8 truncation). OPTION `func_8013A334` shows the same
   shape for an argument (`D_800E6280.unk_F5F & 0xF`). With `u8` locals the root is a CVT and the load
   goes to a ugen temporary (2 temps per mask). The value locals are `s32`.
3. **Frame, closed.** IDO frame = round8(locals + 4), locals laid out top-down in declaration order, so
   register-allocated scalars take slots too. Target IDO frame 0x20 with `sel` at 0xC means two words
   above `sel` and two below. Four `s32` values plus a separate `i` do not fit. `s32 a; s32 b; s32 sel[3];
   s32 c; s32 d;`, with a value local as the loop counter, gives 0x30 and `sel` at 0x1C, no padding.
4. **`c` split, closed with a FAKE.** `c = D_80120BA3 & 0x7F` is still available in the `sel[0]` arm
   because BA3 is stored only after `sel[1]`. uopt copy-propagates it into `c |= 0x80`, and the result is
   a CSE range plus a separate `c` range: two registers, and `$a0` was taken from `arg0`. A selector store
   that may alias the globals, `((s32 *)(u32)sel)[D_8014D0D4 % 3] = 1;`, kills the availability. `c` is then
   one range in `$v1`, and `c = (u8)(c | 0x80)` gives the target `ori t8,v1,0x80; andi v1,t8,0xFF`.
   Plain `sel[...]`, `(&sel[0])[...]` and byte-offset spellings do not kill it. A self-store
   `D_80120BA3 = D_80120BA3;` is removed before copy propagation runs.
5. **arg0 colour, closed with a FAKE.** Unconstrained ranges are coloured in bit order, and bits follow
   the first reference. `arg0` is first referenced at the test, after the masks, so `c` took `$a0` and
   `arg0` moved to `$a3` (`move a3,a0`). An empty `if (arg0) {}` before the loop gives `arg0` bit 0 and
   keeps it in `$a0`. No natural source for this early reference was found.

## First divergence left (body_fallthrough.c)

`$v0`. The target keeps the loop end bound and `d` in `$v0`. In IDO 5.3, a range with a unit in the zero
arm, which ends in `return 0` and so defines the return register, cannot take `$v0`. Every value local is
stored in the zero arm, so `d` gets `$a3`. If the counter is `c` or `d`, the loop end bound gets `$v1`
too. A traced counter-`d` control gets `$v1` even when it is coloured first. The `sel[2]` arm also loses
`d`'s truncation: `d` is dead after `D_80120C2B = d`, so the update is folded into the store
(`ori; sb`), while the target materialises `andi v0,t4,0xFF; sb v0`.

`body.c` instead ends with `return d;` and sets `d = 0;` in the zero arm. Semantically this is exactly the
target, where `$v0` holds `d` on the nonzero exit. It keeps the truncation, but IDO adds `move v0,dX`,
because a range live into the final return block cannot be `$v0`. Earlier agents measured the same
extra instructions for explicit final returns.

## Negatives added (do not repeat)

- Loop: `u32`/`s16`/`u8`/`u16` counters, `!=`, `<=`, down-counting, while, sizeof bound: all `bne`.
  Pointer do/while or for loops give `sltu` but keep `&sel` as a CSE (frame +8, extra copy).
- Locals: an exhaustive sweep of 512 type, update, init, exit and early-arg0 combinations over the
  T-24132 layout: best 69. A type sweep of each local in the counter-reuse body: no change for
  `s32`/`u32`, the others worse.
- Globals in place (`D_X &= 0x7F; ... |= 0x80`, record-array view `((DisplayActor44 *)D_80120650)[19..22]
  .unk_03`, volatile reads, `(s32)`/`(u8)`/`(u32)`/`(s16)` casts on the load): the narrow global keeps
  its CVT in arithmetic (cvt_pass), so it is loaded into a temporary. Empty `if (D) {}` tests promote all
  four globals (they get bits in test order), but the entry loads still go to temporaries. 99 to 123 rows.
- Masks split across both arms (PRE hypothesis): IDO does not hoist; each arm reloads.
- `return d` variants: always `move v0,dX`.
- Dead pre-stores (`d = 0; c = 0;` before the loop) reorder bits, but `d` still cannot take `$v0`.

## Next hypothesis

The four masked values are separate uopt ranges whose zero-arm uses go through the AND expression (the
ugen temporary), not through the locals. If the zero-arm stores read a CSE, the CSE register's `move` is
dead after as1 rewrites the stores to the temporary, and as1 deletes it. `d` would then have no unit in
the zero arm and could take `$v0`. Test a source where the zero arm stores the expression and the
nonzero path keeps the local. For example: compute `d = D_80120C2B & 0x7F` once, write the zero arm as
`D_80120C2B = D_80120C2B & 0x7F` (same value number, CSE), and then check in `R9UNIT` that `d` has no bb6
unit. Combine this with the `body_fallthrough.c` structure. If `d` gets `$v0`, the `sel[2]` truncation is
the last open item. If no source can do this, document the zero-arm `$v0` rule in
`wiki/build-step-gaps.md`. Also look for a natural replacement for the two FAKEs: something that aliases
`sel` (a pointer parameter or a shared helper view), and an earlier natural reference to `arg0`.
