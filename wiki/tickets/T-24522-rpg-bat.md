---
id: T-24522
title: Match remaining RPG_BAT functions
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[families/rpg-bat-family-results]]"]
---

## Goal

Investigate the six RPG_BAT assembly functions from baseline dba33c4f5 in an isolated worktree. Count only literal default C with linked proof.

## Acceptance criteria

- New hypotheses informed by previous negative results.
- Any matches pass literal linked proof, 27 binary SHA1 checks, header/global/prototype guards and standards review.
- Preserve useful unresolved C separately without claiming coverage.

## Notes

Assigned lane: func_801447E0, func_80144C84, func_8014BB80, func_8014BFE4, func_80154DD8, func_8015528C. Docker only, existing arm64 image, compiler jobs 1. Prior T-17113 and T-24027 negatives read before probes.

## Comments

### 2026-10-20 func_8014BB80 matched

Typed `GameState *gs` reads for both menu-choice switches split their promoted range from the plain `D_800E6280.unk_1093` condition read. Removing the historic unused `pad` lets the real pointer occupy its frame home. The volatile read and both fake comments are removed. Existing file-wide implicit-int callee view is retained; no new override or FAKE is needed.

Current full-unit scorer: inherited volatile baseline 43 literal rows/43 aligned (historic T-24301 22 is an older comparator result); pointer switches 22/22 consisting solely of frame offsets; pointer switches without historic pad 0/0. Whole-aggregate volatile initial switch reached 21/21 but retained an added address instruction; byte bit-fields did not help. No broad compiler grid.

Verification: Docker `ninja -j1 build/ovl/RPG_BAT.ok` passes. Docker `funcdiff.py --resolve --version default --no-build RPG_BAT:func_8014BB80` reports MATCH, linked function bytes equal immutable target, 1124 bytes. Actual default C has no NON_MATCHING guard or INCLUDE_ASM fallback. RPG_BAT SHA1 142d13e317aa7ae6def6a300085c38f645c19db6. 27/27 SHA1 passed using private RPG_BAT plus 26 unchanged read-only root build outputs; this is not a claim of rebuilding all 27 binaries. `check_headers` passes. Whole-repo migration check requires unrelated overlay reference asm: rerun using root generated asm read-only.

Standards self-review: C89 declarations at block head, fixed-width project types and existing GameState aggregate; no asm/header/global edits or added fake idioms, no game data staged. Coordinator independently reviewed source against CODING_STANDARDS.md and requested only indentation repair, applied before proof. Ticket remains In Progress for the remaining five functions.

Final declaration guards passed: headers OK; migrate_globals --check globals OK with unchanged generated reference asm read-only; sync_protos --check-branch 0 new disagreements, 15 known, branch check OK.

### 2026-10-20 func_80144C84 source reconstruction

Current scorer: archived T-17113 baseline252/224 aligned. Write the damage result directly through D_8015EBB0:138/134. Read/write unk_00 through a typed RpgStat pointer:92/88; replacing missing frame home:82/78. Staged D_8015EBB0 multiplication and divisions recover original v1 intermediate ranges:44/44. Multiplication operand spellings close4 rows; multiply-by-one wrappers on the five sum operands close10:30/30. These wrappers and the extra frame word would require FAKE documentation if promoted. No production edit yet.

Remaining30 rows come from third-arm unk_08/EBE0 store/reload scheduling plus branch distances and one earlier store order. Additional signed casts do not change code. Reusing the roll local, pointer accesses for all stats, explicit field writebacks, alternate clamp-local form and threshold/clamp pointer views did not close this; no coverage claimed.

### 2026-10-20 func_801447E0 matched

The same staged-result reconstruction solved the other battle calculator: put the random result directly in D_8015EBB0, then explicitly multiply/divide that global in sequence. This lets uopt promote the original shared v1 range and places the two flag webs in a0/a1. The typed source scores154/148 baseline,146/146 staged local,8/8 staged global. Declare actual locals `k` before `lim` to place the limit spill at0x38:0/0. No fake home, override, or compiler modification.

