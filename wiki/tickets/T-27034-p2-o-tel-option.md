---
id: T-27034
title: Phase 2 analysis - Telephone and options
status: In Progress
assignee: luna
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-tel-option analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [ ] Evidence-backed proposed-name table at `config/renames/proposals/o-tel-option.tsv`; hypotheses explicit, O.BIN names retained.
- [ ] Port spec at `wiki/spec/o-tel-option.md` describing state, rules, transitions, tables and open questions.
- [ ] Independent standards and spec review resolved before Done.
- [ ] No applied source renames or copyrighted game content.

## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Queued for the continuation.

2026-10-24 continuation: Drafted source-based TEL/OPTION proposal and port spec. The pass covers phone call dispatch and reply gates, next-28-day/date-place eligibility, invitation chance and appointment write, evaluation/profile views, options stage dispatch and OPTION stage-7 sound-test steps 21–23. Fifteen function rows are proposed. Full per-step/per-page coverage, loader routes, final table labels, runtime checks and independent review remain open; ticket remains In Progress.

2026-10-24 review corrections: Verified invitation stat changes occur after the success/failure branch, so affection `frame_count & 3` and syoushin -12/clamp apply on both outcomes; the signed failure delta is always <0x66, so the source always chooses substep 0x40. Corrected sound-test start cross to route forward by two steps, and documented that advance handler checks cross only (no circle check). Rechecked `src/ovl/TEL/80132000.c` and `src/ovl/OPTION/80132000.c`.

2026-10-24 independent narrow Sol 6.1 audit verified corrections: relationship changes occur on both invitation outcomes; signed failure routing selects substep0x40; sound-test input is Cross only and advances to exit handling. Fifteen primary-flow function proposals are a partial checkpoint, not a completed unit or applied names.
