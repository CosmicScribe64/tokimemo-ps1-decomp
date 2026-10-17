---
id: T-21013
title: "RPG_BAT motion work area D_8015EC28..EC6F: declare the overlapping views once or mark the casts"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-14900-rpg-near-matches]]", "[[tickets/T-12040-codex-cx4]]"]
---

## Goal

RPG_BAT motion work area D_8015EC28..EC6F: declare the overlapping views once or mark the casts. Four sites use `(RpgMotion20 *)&D_8015EC50` or `(RpgMotion30 *)&D_8015EC28` (src/ovl/RPG_BAT/801321D0.c:50, 801484E0.c:171, 8014DF60.c:107, 8015A1F0.c:125) with scalar externs D_8015EC50/EC54/EC6C still declared. The views overlap, so try a union or one aggregate via tools/type_recovery.py and let the build decide; if no declaration keeps the unit matching, add a FAKE-style comment with the ticket and the reason (alias view stops IDO moving the EC6C load ahead of the EC50 store) at each use site. Run tools/migrate_globals.py --check and the RPG_BAT SHA1 afterwards. Low priority.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-14900-rpg-near-matches|T-14900]], [[tickets/T-12040-codex-cx4|T-12040]].

## Comments