Docker `ninja -j1 build/ovl/RPG_BAT.ok` passes and default `funcdiff.py --resolve --version default --no-build RPG_BAT:func_801447E0` reports MATCH,924 linked bytes equal immutable target. The literal production C replaces INCLUDE_ASM; it has no fallback/define. Parent source review against CODING_STANDARDS.md found no blockers: C89, real locals, canonical GameState aggregate, no new FAKE/header/asm/game-data changes. Worker self-review agrees. Guard and27-hash evidence follows.

Final second-match checks: check_headers OK; migrate_globals --check globals OK; sync_protos --check-branch0 new/15 known, branch check OK.27/27 SHA1 verified with private RPG_BAT142d13e317aa7ae6def6a300085c38f645c19db6 and26 unchanged root outputs mounted read-only. Source standards self-review and independent coordinator review pass.

### 2026-10-20 bounded remaining-lane outcome

func_80144C84 reaches28/28 after matched sibling's comma-return spelling; typed threshold/clamp/trailing-record pointers and switch/while/else forms did not solve the remaining third-arm reload/store gap. Annotated source-only lead saved at drafts/T-24522/RPG_BAT/func_80144C84/body.c; no production edit. Its frame home, operand wrappers and comma return are explicitly FAKE.

func_8014BFE4 current baseline128/128; meaningful actor-base pointer207/201, addressed-argument form185/185, combined247/247. These are negative results; no production change. Recent T-24027 DD8/5528C drafts and negatives were read; no new hypothesis justified rerunning their broad gaps. Source archive README documents the lane and retains every loose probe under ignored build/T-24522. Two exact matches total2,048 bytes; four remain assembly.

Final archive compilation confirmed28 literal rows /28 aligned after documentation and formatting. Work is In Review for the complete lane archive; both production functions already have independent coordinator source review and complete verification.

### 2026-10-20 archive review and read-only residual analysis

Coordinator reviewed source archive1564d1bf1 against CODING_STANDARDS.md: no blockers. The source-only28-row body explicitly denies production coverage, fake home/wrappers/comma-return are documented, and no instruction listings/game assets or production modifications are included.

Read-only C84 dependency map from saved28-row diagnostics and immutable target assembly:

- First divergence: the EBE0 load at target80144D9C precedes the initial counter writebacks; the source draft moves that identical load later. The typed first-stat pointer plausibly creates a different alias dependency for this load, but that interpretation is unproven.
- Target third threshold80144E34 loads extra weight(a3) and field08(a1), writes fields04/00 back, and tests their sum against roll.
- Target enemy-kind branch80144E80 fills its delay slot with field08's address preparation. On the noncall successor80144EA8 it reloads field08 and EBE0, decrements/stores field08, and schedules extra-minus20 in the field08-sign-branch delay slot.
- Current draft carries a1/a3 forward from the threshold. It speculates extra-minus20 into the enemy-kind branch delay slot and stores field08 on the call path to free a1 for the argument. Thus two target reloads disappear, an extra call-path store appears, and downstream branch distances change.

One precise untested source hypothesis: represent only the third threshold's extra/field08 values as an unescaped two-word local aggregate snapshot, then keep actual clamp mutations through plain globals. This could separate the threshold ranges from mutation ranges while replacing existing scalar homes; it might still be folded away by GVN. Ordinary field-pointer and branch-spelling variants were already tested and are not this hypothesis. No compiler was run for this read-only analysis, and no compiler limitation is claimed.

### 2026-10-20 bounded aggregate-snapshot result

Coordinator explicitly authorized the new two-word snapshot hypothesis and at most a few meaningful variants. Compiled three at j1: threshold-only capture; phase-boundary refreshes for bound and threshold; reversed member/assignment order. Every result34 literal/34 aligned. Diff equals the prior28 rows plus6 limit-spill offsets(0x2C->0x28); no separate threshold snapshot instruction survives. Optimizer folding therefore defeats this particular attempt to separate threshold and mutation ranges. Best source-only body remains28/28, production unchanged. Parent archive review and residual dependency graph remain the authoritative checkpoint; no more compiler probes planned without a new structural idea.
