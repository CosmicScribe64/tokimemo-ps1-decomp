---
id: T-21018
title: "Match EVENT:func_80118010 from the T-15200 near miss"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-15200-event-large]]"]
---

## Goal

Match EVENT:func_80118010 from the T-15200 near miss. The 680-byte handler was at 26 differing rows (13 register-only instruction positions) with a hand draft in drafts/T-15200/func_80118010/body.c (with contexts and 29 rejected variants in drafts/T-15200/experiments); it is not on T-17400's near-miss list. Known blocker from the traced uopt: stride 0x34 lands in v1 instead of a0 and three computed record addresses in v0 instead of v1; passing 0x34 as a call argument fixes the stride register (18 rows left) but the callee ABI is unknown. Re-measure against current main (EVENT.h and Rec34 types changed since), try the as1 ordering recipes from T-20620, and promote only with a literal funcdiff MATCH and EVENT SHA1.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-15200-event-large|T-15200]].

## Comments
