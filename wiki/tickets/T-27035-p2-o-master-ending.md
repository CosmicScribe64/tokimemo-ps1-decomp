---
id: T-27035
title: Phase 2 analysis - Club mastery and endings
status: In Progress
assignee: luna
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-master-ending analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [ ] Evidence-backed proposed-name table at `config/renames/proposals/o-master-ending.tsv`; hypotheses explicit, O.BIN names retained.
- [ ] Port spec at `wiki/spec/o-master-ending.md` describing state, rules, transitions, tables and open questions.
- [ ] Independent standards and spec review resolved before Done.
- [ ] No applied source renames or copyrighted game content.

## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Queued for the continuation.

2026-10-24: Drafted bounded analysis in `config/renames/proposals/o-master-ending.tsv` (10 proposed function rows) and `wiki/spec/o-master-ending.md`. Read source bodies for dispatch, MASTER tournament selection/result exit/logging, and ENDING candidate selection/rhythm scoring. Branch directions, strict score comparisons and tie behavior were checked against source. Uncertain scene/flag semantics are explicitly left open; no rename applied.

2026-10-24 independent factual review corrections: moved the practice-points margin attribution from `func_801391E4` to `func_80139CC8` step 6 (the former only calls `normal_date_speak` at step 6); changed index 11 from “boy” to Miharu and renamed `func_801391E4` as candidate selection because its selector feeds voice/portrait/current-girl setup, not a proven tournament winner; renamed `func_80137730` to `ending_audio_level_update` and removed unsupported rhythm/performance interpretation; corrected `func_80133460` flag extraction to `flags.w` bits 9..11. Source bodies checked at `src/ovl/MASTER/80139000.c` and `src/ovl/ENDING/80133030.c`, `80137340.c`.

2026-10-24 independent narrow Sol6.1 audit: corrected all reported factual findings; final verification found none unresolved after root clarified Rei flag-byte indexing. Body-backed partial checkpoint only; broader coverage and independent full-unit review remain open. No game identifiers applied.
