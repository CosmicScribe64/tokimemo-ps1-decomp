---
id: T-20910
title: "Targeted type pass: OPTION"
status: In Progress
assignee: claude-opus (ty-option)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]", "[[option-family-results]]"]
---

## Goal

A second, targeted type pass for OPTION, one of the least-matched overlays (32 functions unmatched after the
T-17800 types-first pass, 2 of them guarded). Find the shared structures the remaining functions use (work
structs, menu and option records, callback tables, stack tables, bit-fields) from the asm access patterns and
the matched siblings, declare them in `include/ovl/OPTION.h` (`include/game.h`: additions only), convert matched
OPTION code to them byte-identically, and redraft the unmatched functions with `tools/redraft.py`, saving the
drafts under `drafts/T-20910/` with `result.json` (`scope`, `score`, `census_score`). Report the paired score
changes. Scope: `src/ovl/OPTION/*.c`, `include/ovl/OPTION.h`, additions to `include/game.h`.

## Acceptance criteria

- [ ] New OPTION types declared with offsets and sizes; matched code converted where it stays byte-identical.
- [ ] Every matched function byte-identical (clean build 27/27); `check_headers`, `migrate_globals --check`,
      `sync_protos --check-branch` pass.
- [ ] Unmatched OPTION functions redrafted into `drafts/T-20910/`; paired score changes recorded.
- [ ] [[data-types]] and [[option-family-results]] document the types.
- [ ] Code review against `CODING_STANDARDS.md` (before Done).

## Notes

- Not touched: `src/ovl/TT/*` (issue #2), main `dialogue_render` in `src/main/8007C030.c` (issue #45).

## Comments
