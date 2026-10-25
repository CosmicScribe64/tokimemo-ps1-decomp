---
id: T-27036
title: Phase 2 analysis - Stage shows and school trip
status: Done
assignee: luna
created: 2026-10-24
updated: 2026-10-25
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-bunkasd-shugaku analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [x] Evidence-backed proposed-name table at `config/renames/proposals/o-bunkasd-shugaku.tsv`; hypotheses explicit, O.BIN names retained.
- [x] Port spec at `wiki/spec/o-bunkasd-shugaku.md` describing state, rules, transitions, tables and open questions.
- [x] Independent standards and spec review resolved before Done.
- [x] No applied source renames or copyrighted game content.
## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Queued for the continuation.

2026-10-24: Initial body-backed gameplay/state pass drafted in `config/renames/proposals/o-bunkasd-shugaku.tsv` and `wiki/spec/o-bunkasd-shugaku.md`. Scope is the stage-show dispatcher/script interpreter plus SHUGAKU route/scene selection and explicit status/ending deltas; no names applied. Function bodies prove a mode-stage/step script, a BUNKA_SD numeric selector with five script/girl arms, SHUGAKU callback dispatch keyed by mode_step, and trip records that adjust affection/tokimeki/syoushin. Narrative meaning of BUNKA_SD selector arms and SHUGAKU's saved-stage special branches remains unproven. Function census / unresolved bodies are recorded in the spec; ticket remains In Progress for review and remaining scene/table census.

2026-10-24 independent narrow Sol6.1 audit: corrected all reported factual findings; final verification found none unresolved after root clarified Rei flag-byte indexing. Body-backed partial checkpoint only; broader coverage and independent full-unit review remain open. No game identifiers applied.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

