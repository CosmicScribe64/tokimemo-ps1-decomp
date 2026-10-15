---
id: T-12902
title: Triage saved small OPTION and audio drafts
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-12900-small-option-main]]"]
---

## Goal

Bounded full-unit scoring of saved cx6/cx8 drafts under 600 bytes. Recover canonical aggregate views and calling conventions; pursue the smallest residual diffs first.

## Acceptance criteria

- Enable only functions with literal linked-byte proof and affected binary SHA1.
- Preserve all unrelated functions and shared headers unless justified.
- Review against CODING_STANDARDS.md before Done.

## Comments

- 2026-10-15: Started after the first 548 verified bytes in 6aa2f091.

- Full-unit triage of saved cx6/cx8 drafts: no direct matches; canonicalized GameState/Work80125D10 views. Missing actor aliases and stale implicit callees prevent several drafts from compiling. No stale whole candidate copied over unrelated functions.
- func_80079F00: matching narrow prototype for the second/third func_8008BFB0 arguments removes redundant/missing conversion differences. Moving the independent volume assignment before the flag update gives zero full-unit diff rows (264 bytes). Add a narrowly scoped MAIN_API_OVERRIDE guard; preserve the old canonical declaration for every other file. All 27 checks required before handoff.

- Docker funcdiff --resolve: func_80079F00 MATCH, literal linked target bytes (264). Existing first checkpoint functions still prove 188 and 360 bytes. Main binary SHA1 OK after override. Full 27-check build running serially for the shared-header guard.
