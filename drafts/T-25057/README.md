# T-25057 NAME_ENT func_801398EC

Base 633d95356, isolated codex/t25057. The completed candidate matches all 123
words and passes the literal 492-byte default-build linked check. It is installed
as default C in src/ovl/NAME_ENT/80139740.c. The full build and all 27 binary hashes
pass, as do header/global checks and the prototype baseline (0 new, 15 known).
Hash results are preserved in gate-results.json. Root's direct standards/spec
review found no issues; integration and final acceptance remain with root.

## What closed the match

The ordinary thirteen-record indexed loop lets uopt produce the target's peeled
first record and six paired trips. The old manual paired guard had126 words and185
printed diff rows. Initialize i before the selector to give them v0/v1. A discarded
unsigned read of the current record establishes the induction address in a0.

A one-element u32 local prevents copy propagation from replacing the remainder
variable with a later expression range. Its dead initial frame assignment numbers
that local before the frame and index ranges. The flag-zero arm overwrites the
local with the real remainder before reading it; the other arms never read it.
Uopt removes the initialization and promotes the array completely. No stack access,
frame adjustment or extra runtime source load remains.

The array restored a1 remainder/a2 frame/a3 index, but uopt reordered the coordinate
sum. Captured optimized ucode showed tile-multiply before row-divide/multiply,
whereas the target's temporary allocation requires the row expression first.
Changing the sum to `row - (0U - tile * 16) + 128` preserves that order until ugen.
Unsigned arithmetic makes this exactly equal to the original sum modulo2^32.
At use, i is0..12 and tile is0..2, so the signed row product is bounded as well.

All four artificial source constraints carry FAKE/T-25057 comments. Removing the
earlier unsigned tail-use test was neutral, so it is absent from the final source.
No shared header, prototype, compiler, generated assembly or configuration changed.

## Removal controls

Each control compiles the complete translation unit with one constraint removed.
All controls below have123 words. Printed rows count both sides of each changed
word; these equal twice the differing positions because the lengths are identical.

| Source | Identical words | Differing words | Printed rows |
| --- | --- | --- | --- |
| Final annotated source |123/123|0|0|
| With redundant tail test |123/123|0|0|
| Remove discarded record read |87/123|36|72|
| Remove dead frame initialization |98/123|25|50|
| Replace array with scalar |98/123|25|50|
| Replace subtraction identity with addition |106/123|17|34|

The earlier source checkpoints remain as evidence: func_801398EC.c matches108/123
positions with frame/remainder registers exchanged; func_801398EC-array.c matches
106/123 with the expression-evaluation difference. They are historical unmatched
controls. The final source is func_801398EC-matched.c and the production definition.

## Reproduction

Use the existing tokimemo-decomp-arm64 image, this worktree mounted at /work, the
immutable original disc mounted read-only at /work/disc, and /work as cwd. The
scripts import the existing T-24515 full-translation-unit probe. Run serially.

```
python3 drafts/T-25057/probe.py current indexed remainder-test remainder-late-test
python3 drafts/T-25057/lifetime.py remainder-unsigned-test index-first
python3 drafts/T-25057/record_order.py record-word-read record-byte-read
python3 drafts/T-25057/remainder_source.py signed-remainder precompute-remainder remainder-byte
python3 drafts/T-25057/remainder_memory.py remainder-array remainder-address
python3 drafts/T-25057/array_conditional.py array-conditional array-early-zero array-early-frame
python3 drafts/T-25057/array_dead_store.py array-dead-frame
python3 drafts/T-25057/array_store_order.py array-store-first array-sum-flipped
python3 drafts/T-25057/array_subtract.py sum-negative-product sum-negative-expression
python3 drafts/T-25057/close.py annotated-match remove-tail-use remove-record-use remove-dead-frame remove-array remove-subtraction
```

The final promotion helper expects the original guarded production definition and
runs only once. After installation, run `ninja -j2`, then
`python3 drafts/T-25057/gates.py` for default-C linked proof, all27 SHA1 checks and
the header/global/prototype guards. The earlier guards can be replayed from base
633d95356; the archived sources and controls are independent of production status.

For optimized-ucode capture, also mount the existing external
/private/tmp/t24047-uopt at /scratch read-only, then run:

```
python3 drafts/T-25057/capture.py precompute-remainder array-store-first
```

This observes the traced compiler without changing its decisions. Detailed objects,
ucode and assembly stay in ignored build/T-25057. The committed JSON files contain
only source names, counts and hashes; no game data is included.

## Rejected mechanisms

Signedness, in-place arithmetic, commuted addition, reassociated addition and a
literal OR-zero all normalize to the same wrong expression tree. A negative
multiply remains arithmetic and adds instructions; subtracting an unsigned negation
is the successful form. Local phi arms retain unwanted ranges or change the loop.
An early live frame read changes load placement. A dead initialization orders the
same range without retaining that load. Late-zero index dependencies can survive
in the second unrolled record. A defined goto diamond changes range numbering but
leaves a three-register permutation.

The remaining scripts and results.jsonl preserve these causal controls. They earn
no extra coverage beyond the one completed492-byte function.
