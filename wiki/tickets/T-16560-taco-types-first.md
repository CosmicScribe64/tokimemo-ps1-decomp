---
id: T-16560
title: "Types-first pilot: TACO"
status: In Progress
assignee: claude (pt-taco)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[taco-family-results]]", "[[data-types]]"]
---

## Goal

Recover the TACO overlay's shared data types (structs, arrays, stride tables, bit-field views, callback tables) in
`include/ovl/TACO.h` before function matching, then redraft every unmatched TACO function with the new headers and
measure how the drafts change. Pilot of the types-first approach from [[speedup-research-2026-10-16]] (in
BUNKAKEN/BUNKASAI fresh m2c drafts matched 67% of the time once shared types were in headers, against 1.6% elsewhere).

## Acceptance criteria

- [ ] Survey: type_recovery, siblings, census drafts, wiki notes for TACO
- [ ] Types declared in `include/ovl/TACO.h`, globals given aggregate types; every matched function still matches
- [ ] Fresh draft for every unmatched TACO function under `drafts/T-16560/TACO/<func>/`, scored like the census
- [ ] Four file-disjoint work packages (T-16561..T-16564) for the matching stage
- [ ] Family page section "Types-first pass (T-16560)"; clean build 27/27, header, globals and prototype checks OK

## Notes

Worktree `pt-taco` (branch `pt-taco`, from `w2-good`). Owns `src/ovl/TACO/` and `include/ovl/TACO.h`; additions only
in `include/game.h` and `include/main_api.h`. New drafts only under `drafts/T-16560/`.

## Comments
