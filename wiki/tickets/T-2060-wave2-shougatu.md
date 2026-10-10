---
id: T-2060
title: "Wave 2: SHOUGATU"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[decompile-workflow]]"]
---

## Goal
Match the remaining unblocked functions of the SHOUGATU overlay (`src/ovl/SHOUGATU.c`, `include/ovl/SHOUGATU.h`).

## Acceptance criteria

- [x] At least 60 functions matched (152 matched)
- [x] Clean rebuild, 27 of 27 sha1 OK
- [x] Inline code review against CODING_STANDARDS done

## Notes
152 functions matched (SHOUGATU 30 -> 182 of 400, grand total 1053 of 6962). Method: m2c drafts for all 234 unblocked queue entries, applied in bulk, funcdiff, revert misses. 22 T-0018 rows added to [[data/t0018-cases]]; patterns in [[matching-notes]] ("Wave 2: SHOUGATU"). Tooling bug found: `tools/m2c.py` picks the wrong overlay's asm for shared addresses.

Inline review (CODING_STANDARDS): C89 only, no `//`, declarations at block top, no FAKE code or fakematch tricks (no dummy locals/volatile), no NON_MATCHING blocks, externs for SHOUGATU-only symbols in `include/ovl/SHOUGATU.h` (unused ones removed, types checked against lbu/lb/lh access widths; `tools/check_headers.py` OK), no commented-out code, no game data committed. Findings: redundant `(s32)` casts removed. Open: none.

## Comments
