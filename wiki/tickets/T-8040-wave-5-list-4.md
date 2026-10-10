---
id: T-8040
title: Wave 5: list 4
status: Done
assignee: w5-4
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match as many functions as possible from wave-5 list 4 (37 files, 151 functions).

## Acceptance criteria
- [x] Matches verified by ninja (27/27 OK) and funcdiff
- [x] Inline code review against CODING_STANDARDS.md

## Notes
Results and patterns: [[matching-notes]], section "Wave 5 list 4". Blocked functions: [[data/t0018-cases]] (18 rows).

## Comments

Inline code review against CODING_STANDARDS.md (no reviewer sub-agent): C89, declarations at block tops, /* */ comments, SDK names (SetSemiTrans, GetWorkBase, safe_env) used; new externs only in main_api.h (via sorted insertion) or overlay headers; FAKE comments with reason on func_8013BB0C (pointer temp), func_80100DD8 (one-line do-while), func_80153AA0 (0xA0 buffer); check_headers, sync_protos --check-branch, migrate_globals --check pass; all 27 units OK. No findings open.
