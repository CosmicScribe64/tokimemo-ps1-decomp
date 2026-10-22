# T-25023 bounded BUNKA scheduler consultation

Base: 883760224. Sole source: `src/ovl/BUNKA_SD/801388D0.c`, function
`func_80139290`. Source SHA256:
`d6a757508f068a96499bd0b43190c440cbf04a5697dd41ed3c71ca3eab927582`.
No source changes or source variants in this initial investigation.

## Reproduction and metric

In isolated `/private/tmp/tokimemo-t25023`, Docker image
`tokimemo-decomp-arm64`, normal IDO 5.3 build context, whole translation unit:

```
python3 tools/funcdiff.py --resolve BUNKA_SD:func_80139290
```

Refreshed the single baseline object after copied artifacts failed freshness.
The guarded C has 379/379 instructions (1516 bytes), four changed zero-based
positions 166/167/171/172, eight printed +/- lines, aligned cost 4. These
are distinct metric conventions. First divergence is address 0x80139528:
target year-byte lbu, candidate store-address lui; the next instruction swaps
them. Second pair starts 0x8013953C. Registers and instruction count agree.
No C match; production retains assembly fallback. No coverage added.

## Exact question

In arms 0x49/0x66 (bb43/45), make the speculative year-byte lbu precede
the D_801206AC store-address lui while retaining compare-head bb42's
li t2,0x60 filler and bb44's li t8,1 filler. Preserve all other instruction
words and register ownership. Source-order ties and global eligibility are
coupled: changing only local priority has already broken the head fillers.

At most three new causal source hypotheses, each with an explicit prediction
for uopt/ugen emission, as1 DAG/pick order and global candidate choice.
Read-only consultation; no compiler/build-rule/header changes, no experiments
by the consultant, no generic option/syntax/declaration sweeps. Reject ideas
that merely repeat the ledger below. A negative answer is useful if it states
the missing mechanism and exact evidence required before testing again.

## Previous ledger

- T-24709: 692 autotweak edits, conjunction swap, one-line arms and shared
  selector local; no gain. Do not repeat.
- T-24927: explicit year local fixes local order but creates a roughly
  30-row allocator cascade (year a3, case constant a1 instead of ring t1,
  other live ranges displaced). Do not repeat real-local work.
- Volatile-year is the wrong mechanism for local scheduling and destroys
  merged/speculative reads. Do not repeat.
- T-25020: empty field tests in all three arms fix local order but hoist
  early lui t0 into bb42/44 slots, replacing required li fillers (16 printed
  lines, aligned cost 10). Do not repeat.
- Empty field test only in 0x3B fixes that arm without regressions and is
  the preserved four-position source. Its compare slot already holds
  move a1,v1, so it has no corresponding free-slot regression.
- Recovered bb60 is unrelated: eight moved instructions from a later modulo
  switch, not nine; not the year-byte residue.

## Private reproducible evidence

No raw dumps/binaries committed. Existing context is read-only in
`/Users/lila/Documents/Projects/tokimemo-wt/codex-t25018-kangei`:
`build/t25020-cap/` (seven-position baseline),
`build/t25020-emptyfield-cap/` (all-arm negative),
`build/t25020-empty3B.c` (preserved source). Recovered artifacts:
`/private/tmp/t25016-recovered-20261022`.

Fresh four-position capture in this isolated tree:
`build/t25023-cap/{unit.c,in.G,in.T,unit.o,bbdbg.log,xtr.log,function-trace.txt,measurement.json}`.
Capture via existing `drafts/T-24900-as1/cap.py`, `cc_cap.py`, `shim.py`.
Existing read-only trace binary `/private/tmp/t24047-uopt/variants/as1-trace`.
Stock versus trace output matches all four present relevant sections:
.text, .rodata, .rel.text, .rel.rodata. Preprocessed SHA256:
`e37a766a6081f74cd82ef436c5b48e7d2e9c0008d5982285a1970b710f3bbb0c`.
Initial-node/pick records are in bbdbg/xtr logs; input G/T preserve the
pre-schedule representation, output objects preserve post-schedule evidence.

See `wiki/tickets/T-25020-bunka-sd-trace-closure.md`,
`wiki/tickets/T-24927-func-80139290-801388D0.md` and
`drafts/T-24927/NOTES.md` for prior measured mapping. No existing source lever
outside those negatives was established in the Sol diagnosis; requesting one
bounded Astra consultation rather than rerunning exhausted forms.

## Astra result

Read-only GPT-6 Astra consultation returned no justified new source edit.
No consultant or subsequent Sol experiments were run. Important distinctions:

- Global scans follow original INST emission order, not local pick order.
  Current bb43 rejects store-address lui on the path, skips its store, then
  accepts li t2,0x60 at INST 4 (`xtr.log:11068`). Current bb45 immediately
  accepts li t8,1 at INST 2 (`xtr.log:12302`).
- In the all-arm negative it is the **year-address** lui t0 that becomes
  INST 2 and replaces the required fillers; not the store-address lui at.
- Correction to T-24927's explanatory prose: li t2,0x60 has aftercycles 0
  in these arm DAGs, not 2. Its role is earlier global-candidate position;
  the local aftercycles-2 tie concerns lbu versus store preparation.

A sufficient assembler-input arrangement would put the required li ahead
of the year-address lui in each arm, but the year lbu ahead of the store
address lui. With unchanged dependencies/CFG/precolours this predicts the
required li is scanned first globally; year lui wins local aftercycles 3,
then lbu wins the aftercycles-2 tie before store lui. This demonstrates a
feasible IR arrangement, not a C source solution.

Exact revisit condition: establish from uopt-output/ugen-emission evidence
how source can independently emit the store-value constant and the year
memory-expression group while preserving existing precolours, particularly
bb45's store constant versus address preparation. Existing as1 G/T and logs
show the resulting order but not that upstream mechanism. Comma, ternary,
empty-comparison or assignment variants without this evidence would repeat
syntax searches. No compiler-limit or impossibility claim follows.
