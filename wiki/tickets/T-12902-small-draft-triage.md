---
id: T-12902
title: Triage saved small OPTION and audio drafts
status: Done
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

## Final verification and review (2026-10-15)

- Docker full default build: 27/27 binary SHA1 checks pass; final source-comment rebuild passes main/OPTION checks again.
- Final funcdiff --resolve proves default C linked bytes: OPTION func_80137E38 360, main func_8007A868 188, main func_80079F00 264; total 812 bytes.
- Main SHA1: e823bd844a8f8fa4d05483b59c66bc54b8393b26. OPTION SHA1: 3fd443d1e32404e5febd57d0586ef1c70ce8a811.
- sync_protos --check-branch: branch check OK, 0 new disagreements, 15 known.
- Separate code-review skill standards/spec reviewers found no correctness, scope, or FAKE issue. Standards requests for source-local matching comments and one-source-file commits are resolved and re-reviewed. No outstanding findings.
- Proofs retained locally under build/t12900/experiments/: all27.txt, final-build.txt, final-proof.txt, protos.txt. Scratch is ignored; no game inputs or generated output committed.

## Remaining candidates

Saved draft families remain guarded/asm. func_8007A50C needs consistent halfword types and register allocation; func_80079C70 needs its loop live ranges recovered. func_8007B64C saved void draft drops the return value and writes through a word instead of a halfword; no candidate was enabled. OPTION func_80132384 reconstructs the menu movement but differs in strength reduction, saved registers and frame; scratch only. Actor-alias/callee fixes made several old OPTION drafts compile but did not produce another exact match. Avoid restarting long mutation runs from these stale scores.
