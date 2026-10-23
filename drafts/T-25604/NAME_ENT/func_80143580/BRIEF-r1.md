# T-25604 fresh-start round 1 brief: NAME_ENT func_80143580

Digest of everything known about NAME_ENT `func_80143580` (introduction scene, 1077 instructions
plus a 20-entry jump table, `src/ovl/NAME_ENT/80142A80.c`), written 2026-10-23 (21:14 to 21:50 UTC)
in worktree `f-ne-r1-digest` for the three lens agents of round 1. Words only; no asm here.

Measure (whole unit, NON_MATCHING forced; a MATCH from the INCLUDE_ASM default build means nothing):

    tools/docker.sh python3 tools/funcdiff.py func_80143580 --unit NAME_ENT --resolve --version c

Scratch helpers committed with this brief (`fresh-r1/`, no game data in them; build output goes to
the ignored `build/T-25604/fresh-r1/`):

- `replay.py BODY.c TAG` compiles a body (the format of `fresh-r1-best.c`) in the whole unit and
  prints words, funcdiff rows and the Levenshtein score. `--rev1` (or `--rev1=PARTS`, PARTS a
  subset of `034`) applies the rev1 source delta instead and only writes the object.
- `rev1.py` holds that delta (see "Older releases").
- `rev1cmp.py X.dis COPY.s [x]` aligns a compiled function (objdump text) with an asm copy (rev1
  or the target) on opcodes and counts the register substitutions.
- `colours.py X.dis` prints the registers of the key ranges (selector, fades, 0x80000000,
  0x01000000).
- Disassemble in Docker: `mips-linux-gnu-objdump -d --disassemble=func_80143580 OBJ`.
- The traced uopt for `tools/regdiff.py` is not on main. A copy sits in this worktree at
  `build/scratch/r9-uopt/variants/trace` (copied from the r1-compiler worktree). Copy it into your
  worktree's `build/scratch/...` and run regdiff with `TOKIMEMO_SCRATCH=<worktree>/build/scratch`.
  The interference lister `interf.py` is on branch `g-NAME_ENT-80142A80-80143580-r1-compiler`
  (`drafts/T-25604/NAME_ENT/func_80143580/r1-compiler/interf.py`; run it on regdiff's `draft.log`
  with a range bit). It lists only ranges with a reference unit in a shared block, not ranges
  that only pass through a block.

## Best body and rows

`fresh-r1-best.c`, also the NON_MATCHING guard in this worktree's `src/ovl/NAME_ENT/80142A80.c`
(not merged): **122 funcdiff rows, Levenshtein score 120, 1077 of 1077 words**, frame 0x90,
tiles at sp+0x6C, counter spill at sp+0x64. Needs `s32 func_80143498(void)` outside the guard
(T-25319 view; func_80143498 still matches). The default build passes (NAME_ENT.bin SHA1 OK,
globals OK).

It is a cross of the round-1 lens branches, which all stopped with a "WIP snapshot" commit:

