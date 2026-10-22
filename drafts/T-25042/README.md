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

## Store alias and late-edge checks

Unsigned scalar aliases still promote the same shared global (score134), rather
than isolating the default store. A provably zero array index, computed from an
unsigned comparison, does keep the access indirect through uopt, but the final
code retains a separate address addiu and still hoists the shift. Local final-result
boundaries retain172 words but change a promoted arithmetic register (score30/32).

Signed tests of masked or shifted nonnegative values reach as1, unlike unsigned
`< 0`, but as1 does not remove those tests: the final code retains the mask/shift
and bgez. Legal nested case labels (inside a constant if or a never-repeated loop)
do not supply a retained default predecessor. Conditional mathematically neutral
assignments also fail: a narrow local retains sign extension, while a full-width
local keeps conditional copies. Widening the function's v changes its earlier
allocation and instruction stream. These tests produced no candidate better than
the unchanged T-25011 guard.

## Allocation and unreachable-case follow-up

A u32 selector local with `s = (u8)func_80051A68(...); s &= 0x7F;`
reproduces the uncast control exactly (172 words, score26). This isolates why the
previous direct u32 declaration lost two initial instructions: staging the byte
conversion restores the helper-result move, masked temporary and subsequent
scratch-register sequence. A direct split without narrowing, a second cast on the
mask, and an empty pre-mask use instead retain the170-word/score188 difference.
Conditional signed AND-minus-one, OR-zero and late unsigned-false identities on
this restored full-width local all disappear before scheduling and retain score26.

Unreachable case labels outside the byte selector's domain (-1 and256), sharing
the default body, both retain extra dispatch code (178 words, score128). Thus they
do not provide a zero-runtime additional predecessor. The separate ENDING scan
budget finding does not directly transfer: KANGEI's undesired default is already
its first candidate, before the desired dispatch block.

## Region and conservative-liveness checks

- Setting otherwise discarded high bits of the restored u32 local retains real
  code:174 words unconditionally,176 under a predicate (score32/34). The final
  halfword store does not make as1 remove those operations across this sequence.
- A one-execution wrapper around the whole switch gives171/167 words with changed
  allocation; the literal-zero form still hoists the default shift. Preinitializing
  the default global before the switch instead sinks/shares substantial work and
  produces162 words, score190.
- Narrowing a translated switch domain prevents early affine cancellation, but
  leaves175/176 words (score151/166). A named translated selector with an empty test
  instead reaches172 words, score130: as1 combines its +1/-1 to `addiu t1,a1,0`, but
  keeps that copy and the changed allocation. This is distinct from the earlier
  plain translation, which uopt canonicalizes completely.

A print-only diagnostic assembler adds phase and path-live-set output to the
existing as1-probe source. The captured baseline, uncast and promoted-selector
objects reproduce the stock resolved target instructions exactly. The cast's path
failure is specifically the default shift's t1 definition bitmap0x00400000 against
exit block19's live-in bitmap0x3c40ff0e. The uncast t0 definition does not intersect
that bitmap. Further phase tracing is checking whether conservative liveness can
outlast an instruction removal; no source recipe or match is claimed from this.
