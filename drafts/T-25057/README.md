# T-25057 NAME_ENT record selector

Base633d95356, isolated codex/t25057. No new matched functions or C bytes.
Owned production file: src/ovl/NAME_ENT/80139740.c. Production is unchanged.

The current func_801398EC guard has126 words/185 printed diff rows against123
original words. The older ordinary indexed loop has126/173. A named unsigned
frame remainder followed by an always-false unsigned comparison after the sprite
store preserves the remainder as an allocated uopt range and produces123 words.
Initializing i before the selector restores v0/v1. An exact-width discarded read
of the current record before flag testing orders the induction address into a0.
Precomputing the remainder before the flag tests lets uopt sink its calculation
back to the original use paths. The saved func_801398EC.c has123 words/30 printed
rows (15 differing words).
It is a diagnostic source, not a verified match.

The saved source retains the thirteen-record loop, lazy source loads, peeled first
record and six paired trips. The discarded record and remainder tests add no
runtime instructions. Tile is assigned before every read. Target and draft differ
only in frame a2/a1 and remainder a1/a2 allocation. Scheduling and all other
registers agree.

Trace of the current best full translation unit gives these unconstrained ranges:

| Value | Bit | Register |
| --- | --- | --- |
| i |0|v0|
| selector remainder |7|v1|
| record induction |10|a0|
| frame |17|a1|
| frame remainder |20|a2|
| record index |24|a3|

The trace comes from the existing external instrumented uopt; production compiler
files were not changed. Detailed objects and trace outputs remain under ignored
build/T-25057 and build/scratch/regdiff/func_801398EC.

Run scripts serially inside the existing tokimemo-decomp-arm64 image, with this
worktree at /work, the immutable original disc mounted read-only at /work/disc,
and /work as cwd. No host Python or dependencies are used. Scripts import the
existing T-24515 full-translation-unit probe, replacing only its output directory.

```
python3 drafts/T-25057/probe.py current indexed remainder-test remainder-late-test
python3 drafts/T-25057/lifetime.py remainder-unsigned-test index-first
python3 drafts/T-25057/range_order.py record-early tile-initialized
python3 drafts/T-25057/record_order.py record-word-read record-byte-read
python3 drafts/T-25057/remainder_phi.py remainder-phi remainder-phi-no-tail
python3 drafts/T-25057/remainder_local.py local-index-arm local-early-ref
```

An early address-only comparison adds four instructions. Tile initialization alone
is neutral. A remainder-dependent unreachable alternate assignment adds a second
expression range. An index-dependent alternate assignment loses the initial
selector move and changes scheduling; a late-zero local reference adds instructions.
These controls do not improve the saved source.

## Precomputed remainder checkpoint

The latest saved source improves to123 words/30 printed rows (15 differing words).
It computes tile before the flag tests; uopt lazily sinks the calculation back into
the original flag-zero paths. All instruction scheduling now matches. The only
residue is frame a1 versus target a2, and remainder a2 versus target a1. New trace
bits are17 for frame and20 for remainder; both remain unconstrained. The record
index now occupies a3 correctly. The current table above records this30-row source.

```
python3 drafts/T-25057/remainder_source.py signed-remainder precompute-remainder remainder-byte
python3 drafts/T-25057/remainder_join.py remainder-join remainder-join-identity
python3 drafts/T-25057/precompute_local.py precompute-local precompute-local-same
python3 drafts/T-25057/remainder_inplace.py inplace-remainder inplace-local
```

Signed and in-place locals are neutral. A byte local adds six words. Local merge
and unreachable alternate-definition controls retain unwanted assignments or change
loop structure; none replaces the saved source.

## Memory-local alternative and first temporary event

The second saved source, func_801398EC-array.c, uses one u32 array element to keep
the local at bit15, before its RHS frame at bit17. Its initial frame assignment is
dead on all use paths: the flag-zero arm overwrites it with the remainder before
reading it, and both other arms never read it. Uopt removes that initialization;
all runtime frame loads remain at their original sites. The array is fully promoted,
with no stack access or frame adjustment. Its count is123 words/34 rows.

Moving the zero record store before the array assignment restores every scheduler
choice. All uopt ranges then use their target registers. The17 differing words are
ugen expression temporaries and the addition operand order. Captured optimized
ucode proves the first cause: the scalar candidate emits row div8/mpy48 before
remainder mpy16; the array emits remainder mpy16 first. Target allocation follows
the scalar tree, although as1 schedules the remainder shift before the signed
division branch. No claim is based merely on final disassembly alignment.

Signed local/use controls, both addition orders, both associations and a literal
OR-zero normalize identically. Index-dependent late-zero identities create another
range or surviving instructions. A fully defined goto diamond to place the scalar
frame definition after its remainder preserves123 words but produces50 rows;
copy propagation does not yield the desired frame/remainder order.

Reproduction continues serially from the scripts above:

```
python3 drafts/T-25057/remainder_memory.py remainder-array remainder-address
python3 drafts/T-25057/array_conditional.py array-conditional array-early-zero array-early-frame
python3 drafts/T-25057/array_dependency.py array-index-zero
python3 drafts/T-25057/array_dead_store.py array-dead-frame
python3 drafts/T-25057/array_store_order.py array-store-first array-sum-flipped
python3 drafts/T-25057/array_signed.py array-signed array-signed-use
python3 drafts/T-25057/array_sum_tree.py sum-group-tile sum-group-row sum-row-identity
python3 drafts/T-25057/array_sum_identity.py tile-late-add row-late-add
python3 drafts/T-25057/array_local_identity.py tile-pair-zero
python3 drafts/T-25057/scalar_order.py scalar-goto-order
```

For pass capture, mount the existing external /private/tmp/t24047-uopt
at /scratch read-only and run:

```
python3 drafts/T-25057/capture.py precompute-remainder array-store-first
```

The capture script observes input/output ucode without changing compiler decisions.
The results.jsonl file records each measured serial control; raw objects, ucode and
assembly remain ignored. These two source alternatives are the current frontiers.

Annotated-source verification is `python3 drafts/T-25057/verify_saved.py`.
It recompiles the complete translation unit and records position-wise resolved
words in saved-results.json: scalar108/123 identical words, array106/123. These
counts include the target's trailing zero word. They are diagnostic comparisons,
not literal linked MATCH verdicts. Production source is unchanged, so no new
whole-binary gate is claimed for these unmatched drafts.
