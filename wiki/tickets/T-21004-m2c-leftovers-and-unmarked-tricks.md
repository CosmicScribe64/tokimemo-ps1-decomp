---
id: T-21004
title: "Clean up m2c leftovers and unmarked tricks in matched C (review findings of T-10900, T-11070, T-11270)"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11270-exact-rescore-autotweak-bulk-m2c]]", "[[tickets/T-10900-big-functions-astra-22]]", "[[tickets/T-11070-small-functions-gemini-30]]"]
---

## Goal

Clean up m2c leftovers and unmarked tricks in matched C (review findings of T-10900, T-11070, T-11270). Each item must keep the build at 27/27 and be its own small commit; if a removal changes the bytes, keep the code and add a FAKE comment with reason and ticket. (1) src/ovl/SHUGAKU/80139D00.c func_80139FD4: remove the no-op 'goto block_11;' (T-11270). (2) Replace the 113 'unsigned char' occurrences with u8 in DATE/80132000.c (32), EVENT/8010FE10.c (68), TACO/80146D60.c (10) and 3 single uses in ENDING/801396E0.c, SHUGAKU/80139D00.c, main/8006CB30.c (T-10900, T-11270). (3) src/ovl/EVENT/8010FE10.c func_80117390: write *(s16 *)(D_800B0860 + D_800B1746 * 0x34 + 0x1A6) as D_800B0A04[D_800B1746].unk_02 (Rec34 in main_api.h), or tag it FAKE (T-11070). (4) func_80119370 in the same file: give Rec34 a real birthday field (offset 0x10) or tag the EventBirthday4 casts of pad10[0] and D_800B0948 with FAKE and a reason, and comment why the bit-field is needed (T-11070).

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-11270-exact-rescore-autotweak-bulk-m2c|T-11270]], [[tickets/T-10900-big-functions-astra-22|T-10900]], [[tickets/T-11070-small-functions-gemini-30|T-11070]].

## Comments
