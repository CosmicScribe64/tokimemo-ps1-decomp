---
id: T-16900
title: "Compiler experiments: selector-copy rule and ugen survey"
status: In Progress
assignee: claude-opus (blissful-fermi-43d29a)
created: 2026-10-15
updated: 2026-10-15
links: [[[matching-breakthroughs]], [[uopt-register-allocation]], [[toolchain]], [[tickets/T-11570-w1-ovl-name_ent-80139740]], tools/cvt_pass.py, tools/regdiff.py]
---

## Goal

Two time-boxed experiments that decide whether more compiler work pays for the remaining matching:

1. The signed narrow selector-copy rule from [[tickets/T-11570-w1-ovl-name_ent-80139740]] (item 4 of
   [[matching-breakthroughs]]): does a uniform change to `tools/cvt_pass.py` (widening after narrow signed global loads,
   preserving a local copy of a global, or both) match the 7 NAME_ENT selector-copy drafts without changing a matched
   function?
2. A regdiff survey of the drafts 21 to 100 diff lines from matching (`drafts/T-14500/best-quality-catalog.jsonl`, no TT):
   which pass (uopt or ugen) makes the first register difference? Decides whether a 3-day ugen trace is worth doing.

No `src/` file changes on this branch. The scratch copies of the pass and the frozen drafts stay outside the repository.

## Acceptance criteria

- [ ] Experiment 1: the 7 targets and the matched controls frozen; widening, copy preservation and both tested separately.
- [ ] Experiment 1: any variant that helps gets the full matched regression (clean rebuild, 27/27 sha1, zero changed
      matched functions). Success: at least 3 of 7 targets match with 0 matched functions changed; otherwise a
      negative result.
- [ ] Experiment 2: regdiff over the 21-100 drafts; share of first register differences in ugen temporaries ($t6-$t9,
      $at). Verdict: ugen trace if 25% or more.
- [ ] Results in this ticket and in [[uopt-register-allocation]]; log entry.

## Notes

## Comments
