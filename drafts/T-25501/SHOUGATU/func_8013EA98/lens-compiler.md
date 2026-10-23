# T-25501: SHOUGATU func_8013EA98, compiler-internals lens (wave 12)

Start 2026-10-23 16:20:51 UTC (worktree build until 16:40). Worktree
`/Users/lila/Documents/Projects/tokimemo-wt/w12-shougatu-compiler`, branch `w12-shougatu-compiler`.
No match. Production source unchanged (`INCLUDE_ASM`).

## Result in one paragraph

The first divergence is in uopt, in `globalcolor`: the live range of the selector
`D_800E6280.unk_F5F`. cfe, ugen and as1 then produce the target's code exactly: `diag/y14.c`
(FAKE-laden) compiles to the target's 230 instructions with the same opcode at every position
(normalised opcode sequence identical, checked with objdump). Only 7 uopt ranges sit in other
registers. Reading uopt's source shows that the target's selector allocation can only come from a
**cost split of the selector range whose piece starts at the `== 5` block (L_EBE8)**. With the
natural C, the range's total saving is about -9 (one loop-frequency reload too many), so uopt never
considers it for colouring and never splits it. Two FAKE kinds make the arithmetic work:
`u8 >= 0` reads, which uopt keeps and ugen folds away, and a hole in the `>= 9` block. They also
add flow-graph blocks, and those shift the colouring order of the other loop ranges.

## Tools and setup

- `build/ea98/` holds the T-25143/T-25305 scripts (`m.py`, `tr.py`, `pri.sh`, `show.sh`) and the
  `uopt-trace` binary (`/private/tmp/tokimemo-t25044/build/T-25044/uopt-trace`, copied to
  `build/ea98/tr/`).
- `scripts/gv.py OUT OPTS`: writes a variant of the T-25046 body (`diag/base-T25046.c`). Options:
  `latch=EXPR` (condition becomes `(chain) && EXPR`), `hole5`, `alias`, `plain`, `flag`, `postnat`.
- `scripts/run.sh LABEL`: whole-TU score (`m.py`), uopt trace, selector live ranges (`pri.sh`
  format: unit block, `L` loads, `S` stores, `r` reload, `s` save).
- `scripts/search.py`: 24 runs over the hole FAKE, the B-side tautology position and its spelling.
- uopt reference: n64decomp/ido d068e43 `uoptmain.c`, `uoptcontrolflow.c`, `uoptreg1.c`,
  `uoptreg2.c`, `uoptkill.c`, `uoptloop.c` (read in scratch, not copied).

Block names below: header = L_EB78; bb5 = the `>= 9` block; bb6 = the empty node between bb5
and L_EBE8; bb7 = L_EBE8 (`== 5`); bb10 = `== 6`; bb13 = the `D_80146054` compare; bb14/bb15 =
the final `b1` / `count` tests; latch = L_EC44.

## Rules read from uopt (they decide this function)

1. `one_block` (uoptmain.c): a `lab` that follows a `fjp`/`tjp` gets an empty fall-through node first
   (only a `Unop`). That node is bb6 in every do-while chain. `controlflow` does no jump
   threading. The empty latch `lab; ujp` survives uopt: `c3` (continue form) has latch node 19 with
   five predecessors, and its uopt output still has `lab L37; ujp L30`. ugen threads it later, when
   it is empty. This corrects T-25305 ("uopt threads an empty latch before register allocation").
2. `makelivranges` (uoptreg1.c): in a block where the variable is in `indiracc` (a hole), its loads
   and stores make no live unit and set neither `loclive` nor `locdef`. So a hole is transparent for
   the liveness that builds the ranges. `regdataflow` adds `indiracc` to `loclive` only after
   `active` is computed, for `liveout`. `localcolor` keeps holes out of `livebbs`. An `ilod`
   through a register base is a hole for every global (`aliaswithptr` is always true for C). An
   `ilod` through an `lda` base is a hole if it overlaps the variable (`indirectaccessed`, uoptkill.c).
