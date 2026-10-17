---
id: T-21007
title: "bulk_m2c header banner clutter and header recovery"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-11190-bulk-m2c-pipeline]]", "[[tickets/T-11210-bulk-m2c-draft-failure-classes]]"]
---

## Goal

bulk_m2c header banner clutter and header recovery. include/ holds 71 copies of '/* T-11190: declarations added by tools/bulk_m2c.py. */' (38 in include/main_api.h, the rest in 11 overlay headers), many of them stacked with no declaration under them. Remove the orphaned banners and change insert_in_header so that it does not add one banner per insertion (none, or one per header). Separately, Run.process (finally block) and recover() restore only the C file, while Run._finish also writes headers: a crash or exception after the header write leaves unused declarations. Save the original header text in the inflight record and restore it in both places. Add tests in tools/test_bulk_m2c.py; tools/check_headers.py and the unit target must stay green and no function may change.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-11190-bulk-m2c-pipeline|T-11190]], [[tickets/T-11210-bulk-m2c-draft-failure-classes|T-11210]].

## Comments
