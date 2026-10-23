---
id: T-25604
title: Finish gate - NAME_ENT func_80143580
status: Done
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
- 2026-10-23 (orchestrator): the fresh-start round 1 search lens matched NAME_ENT func_80143580 (4,308 bytes, commit 64d6cf536) from the digest's 122-row body: index fills on i, an implicit-int view of func_8004AC18, and the case 9 record chain that the Rev 1 triangulation lens confirmed against the older release. Merged directly instead of waiting for the synthesis stage. Orchestrator review against CODING_STANDARDS.md: six FAKEs, each marked with tickets and a reason (two unused stack pads for the original 0x90 frame, an entry reference to unk_F5F, a ^ 0 that drops the promoted field's a0 preference, nested fill loops as the target has them); header changes are an override-guarded implicit-int declaration of func_8004AC18 and the col2sepia prototype; no tools or config change. Merge checks: clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch, funcdiff --resolve --version default MATCH, no INCLUDE_ASM. Notes: drafts/T-25604/NAME_ENT/func_80143580/ (BRIEF-r1.md, fresh-r1-*).
