---
id: T-21001
title: "Recover the wave-10 list 6 near misses: BUNKAKEN func_80143DD0 and RPG_BAT func_80139510"
status: Backlog
assignee:
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-9960-wave-10-list-6]]"]
---

## Goal

Recover the wave-10 list 6 near misses: BUNKAKEN func_80143DD0 and RPG_BAT func_80139510. T-9960 got func_80143DD0 down to 8 changed lines (signed-halfword cast on the shifted character nibble, explicit compare of CharFlags.b2 with 1, direct selected-character accesses; the masked-switch mask still lands in t5 instead of t4) and func_80139510 to 21 lines (paired x/y and w/h RECT stores, eight-byte local). Neither source was committed; both are INCLUDE_ASM on main and the later w3-bunkaken-p01 attempt at func_80143DD0 reached only 330 lines. Rebuild both from the recipes in wiki/matching-notes.md (T-9960 sections), finish with a bounded permuter run, and if they stay open keep them under NON_MATCHING with ticket and line count or archive them in drafts/.

## Acceptance criteria

- [ ] The work in the Goal is done, or each part left undone is recorded in this ticket with the reason.
- [ ] Code review against CODING_STANDARDS.md has no open findings.

## Notes

Opened from the 2026-10-17 ticket cleanup review of [[tickets/T-9960-wave-10-list-6|T-9960]].

## Comments
