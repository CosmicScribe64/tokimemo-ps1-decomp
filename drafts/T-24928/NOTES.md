# T-24928: main:func_8006C988 (src/main/800674B0.c) — 11 rows to 5

Result: **not matched; guard kept at 5 rows** (was 11, T-24301/T-24704 state). The stage-1
base/value `$t0`/`$t1` swap is **resolved**; the remaining 5 rows are exactly the ticket's
predicted stage-2 (the last-value copy placement) plus a two-row induction-init order.

## Exact remaining diff (funcdiff --resolve, 5 rows)

```
  -@row 5 25296280  ; addiu t1,t1,0      ← induction-init lo half
  +@row 5 00003825  ; move a3,zero      ← i = 0 — order pair with row 6
  -@row 6 00003825  ; move a3,zero
  +@row 6 25296280  ; addiu t1,t1,0
  -@row 26 25290008  ; addiu t1,t1,8     ← original: base advance in the b-slot
  +@row 26 310300ff  ; andi v1,t0,0xff   ← draft: cur = val copy in the b-slot
  -@row 30 25290008  ; addiu t1,t1,8     ← original: base advance in the else arm
  +@row 30 310300ff  ; andi v1,t0,0xff   ← draft: cur = val copy in the else arm
  -@row 32 01001825  ; move v1,t0        ← original: ONE join copy in the latch slot
  +@row 32 25290008  ; addiu t1,t1,8     ← draft: base advance in the latch slot
```

All register rows are gone: `lui/addiu t1` base, `lbu t6/v1/t0, imm(t1)`, `move v0,t0`.

## The diagnosis (regdiff + the uopt trace listing)

- The uopt colouring model for this leaf: **unconstrained ranges are coloured in web-bit
  order**, colours tried v0, v1, a0..a3, t0.. — web bits follow **first mention in the
  (post-copy-prop) ucode table** (dumped between REMOVAL OF REDUNDANT STORES and CODE MOTION;
  `M295-20` in the table is the hidden unroll-end temp local of the second loop, bits 46-52
  are the parameters, whose webs are created at their only use `*first = sel` / `*last = cur`).
- Draft baseline: address/induction web bit 78, value (ilod) web bit 79 → base takes $t0,
  value $t1 (swapped against the original). A **local** first-def'd inside the first loop gets
  a bit around 32, i.e. **before** the parameter webs → it takes $a0 and spills `first`
  (this is why every prior "add a local" probe displaced an argument register).
- Target allocation: value web coloured **after** `first`/`last` (their bits) and **before**
  the address web → value $t0, base $t1, M295-20 shares $t0 (disjoint), second-loop walk
  shares $t1 (disjoint), constants 1/2/0xF back at t2/t3/t4. Confirmed reproducible.

## Experiments (measured with funcdiff --resolve; regdiff after each structural change)

1. **E1** `val` local assigned once after the if (`val = ...; cur = val;`): copy-propagated
   away (first-loop rows identical to the baseline), only caused the $a0/spill. Confirms the
   T-24704 "separate value local" probe — a one-def local is transparent to the copy
   propagator.
2. **E2** `val = g->unk_75E[n].unk_00;` in **both arms**, `sel = val;` in the else arm,
   `cur = val;` at the join: the val web survives (2 defs → join use, exactly the original
   web shape; loads go straight into val's register) but takes **$a0** and spills `first`
   (fifth local, bit 32 < 46). Still 11+ rows (second-loop registers shifted too).
3. **E3** E2 + dead early uses `if (first) {}` / `if (last) {}` **between the inits and the
   loop**: the parameter webs move before val's → **val takes $t0**, base **$t1** — the swap
   is fixed — but the tests split the preheader and the induction init **unfolds**
   (`lui t7; addiu t7; sll t6,zero,3; addu t1,t6,t7`, const in t7, unk_04 load pushed to t8).
4. **E4 (current)** the two dead tests moved **to the very top** (before `sel = 0xFF;`):
   the preheader is unbroken again, the induction init folds (`lui t1; addiu t1`), val keeps
   $t0, base $t1, and the loads/`move v0,t0` match. **11 → 5 rows.**

## What is left and the exact next hypotheses

- **Stage 2 (rows 26/30/32):** the ucode trace shows the copy propagator rewriting the join
  `cur = val` into per-arm `ustr cur {ilod}` nodes (bit 85 in the E2 trace listing), so ugen
  emits the narrowing copy per arm and as1 parks it in the `b` slot / after the sel copy,
  while the induction advance keeps the latch slot. The original has the **advance per arm**
  and one `move v1,t0` in the latch slot. Next, in order:
  1. a join read the propagator will not rewrite (`cur = val | 0;`, `cur = (u8)(val ^ 0);`)
     — one of these may also flip which instruction as1 picks for the latch slot;
  2. a second (dead, FAKE) use of `val` in the join region after the loop, so the def-use
     chain does not split per arm;
  3. if the copy stays per-arm regardless: make the arms carry the advance instead — the
     original's per-arm `addiu t1,t1,8` suggests its loop tail evaluated the next record
     address inside each arm; try `g->unk_75E[n + 1]` addressing for a *following* statement,
     or an `n++` placement that moves the induction increment into the arms.
- **Row 5/6 order:** the induction-init `addiu t1` wants to schedule **before** `move a3`
  (i = 0). The g-init/induction-init statement position decides it; try `g = &D_800E6280;`
  as a statement between `n = 0;` and `i = 0;` (declaration stays at the top), and the
  for-loop spelling of the first loop (`for (i = 0; i < 0x100; i++)`, which IDO rotates).

## Prior probes not repeated (ticket forbidden list)

`s32 sel/cur`; `g->unk_75E[n++]`; a one-def separate value local (`u8`/`s32`); direct
`D_800E6280.unk_75E[]` without `GameState *g`; autotweep 323/424 edits; viewsweep.

## Checks

- `tools/docker.sh python3 tools/funcdiff.py --resolve main:func_8006C988`: DIFF, 5 rows
  (rows 5, 6, 26, 30, 32), all register swaps resolved.
- `tools/docker.sh ninja build/src/main/800674B0.o`: passes (default build unchanged; the
  guard keeps `INCLUDE_ASM` in the default path).
