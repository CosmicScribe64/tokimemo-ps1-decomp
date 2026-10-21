# T-24927 notes: BUNKA_SD:func_80139290 (src/ovl/BUNKA_SD/801388D0.c)

Result: **not matched**; the guard stays at **6 rows** (the pre-existing T-24709
draft, untouched). The target rows were fully understood and one lever was
measured that removes all of them, but it costs a ~30-row uopt register cluster
(kept as a patch, not applied). Ticket set to In Review.

## Classification (step 1, measured with the T-24900 as1 trace)

**Same-block order, as1's per-block list scheduler. NOT the global delay-slot
filler** (the T-24211/T-24212 default-arm family the ticket guessed): loads are
not movable by the global mover, the `lbu t0` is inside the arm block as as1
receives it, and the arm's final order equals the per-block pick order exactly
(`drafts/T-24927/trace-arm-0x3B-builtin.txt`).

The remaining 6 rows are the position of the speculative `lbu t0,
%lo(D_800E62BE)($t0)` (the load of `g->unk_03E` = `D_800E6280.unk_03E`, a
**main global** declared in `include/main_api.h`) inside three arms of the
`switch (g->unk_1104.u - 0x70000000)` compare chain (cases 0x3B, 0x49, 0x66;
funcdiff rows 159-161, 166-167, 171-172). The value is live from every switch
arm to the two tests at 0x80139788 (`beq t2,t0`) and 0x80139804
(`bne t2,t0`), which read `$t0`/`$t2` with no loads of their own. The loads in
the 0x49/0x66 arms are dead on their own paths (their `$t0` is never read
there) - the signature of uopt's per-path duplicated join preamble, not of a
source read.

Per-block scheduler rule, measured on the built unit (all three arms):

- Priority = `aftercycles` DESC (critical path first): `lui t0` (aftercycles 3,
  feeds the load's 2-cycle hazard) is always picked first - it matches already.
- Ties broken by **INST number ASC = ugen emission order = source line order**.
  The arm's store prep (`li t7,2` + `lui at` from `D_801206AC = 2;`) and the
  duplicated preamble (`li t2,0x60` + `lui t0` + `lbu t0`) all have
  aftercycles 2, so the store prep wins iff its source line precedes the
  preamble's line.

The preamble's line number is the arm's **exit statement** (the `break;` line;
the default arm's copies carry the `switch` line itself). With the arm's store
in source (as the matching asm demands: the store is entered straight from the
compare chain, no second test exists anywhere in the original), the store's
line is always before the break's line, so this C structure can never win the
tie. The original must have had the preamble emitted before the arm's store.

## The lever that works: read `unk_03E` in source inside each arm

`drafts/T-24927/local-m-order-lever.patch` (measured, then reverted): declare
`u8 m;` in the guarded body, read `m = g->unk_03E;` as the first statement of
each of the five paths (cases 0x3B/0x49/0x66/0xA1 + default), and make the two
post-switch tests read `m` (`... == 0x3B && m != 0x60`,
`... == 0x68 && m == 0x60`).

Measured effect: **all six target rows disappear** (the arms emit
`lui/lbu` first, exactly the original), the whole rest of the arm/join
structure stays identical. But uopt then colours the cluster differently and
the diff becomes ~30 register rows:

- `m` (a real local range, no ugen ring precolour) takes `a3`; the original
  value keeps the extended ring temp `$t0`.
- The `0x3B` case constant becomes an **allocated** temp (`li a1,0x3B`,
  `beq v1,a1`) instead of keeping the ring `$t1` (`li t1,0x3B`, `beq v1,t1`).
- Cascade: the `v1` copy `move a1,v1` -> `move a2,v1`; `D_80122EAC`'s value
  `lw a2` -> `lw t0` (all `andi/beqz` users shift with it).
- The `m != 0x60` compares come out with the operand order flipped
  (`beq a3,t2` instead of `beq t2,t0`).

So the brief's two-stage rule applies: the lever fixes its target and breaks a
second thing. Solving the second thing is uopt allocator work, out of the
45-minute box, so the better guard (6 rows, field reads, which colours
everything right) stays in the tree and the lever is kept as the patch.

## What does not work (do not retry)

- **`volatile` on `D_800E62BE`** (ticket step 3): wrong mechanism. `volatile`
  marks candidates for the **global** mover; here the wrong pick is the
  **per-block** scheduler's tie-break, which volatile does not touch. Worse,
  volatile would stop uopt from merging the three reads (0xA1 arm + the two
  post-switch tests) into the per-path preamble, so the arms would lose the
  `lui/lbu` entirely. Not applied, no need to re-run the cross-unit check.
- Empty arms + post-switch `if (v == 0x3B) D_801206AC = 2;` etc. (reasoned,
  not built): uopt has no evidence of merging a post-switch if into a switch
  arm (the two `u == 0x70000000` tests at 0x94B0/0x7710 are *not* merged), and
  a non-merged if adds compare blocks - the arms' stores have no second entry
  in the original.
- T-24709's grid (692 autotweak edits, `&&` swap, one-line arms, shared
  selector local) - already measured no-gain, per the ticket.

## Next hypotheses (in order)

1. **Make the local keep a ring precolour** so `m` colours `$t0` and the
   `0x3B` constant stays the ring `$t1`: per `wiki/uopt-register-allocation.md`
   the precolour-disagreement penalty is `m * F / 10`, and ranges are handled in
   bit-position order; the local form creates a fresh early range that beats
   the ring temps to `a1/a2/a3`. Try: the arms' reads through a spelling that
   keeps the value an expression temp of the post-switch tests *and* emitted
   early (unknown shape - this is the core open question), or a dummy local
   range that consumes `a3` and re-checks whether the constant goes back to
   `$t1` (the constant's `a1` allocation is the root cause of the cascade).
2. If (1) lands, the compare operand order (`beq a3,t2` vs `beq t2,t0`) needs
   the cheatsheet §4 recipes (local on the right / `0x60` on the left).
3. Check `tools/regdiff.py` FIRST DIVERGENCE on the patch-applied build to
   confirm the a1/a2/a3/t0 story before spending time on (1).

## Files

- `drafts/T-24927/NOTES.md` (this file), `README-trace.txt`,
  `trace-arm-0x3B-builtin.txt` (the arm's node list + picks),
  `local-m-order-lever.patch` (the measured lever).
- `src/ovl/BUNKA_SD/801388D0.c`: **unchanged** from the T-24709 state (6-row
  guard). `include/ovl/BUNKA_SD.h`, `include/main_api.h`: untouched.
- Trace setup under `build/as1tr/` (gitignored): cap24927/xtr.log is the raw
  as1 trace of the whole guarded unit.
