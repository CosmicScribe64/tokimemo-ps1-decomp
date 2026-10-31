---
id: T-27032
title: Phase 2 analysis - Girl encounters
status: Done
assignee: sol-6.1
created: 2026-10-24
updated: 2026-10-25
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-geko analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [x] Evidence-backed proposed-name table at `config/renames/proposals/o-geko.tsv`; hypotheses explicit, O.BIN names retained.
- [x] Port spec at `wiki/spec/o-geko.md` describing state, rules, transitions, tables and open questions.
- [x] Independent standards and spec review resolved before Done.
- [x] No applied source renames or copyrighted game content.
## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Claimed for the first continuation batch.

2026-10-24: Added `config/renames/proposals/o-geko.tsv` and `spec/o-geko`. The proposal covers all 391 source definitions with technical role names, plus rule tables, script-section pointers, local state, header-owned types and essential fields/locals. The spec describes five mode entries, walk-home topics and choice effects, birthday branches, invitation eligibility/weighted selection/appointment packing, all queued girl/scene dispatch counts and key score/progress writes. The `girl_encounters` grouping remains HYPOTHESIS pending ADR-required emulator confirmation. Twenty-eight duplicate local callback typedef declarations are deferred comment proposals. Docker structural checks pass (five TSV columns, target existence, no duplicate old/new keys); no names applied and no byte-match claim. Independent review remains required before Done; orchestrator owns board/log/index/status transitions.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

