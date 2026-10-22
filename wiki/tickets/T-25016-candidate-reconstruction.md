---
id: T-25016
title: "Candidate reconstruction for the fragment functions: extractor v2, canonical census, then per-function matching"
status: In Progress
assignee: octa
created: 2026-10-21
updated: 2026-10-21
links: [[[status]], [[matching-notes]], [[build-step-gaps]], [[tickets/T-25015-lever-expansion-tooling]], [[tickets/T-25014-allocator-source-shape-automation]]]
---

## Goal

T-25015's census showed the remaining wall is candidate quality: of
96 INCLUDE_ASM functions in src, only 9 have a machine-usable
candidate; the other 87 keep their work as fragments in drafts/ or in
guard forms the extractor cannot read yet. With references for all
96 now building in the remote scratch (mirrored asm bundle), the work
to actual matches is:

1. Extractor v2: assemble candidates from drafts/ fragments (wrap
   body fragments in the src signature), score every variant, keep
   the best per function.
2. Register-canonicalized census scoring, so register-only near
   misses rank by true distance (the packet metric overstates them).
3. Re-census all 96, rank, and work the list top-down with the shape
   engine plus hand reconstruction; every function that reaches zero
   locally is prepared for the CI gate on a PR.

## Acceptance criteria

- [ ] Extractor v2 + canonical scoring landed; census covers the
      large majority of the 96 with a scored candidate.
- [ ] A ranked work list exists in the sweep ledger (baseline scores
      recorded per function).
- [ ] Each function attempted is recorded (method, budget, best) in
      the ledger and, on success, in matching-notes with a PR for CI.

## Notes

