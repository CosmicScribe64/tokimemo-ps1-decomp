# T-25301: NAME_ENT func_8013AF78 (74 words), second pass, not matched

Wave 10 second pass (umbrella T-25300), worktree w10-NAME_ENT-80139740, about 95 minutes.

## Start state

T-25153 left plain `INCLUDE_ASM` and two drafts: `body.c` (57 funcdiff rows) and `body_fallthrough.c`
(61 rows). Its first divergence: `d` and the loop end bound need `$v0`, which IDO 5.3 refuses to a
range used in the `arg0 == 0` arm, the block that ends in `return 0`.

## Result

- Guard in `src/ovl/NAME_ENT/80139740.c`: 22 funcdiff rows (11 words), all in the mask block after the
  loop. Every register, every ugen temporary, the loop, the frame and both arms match. What differs is
  only the as1 order of the 17 mask-block instructions.
- With global copy propagation off (`OPTN 405 0`, a scratch variant of `tools/cvt_pass.py`) the same
  body matches every instruction of the function (0 rows in the scratch harness). This is a build-step
  gap, not a C problem (see "Copy propagation" below).
- `body_18rows.c`: 18 rows (9 words). Mask statements in d, c, b, a order give the original schedule,
  but then the ugen temporaries follow that order. It needs the T-25153 `(u32)` cast FAKE as well.

## Scratch harness

`harness/` (copy it to `build/t25301/` in a worktree; it writes only under `build/`):
`cmp.py BODY.c [-q] [--entry]` splices a body into a copy of the file, compiles it with `tools/cc.py`
and diffs the function against the `.s` file with the relocations resolved (12 s per variant on a
loaded machine; its row count equals funcdiff's). `cmpz.py` is the same with `cc_zc.py`, which swaps
the uopt shim for `cvt_zc.py` (cvt_pass with zcopy off for every procedure of the unit). `lst.py`
prints ugen's `cc -S` listing of the body. Traced uopt: `regdiff.py --src build/t25301/o/<tag>.c -v`
with a copy of `r9-uopt/variants/trace` in the scratch dir.

## What was measured, in causal order

1. **The zero-arm rule, closed with a FAKE.** Diagnostic: removing the zero-arm `D_80120C2B = d`
   store gives `d` `$v0`, so the T-25153 rule is right. A label between the four zero-arm stores
   and `return 0` (`goto ret0; ret0:`) moves the return into its own block; no range used in the
   stores' block is forbidden `$v0` any more. The ugen listing is unchanged (ugen merges the blocks);
   only uopt's colouring changes. `do {} while (0);` or `while (0) {}` before the return work the same.
   `if (arg0 == 0) { ... } else { ...; return; } return 0;` gives the same colours but puts the
   return block last (layout wrong). `return arg0;` is worse (83 rows).
2. **Loop registers, closed.** With the label, the loop counter must be `d`
   (`for (d = 0; d < 3; d++)`): `d` is coloured first and takes `$v0`, so the pointer gets `$v1` and
   the end bound `$v0`, as in the original. Counter `c` with a dead `d = 0;` gives the right value
   registers but swaps pointer and bound.
3. **Colour order of the four values, closed.** Unconstrained ranges are coloured in bit order. A
   local's bit comes from the statement that first assigns it, so the masks `b, c, a, d` give `b`
   `$v1`. Bits d, c, b, a are needed. Two ways: masks written in d, c, b, a order
   (`body_18rows.c`), or the copies `d = D_80120C2B; c = ...; b = ...; a = ...;` in their own block
   followed by `b &= 0x7F; c &= 0x7F; a &= 0x7F; d &= 0x7F;` (the guard). The second empty
   `if (arg0) {}` ends the copy block. Without it uopt folds each load into its mask in the same
   tree and the bits follow the masks again.
4. **Ugen temporaries.** The original's masks use t6 (b), t7 (c), t8 (a), t9 (d): ugen emitted the
   masks in b, c, a, d order (FIFO free list from t6). Its as1 order (luis d c b a, lbus a b c d,
   masks a b c d, moves d c b, `a` in the delay slot) is what as1 makes of one source line whose
   loads come in d, c, b, a order: in a one-line group as1 keeps the `lui`s and the moves in
   emission order and reverses the loads and masks. So the original emitted the four loads
   (d, c, b, a) apart from the four masks (b, c, a, d). A volatile declaration of the four globals
   (scratch header, reverted) keeps the loads apart: the temporaries then match, but volatile pins
   the `lui`/`lbu` pairs (58 rows). Copy propagation off does the same without pinning: 0 rows.
5. **Nonzero arm ring, closed without FAKEs.** The original spends one ugen slot more in the sel[0]
   arm and one more in the sel[1] arm than `D_X = v | 0x80;`. Writing every update through its
   local, `a = (u8)(a | 0x80); c = (u8)(c | 0x80); D_80120BE7 = a;`,
   `b = (u8)(b | 0x80); D_80120B5F = b;` and `D_80120BA3 = c = (u8)(c | 0x80);`, gives every
   temporary, the two hoisted `ori`s (t7, t8) in the div shadow, and the unhoisted `ori t0,a1`.
   (`(v | 0x80) & 0xFF` on the stores does the same, FAKE; the `a` store must come after the `c`
   update for the `andi v1; sb t7` order.)
6. **Alias FAKE gone.** With the copies apart from the masks, the plain `sel[D_8014D0D4 % 3] = 1;`
   matches; the T-25153 `((s32 *)(u32)sel)` cast is only needed in the `body_18rows.c` shape.

## Copy propagation (the gap left)

In the guard, IDO 5.3's global copy propagation substitutes each `d = D_80120C2B` copy into
`d &= 0x7F` across the empty-test block boundary, so ugen emits load and mask together in mask order
(b, c, a, d) and as1 cannot produce the original's load order. Casts on the copy (`(s32)`, `(u32)`,
`(u8)`, `(s16)`, `(u16)`, `+ 0`, `| 0`) are propagated too. A second use of the raw value (empty
`if (d) {}` tests) keeps it in its own range and register (79 to 96 rows). The scratch
`cvt_zc.py` (zcopy off for every procedure) matches this function with the guard body unchanged.
This is the T-11570 / T-16900 copy-preservation family: there the original kept a local copy of a
narrow signed global; here it keeps four copies of narrow unsigned globals that are masked in a later
block. T-16900 showed that zcopy off everywhere changes 12 matched functions, so a pass needs a
narrow rule. A candidate to test against the matched corpus: zcopy off for a procedure that stores a
direct load of a narrow unsigned global into a local and the next block reassigns that local from
itself (`x = D; ...; x &= k;`). That is a build-step change and needs approval.

