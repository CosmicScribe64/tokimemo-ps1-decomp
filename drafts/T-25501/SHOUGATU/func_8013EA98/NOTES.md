# T-25501: SHOUGATU func_8013EA98, synthesis pass (wave 12)

Start 2026-10-23 18:12 UTC, worktree `w12-shougatu-final` (base w2-good). **Matched** at about
19:30 UTC. `func_8013EA98.c` next to this file is the C now in `src/ovl/SHOUGATU/8013E330.c`
(plain C, no guard). Checks: `funcdiff --resolve --version default` MATCH (920 bytes), clean build
27/27. No tool or config change.

The match has FAKEs, all marked with T-25501:
- the `* 0` references in the selection loop (b1 alias hole, selector and counter references);
- the forced choice held in the counter variable;
- an alias-hole record read in the D_80146054 compare;
- the forced store and the two following reads through the `D_800E71DF` alias. This needed a new
  keep line in `config/migrate_globals.txt`: `proof=func_8013EA98:130:cb9e034c`, proven by
  `tools/keepproof.py check` (130 rows with the symbol folded into the aggregate);
- the byte view in `func_800847B8`'s argument.

The sections below record how it was reached. The first part, up to "Verdict", was written at the
120-row checkpoint (U1) and is kept as history. The closing steps are in "Closing the post-loop"
at the end.

## The lenses

- **cleanroom** (`lens-cleanroom.md`): best W1a, 60 rows, with nested accept ifs and `break` for the
  final test. It measured that the latch block survives uopt and that only two latches of this shape
  exist in the whole game. It also isolated bb6, the empty node between the `>= 9` block and L_EBE8,
  and reproduced the target's whole loop shape with extra code (F5, F11). Negatives: `-Wo,-createbb`
  and rewriting the latch ucode.
- **compiler** (`lens-compiler.md`): the uopt rules that decide this function (empty edge nodes,
  holes, reload placement, split start and growth, cost split). Its y14 is opcode-identical to the
  target with 7 ranges in the wrong registers. It worked out the priority inequalities the
  original's order implies, and found `u8 >= 0` reads, which uopt keeps and ugen folds.

## What the synthesis found

1. **The loop form fixes the constant order without help.** In W1a's nested-if form the loop has 13
   flow-graph blocks. The do-while chain of y14 and of T-25046 has 14 to 16. With 13 blocks D is 4 for
   10/5/8 (priority 2.5), 5 for 9 (2.2), 11 for base (2.0), 6 for D_80146054 (1.83), 7 for 6 (1.57)
   and 10 for 0x38 (1.2). That is the original's order. The two `break` blocks of the final test lie
   outside the loop, so they add to 9's range and not to 10's. This solves the compiler lens's
   inequality set, which it had thought needed a bigger flow graph. So the original's loop very likely
   had this shape: the rejections jump to the end of the body, `while (1)` is the latch, and the two
   accepts are `break`s.
2. **Hole in the `>= 9` block without a base reference.** `9 + rec[F5F].b1 * 0` (W1d) is CSE'd with
   the header's b1, so it adds a b1 reference but no base or 0x38 reference. `rec[status * 0]`
   adds base (+10, which puts base first). `*(u8 *)status` is a register-base ilod, which is a hole
   for every global: it also drops D_80146054 out of its register. With the alias forced store
   (`D_800E71DF = FE4`, as in s2 and y5), W1dA already has the target's full constant order and
   leaves the selector uncoloured (-1.5).
3. **Free references.** In the loop, `(b2 >> 4) + *(u8 *)&F5F * 0` is a reference that uopt counts
   (+10 at loop frequency) and ugen folds. It adds no block. Never put it on the constant side:
   `!= 8 + F*0` merges the constant away (G1, 229 words). A plain-member `F5F * 0` after the loop is
   CSE'd to nothing (I3). The byte view in a store expression makes PRE move loads into the break
   blocks (I1, I2: extra code).
4. **Selector range window.** The original colours the selector after 6 and before 0x38. That needs
   a whole-range priority in [1.2, 1.571), and a total no higher than the cheapest register cost
   (11.6), so that a cost split happens. The split piece then starts at L_EBE8:
   {7 L r, 8, 10, 11, 13, latch r}. That is the original's piece, with reloads at L_EBE8 and the latch
   and none in bb6. With two free references the total is 11 (1.83, too high). The post-loop hole
   `FE4 + rec[FE4 * 0].unk_06 * 0 == 9` makes the next two blocks reload (-2) and takes one block
   out of the range. The `FE4 >= 0` block puts it back (D stays 6). The total is then 9/6 = 1.5. The
   tautology changes no other range's D bucket.