- Branch octa/t25016-reconstruction, stacked on octa/t25015-tooling
  (PR #66 carries the T-25013/14/15 stack into main).
- Assembly of the reconstruction happens in the remote scratch;
  drafts produced here are filed under drafts/T-25016/ per project
  convention before any src change is proposed.

## Comments

### Census v2 ranked list (2026-10-21, canonical rows)

Extractor v2 censused 24 of 96 functions (71 fragment candidates
failed to compile when wrapped; 1 had no candidate). Ranked by
register-canonicalized rows:

- func_801511C0 canon=4 raw=4 instruction (drafts/T-14500/TACO/func_801511C0/context.c)
- func_80133024 canon=8 raw=22 addressing (drafts/T-17700/GEKO/func_80133024/hand.c)
- func_8013B460 canon=8 raw=33 register-only (drafts/T-19030/TACO/func_8013B460/body.c)
- func_8013907C canon=9 raw=163 instruction (drafts/T-24019/GYOZI/func_8013907C/body.c)
- func_80140E0C canon=12 raw=14 addressing (drafts/T-24907/v6.c)
- func_80136C20 canon=15 raw=15 addressing (drafts/T-17700/GEKO/func_80136C20/context.c)
- func_8013852C canon=16 raw=17 addressing (drafts/T-24054/body.c)
- func_8013AF78 canon=17 raw=105 instruction (drafts/T-22023/NAME_ENT/func_8013AF78/body.c)
- func_80132B24 canon=18 raw=41 register-only (drafts/T-24503/GEKO/func_80132B24/body.c)
- func_80133C50 canon=20 raw=38 addressing (drafts/T-24503/GEKO/func_80133C50/body.c)
- func_80139290 canon=20 raw=51 register-only (drafts/T-24516/bunka-39290.c)
- func_80134724 canon=24 raw=113 register-only (drafts/T-22016/KANGEI/func_80134724.c)
- func_80047560 canon=30 raw=58 instruction (drafts/T-23006/main/func_80047560/body.c)
- func_80134D94 canon=32 raw=90 instruction (drafts/T-24516/bunka-34d94.c)
- func_8013E50C canon=33 raw=89 instruction (drafts/T-24523/SHOUGATU/func_8013E50C/body.c)
- func_80146D4C canon=41 raw=75 instruction (drafts/T-24303/ETC/func_80146D4C/body.c)
- func_80138C1C canon=41 raw=83 instruction (drafts/T-24515/func_80138C1C.c)
- func_801398EC canon=42 raw=110 instruction (drafts/T-24515/func_801398EC.c)
- func_8005B43C canon=55 raw=75 addressing (drafts/T-24301/main/func_8005B43C.c)
- func_8006C988 canon=57 raw=84 instruction (drafts/T-17000/main/func_8006C988/context.c)
- func_80075C84 canon=59 raw=111 instruction (drafts/T-24303/main/func_80075C84/body.c)
- func_8014AB50 canon=104 raw=236 instruction (drafts/T-16560/_runs/typed/TACO/func_8014AB50/body.c)
- func_800455C4 canon=140 raw=261 instruction (drafts/T-24041/func_800455C4.c)
- func_80135F4C canon=343 raw=343 addressing (drafts/T-24047/body.c)

### Engine pass complete (2026-10-21 evening)

The shape engine has now run over all 24 censused functions (budget
400 each, results in the sweep ledger). No function reached local
zero. The engine's wins were in raw-score reduction on the
register-heavy functions: func_8013907C 163 -> 15, func_80134724
113 -> 28, func_800455C4 261 -> 171, func_801398EC 110 -> 86,
func_80047560 58 -> 37. The nearest function overall remains
func_801511C0, plateaued at 4 rows (a single small instruction hunk
the transforms cannot move). Everything else sits at 14 or above.

What remains for actual matches is hand reconstruction on the near
set (func_801511C0 first), using the observatory to read the first
divergence per function. The engine pass and census are complete;
the ticket stays In Progress for that hand phase.

### Hand phase (2026-10-21 night)

Two measurement corrections first:

1. Census v2 scored several functions from stale drafts/ files. The
   src tree carries newer NON_MATCHING bodies for 13 of the 24, and
   several rank far better from the in-tree body (func_8014AB50
   canonical 104 -> 6, func_80134724 packet raw 113 -> 20,
   func_80139290 51 -> 17). Future census runs should prefer the
   in-tree body over drafts/ fragments.
2. The packet metric counts symbolization differences as diffs
   (a member access and a standalone symbol at the same address
   produce different normalized text). funcdiff's after-relocation
   section resolves those away and is the local ground truth. By
   it, the near set is much closer than the packet numbers said.

funcdiff resolved rows for the best current body of each of the 24:
func_8013852C 2, func_80136C20 2, func_80134724 4, func_801511C0 5,
func_80140E0C 6, func_80139290 7, func_8014AB50 7, func_80133024 20,
func_80135F4C 21, func_80133C50 22, func_8005B43C 33,
func_80132B24 40, func_8013B460 41, func_80146D4C 52,
func_80138C1C 54, func_8013AF78 63, func_80047560 72,
func_80134D94 78, func_8013E50C 79, func_8006C988 79,
func_801398EC 89, func_8013907C 136, func_80075C84 169,
func_800455C4 254. No function reached local zero.

Every residue in the top group is a local ordering choice by as1
on an otherwise identical instruction stream: a hoisted address
load (func_8013852C, func_80139290), the delay-slot candidate pick
of T-25011 (func_80134724), a two-instruction prologue placement
(func_801511C0), the zero fold documented in the src comment
(func_80140E0C), and the v0/v1 constant allocation (func_8014AB50).

Hand attempts this session:

- func_801511C0: statement orders, all 120 declaration
  permutations, local-copy and index-temp forms, loop forms,
  empty-if variants, no-base forms. All plateau at the same hunk.
- func_8013852C: standalone-symbol rewrite takes the packet score
  17 -> 2 but the resolved rows stay 2; statement splits,
  expression variants, and the 5.3mix / 7.1 compiler variants do
  not flip the remaining swap.
- func_80134724: default label first (no change), merged
  case 5/6 (worse).
- func_8014AB50: constant-form variants at the four `= 1` sites
  (no change) and a 302-trial engine run on the in-tree body
  (no improvement).

The ticket stays In Progress. For the top group the remaining
lever is as1 scheduling inputs (block weights and region
structure), not source shape.

### Phase 2 + resolution plan (2026-10-22, Octa)

Phase 1 (as1 block weights) closed with no flip: the traced
weights for func_80134724 confirm the model in
[[as1-scheduler-rule]] (head 1, default 4, dispatch 4), and the
default block's predecessor structure cannot change because no
switch arm shares the default arm's body. func_80136C20's
delay-slot residue survives the standalone-symbol form and both
compiler variants.

Phase 2 bulk reconstruction over the 95 INCLUDE_ASM functions
census v2 never scored (scratch pipeline, m2c with a per-file
preprocessed context, drafts stocked as beds for all 95):

- 2 scored: func_80139FB8 canon 69, func_8015528C canon 75.
- 66 compile failures, by first error: 21 syntax errors and 13
  Shift-JIS encoding errors (pipeline bugs in draft handling,
  not draft quality), 11 selector-on-non-struct type mismatches,
  about 8 undefined globals the bed header does not declare,
  the rest scattered.
- 16 produced no m2c draft; 11 references failed to build from
  the stub TU.

Resolution plan by common issue, in the order being executed:

1. Encoding and syntax class (about 34): normalize drafts
   (escape non-ASCII in string literals, drop it in comments,
   drop m2c uncertainty lines) and recompile. Patch run in
   flight; this is a pipeline fix and should unlock most of
   the class.
2. Undefined globals (about 8): synthesize bed-level extern
   declarations from the repo's own declarations for measuring;
   the in-tree fix at promotion time is the existing keep-line
   mechanism. Also part of the patch run.
3. Reference failures (11): build the reference by compiling
   the whole src file in the scratch tree and extracting the
   function. Also part of the patch run.
4. Selector type mismatches (11): rewrite the access using the
   form the matched corpus uses for the same address
   (precedent index), or pointer arithmetic with the same
   offsets where the corpus treats the value as bytes.
5. No m2c draft (16): retry without context (done in the patch
   run where the context was the failure), else hand
   reconstruction; several are string-heavy and tied to class 1.
6. Near set (2 to 7 resolved rows, as1 ordering on identical
   streams): the untested levers are (a) case-label permutation
   to renumber the tied blocks, checked against the trace
   before funcdiff; (b) naming the mover's tie-break variable
   from the pinned as1 source, then designing the source form
   that sets it; (c) for the prologue case, a form that makes
   one of the two instructions depend on the other; for the
   constant case, a form whose first use of the constant is
   the compare.
7. Mid band (20 to 89 rows): iterate on the first funcdiff
   divergence only; later rows are usually cascade from the
   first. The precedent index supplies the matched form for
   the diverging construct.

Endgame for any local zero: stage under drafts/T-25016/, run
the full local gate (clean build, check_headers,
migrate_globals --check, sync_protos --check-branch), then one
batched PR so CI gives the formal verdicts together.