3. `updatelivran`: any unit or reaching block that has a predecessor outside the range needs a
   reload (`needreglod`). `compute_save` charges `F` for it, unless the block is a loop-first block
   and `canmoverlod`.
4. `globalcolor`: a reload moves to the predecessors only if the block has two or more of them, one
   of them holds the value, and no non-member predecessor ends in `fjp`/`tjp` (`docreatebb` is off).
5. `split`: the piece starts at the first unit whose first access is a store (and whose block has a
   free register). Otherwise it starts at the first unit with references. List order is node order,
   and `depth_first_order` visits the fall-through successor first. The piece grows along
   successors only. In a cost split (`arg3`), call blocks are added but not expanded.
6. A cost split happens when `adjsave * unk1C` (the total saving) is at most the cheapest register
   cost. The other split, `needsplit`, needs `regsleft == 0`. That cannot happen here: the target
   uses no callee-saved register, so s0..s8 are always free.

## What the target needs, derived from those rules

- The header is a hole (natural: `unk_1BC[F5F]`). bb5 is a hole too: its operand is ugen
  store forwarding (`andi t9,t8,0xff`). A member bb5 would reload into the range register, and ugen
  would forward that reload (`move t4,t9` in the T-25046 body).
- There is a latch block that the five rejections reach. bb5 jumps to it with `tjp`, so its reload
  stays at the block start (rule 4). In the continue form (`c3`), the non-member predecessor ends in
  `ujp`, so the reload moves into the continue blocks.
- bb6 must stay out of the coloured piece. bb6 is active, not a hole, and its predecessor bb5 is
  outside the range, so a range that holds bb6 gives it a reload. ugen forwards that reload into a
  stray `andi tX,t8,0xff` in the delay slot of the `>= 9` branch (`y1`, `d4`, `d6`; the target has
  `nop`). By rule 5 the only way to leave bb6 out is a split whose piece starts at bb7.
- The split must be the first one. With the natural forced store, the first piece starts there
  (`unk_F5F = D_80145FE4` is a first-store unit) and takes {forced store, L_EC7C}. The remainder
  then has no call cost, because its post-call units reload, so it is coloured whole with bb6
  (`d6`: register ra) or not at all. So the forced store must not be a store of this variable.
  `D_800E71DF`, as in s2, has a keep precedent in `config/migrate_globals.txt` (DATE, T-20820).
- The piece must not grow out of the loop. The natural final test re-reads `unk_1BC[F5F].b1`, so
  bb14 is a hole and blocks the count path. The latch must be unconditional at allocation time: a
  test in the latch adds an exit edge, and the piece then leaks into L_EC7C and takes s0 (`y5`).
- Arithmetic: `0 < W_total <= cheapest register cost` (about 11.6 here, s0's first-use cost; every
  caller-saved register is taken or costly). Natural units: bb6 r -10, bb7 L+r 0, bb10 +10, bb13 +10,
  latch r -10, bb15 r -10, exit r -1, L_EC7C L2 +2, post-call L1+r 0 each, so W_total is about -9.
  One extra loop-frequency reference on each side gives `y12`: W_total = 11 (priority 1.83,
  D = 6), piece B = {bb7, the tautology block, bb10, bb13, latch} with priority 6.67, remainder C
  negative.

## New construct: reads that uopt keeps and ugen folds

Under `-cckr` a `u8` compared `>= 0` is an unsigned compare. cfe and uopt keep it, so it counts as a
reference of the variable during allocation. ugen folds it to an unconditional branch with no
code. `y1` (latch `&& D_800E6280.unk_F5F >= 0`) gives the target's latch exactly
(`lui a3; lbu a3,F5F; b header`) and the original final-test layout. T-25305 asked for exactly
this kind of construct. Inside the chain it adds a reference worth +F at no code cost. As the latch
condition, it adds an exit edge (see above).

