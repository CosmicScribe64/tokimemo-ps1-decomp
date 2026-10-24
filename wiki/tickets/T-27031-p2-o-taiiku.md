---
id: T-27031
title: Phase 2 analysis - Sports festival
status: In Progress
assignee: luna
created: 2026-10-24
updated: 2026-10-24
links: ["[[tickets/T-27000-phase-2]]", "[[phase2]]"]
---

## Goal

Complete the o-taiiku analysis unit from [[phase2]] for the Godot port.

## Acceptance criteria

- [ ] Evidence-backed proposed-name table at `config/renames/proposals/o-taiiku.tsv`; hypotheses explicit, O.BIN names retained.
- [ ] Port spec at `wiki/spec/o-taiiku.md` describing state, rules, transitions, tables and open questions.
- [ ] Independent standards and spec review resolved before Done.
- [ ] No applied source renames or copyrighted game content.

## Notes

Model assignment follows the user request: Luna by default, Sol 6.1 for harder analysis/review. Orchestrator owns shared board, indexes, log and commits.

## Comments

2026-10-24: Claimed for the first continuation batch.

2026-10-24 checkpoint: Luna retained 38 evidence-backed proposal rows after removing unsupported caller-only guesses: 15 functions, eight types, eight fields and seven data labels. Removed an out-of-unit comment request for main k_disp_start. Spec records concrete inspected rules and a 213-definition/19-file census. Runner completion/results and broad remaining function/data coverage stay open. No source renames; independent review remains pending; In Progress.