1. The clean-room branch body (`g-...-r1-cleanroom`, its src guard; 152 rows / score 146 here).
   Its changes against T-25319: the compare status of the four `x--`/`x++` position tests
   (cases 1, 3, 5, 8) held in `index` (`index = x-- < K; if (index)`, now marked FAKE, 32 rows);
   the nested fills of cases 6/13/16 as index loops on `address` with `tiles[address] = BASE +
   address * 0x4000`; case 9 chain order `c11 = c12 = c10 = 0x80`; cases 11/16 actor set-up as
   `i = 0x60; func_80048F64(i); ((DisplayActor44 *)D_8011ECD0)[i].field = ...` (this also gives
   case 11's `slti` naturally: the counter is assigned again after the loop); the case 13 copy loop
   on `i`.
2. From the structure branch: `void col2sepia();`. This is a real declaration, not a view:
   col2sepia is a K&R `void` function (`src/main/8004AD60.c`), and NAME_ENT has no declaration,
   so it was an implicit int. It fixes the case 13 copy loop (24 rows). It should end up in a
   NAME_ENT header, and the whole unit must be re-checked when it moves.
3. From the structure branch: in case 17's else arm, `unk_110D++` before `unk_1104.u = 0`
   (2 rows).

### Every candidate measured (this digest, same scorer)

| body | words | rows | score |
| --- | --- | --- | --- |
| T-25319 draft (main, and the permuter base) | 1076 | 293 | 293 |
| structure branch src guard | 1077 | 262 | 258 |
| structure lens `best.c` | 1077 | 226 | 222 |
| compiler lens `body.c` (= T-25319) | 1076 | 293 | 293 |
| compiler vP3 (fills as index loops on `i`, dead `i = 0` after case 11) | 1077 | 248 | 242 |
| compiler vU5, vU6, vW1-3, vX1-2 | 1077 | 248 | 242 |
| compiler vQ3 (vP3 + `if (i) {}` at case 3 head, FAKE probe) | 1077 | 196 | 190 |
| compiler vR3 / vR1, vR2 / vU4 | 1078 / 1078 / 1089 | 213 / 333 / 304 | 211 / 333 / 304 |
| compiler vT1-3 | 1071-1074 | 637-691 | 631-681 |
| permuter best (w14-perm `output-4768-1`) | 1077 | 260 | 252 |
| clean-room branch src guard | 1077 | 152 | 146 |
| clean-room + compiler fill form for every fill (on `i`) | 1077 | 244 | 236 |
| clean-room + compiler form for the entry fill only | 1077 | 276 | 268 |
| clean-room + compiler form for the nested fills only (entry unrolls) | 1085 | 782 | 740 |
| clean-room + case 17 order | 1077 | 148 | 144 |
| **clean-room + case 17 order + `void col2sepia();` (best)** | 1077 | **122** | **120** |
| best without the F5F self-store | 1077 | 214 | 208 |
| best without `^ 0` on the case 5 argument | 1076 | 199 | 199 |
| best without the compare statuses in `index` | 1077 | 184 | 182 |
| best without `if (address) {}` | 1077 | 184 | 180 |
| best + `func_8004A1A0` implicit int (`MAIN_API_OVERRIDE`) | 1077 | 122 | 120 (0x80000000 v0 -> v1) |
| best, dead `index = unk_F5F;` instead of the self-store | 1077 | 122 | 120 |
| best, `register s32 index;` | 1077 | 122 | 120 |
| best, both (copy propagation off, the ETC lever) | 1071 | 684 | 684 |

The implicit-int view keeps the row count but is not neutral. The 0x80000000 constant moves from
v0 to v1 in all six cases, one step towards the original's t1: the call result now blocks v0
in the constant's blocks, the ETC func_8013FFAC mechanism. func_80142E24, the other caller in the
file, is unchanged by it. It is the first lever that moves that constant at all; keep it in mind
for lens 2.

## First divergence

Word 3, the entry fill: the value and end constant are in a1/v1, the original has v1/a0. The
cause is the T-25319 FAKE `if (address) {}` in case 6. It keeps `address` live from the entry
fill into case 6 (priority 18, colour a1), which is what moves the selector to t0. In the rows
left (122) everything follows from that, plus one independent gap:

- **a1 taken by `address`** (40 register rows): the spr0 fade value (promoted `unk_10`) is a2
  for a1 in cases 0/2/4/6, case 9's promoted `unk_10` is a2 for a1, and the outer fill end
  constant of cases 6/13/16 is a2 for a1. The entry fill takes 7 more.
- **0x80000000 in v0 for t1** (13 rows, cases 2/4/6/9/13/16). In the trace the main range has
  priority 1, 9 uses and units in the six case blocks (a second, 2-use range has priority 20). It shares blocks with ranges in a0, a1
  (`address`), a2, a3, ra and t2, and with the selector (t0, coloured after it); nothing holds v0
  or v1 there, so it takes v0.
- **Case 9 store order** (2 structural hunks): the `D_801217D0[2]` 0x80000000 store comes between
  the 0x80 stores, where the original has it last. One `lui` order at the case 4 tail goes with
  the constant's register.

Without the FAKE (184 rows) the fade value, case 9 and the end constants are right (a1), and the
selector is a2 for t0 (58 rows). The knot: the selector (promoted `unk_110D`, priority 0.5,
coloured last) needs v0..a3 all taken in its blocks. In the trace its block-sharing neighbours
are v0, v1, a0, a2 (spr0 `unk_10`, only because `address` holds a1), a3 (the cfe chain VREG
`M108-46` of case 9, a range with no instruction of its own), ra and t2. In the original, a2 and
a3 are used outside call set-ups only inside the case 6 record loop, where the selector is not
live (it is reloaded after the loop). So whatever blocks a2 for the selector in the original is a
range that emits no instruction of its own (a chain VREG, a dead-def range or a free reference)
or a range that only passes through the selector's blocks.

## Solved similar cases

- **ETC func_8013FFAC (T-25314, matched)**, the closest scene: SpriteSlot24 fades as chains,
  func_8004A1A0 tile tables, 0x01000000/0x80 constants in late-coloured ranges, unk_F5F and the
  unk_110D selector. Levers: (1) uopt interference is per block, and an empty
  `goto split; split: ;` after a statement takes a range out of a constant's block; (2)
  `func_8004A1A0` implicit int, so its v0 result blocks v0 after the call. (1) and (2) together
  moved ETC's 0x01000000 off v0. Here (2) alone moves 0x80000000 from v0 to v1 at the same row
  count (see the table); (3) `register s32 i;` plus a dead entry
  read `i = unk_F5F;` turns copy propagation off, and unk_F5F's range then starts at the entry
  in t0 (684 rows here: negative). Untested here: the empty-label splits, aimed at the
  0x80000000 and selector blocks.
- **SHOUGATU func_8013EA98 (T-25501, matched)**: the selector there is a cost-split piece. A
  precoloured unit (the selector passed as a call argument through a byte view) forces the split.
  Free references (`x + G * 0` on the non-constant side of a compare or a stored value) add uopt
  references and no block, and ugen folds them away. The block count of a region decides D in
  the priority, so it decides the colouring order. Similar: our selector and 0x80000000 colours
  are order and priority problems.
- **TACO func_8014AB50 (T-25043, playbook)**: a call result captured in a never-true arm raised a
  constant's priority from 0.5 to 1 and moved it from v0 to v1. This is the same lever class as
  the 0x80000000 constant, run the other way.
- **main func_80067870 (T-25406, matched)**: one more FAKE empty test splits a block so that two
  ranges stop sharing it. A second per-block-interference precedent.
- **GYOZI func_801368B8 (T-25129)**: a dead entry read of u8 globals makes tools/cvt_pass.py drop
  their CVTs. This is the same mechanism as our F5F self-store (the entry rule).
- **TACO func_8014C8DC and the compiler lens's toys**: a counted loop keeps `slti` only when the
  counter is assigned again on the exit path. Our case 11 gets it from `i = 0x60` after the loop.
- **TAIIKU func_80137784 (T-25502)**: uopt's linear-function test replacement turns index tests
  into pointer compares. The compiler lens showed the same for the fills: index loops give the
  original's address compares, two end-constant ranges and a re-materialised start.
- **NAME_ENT func_80142E24** (matched, same file): same tile-table and `step = unk_110D` style.
  Its selector is a ugen temporary (`unk_110D = unk_110D + 1; return;`), not a promoted range, so
  it is not the same idiom.
- An asm scan of every matched function finds no other promoted unk_110D selector held in t0
  across reloads (only DATE func_8013AF1C has t0 at a switch head, 150 words, no reloads);
  matched promoted selectors are v0/v1. So t0 here comes from interference particular to this
  function, not from a known source idiom.

## Older releases

- **Rev 2 and Rev 4**: identical to the target. Same 1077 instructions, every word equal after
  masking relocations.
- **Rev 1**: 1111 instructions, same compiler. The source differs in three places only:
  - case 0 has no `D_8014D170 = 0;`
  - case 3 runs the body of func_80143498 inline, inside the `if`, after the selector advance:
    func_80046290, func_80044750, func_8004E58C, three set_kanji_string calls, k_disp_start(3)
    and three k_disp_switch calls, without the button condition and the `D_8014D170 = 1`
  - case 4 has no call at its head
  Everything else is the same instruction sequence with other registers: selector t2, spr0 fade
  value t1, spr0 high part t0, spr1 fade t3, 0x80000000 t4 (Best: t0, a1, a0, t2, t1). Rev 1's
  case 9 store order is the order our draft gives, not Best's.
- **Measured, the best body with the rev1 delta** (`replay.py ... --rev1`, `rev1cmp.py`): 1111
  words, **0 structural hunks** (case 9 order right), 70 register substitutions: fade t0 for t1
  (28), 0x80000000 v0 for t4 (15), outer fill end a2 for a1 (12), high part v1 for t0 (8), entry
  fill (7). The selector is already rev1's t2. **The `if (address) {}` FAKE makes no difference
  under the rev1 delta** (identical object).
- **Each FAKE on both versions** (Best rows / rev1 structural hunks + register substitutions;
  best = 122 / 0 + 70): no F5F self-store 214 / 3 + 114; no `^ 0` 199 / 3 + 100; no compare
  statuses 184 / 2 + 104; no `if (address) {}` 184 / 0 + 70. The first three help both versions.
  `if (address) {}` is the only one that helps Best alone.
- **Partial deltas** (`--rev1=PARTS`, key colours with `colours.py`): the case 0 store and the
  case 4 call move no key colour (selector a2, fade a1 without the FAKE, as in Best). The case 3
  inline block alone gives selector t3 and fade t1, with or without the FAKE. Case 3 plus case 4
  gives selector t2, fade t0, spr1 fade t3.

What this suggests:

1. The case 9 order is not a case 9 source problem. Unchanged case 9 source gives Rev 1's order
   in our build, and Best's order differs while Best and Rev 1 differ only in cases 0/3/4. The
   order follows from something outside case 9 (register choice and as1 scheduling). Stop
   permuting case 9 statements.
2. In both releases 0x80000000 sits just above the high part, fade, selector and spr1 fade
   ranges (Rev 1: t4 above t0..t3; Best: t1 above a0, a1 and t0, with v0..a3 blocked). That fits
   a constant coloured after the selector (priority below 0.5, so a longer range or fewer net
   uses) whose blocks also hold v0/v1 ranges (fill loops, D4). In our draft it has priority 1, is
   coloured before the selector and finds v0 free.
3. In Rev 1 the high part (t0) and the fade value (t1) sit above every argument register. Their
   original ranges reach case 3's call sequence; ours do not (high part v1 under the delta). So
   the original's spr0 base/fade ranges are longer than ours.
4. The FAKE that moves the selector in Best is inert in Rev 1, where the selector comes out right
   anyway. Rev 1's extra call block does in Rev 1 what something unknown does in Best; the address
   FAKE is a stand-in for that unknown.

Never commit anything from `versions/` (gitignored); the helpers read it at run time.

## Permuter

`/Users/lila/Documents/Projects/tokimemo-wt/w14-perm/build/permute/func_80143580`: the base is the
T-25319 body (293 rows; permuter base score 5260). Best so far 4768 (`output-4768-1`, found in the
first 5 minutes; the log was at 1446 of 21587 s when read). Candidate: `int new_var = 1;` after the
selector read, then `new_var` for nearly every literal 1 (sprite index `S[1]`, call arguments,
`& 1`, `+= 1`). It measures 260 rows / 252 here, against the stale base. That is a
constant-in-a-variable FAKE, not a source. It does fit a reading in which one more long range
across the cases moves the colours (lens 2). The base is stale: the long run should be restarted
on `fresh-r1-best.c`. This digest started no run.

## Negatives (measured; do not repeat)

From this digest: compiler-lens fill form crossed into the clean-room body (236-740); copy
propagation off via
`register` + dead F5F read (684); dead `index = unk_F5F` read or `register index` alone (no change).
From the round-1 lenses: vT/vR/vU4 variants (see table); dropping `index`/`address` declarations
changes the frame (0x90 to 0x88, vP2).
From T-25131/T-25319/T-24526 (their NOTES.md have the details): selector local types and
`switch (G)` forms; `register u8 step`; empty tests before every advance; case 9 chain orders
other than the clean-room one, cast drops and store moves; `x--` compare spellings (`<= K-1`,
casts, swapped operands, `x--; if (x < K-1)` +2 words); D4 argument views (`(s16)x`, `x + 0`,
`x | 0`, unprototyped callee); unprototyped func_8004B19C; F5F with a `girl` local, `if (F5F) {}`
or `(void)F5F` at the entry; entry self-stores of unk_110D or the fade fields; record-loop pointer
walks (the indexed `for` is the original); `if (address) {}` anywhere else (329-377 rows); a
diagnostic zcopy-off cvt_pass copy at the old base (753).

Warning: the clean-room branch committed `out/*/exp.txt` and `built.txt` (instruction dumps of
the original) and `mini/*.o`. Those are game-derived and must never be merged. Take bodies from it
file by file.

## The three lenses for this round

### Lens 1: two-version triangulation (Rev 1 as a second target)

Hypothesis: the missing ranges are the same in both releases, so a correct body explains Rev 1
too. Rev 1 exposes what Best hides: the high-part and fade ranges are longer in the original (they
reach case 3's calls), and 0x80000000 is coloured after the selector. Evidence: the rev1 delta of
the best body is structurally exact (0 hunks, case 9 included), the selector is already right
there, and `if (address) {}` is inert there; the per-FAKE and partial-delta tables above.
Method: score every candidate on both versions (`replay.py` plain and `--rev1`, `rev1cmp.py`) and
keep only changes that do not hurt Rev 1. First targets: the spr0 high-part range (Rev 1 t0, ours
v1), then the fade value (Rev 1 t1, ours t0), then 0x80000000 (t4). Read the Rev 1 trace (regdiff
with `--src` on a `--rev1` source; save the source from `rev1.apply`) to see which blocks the
original's long ranges must cover. Then carry each fix back to Best and try dropping
`if (address) {}`. Proof: Best rows fall with the FAKE gone, and Rev 1 register substitutions
fall at the same time.

### Lens 2: allocation solver on the selector / 0x80000000 / address knot

Hypothesis: three range facts settle the remaining 122 rows. (a) The selector needs a2 and a3
blocked in its blocks by ranges with no instruction of their own (chain VREGs, dead-def ranges,
free references), not by `address`. (b) 0x80000000 needs a priority below 0.5, so that it is
coloured after the selector, and a v0/v1 neighbour in one of its six blocks. (c) `address` must
not hold a1 from the entry to case 6. Method: from the regdiff/R9 trace of the best and of the
no-FAKE body, list every range with its priority terms (uses, defs, live units, passed-through
blocks, D) and colour against the original's. Simulate the colouring order and write each needed
change as "range X needs priority above/below Y" or "range X must (not) share block B". Then map
each to a known lever: free references `+ G * 0` (T-25501), empty-label block splits and the implicit-int
func_8004A1A0 (T-25314; the latter already moves 0x80000000 to v1 here),
never-true call-result arms (T-25043), compare statuses or reused locals (`index`/`address`, as in
the clean-room lens), fill-loop shapes. Use the trace of `M108-46`/`M108-47` (chain VREGs) to
see what in the source creates them. Check every candidate on Rev 1 as well (lens 1's scorer).
Proof: a candidate that gets selector t0 without `if (address) {}`, or 0x80000000 in t1.

### Lens 3: search on the new best, scored on both versions

Hypothesis: the gap is small enough (122 rows, 4 families, no structural gap except case 9) for
guided search, now that the base is good. Every earlier search ran on the 293-row T-25319 body:
the permuter (its best is the `new_var = 1` FAKE) and autotweak (emptytest, order and chain kinds
found T-25131/T-25319 steps 6-8). Method: start `tools/permute.py` on `fresh-r1-best.c` (`-j 1`,
`nice -n 19`, a few hours, harvest before finishing) and run autotweak's emptytest/order/chain/
gstate kinds on it, each candidate also scored with `replay.py --rev1` + `rev1cmp.py`. For each
gain, read the regdiff trace and say which range moved and why, then replace FAKE forms with the
closest real spelling (a reused local, a statement order, a declaration). Reject gains that hurt
Rev 1. Proof: a measured drop below 122 rows that keeps Rev 1 at 0 hunks with no more register
substitutions.