5. **Counter above b1.** Two `status * 0` terms in the same expressions give status 51/5 = 10.2
   against b1's 40/4 = 10.0, so the counter is in v0 and b1 in v1 (M1).

Path (rows): W1a 60, W1dA 82, H1 170, g1an 160, J1 146 (first body with the full order), L1 138,
M1 120, U1 120 (plain post-call reads, the kept body). Bodies are in `final/b/`.

## What is left (U1, `final/odiff.py U1`)

- The post-loop `lbu a0,%lo(F5F)` takes its `%hi` through t6. The original uses v1. This is ugen's
  temporary choice. Every later temporary (t6..t9) is one register off from here, which is most of
  the 120 rows, including the then-branch's `li t9,0x18` scheduling.
- `FE4 == 9` gives `bne t0,v0` where the original has `bne v0,t0`. The byte view of a scalar global
  is `lda; ilod` in cfe's output, so uopt swaps the compare to constant first. A local copy
  (`status = D_80145FE4`, `final/b/Q_status.c`) gives `bne v0,t0` but turns `beq t2,v0` (the
  D_80146054 test) into `beq v0,t2`. No spelling of either operand changed that (R1-R3, S1-S3,
  P1-P6). The plain member gets the CVT under the cvt_pass entry rule, and uopt then puts the
  constant first again. Every body since T-25046 has these two post-loop rows.

## Where the post-loop `lui v1` comes from (second hour)

The pattern `lui R,%hi(F5F)` ... `lbu a0,%lo(F5F)(R)` is ugen's reload of a register range R whose
first use is the call argument, with the copy into a0 folded away. X4 shows it with R = t4 when the
selector range is coloured whole, and W1a with R = a0 for the forced-store piece. A plain load
through a temporary gives t6..t9, rotated by the number of folded `* 0` terms (W1..W4: t8, t9, t7,
t6). It never gives v1. So in the original the selector's value at L_EC7C was a coloured range in
**v1**: the register b1 frees at the loop exit.
- `final/b/syn_Z1.c` shows it. It reads the two post-loop loads through the D_800E71DF alias as well, so
  they form their own range {forced store S, L_EC7C L2 r} (0.5). That range is coloured v1 and gives
  exactly the original's `lui v1` (delay slot of the `== 7` branch) and `lbu a0,%lo(v1)`. But then
  the selector range no longer reaches into the call block, its cheapest caller-saved register
  (t4) costs 1, and it is coloured whole (bb6 reload, stray `andi`): Z1 152, Z12/Z13 112..116,
  Z4/Z5 112.
- In U1 the plain post-loop loads keep the selector range in the call block. No caller-saved
  register is then available below s0's 11.85, so the cost split happens. Its remainder
  {bb6 r, bb16 r, bb15 r, bb21 r, L_EC7C L2 r, post-call L1 r} is -5.25 and stays in memory, so
  L_EC7C loads through a temporary (t6).
- A real forced store (`final/b/syn_Y1.c`) splits the {store, L_EC7C} piece first (a0), and the remainder is
  coloured whole (t4, stray `andi`), as the compiler lens found.

- Why U1 splits and Z13 does not. U1 and Z13 have the same selector units except at L_EC7C (L2 in
  U1, L1 in Z13). In U1 the second load is the call argument itself, so that live unit is
  precoloured to a0. uopt's `cupcosts` (uoptreg2.c, n64decomp/ido d068e43) returns 1e20 for a
  precoloured unit when the candidate register is in `regscantpass`. That leaves s0 (11.85) as the
  cheapest register, above the range's total of 9, and causes the cost split. In Z13 the argument is
  an expression, t4 costs 1 (one call-adjacent unit) and the range is coloured whole. In Z1 the alias
  range's precoloured unit still gets v1, so v1 is outside `regscantpass` and t4 is inside it.
- More negatives in this direction:
  - Folded `* 0` terms in the loop rotate ugen's temporary for the `lui` (W1..W4: t8, t9, t7, t6)
    but also move the constant ranges. The same terms after the loop (AB1..AB3, AC1..AC3) do not
    rotate it.
  - Triggering the cvt entry rule for FE4 and D_80146054 with an empty `if (G) {}` at the entry
    (AA1..AA4) promotes them from the entry: 242-259 rows.
  - Moving the D_80146058 store into the call argument (X1), or reading the argument through
    D_80146058 (X2), breaks the split.
  The `syn_*.c` files in `final/b/` keep the representative bodies.