Matched precedents of the same FAKE: SHUGAKU `func_801398E8` (`if (D_800E6280.unk_F5F >= 0)`,
"promote unk_F5F to $a1 while folding the unsigned >= 0 guard", T-24114) and main
`func_80077CA8` (T-24108). So the construct is accepted in review as a marked FAKE.

## Measurements (whole TU, `m.py`: printed rows / cost; words 230 unless noted)

| body | change | rows/cost | selector ranges (pri.sh) |
|---|---|---|---|
| base-T25046 | best known | 84/36 | W 2.2, split from forced store; loop piece incl. bb5 colored t4 |
| c3 | continue form | 191 (227 w) | not colored (-1.5); latch node 19 exists in uopt |
| v3 | latch `>= 0` + hole5 | 214 (232 w) | W 0.17, split from forced store, remainder -0.2 |
| y1 | v3 + flag + postnat | 328 (234 w) | W 2.0 colored whole (a3): target latch, stray bb6 `andi`, s0 elsewhere |
| y2 | v3 + postnat | 214 (232 w) | remainder -0.17 |
| y5 | y2 + alias | 112 (232 w) | split from bb7 ok (4.2) but leaks via latch exit edge into L_EC7C (s0) |
| y7 | latch `(status,1)` + hole5 + flag + postnat | 225 | remainder exactly 0 |
| d4 | y2 + `>= 0` in the `== 5` conjunct | 154 | remainder 1.5 colored whole (t4), bb6 `andi` |
| d6 | y2 + two freq-1 `>= 0` | 184 | remainder colored whole (ra) |
| y11 | `(status,1)` + hole5 + `>= 0` in `== 5` + alias + postnat | 198 | W 0.17 -> B {7,8,11,14,18r} 6.67, C -4.75: loop structure exact |
| y12 | y11 + `>= 0` in final conjunct | 176 | W 1.83 -> B 6.67 |
| y14 | y12, hole via `unk_1BC[status * 0].unk_06 * 0` | 170 | same; **opcode sequence identical to the target** |
| search | 24 combinations | best 170 | spelling and B-side position change nothing |

The y14 rows are registers only. Target to y14: `&D_800E6280` t1 to a3, `D_80146054` t2 to t3,
6 t3 to t5, 9 t0 to t4, 8 a3 to t1, 0x38 t5 to t0, selector t4 to t2. status v0, b1 v1, count a0,
10 a1 and 5 a2 are right. The target's colouring order is
10 > 5 > 8 > 9 > base > `D_80146054` > 6 > W (split, B gets the next register) > 0x38. In
y12/y14, base and 0x38 rise (the hole FAKE adds uses in bb5), 9 and `D_80146054` tie at 1.83, and
`D_80146054` wins the tie because variables have lower bit numbers than constants. The T-25143
t0/t2 swap is the same tie.

### Fingerprint of the original's flow graph (from the target's register order)

Priority = total / D, with D = floor((N - 2) / 4) + 2, where N = units + reaching blocks. Ties go
to the lower bit, and variables have lower bits than constants. The target's registers fix the
colouring order, and that order gives these inequalities. Totals are measured; bits are from y12.

- 10, 5, 8 (total 10 each, bits 148/150/151) >= 9 (total 11, bit 153);
- 9 > base `&D_800E6280` (total 22, bit 146), strictly;
- base > `D_80146054` (total 11, bit 60), strictly;
- `D_80146054` >= 6 (total 11, bit 152);
- 6 > selector range W (bit 24), strictly; W >= 0x38 (total 12, bit 149).

In the base-36 flow graph the loop has 13 blocks (D = 4 for 10/5/8, so 2.5 > 2.2). Every latch graph
built here has 14 or more (four empty edge nodes, the latch and nine real blocks), so 10/5/8 drop to
2.0 and 9 rises above them (y11) unless 9's range crosses D = 6 (y12). y12 still breaks three
inequalities: base 22/11 = 2.0 is not below 9 (1.83), `D_80146054` ties 9 at 1.83, and W 1.83 is
not below 6 (1.57). A candidate flow graph for the original must satisfy all six.

