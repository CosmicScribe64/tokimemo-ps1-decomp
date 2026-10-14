---
id: T-11240
title: Type-mismatch fixes for bulk_m2c and autotweak
status: In Review
assignee: g41
created: 2026-10-14
updated: 2026-10-14
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[tickets/T-11210-bulk-m2c-draft-failure-classes]]", "[[tickets/T-11220-regdiff-guided-autotweak]]"]
---

## Goal

New module `tools/typefix.py`: (A) post-m2c rewrites for the compile failures left after T-11210 (dereferenced a non-pointer, syntax error, non-function call, header problems after a verified match, M2C_ERROR/M2C_LWL), (B) autotweak edit kinds for type-driven mismatches (lh/lhu, lb/lbu, sra/srl, sltu/slt, andi/sll+sra).

## Acceptance criteria

- [ ] `tools/test_typefix.py` covers each rewrite and edit
- [ ] bulk_m2c re-run of the failed drafts; new matches committed separately
- [ ] ninja 27/27, headers and globals OK
- [ ] code review against CODING_STANDARDS.md

## Notes

Only minimal hooks in `tools/bulk_m2c.py` and `tools/autotweak.py` (parallel work on bulk_m2c for T-11230).

## Comments