Under IDO 5.3's costs, then, the original's three facts can't hold together: the loop piece is a
cost split from L_EBE8, the post-loop piece is coloured in v1, and bb6 gets no reload. The
remainder that would hold the post-loop piece has two loop-frequency reloads (bb6, bb16) that make
it negative. This is the strongest sign so far that the original's uopt costed reloads differently
from 5.3 (compare the T-3100 hypothesis that the original charges no entry load). It is not a rule
that can be built: T-9800's `free_entry_load` and `half_move_cost` candidates change hundreds of
matched functions.

## Verdict

Mechanism: uopt's colouring of the selector range plus the flow-graph block counts behind the
priority order. Both are reproduced. The loop needs these FAKEs: the b1 hole, two selector and two
counter free references, the alias forced store, the post-loop hole and the tautology block, plus
the inherited byte views. The leftover rows come from ugen's temporary choice and the operand order
of two post-loop compares, not from allocation. No build change is justified. No uniform rule is in
sight, and the T-9800 and T-20610 censuses already rule out allocator and entry-rule changes that
touch other functions.

## Next step

Work only on the post-loop block of `func_8013EA98.c` (U1). Keep the loop and its FAKEs as they are.
1. Get a coloured post-loop selector piece in v1 (as Z1 does) while the loop piece stays a cost
   split from L_EBE8 (as U1 does). The open question is what makes U1's whole selector range miss
   t4 (s0 at 11.85 is its cheapest register), while Z13's range, which has the same loop units,
   gets t4 at cost 1. Reading uopt's `cupcosts` for a global that is live into a call block would
   settle it.
2. Get FE4 first against 9 and D_80146054 first against FE4 together. Candidates: an FE4 local
   with the D_80146054 compare written through a form that is not a plain variable, or FE4 read
   before the loop (the entry rule then drops its CVT).

## Tools (run from the worktree root)

`final/ev.sh L...` scores `build/ea98/L.c` (whole unit, `m.py`). It traces uopt with the T-25044
`uopt-trace` binary (`build/ea98/tr/uopt-trace`, copied from the compiler lens) and writes
`build/ea98/L.order`, the colouring order with labels. `final/order.py` prints that order,
`final/cfg.sh` the flow graph, `final/odiff.py` an instruction diff against the target with
pseudo-ops normalised, `final/mk.py` makes literal-replacement variants, and `final/grid.py` holds
the post-loop grid. `m.py`, `tr.py` and `pri.sh` come from T-25143/T-25305.

## Closing the post-loop (match)

1. **The split comes from a precoloured unit.** This was the cost-split question left open above.
   When the selector itself is a call argument (a byte view, so no CVT and no conversion), uopt
   precolours that live unit to a0. `cupcosts` then gives every register in `regscantpass` the
   cost 1e20. The cheapest register left is s0 (11.85), above the range's total, so the cost split
   happens. Moving that unit from L_EC7C to the post-call `func_800847B8(*(u8 *)&F5F)` keeps the
   split.
2. **The two L_EC7C reads go through the alias** (`D_80146058 = D_800E71DF;`
   `func_80051A68(D_800E71DF)`). With the alias forced store they form their own range,
   {store, L_EC7C}, which is coloured v1. That gives the original's hoisted `lui v1` and
   `lbu a0,%lo(v1)`, and every later temporary matches.
3. **A free reference in the forced store** (`status + *(u8 *)&F5F * 0`) keeps the selector range
   live through the exit path. Its priority is then 9/6 = 1.5, inside the window (AD1: 2 rows).
4. **Operand order of the post-loop compares.** Copying FE4 into the counter variable makes
   `status == 9` give `bne v0,t0`. Putting the hole term on the right of
   `D_80146054 != status + rec[status * 0].unk_06 * 0` gives `beq t2,v0`. AE1 matched with 0 rows.
5. **Removing FAKEs, one at a time, measured.**
   - Not needed: the `>= 0` block, the loop's byte view of D_80146054, the byte view in `!= 6`,
     and the byte view in the first free reference.
   - Needed (rows when removed): the hole in the D_80146054 compare (22), the counter references
     (18), the alias forced store (152; a real store gives 155), the b1 hole (198), the byte view
     in `func_800847B8` (110), the alias reads (136), the `(u8)` narrowing (108). Making all loop
     views plain gives 226.
   - Combinations interact: several pairs of single removals that each give 0 move the frame to
     0x60 (k1..k4, p1, p2, p4, q1, q2). p3 is the kept body.
