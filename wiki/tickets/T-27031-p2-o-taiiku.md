---
id: T-27031
title: Phase 2 analysis - Sports festival
status: Done
assignee: luna
created: 2026-10-24
updated: 2026-10-25
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-taiiku analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [x] Evidence-backed proposed-name table at `config/renames/proposals/o-taiiku.tsv`; hypotheses explicit, O.BIN names retained.
- [x] Port spec at `wiki/spec/o-taiiku.md` describing state, rules, transitions, tables and open questions.
- [x] Independent standards and spec review resolved before Done.
- [x] No applied source renames or copyrighted game content.
## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Claimed for the first continuation batch.

2026-10-24 checkpoint: Luna retained 38 evidence-backed proposal rows after removing unsupported caller-only guesses: 15 functions, eight types, eight fields and seven data labels. Removed an out-of-unit comment request for main k_disp_start. Spec records concrete inspected rules and a 213-definition/19-file census. Runner completion/results and broad remaining function/data coverage stay open. No source renames; independent review remains pending; In Progress.

## Final integration evidence (2026-10-25)

Reviewed source digest: `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. Final clean matching proof: 27/27 binaries, 6,952 functions, 2,276,920 bytes. Independent original Astra review and separate fixer Astra review passed this exact source digest. See `.claude/orchestrator/state/T-27000/final-tracking/criterion-evidence.md`.

