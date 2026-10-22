# T-25042: KANGEI default-arm emission

Target: `func_80134724`, `src/ovl/KANGEI/80134120.c`, 172 words / 688 bytes.

The guarded T-25011 source reproduces four changed instruction positions (159, 160,
161 and 163): its default arithmetic uses t1/t2/t3 where the target uses t0/t1/t2.
Its range-check delay slot already matches. The uncast control gives `diff_score` 26
because the default shift moves into the range-check slot, shifting the intervening
stream. This is consistent with T-24902's documented score26; its "2 rows" used a
different alignment/counting convention. Do not interpret the numeric scores as
changed physical word counts.

Captures use the complete translation unit and stock compiler pipeline. The ignored
`build/t25042` directory retains each source, object, optimized ucode, binasm, stock
`-xbbdbg 8` trace and resolved comparison. No compiler output is patched.

## First source tests

- Empty read of the stored `D_80139ADC` after the default store, with/without the
  u16 cast and with either ordinary or unsigned-false condition: 172 words, score134.
  Unlike T-25041's single-store function, uopt promotes the value across every switch
  arm, altering the earlier expression stores too. Reject this transfer.
- Empty local `v`/`s` test before default arithmetic, ordinary or unsigned-false:
  171 words, score111/83. These change whole-function register allocation and collapse
  an earlier branch instruction; they do not isolate default scheduling.
- Baseline optimized ucode has `cvt L; cvtl L16; mpy L4` in the default expression.
  The stock local schedule already has no mask, with shift destination t1. Its missing
  initial INST and shifted temp ring are consistent with the cast consuming a
  temporary before as1 eliminates the mask. The live dispatch selector t1 gives a
  concrete reason why this shift cannot be hoisted into the range-check slot. Further
  phase evidence is needed before treating that as the complete mover explanation.

Current production source remains unchanged. Next checks separate a conversion
assignment or an empty global preload from default arithmetic; these target emitted
blocks and temporary lifetime instead of the old switch-weight/selector grids.

## Phase evidence and focused follow-ups

Replaying both original binasm captures with the existing diagnostic as1 binary
confirms the live-register interpretation. In both versions bb18 has weight4 and is
visited before dispatch bb19. Cast variant: bb18 slot3 `sll t1,v0,2` is movable,
`graph_reject=0`, `path_accept=0`; bb19 slot1 `sll t1,t1,2` is accepted. Uncast:
bb18 slot2 `sll t0,v0,2` has `path_accept=1`. Thus the cast does not solve this by
changing block weights. The ugen binasm directly shows `and t0,v0,65535; mul
t1,t0,4; addu t2,t1,v1; addu t3,t2,3; sh t3` before mask removal.

Further source results (all full translation units, production unchanged):

| Mechanism | Result |
| --- | --- |
| Empty year/selector/cleared-global/club read before default | 171 words; allocation and earlier branch change, score111/123 |
| c2 assignment or compound v multiplication | 172 words, score32; extra truncation temps and hoist |
| u16/u32 local conversion or local mask/compound multiplication | Same four-position cast miss, score8 |
| Empty unrelated global/address test after default | Old uncast hoist, 172 words, score26 |
| Same test before default, including known address/unsigned comparison | 171 words, score111 |
| Empty test of entire default expression | 172 words, score32 |
| Constant/unsigned-true guard or one-execution loop around default | 171 words, score111/113 |
| Pointer-address guard/loop | 174/176 words; address code survives |
| Constant/unsigned-true case6 exit or empty condition | Either unchanged hoist or earlier allocation change; pointer truth keeps extra code |
| Signed/unsigned shift-extension idioms | 173 words; extra shift survives, score11 |
| Masked product or unsigned truncated product | Same four-position cast miss, score8 |
| Signed truncated product | Hoist plus extra temp shifts, score32 |
| Reorder local declarations under constant-guard control | No change to171-word result |
| Add earlier empty address test or guard the final s=3 assignment | No change to171-word result |
| Earlier empty s test | Changes allocation again (score83), not restoration |

These separate two obstacles: the default shift must fail the mover path check,
and a new source block must avoid changing uopt's allocation and the earlier s=3
branch. Neither follows merely from making the predicate logically empty.

## Later boundary checks

- Uniform constant controls on all stores do not restore the earlier allocation
  (171 words, score123). Sharing the formula from case6 by incrementing v or s
  produces 168/169 words, rather than the target's separate arithmetic arms.
- A volatile-qualified value cast has no scheduling effect (old hoist, score26).
  This is distinct from the prior volatile-global experiment, but also fails.
- An explicit late-false goto from case6 to the default label changes earlier
  allocation and the default expression's promoted range; it still hoists a shift.
- Constant nested switches produce the same171-word/score111 result as the constant
  guard. A default cleanup/return keeps extra calls (177 words); sharing only its
  last call keeps175. An empty late-false loop keeps the earlier allocation problem.
- Additional case7 jump/fallthrough forms retain the widened switch bound; a
  separate copy of its body retains extra arithmetic. Sharing case6 and default
  with an explicit selector check retains that check.
- A named u16 or masked-u32 local used by an empty test does change conversion
  allocation: optimized ucode puts the mask into a register variable, rather than
  spending the extra scratch temporary. The default shift returns to t0, but its
  mask/control boundary is removed before global scheduling, so the old hoist
  returns. This also changes earlier allocated variables. Keeping the local alive
  until an empty test after the store gives171 words, score113; adding a shifted
  zero use leaves extra arithmetic.

The remaining issue is therefore a phase-order constraint, not an unexplained
choice among equally ready instructions: a harmless extra default predecessor or
boundary must survive until the global move, while leaving no instructions or data
in the final binary. The stock as1 pipeline has a branch optimization stage after
its global scheduling stage; locating a C form whose edge lasts until that stage
remains open. This is a research lead, not a proof that such a form exists.
