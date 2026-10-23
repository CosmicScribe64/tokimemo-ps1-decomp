# T-25037 BUNKA_SD func_80139290

Base242c0c3a3; source src/ovl/BUNKA_SD/801388D0.c. The baseline has379 words
and four changed positions166/167/171/172 (eight printed lines). This task
closes all four through C source and the unchanged normal IDO5.3 pipeline.

## Cause and source construction

A copying shim at the normal ugen boundary captured the optimized ucode.
Baseline cases0x49/0x66 emit their store, then an edge preload of constant0x60
and the year byte. Adding an early year test places its preload at block
entry, ahead of the store, but lets as1's global mover take the year-address
lui instead of the required constant filler.

Pinned upstream uoptemit.c at d068e439f52615763a3facd6944873899ebad2fd
explains the observed ordering: emit_rstrs emits entry reloads in ascending
register order; emit_rlods follows the edge's rlodbb linked list. Reading
constant and year together early therefore emits year t0 before constant t2.
The source reference is supporting explanation; actual captures and final
stock-pipeline byte proofs establish the result.

Case0x49 stages the constant and year in separate blocks:

```c
if (g->unk_1104.u == 0x60) {}
if ((u32)g->unk_03E < 0) {}
D_801206AC = 0;
```

The first empty test makes the constant preload precede the year-address
lui globally. The second empty condition retains the year preload until
late constant folding, then vanishes. A plain empty year test instead leaves
an extra conditional/unconditional branch after assembler shortcutting.

Case0x66 puts an empty frame comparison before its original store and the
unsigned-false year test after it:

```c
if (g->unk_1104.u == 1) {}
D_801206AC = 1;
if ((u32)g->unk_03E < 0) {}
```

This gives the same final register identities and instruction words. Putting
the store before the first empty test fixes scheduling but causes uopt to
recompute the selector difference with an extra subu. Moving the test before
the store removes that recomputation. All tests are semantically empty;
unsigned values cannot be negative. Each source mechanism is marked FAKE.
The existing T-25020 case0x3B empty test remains necessary.

## Negative ledger

Metrics are resolved aligned cost, with word count separately stated. No
zero-cost result was called a match until the private literal link passed.

| Family / saved scratch label | Words | Cost | Finding |
| --- | ---: | ---: | --- |
| baseline |379|4|Two lbu/lui swaps |
| compare49, compare49right |381|18|Early year before constant; extra branch |
| compare66 |379|6|Early year becomes wrong global filler |
| constant-before-year49 |381|14|Correct ordering, redundant branch remains |
| constant-after-store49 |380|15|Extra selector recomputation and ordering |
| constant-before-store49 |379|4|No change |
| nonnegative66 |379|6|Correct constant register, wrong filler |
| notnegative66 |379|178|Constant allocated; ring cascade |
| zeroshift66 |380|187|Shift survives; ring cascade |
| selfcompare66 |379|4|Folds too early |
| latefalse49, latetrue49, latefalse-store49, latefalse49max |379|2|Case0x49 fixed exactly |
| latefalse49and |373|300|Conjoining conditions destroys required CFG |
| split66, duplicatestore66 |380|15|Constant0x60 preload also changes filler |
| falseafter66, ternary66, conditionalstore66 |379|4|Case0x66 year still first |
| splitassign66 |380|9|Correct scheduling, extra subu only |
| compound66, bitor66 |380|143|Promotion changes store value and registers |
| difference66, frameunsigned66, differenceidentity66 |380|9|Moving/rephrasing selector use only moves extra subu |
| year66 |379|4|Year preloaded before store constant |
| reversedassign66, reverseddifference66 |380|9|Operand reversal does not remove extra subu |
| firstcondition66, firstdifference66, branchstore66 |379|0|Matched stock compiler output |
| compoundcondition66 |374|301|Conjoined CFG rejected |
| simplecondition66 |380|11|Zero test does not give required sequencing |

The case0x66 families use the already-fixed case0x49 source. Baseline and
first families use the original four-position source. All inputs, copying
shims, optimized ucode, binasm, local scheduler logs, output objects and
measurements stay ignored in /private/tmp/tokimemo-t25037/build/t25037.
Production compiler code, flags, headers and build rules are unchanged.

## Verification

Both the guarded candidate and promoted default C pass private normal
linking through tools/funcdiff_verify.py exact_match: function address and
span agree; all1516 bytes equal the immutable target. No assembly fallback
is present in the final source. Root owns the full27-unit integration gate.

Affected-overlay verification rebuilt all11 current BUNKA_SD C units using
the generated Ninja commands, then ran the normal linker, objcopy and SHA1
commands: BUNKA_SD.bin OK. The initial ordinary Ninja attempt stopped when
it tried to regenerate assembly in the read-only reference mount; no shared
input was modified. Header, global migration and branch prototype checks
pass (0 new disagreements,15 known). Root standards/spec review has no
findings; ticket remains In Review for integration.
