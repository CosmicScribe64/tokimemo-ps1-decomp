# T-25040 MAIN flag dispatcher

Target `func_8006D138`, 872 bytes / 218 words, in `src/main/8006CB30.c`.
Production is default C with guarded/default literal 872-byte proofs and
complete MAIN SHA1 `e823bd844a8f8fa4d05483b59c66bc54b8393b26`. This change
preserves the T-25036 match in the same translation unit.

## Closure

`two-word-b8-dead-byte-view` matches all218 words. An explicit initial flags
snapshot preserves the five first-arm flag tests. A late-zero address on
the first b7 read keeps it base-relative, but initially moves the absolute
representation to the first b8 byte read. Applying a late-zero address to
b8 alone moves the representation again, to a live word read.

The final control keeps a discarded static view at exactly the same byte
width and offset: `if ((u32)gs->unk_F64[1] < 0) {}` immediately before the
late-zero b8 load. This holds the static alias in uopt, while the real load
remains indirect. Ugen drops the false comparison and its read entirely.
Together with the year98 identity below, this gives the literal match.
Every unsigned comparison is false for every input, pointer offsets are
zero, and no extra runtime loads, stores or branches are emitted. All odd
controls, including the empty date tests, carry FAKE annotations.

```
python3 drafts/T-25040/probe.py two-word-b8-dead-byte-view
python3 drafts/T-25040/prepare.py
python3 drafts/T-25040/prove.py guarded-proof build/t25040/final/guarded.c guarded
python3 drafts/T-25040/prove.py default-proof src/main/8006CB30.c default
```

Proofs additionally mount the verified integration checkout read-only at
`/baseline`; only this newly compiled TU is substituted into the private
link. No baseline object is changed or copied. Header/global/prototype
checks and independent review are recorded in the ticket. Root owns the
full27 integration gate.

## Earlier three-instruction checkpoint

`near.c` is the `direct-98-latezero` body. Normal default full-TU compilation
has 218/218 words and only three differing instructions: the first b7 byte
read uses an absolute address instead of the persistent GameState base,
and its preceding branch/delay slot differs. Every other 215 words agrees.
This is not a literal match and must not be counted as C coverage.

Two source controls explain the reduction:

* An empty entry switch changes the entry control-flow shape. The combined
  flags/base empty test preserves the entry flag load. Correction from T-25052:
  actual input ucode for the accepted function has `OPTN 405 1` (zcopy on);
  the earlier explanation that this switch activates zcopy off was incorrect.
  The literal match and the measured source controls remain valid.
* In the empty year-98 comparison, `0x62 + ((u32)gs->unk_03F < 0)` is exactly
  98, but the comparison folds after uopt. It removes one unprofitable
  constant range from the interference graph. Year and base each go from
  22 interference edges to below the constrained threshold. Unconstrained
  bit order then gives flags v0, year v1, base a0 and constants a1/a2/a3.

The source controls require FAKE annotations if promoted. All tested late
zero comparisons are unsigned and therefore false for every input.

## Reproduction

Run inside the existing `tokimemo-decomp-arm64` image, mounting this checkout
at `/work`, original disc at `/work/disc` read-only and baseline assembly at
`/work/asm` read-only:

```
python3 drafts/T-25040/probe.py direct-entry-switch-bits direct-98-latezero
```

Scratch output is under ignored `build/t25040/<label>/`, including the full
translation unit, disassembly, measurement and resolved instruction diff.
The harness asserts that production source is untouched. `capture.py`
records the normal/optimized ucode using the existing read-only tracer at
`/tracer/trace`. `trace_constraints.py` creates an instrumented scratch
tracer from `/diagnostic/variants/trace.c` to report interference counts;
this diagnostic changes no compiler decision and is not used for proofs.

## Negatives and boundaries

* Explicit snapshot and signed/union spellings restore entry load but
  introduce another range; source-only declaration changes do not solve
  constrained allocation. Direct bit fields preserve target scratch numbering.
* A late-zero base pointer splits pointer ranges and grows to 229 words.
* Late-zero output indices fix the first three registers but introduce
  alias reloads and 255 words. Field-address/array-object forms are identical.
* Extending a snapshot through all output values gives the correct first
  three registers, but constrains the year constants and changes scratch
  numbering. Shorter lifetime/dead branch controls do not solve this.
* Empty entry switch alone gives 218 words and cost130. Volatile output
  stores are neutral; absolute date views and arm-local year copies regress.
* Read-only constraint tracing corrects the initial hypothesis: the leaf
  threshold here is 22 total colors, not 12 caller-save colors. Direct
  snapshot has only five edges, year/base each22, constants20/21.
* Late-zero address/volatile view on the first b7 read removes its absolute
  address but disrupts the first b3 snapshot reuse, growing to220 words.
* Late-zero identity on the first 128 store is neutral. The year98 identity
  removes the critical range and reaches the checkpoint above.
* Explicit snapshot plus first b7 address control reaches216/218 exact
  words, with only the first b8 load absolute. Altering all flag reads
  removes the static representation but changes month promotion. Adding
  pointer identity assignments, even locally restored, changes alias
  reloads. Existing union views are neutral. A discarded word view is
  insufficient; the successful discarded view must match the byte width.
