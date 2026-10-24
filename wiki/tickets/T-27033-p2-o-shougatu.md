---
id: T-27033
title: Phase 2 analysis - New Year and Christmas
status: In Progress
assignee: luna
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-shougatu analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [ ] Evidence-backed proposed-name table at `config/renames/proposals/o-shougatu.tsv`; hypotheses explicit, O.BIN names retained.
- [ ] Port spec at `wiki/spec/o-shougatu.md` describing state, rules, transitions, tables and open questions.
- [ ] Independent standards and spec review resolved before Done.
- [ ] No applied source renames or copyrighted game content.

## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Claimed for the first continuation batch.

2026-10-24 continuation: Reworked proposals after reading the source definitions. Current primary-flow slice: 9 SHOUGATU and 6 GYOZI function proposals; source census is 400/27 and 417/33 respectively. Spec now records the met-girl rank/tie-break gates, branch score changes, omikuji/year sequence behavior and Dec 24 birthday mode redirects. External loader/date eligibility rules, most subscenes/helpers, runtime confirmation and exhaustive table-consumer coverage remain open; ticket remains In Progress pending review.

2026-10-24 review corrections: Independent source review caught the GYOZI rank-tie direction; `func_801347D4` selects the larger score sum. Corrected the zero-flag score branch in both overlays to apply tokimeki +1 only when the selected target is index 0 (Shiori). `func_801359FC` now records selector mapping and stress -5/+5 effects followed by `check_para_limit`. Rechecked against `src/ovl/GYOZI/80134520.c`, `src/ovl/GYOZI/80134C10.c`, `src/ovl/SHOUGATU/80132D00.c`, and `src/ovl/SHOUGATU/80134890.c`.

2026-10-24 independent narrow Sol 6.1 audit: corrected tie-break direction, Shiori-only tokimeki condition and proven omikuji stress changes. Reviewer verified TSV corrections; root removed the last contradictory unresolved-stress sentence as requested. This is a reviewed partial checkpoint (15 function proposals), not full coverage of 817 definitions or runtime confirmation.
