---
id: T-21002
title: "Declare the second set of nine GsRec0FC records in GameState instead of raw unk_120 casts"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-10890-big-functions-astra-21]]"]
---

## Goal

Declare the second set of nine GsRec0FC records in GameState instead of raw unk_120 casts. src/main/80062CD0.c reads 18 halfwords as *(s16 *)&D_800E6280.unk_120[0x5A], [0x5E], ... [0x7A] (step 4), each compared with unk_0FC..unk_11C .unk_02 in the same expression, which suggests nine more GsRec0FC records at GameState+0x178 (include/main_api.h splits unk_120 as u8[0x9C]). Declare them (documented offsets), switch the casts to member reads, and check whether the two unk_120[0x20] casts (80062CD0.c, 80041000.c) are a different field. Keep the unit byte-identical: funcdiff --resolve, tools/docker.sh ninja 27/27, check_headers, migrate_globals --check.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-10890-big-functions-astra-21|T-10890]].

## Comments
