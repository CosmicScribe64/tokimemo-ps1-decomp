# T-25040 MAIN flag dispatcher

Target `func_8006D138`, 872 bytes / 218 words, in `src/main/8006CB30.c`.
Production remains `INCLUDE_ASM`. This draft preserves the T-25036 match in
the same translation unit.

## Current checkpoint

`near.c` is the `direct-98-latezero` body. Normal default full-TU compilation
has 218/218 words and only three differing instructions: the first b7 byte
read uses an absolute address instead of the persistent GameState base,
and its preceding branch/delay slot differs. Every other 215 words agrees.
This is not a literal match and must not be counted as C coverage.

Two source controls explain the reduction:

* An empty unsigned entry switch activates the existing uniform switch-copy
  boundary. Combined flags/base empty test preserves the entry flag load.
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

## Root-replayed two-instruction snapshot

`snapshot-near.c` preserves the newer explicit initial snapshot, late-zero first
b7 address and year98 identity. Root replay on current main independently gives
218/218 words, cost4 (two differing words): row116 target nop versus absolute
address lui; row117 target base-relative lbu versus absolute lbu. All other216
words agree. Run `tools/docker.sh python3 drafts/T-25040/replay-snapshot.py`.
Production remains assembly. Worker continues overlapping flag-view research;
rewriting further reads has so far moved the absolute-address residue elsewhere.
