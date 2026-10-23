---
id: T-24004
title: Complete SHOUGATU scene package
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-20
links: ["[[shougatu-family-results]]"]
---

## Goal

Recover verified C for SHOUGATU 80141780.c: func_8014265C, func_801430A0, func_80142DA0, func_801421D4, func_80141D20.

## Acceptance criteria

- [ ] Verify recovered functions with default linked proof and overlay checksum.
- [x] Pass header/global/prototype checks and independent review.

## Notes

Reuse the clean built codex/complete-option worktree. One ninja job. Bounded shared-family reconstruction from T-17500 and archived wave-4 package.

## Comments

2026-10-17: Consulted wiki index, SHOUGATU family results, worklist w4-p36 and current sibling recipes before code work.

2026-10-17: `func_801430A0` matched and promoted (352 bytes), typed FnTbl48 dispatcher with direct counter mutation and consistent DisplayActor44 record-base accesses. Fall-through s32 definition retains the counter result in v0; no forced registers, fake statements, padding locals, or function-pointer return cast. A required void func_80143228(void) forward declaration was added to SHOUGATU.h. Docker ninja -j1 build/ovl/SHOUGATU.ok passes; funcdiff --unit SHOUGATU --resolve --version default --no-build func_801430A0 confirms literal linked target-byte equality (352 bytes).

2026-10-17 final: Package result 1 of 5 matched, 352 bytes. All four others stay INCLUDE_ASM; no unverified draft promoted or guarded. Source-only corrected drafts and exact remaining allocation/scheduling blockers are archived in drafts/T-24004/README.md. Header check and absorbed-global check pass. Branch prototype check passes (0 new disagreements, 15 known). Self-review checked C89, real callback-table/display types, no new FAKE, scoped single header declaration, raw data excluded. Independent review remains required before Done.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
