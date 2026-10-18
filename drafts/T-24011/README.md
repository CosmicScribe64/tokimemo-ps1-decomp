# T-24011 DATE dispatcher near matches

These C files are source-only drafts derived from `drafts/T-17207/DATE/`.
Neither is a verified C match. Production sources retain `INCLUDE_ASM` for
all three assigned targets; there are no new verified bytes in this package.

## Reproduce

From the repository root, after normal Docker configuration:

```sh
tools/docker.sh python3 drafts/T-24011/probe.py
tools/docker.sh python3 drafts/T-24011/probe.py --standalone
```

The harness writes scratch C, objects and resolved diagnostic comparisons
under ignored `build/T-24011/`. It never edits production C or headers and
caps its header-check build at one job. The temporary declarations are the
missing DATE callback prototypes and `extern u8 D_8015BF48`; promotion would
require evidence-backed declarations in the overlay header and verification
of every affected function.

## Results and remaining blockers

`func_801449D4`: access `unk_110A` and `unk_03E/03F/040` through the recovered
`GameState *g` view. Declaring `g` as `register` and retaining three unused
pad words gives the original frame `0x170` and table base `sp+0x2C`. The pads
and copied selector remain explicitly marked FAKE. The isolated comparison
prints 11 added/removed resolved rows, including terminal object padding.
The full translation unit prints 48 positional added/removed rows. These
counts use different alignments: `funcdiff.py` pairs equal-length functions
by instruction index and uses sequence alignment when lengths differ.

The remaining small-dispatcher blocker is constant 16 placement: the target
loads it in the selector-21 branch delay slot and again at the shared
fallback entry; the draft hoists one load into the selector-17 test's delay
slot. The resulting instruction shift and branch displacements are real.
No cast, local-width, selector/fade byte view, inverted goto, or flattened
condition tested here resolves it. An `s32` no-return view also leaves 11
isolated rows. The coordinator is investigating this blocker separately.

A bounded Astra follow-up tested 17 more type and control-flow forms without
improving 11. In the retained `small-constant-local.c` variant, the pre-as1
`cc -S` listing places the constant-16 load at the shared fallback entry;
the final object moves it before the selector-17 branch. The observed
hoisting therefore happens during assembler optimization. This does not
establish an original assembler-version mismatch. Reproduce this narrow
stage observation with `tools/docker.sh python3 drafts/T-24011/stage.py`;
outputs stay under ignored `build/T-24011/`. The next useful hypothesis is
the source control-flow/label layout and constant availability before as1.

`func_80144E20`: use `g->` for every GameState field, and read the signed
header symbol `D_8015BF44` through a byte view. This fixes the shifted
register allocation and keeps frame `0xE38` plus all 21 table offsets exact.
The unsigned view corresponds to the original `lbu`. The initial `g` home
word retains the earlier draft's explicit FAKE provenance comment.
Both isolated and full-unit comparisons print 72 resolved added/removed
rows; the archived baseline gives 426 isolated rows under this counter.

The remaining large-dispatcher differences are the missing redundant case-0
assignment branch and case-20 branch, the resulting switch branch
displacements, symmetric comparison operand order, and a padding word.
Explicit empty default arms, explicit goto joins, opposite comparison
spelling, fade byte views, an `s32` no-return view, and an initial pointer
local do not improve 72. A pointer view of the selection global gives 88;
named selection locals give 176; a named fade value gives 154; signed
comparison casts give 284 or 292. Moving base initialization earlier gives
78. No compiler, build-pass, or per-function flag exceptions were added.

`func_8015A640` was not reprobed. Existing T-17208 evidence identifies this
416-byte helper and its two siblings as hand-written libgte assembly:
unsigned multiplication, scheduled loads and explicit multiply-result
padding differ from the typed IDO probe in 86 of 104 instruction rows.
See `wiki/gcc-objects.md` and `wiki/matching-notes.md`, T-17208. The original
assembly fallback remains appropriate; no compiler sweep was repeated.

No generated original assembly, executable data, or bulk disassembly is
included in this archive.
