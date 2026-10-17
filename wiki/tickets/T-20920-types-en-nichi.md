---
id: T-20920
title: "Targeted type pass: EN_NICHI"
status: In Progress
assignee: claude-opus (ty-en-nichi)
created: 2026-10-17
updated: 2026-10-17
links: ["[[data-types]]", "[[build-step-gaps]]", "[[speedup-research-2026-10-16]]", "[[en-nichi-family-results]]"]
---

## Goal

A second, targeted type pass for EN_NICHI after the types-first pass of T-18200. Find the shared structures
that the 14 unmatched functions use (work structs, records, callback tables, stack tables, bit-fields) from the
asm access patterns and the matched siblings, declare them in `include/ovl/EN_NICHI.h` (`include/game.h`
additions only), move matched code onto them byte-identically, and redraft the unmatched functions with
`tools/redraft.py` into `drafts/T-20920/`.

## Acceptance criteria

- [ ] New types declared with offsets and sizes (CODING_STANDARDS section 8); matched code uses them; every
      step builds 27/27.
- [ ] Unmatched functions redrafted with `tools/redraft.py`, `drafts/T-20920/<UNIT>/<func>/result.json` with
      `scope`, `score`, `census_score`; paired score changes reported.
- [ ] Types documented in [[data-types]] and [[en-nichi-family-results]].
- [ ] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.
- [ ] Off limits respected: no edits in `src/ovl/TT/*` or main `dialogue_render` (`src/main/8007C030.c`).

## Notes

Branch `ty-en-nichi` from `w2-good`.

## Comments
