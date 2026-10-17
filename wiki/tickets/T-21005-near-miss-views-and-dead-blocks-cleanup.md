---
id: T-21005
title: "Cleanup of unexplained views and dead blocks left by the near-miss tickets"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11310-close-near-misses-sonnet-47]]", "[[tickets/T-11100-big-functions-gemini-33]]", "[[tickets/T-11300-close-near-misses-sonnet-46]]"]
---

## Goal

Cleanup of unexplained views and dead blocks left by the near-miss tickets. Four small source cleanups found in the 2026-10-17 ticket review, none should change bytes. (1) src/main/8005A0B0.c func_8005FA20 switches on '*(u8 *)&D_800E6280.unk_1094' while the field is s8: add a FAKE comment with ticket and reason (CODING_STANDARDS section 7), or find the real type. (2) src/ovl/TACO/80146D60.c func_80146D60 stores '*(s32 *)&D_80127480[i] = 0' although TcObj50.unk0 is a real s32 field now (it was pad0[0x18] when T-11300 matched it): try 'D_80127480[i].unk0 = 0' and keep the cast with a FAKE comment only if the build needs it. (3) src/main/800674B0.c func_80068EC0 casts '(DisplayActor44 *)D_8011ECD0' although the file already declares D_8011ECD0 as DisplayActor44[]: drop the casts if the bytes stay. (4) include/main_api.h has two empty '#ifndef MAIN_API_STRNCAT_T10860 / #define / #endif' blocks that nothing reads and no tool generates: delete them. Gate: clean rebuild 27/27 with unchanged sha1, check_headers.py and sync_protos.py --check-branch OK, one commit per item.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-11310-close-near-misses-sonnet-47|T-11310]], [[tickets/T-11100-big-functions-gemini-33|T-11100]], [[tickets/T-11300-close-near-misses-sonnet-46|T-11300]].

## Comments