## Negatives added (do not repeat)

- Line groupings of the masks in b, c, a, d order: all 16 compositions, with and without the
  `if` on the last line. `lui`s always come out in emission order.
- Masks in d, c, b, a order on separate lines (54) or one line (48 before step 5).
- Split loads and masks in one block, any load order or line layout: uopt folds them (same code as
  the mask form).
- u8 locals (102 rows), u32 locals (no change), `(u8)` casts on the mask loads (no change).
- Empty `if (D_X) {}` tests on the globals to promote them (93 rows); in-place global forms
  (`D_X &= 0x7F`): all four promoted with tests, but each narrow store spends two ugen slots (120).
- Record-array views `((DisplayActor44 *)D_80120650)[19..22].unk_03` with locals: same as the
  plain names; they do not alias each other for uopt.
- Measured with zcopy off on the guard body: without the first `if (arg0) {}` 35 rows (arg0 loses
  `$a0`), without the second one 34, without the label 63, with the plain selector store 0. In the
  normal pipeline the `body_18rows.c` shape without the first test gives 45, without the cast 62.

## First divergence left

Row 7 of the mask block: `lui v0,%hi(D_80120C2B)` in the original, `lui a1,%hi(D_80120B5F)` here,
because the load of B5F is emitted with its mask first. Cause: copy propagation (above).

## Next hypothesis

Either (a) get approval for a narrow zcopy rule in `tools/cvt_pass.py` (start from the candidate
rule above, check it on this function and the T-11570 drafts, then run the full matched regression),
or (b) find C that keeps a copy of a global apart from its later use under IDO 5.3 without a second
use of the raw value. Untested ideas for (b): the copies in a block that has a second predecessor
(a path on which the locals keep other values), or a copy through a local array element read back.
