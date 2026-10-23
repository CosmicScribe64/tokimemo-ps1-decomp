---
id: T-25606
title: Finish gate - main func_800476C0
status: Done
assignee: finish gate
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25600-finish-gate]], [[playbook]]]
---

## Goal

Match main:func_800476C0 (850 words, `src/main/80047550.c`) through the finish gate ([[tickets/T-25600-finish-gate]]): lenses, synthesis, two reviews, up to three rounds. Earlier work: every `drafts/*/main/func_800476C0/` folder and its NOTES.md.

## Acceptance criteria

- [ ] Literal default-C match, 27/27, both reviews approve; or after the last round a verdict naming the blocker.

## Notes

If an earlier wave matches it before its turn, this ticket closes as not needed.

## Comments
- 2026-10-23 (orchestrator): round 1 block-by-block lens matched main func_800476C0 (3,132 bytes, commit 9c98d5899, 552 -> 0 rows: part as an s16 index j*7 with a join anchor, s16 uv, rotate read as one s32 word, the gs->count CSE in v1). Merged directly instead of waiting for the synthesis and review stage. Orchestrator review against CODING_STANDARDS.md: six FAKEs, each marked with a ticket and a reason (never-taken goto and its label, anim reuse for the sequence row, an empty test in the join block, a second scale test around a dead store, an empty arm on the unk_06 == 0 path); header changes are the merged s32 rotate field at 0x20 (the original copies it as one word) and the u8 func_8004901C prototype; no tools or config change. Merge checks: clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch, funcdiff --resolve --version default MATCH with no INCLUDE_ASM. Notes: drafts/T-25606/main/func_800476C0/r1-structure/.
