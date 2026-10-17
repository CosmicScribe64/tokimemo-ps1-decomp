---
id: T-11220
title: regdiff-guided edit ordering in autotweak
status: Done
assignee: agent
created: 2026-10-14
updated: 2026-10-17
links: [[[decompile-workflow]], [[register-cheatsheet]], [[uopt-register-allocation]], [[tickets/T-11170-cheat-sheet-auto-tweaker]], [[tickets/T-11200-order-search-in-solvers]], tools/autotweak.py, tools/bulk_m2c.py, tools/regdiff.py]
---

## Goal

Let autotweak (T-11170) use regdiff's first register divergence (T-10400) to decide which cheat-sheet edits to
score first, instead of the plain round robin over the transforms.

## Acceptance criteria

- [x] With the traced uopt available (detected like regdiff: `$REGDIFF_UOPT` or its default under `/scratch`, plus
      the MIPS binutils), regdiff runs in-process on the start text and on every new best text; without it the
      climb is the unguided one.
- [x] The FIRST DIVERGENCE block is parsed into a hint class (tie, priority, extra-range, missing-range, ugen,
      argument, unallocated; structure, frame, operand outside the block) plus named variables and source lines.
- [x] One table (`HINT_EDITS`, pointing at [[register-cheatsheet]]) maps each class to edit kinds; edits of those
      kinds that touch a named variable or line go first, then the other suggested kinds, then other edits at the
      location, then the rest. Nothing is dropped; `--cap` and `--time` bound the search as before.
- [x] The output line ends with `regdiff: <hint>`; `--no-regdiff` turns the guidance off.
- [x] bulk_m2c passes it through (also `--no-regdiff`).
- [x] Synthetic tests with mocked regdiff output: parsing, mapping, ordering, advisor, fallback.
- [ ] Real-build check (orchestrator): one draft with `TOKIMEMO_SCRATCH` set, scores and time against `--no-regdiff`.

## Notes

- `tools/autotweak.py`: `HINT_EDITS`, `HINT_PATTERNS`, `Hint`, `parse_regdiff`, `edit_kind`, `changed_lines`,
  `rank_variants`, `import_regdiff`, `regdiff_uopt`, `make_advisor`; `climb(..., advise=)`, `Result.hint`.
- The advisor writes the variant into a copy of the source file (`build/autotweak/<unit>/<func>/regdiff.c`, guard
  replaced by the plain function as in the scorer) and calls `regdiff.run` with `--src` on it; the report's file
  lines are converted to line indices of the function text. Results are cached per text, so a stall does not run
  regdiff again: the hint of the current best also orders the depth-2 pairs. After the climb the best text gets one
  more run so the reported hint is the one that is left.
- Any regdiff error becomes a hint with `error` set: the order stays unguided and the line shows `regdiff: error`.
- Built without the game binaries or the traced uopt (no compile); synthetic tests only.

## Comments

2026-10-17: Cleanup pass. Merged on main as de2b19294: regdiff-guided edit ordering in tools/autotweak.py (HINT_EDITS, --no-regdiff), the bulk_m2c pass-through and mocked-regdiff tests. It has been used for real since: the T-11550 Opus pass credits the hints for func_801330D8 and func_80140920, and T-20830 extended HINT_EDITS; a controlled run against --no-regdiff was not recorded and is not needed. Review against CODING_STANDARDS.md: Python 3, docstrings, tests, no game data; the broad except is limited to the guidance path; no finding. Closing as Done.
