---
id: T-24522
title: Match remaining RPG_BAT functions
status: In Progress
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
