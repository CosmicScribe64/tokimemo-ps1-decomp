---
id: T-27037
title: Phase 2 analysis - Remaining small overlays
status: In Progress
assignee: luna
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-small analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [ ] Evidence-backed proposed-name table at `config/renames/proposals/o-small.tsv`; hypotheses explicit, O.BIN names retained.
- [ ] Port spec at `wiki/spec/o-small.md` describing state, rules, transitions, tables and open questions.
- [ ] Independent standards and spec review resolved before Done.
- [ ] No applied source renames or copyrighted game content.

## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Queued for the continuation.

2026-10-24: Initial partial seven-overlay flow/outcome pass drafted in `config/renames/proposals/o-small.tsv` and `wiki/spec/o-small.md`. The table and spec identify the actual mode-loader entries from `wiki/spec/m-flow.md`, body-backed stage/step dispatch, and several persistent result/return rules. KANGEI's summer-camp interpretation and VALEN's possible White Day scope remain explicitly unconfirmed per ADR. No source names applied; remaining scene tables and helper semantics are enumerated as unreviewed in the spec.

2026-10-24: Sol review fixes: DATE2's `girls[0]` relationship deltas are identified as Shiori's, VALEN's candidate builder is `func_80132834` (while `func_80132760` dispatches callbacks), KANGEI 0x5F/0x60 entry behavior is separated (stage dispatch vs scene callback table), and OMIMAI's 0x61/0x62 dispatch paths are documented independently. Corrected source-body census in `wiki/spec/o-bunkasd-shugaku.md` to 98 BUNKA_SD + 182 SHUGAKU = 280. Ticket remains In Progress.

2026-10-24 independent narrow Sol6.1 audit: corrected all reported factual findings; final verification found none unresolved after root clarified Rei flag-byte indexing. Body-backed partial checkpoint only; broader coverage and independent full-unit review remain open. No game identifiers applied.
