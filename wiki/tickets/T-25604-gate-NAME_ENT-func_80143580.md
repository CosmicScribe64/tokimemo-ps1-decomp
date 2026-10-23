---
id: T-25604
title: Finish gate - NAME_ENT func_80143580
status: Ready
assignee: finish gate
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25600-finish-gate]], [[playbook]]]
---

## Goal

Match NAME_ENT:func_80143580 (1098 words, `src/ovl/NAME_ENT/80142A80.c`) through the finish gate ([[tickets/T-25600-finish-gate]]): lenses, synthesis, two reviews, up to three rounds. Earlier work: every `drafts/*/NAME_ENT/func_80143580/` folder and its NOTES.md.

## Acceptance criteria

- [ ] Literal default-C match, 27/27, both reviews approve; or after the last round a verdict naming the blocker.

## Notes

If an earlier wave matches it before its turn, this ticket closes as not needed.

## Comments

- 2026-10-23 (round 1, search-on-new-best lens, worktree `f-ne-r1-search-on-new-best`): **MATCHED** as plain C,
  clean build 27/27, check_headers / migrate_globals --check / sync_protos --check-branch OK; the same source with
  the rev1 delta reproduces Rev 1 exactly. Levers: case 9 `D_801217D0[0] = [1] = [2] = 0x80000000;` (the innermost
  record is read back and promoted in a2), func_8004AC18 called undeclared (implicit int; v0 busy after its calls),
  every tile fill on `i`. Drops the `if (address) {}` and compare-status FAKEs; keeps F5F self-store, `^ 0`, nested
  fills and two frame pads (FAKE). main_api.h: func_8004AC18 override guard, `void col2sepia();`. Notes:
  `drafts/T-25604/NAME_ENT/func_80143580/fresh-r1-search-on-new-best/NOTES.md`. Needs review before Done.
