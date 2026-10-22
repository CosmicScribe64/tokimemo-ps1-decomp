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