The cleanest construction adds no block: `diag/y30.c`/`y31.c` (latch `(status, 1)`, bb5 hole FAKE,
alias forced store, and `count >= 3 + *(u8 *)&F5F * 0` in bb15, which turns bb15's reload into a
use). It gives exactly the target piece B = {bb7 L r, bb10 L, latch r, bb13 L} (3.33),
W = 0.17 and C = -2.25, in 230 words with 2 opcode differences. B is coloured last and gets ra.
The latch can also be `while ((chain) && 1)` (`diag/y32.c`, identical result). cfe keeps the
`&& 1` as a latch block, and uopt's `constarith` turns its `tjp` into an unconditional jump, so no
exit edge is left. In this flow graph 9 (2.2, D = 5) still outranks 10/5/8 (2.0, D = 5). So the original's loop must
have had more blocks than the do-while chain with a latch (at least one more D step for 9's range
than for 10's). Solving the inequalities above with these totals gives D(10/5/8) = 5,
D(9) = 6, D(`D_80146054`) = D(6) = 7, D(base) = 13 (we get 11), and W in [1.2, 1.57). That
points to a larger flow graph in the loop and in the tail, which is a constraint for any
natural-source search.

The D values can be tuned one at a time. A `u8 >= 0` read on `D_80145FE4` in front of the
forced-choice test adds blocks to the ranges of 9, `D_80146054`, base and W only, not to those of
10/5/8 (`1 &&` does not work, cfe folds it). Two such blocks (`diag/r2.c` on y31, `diag/u2.c` on
y30) move 9 to D = 6 (1.83), below 10/5/8, as the target needs. They also give 8 a3 to t0..t1 and
`D_80146054` t2 (r2) or t1 (u2). The inequalities still open:
- status above b1: every bb5 hole FAKE adds a use to b1 (b1 hole: 10.0 against status 6.2) or to
  base and 0x38 (status * 0 hole);
- base `22 / D` must fall strictly between 9 and `D_80146054`: D(base) = 13, and we have 11;
- `D_80146054` must cross to D = 7 (blocks between the `== 9` test and the `D_80146054 !=` test);
- W in [1.2, 1.57): W_total is 1 in y30/y31. It needs about +9 to +10 without a loop block, for
  example a second selector reference in a B block.
Each of these is one more FAKE. I stopped there: the resulting body would carry 6 or more FAKEs plus
the alias keep line, and the synthesis should decide whether that route is acceptable at all.

## Negatives (new)

- Freq-1 `>= 0` reads on the plain member next to another read of the same value are CSE'd into the
  `cvt` temporary (`d1`, `d5`). Byte-view spelling counts (`d6`).
- Hole FAKEs: `9 + unk_1BC[count * 0].unk_06 * 0` 286; `9 + unk_1BC[F5F].unk_06 * 0` 320 (address
  CSE temporary); `((u8 *)&F5F)[count * 0]` 231 words (load through the base register, not
  forwarded); `*(u8 *)(0x80000000 + status * 0) * 0` 234 words (hoisted `lui`).
- A two-step split (forced-store piece first, then the remainder split from bb7) is impossible: the
  remainder never has a call cost (rule 6 and the remainder's reloads after calls).
- `b1` from a local (`flag`) instead of the natural re-read in the final test (`y20`, `y21`): it
  saves the bb15 and exit reloads, but bb14 is then no hole and the piece leaks through the count
  path into L_EC7C (`y21`: piece 2.4 with L_EC7C, s0, frame 0x68). With L_EC7C also read through
  the alias, W is 0 (`y20`). Post-call units always reload, because the call kills the global
  (`ppin` in `regdataflow`). So the original's final test was a hole, as in the natural re-read.
  bb15's reload (-10) is then unavoidable, and the missing +10 has to come from the loop part
  without adding a block (each `>= 0` adds one and shifts D).
- Second sweep (`scripts/search2.py`): post-call spellings (`D_800E71DF` against the member) change
  nothing; `func_80051A68(D_80146058)` drops W to 0 (233 words, 105 rows).

## Mechanism (summary for the synthesis)

uopt colours the selector as one range or as split pieces. To match, the coloured piece must be
{L_EBE8 .. L_EC30, latch}, with reloads at L_EBE8 and the latch, and must leave out the empty node
bb6, which uopt's `one_block` creates for every label after a conditional jump. Only a cost split
whose piece starts at L_EBE8 does this. That needs no store of the variable outside holes, a hole
in the `>= 9` block, an unconditional latch, a hole in the final `b1` test, and a total saving
between 0 and the cheapest register cost. Natural C lands about one reload (-10 at loop frequency)
below zero.

## Proposal

1. Source route (FAKEs, needs review): continue from `diag/y14.c`/`y12.c`. What is left is the
   colouring order of 7 loop ranges. The levers are the per-range block counts D, which the FAKE
   blocks change, and the 9 / `D_80146054` tie. Look for a bb5 hole FAKE that adds no uses to b1,
   base or 0x38 in bb5. Then tune W into the window (1.2, 1.57): above 0x38, below 6. A match this
   way would need FAKE comments (T-25501) for the hole, the two `>= 0` reads and the
   `(status, 1)` latch, plus a keep line with a necessity proof for `D_800E71DF` (precedent:
   DATE T-20820).
2. Natural-source route: a matching natural C must have one more selector reference at loop
   frequency than any candidate so far, with no code and no extra flow edge out of the loop, or
   one fewer reload. Everything else is pinned by the rules above. Candidates not tried yet: other
   tautologies that ugen folds (`< 256`, `!= 256`, `(F5F & 0xFF) == F5F`, compares in `u16`)
   placed inside the chain, and a natural hole in the `>= 9` block.
3. Toolchain, cvt_pass entry rule (supporting evidence, no change yet). The byte views
   `*(u8 *)&D_800E6280.unk_F5F` in the loop compares exist only to get around the T-5010 entry
   rule. With the plain member, cvt_pass keeps the compare `CVT`, uopt CSEs the widened value, and
   the selector range changes. Production-pass controls: `p14` 209 and `pbase` 203, both 227 words.
   I added the T-20610 `stored_k` refinement to a scratch copy of the pass, in this worktree only
   (a late global that the procedure stores and compares with a constant loses its compare
   `CVT`s), and then restored the pass. Under it, the plain-member bodies compile like the
   byte-view ones: `k14` gives the same 170 rows and 85 distinct as `y14`, `kbase` the same 42
   distinct as the T-25046 body. The only differences are two `bne`/`beq` operand orders, which
   the real pass's operand-order step would handle. T-20610 measured `stored_k` as neutral, changing
   0 matched functions. This function is a candidate gain for it once the selector range matches,
   because it removes the views from the C.
4. Toolchain, uopt: no change proposed. Nothing in this pass shows that the original's uopt differs. The
   rules involved are core (CFG edge nodes, hole transparency, split start and growth), IDO 5.2
   gave identical code on 2564 functions, and T-9800 showed uniform allocator patches regress
   widely. A testable idea, if someone rebuilds the patched recomp: in `compute_save`, charge no
   reload for reaching blocks that have no references. Even then the whole-range case keeps
   bb6's reload, so a split is still required. Expect it to fail the preservation check.

## Files

- `diag/base-T25046.c` (best rows, 84/36), `diag/y14.c` (opcode-exact, 170), `diag/y12.c`,
  `diag/y11.c`, `diag/y1.c` (target latch via `>= 0`), `diag/y5.c` (leak), `diag/d4.c`,
  `diag/d6.c`, `diag/y2.c`, `diag/y7.c`, `diag/v3.c`, `diag/c3.c`.
- `scripts/gv.py`, `scripts/run.sh`, `scripts/search.py` (run from the worktree root; bodies and
  traces under `build/ea98/`).
