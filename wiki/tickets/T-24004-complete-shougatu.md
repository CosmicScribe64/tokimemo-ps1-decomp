---
id: T-24004
title: Complete SHOUGATU scene package
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[shougatu-family-results]]"]
---

## Goal

Recover verified C for SHOUGATU 80141780.c: func_8014265C, func_801430A0, func_80142DA0, func_801421D4, func_80141D20.

## Acceptance criteria

- [ ] Verify recovered functions with default linked proof and overlay checksum.
- [ ] Pass header/global/prototype checks and independent review.

## Notes

Reuse the clean built codex/complete-option worktree. One ninja job. Bounded shared-family reconstruction from T-17500 and archived wave-4 package.

## Comments

2026-10-17: Consulted wiki index, SHOUGATU family results, worklist w4-p36 and current sibling recipes before code work.

2026-10-17: `func_801430A0` matched and promoted (352 bytes), typed FnTbl48 dispatcher with direct counter mutation and consistent DisplayActor44 record-base accesses. Fall-through s32 definition retains the counter result in v0; no forced registers, fake statements, padding locals, or function-pointer return cast. A required void func_80143228(void) forward declaration was added to SHOUGATU.h. Docker ninja -j1 build/ovl/SHOUGATU.ok passes; funcdiff --unit SHOUGATU --resolve --version default --no-build func_801430A0 confirms literal linked target-byte equality (352 bytes).
