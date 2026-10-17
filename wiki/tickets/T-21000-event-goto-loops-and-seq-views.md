---
id: T-21000
title: "EVENT: mark or remove the goto loops in func_80118DAC and type the func_8010A6E4 views"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-9940-wave-10-list-4]]", "[[tickets/T-9920-wave-10-list-2]]"]
---

## Goal

EVENT: mark or remove the goto loops in func_80118DAC and type the func_8010A6E4 views. (1) src/ovl/EVENT/8010FE10.c func_80118DAC uses goto positive_loop / negative_loop with no FAKE comment: try for/while forms that keep the bytes, or add a FAKE comment with reason and ticket. (2) src/ovl/EVENT/8010A210.c func_8010A6E4 defines local SeqFlags/SeqBits structs (u8 pad[0x3EC], bare unsigned int bit-fields) and reads D_800B0860 + 0x3EC; replace them with the typed root described in the T-9770 log entry (record 11 flags, root 0x800B0860, Rec34), documented in the EVENT header. Both must stay byte-identical: funcdiff --resolve, tools/docker.sh ninja 27/27, check_headers.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-9940-wave-10-list-4|T-9940]], [[tickets/T-9920-wave-10-list-2|T-9920]].

## Comments
