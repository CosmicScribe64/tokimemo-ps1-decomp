# T-25501: SHOUGATU func_8013EA98, synthesis pass (wave 12)

Start 2026-10-23 18:12 UTC, worktree `w12-shougatu-final` (base w2-good). Not matched.
`src/` keeps `INCLUDE_ASM`. Best body: `func_8013EA98.c` next to this file, 120 rows
(`funcdiff --resolve --version c`, whole unit), 230 words, frame 0x58. No tool or config change.

The selection loop (0x8013EB0C..0x8013EC50) now matches the original instruction for instruction,
registers included. That had not happened in any earlier pass. All nine loop ranges are coloured in
the original's order: 10 a1, 5 a2, 8 a3, 9 t0, base t1, D_80146054 t2, 6 t3, selector t4, 0x38 t5,
with the counter in v0 and b1 in v1. What is left is all after the loop (list below). It is worth more
rows than W1a's 60 because one temporary register differs and every later temporary shifts with it.

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
1. Find out why ugen takes v1 for the hoisted `lui`. Compare uopt's output (`tr.py`, `fn.after.txt`)
   at L_EC54..L_EC7C between U1 and W1a, which gets `lui a0`. Then look for a body where v1 is the
   free temporary at that point, for example by changing what is live in v1 at the loop exit.
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
