# T-24926 GEKO:func_80133024 — two rows, the day-address base

State: the 2-row guard is unchanged (the src file is byte-identical to the committed
T-24503 baseline; all checks pass). The two rows are fully explained by the traced
uopt; the target rows were made to match once (A'' below), which fixes the diagnosis
but broke two other things. No production C changed.

## The exact rows

```
-@row 33 3c06800e  ; lui a2,0x0      (target: base materialised in a2, the load destination)
+@row 33 3c05800e  ; lui a1,0x0      (draft: the day-address range takes $a1)
-@row 35 90c662c0  ; lbu a2,0(a2)    (target: HI/LO of D_800E62C0, offset 0)
+@row 35 90a662c0  ; lbu a2,64(a1)   (draft: HI/LO of D_800E6280, offset 0x40)
```

Resolve-equal forms (the symbol naming is fine); only the register fields differ.

## Diagnosis (instrumented uopt, regdiff FIRST DIVERGENCE + the raw ucode)

- The draft has TWO day ranges: the local `day` (ucode bit 14, colour 5 = `$a2`, the
  load destination) and the field-address range `global D_800E6280/unk_040`
  (bit 16, colour 4 = `$a1`, the `lui` base). `lui a1; lbu a2,64(a1)`.
- The target has ONE: the day value lives in the FIELD node, so the load is
  base = destination = `$a2` (`lui a2; lbu a2,0(a2)`), like the month
  (`lui a3; lbu a3,%lo(D_800E62BF)($a3)`) and the `unk_03E` test load
  (`lui t0; lbu t0,%lo(D_800E62BE)(t0)`, an uncoloured 1-use address).
- The FAKE `if (day) {}` reads the FIELD, not the local: the copy propagation is
  BLOCK-LOCAL, and the test is in the same basic block as the assignment, so its
  operand is propagated to the field node. That keeps the field's VALUE range alive
  next to the local's range, and it is coloured `$a1`.
- The address range (the `lui` constant 0x800E0000) is always coloured (its span is
  tiny, cost 0) into the FIRST FREE colour. With the day value in a local, that is
  `$a1` — it can never share `$a2`. Measured twice (E1/E2 below).
- The month is `$a3` in the draft BECAUSE the day field's `$a1` range takes a1 and
  the day local takes a2 before the month is coloured. In any structure without a
  separate day-field a1 range, the month field takes the free `$a1` (measured, A'').

## Experiments (all measured on the full TU, funcdiff --resolve)

1. **A'' — direct field reads (PROVEN for the target rows).** Drop the `day` local's
   value (keep the `u8 day;` declaration for the frame slot) and read
   `D_800E6280.unk_040` directly at every use (the empty test, the compare, both
   arms). The day load comes out EXACTLY `lui a2,%hi(D_800E6280); lbu a2,64(a2)` —
   base = destination, rows 33/35 fixed. BUT: the month falls to `$a1` (its field
   range takes the first free colour), and the `day + i` sum becomes a coloured `$v0`
   range CSE'd into both arms (`addiu a1,v0,-1` / `subu a1,v0,v1` instead of the
   per-arm `addu a1,a2,v0` / `addu t7,a2,v0` with the `or v0,s0,zero` i-copy in the
   `bnez` delay slot). ~15 rows total. The frame stayed 0x58 with the unused
   declaration.
2. **E1/E2 — branch-split the day test (no row change).** Putting the month's
   assignment/test between the day's assignment and `if (day) {}` makes the day's
   test read the LOCAL (the copy does not cross the month test's branch), so the
   field range is address-only — still coloured `$a1` (first free). Same 2 rows.
3. **A'' + arm-swap (DISPROVEN).** Moving the dead table test
   `if (days.day[month - 1]) {}` AFTER the `func_80132D04` call (to make the
   month's range overlap the a1-argument and skip a1): the month's range then
   crosses the call, uopt allocates `$s2` (callee-saved), the frame grows
   0x58 -> 0x60 and every register shifts (~20+ rows). The original cannot have
   the dead read after the call. Do not retry.
4. Forbidden-list respected: no empty-test spelling grid re-run, no per-file
   options, no build changes, no inline asm.

## Next hypothesis (in order)

a. Start from the A'' structure (it fixes rows 33/35). Keep `u8 day;` declared but
   unused for the frame slot (verified: the frame stays 0x58).
b. Fix the sum: break the CSE between the compare's sum and the arms' sums by
   swapping the compare's operand order — `if ((u32) n >= (u32) (i + D_800E6280.unk_040))`.
   The compare's sum becomes a 1-use node (a ring temp, the target's
   `addu t4,a2,s0`), the arms keep their own `field + i` sums, and the i-cvt
   regains three uses so it is coloured `$v0` with the `or v0,s0,zero` delay-slot
   copy back. UNTESTED (the 45-minute box ran out).
c. Fix the month: it needs `$a1` taken (or costly) at its allocation while the day
   field takes `$a2`. Ideas not yet tried: make the month's value live in its LOCAL
   (coloured) with its field node address-only — the month load then has
   base = destination = the month local's colour, which skips `$a1` only if the
   month local's range overlaps the a1-argument in an arm WITHOUT crossing the
   call (a use between the compare and the call); or run `tools/viewsweep.py` on
   the day/month widths (u8/s8/s16) — a width change may flip the priorities and
   the colour order (ticket option 3).
d. The remaining draft ucode facts for reference: first table index
   `uixa{days}{local month}`, second `uixa{days}{field month}` followed by
   `ustr{n}{second-load}`; the arms use the CSE'd sum node; a re-emitted propagated
   compare path (`uadd{field}{icvt}` / `ugeq`) exists in the listing.

## Tools

The traced uopt was copied to `$TOKIMEMO_SCRATCH/r9-uopt/variants/trace`
(from scratch-T-24705); `TOKIMEMO_SCRATCH=<scratch> tools/docker.sh python3
tools/regdiff.py GEKO:func_80133024` works and leaves the raw trace in
`build/scratch/regdiff/func_80133024/draft.o.trace` (the ucode listing and the
colouring are in the `func_80133024` section).
