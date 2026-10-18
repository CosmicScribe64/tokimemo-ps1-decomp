---
id: T-24057
title: Match two small ENDING fade handlers
status: In Progress
assignee: Codex
created: 2026-10-18
updated: 2026-10-18
links: ["[[ending-family-results]]", "[[tickets/T-18300-types-first-ending]]", "[[tickets/T-20930-types-ending]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Recover only func_80134B18 (208 instruction bytes) and func_80134C2C (240 instruction bytes, approximately 244 canonical bytes) in src/ovl/ENDING/80133C10.c. Another chat owns ENDING/80134D20.c; preserve every other function.

## Acceptance criteria
- Reuse the built worktree on codex/complete-ending-small-pair from frozen T-24051 tip 71833d4e2, preserving ancestry.
- Read ending-family-results and T-18300/T-20930 negative evidence before novel source probes.
- One GPT-6.1 Sol medium compiler, Docker only, serial builds with ninja -j1.
- Report exact necessary shared-header/config changes before edits; no unrelated source or ownership changes.
- Actual C wins remove the target guard/INCLUDE_ASM and pass explicit default linked funcdiff, affected overlay checksum and header/global/prototype guards.
- Pin useful results promptly; parent owns independent review, integration and push. In Review until review passes.

## Notes
Prior 24/40-row leads involve promotion of record 96's fade byte across calls, and loading it above stores to record 97. Whole D_8011ECD0 views change aliasing and the GameState byte allocation; separate record symbols may matter. Recent BUNKA_SD func_801355AC byte fade view and *=0 source idiom is an analogy, not proof. Avoid repeating exhausted scalar-versus-whole-record sweeps.

## Comments
Parent assignment reports main 958e039a8, 6,504 functions and 1,850,912 verified bytes (81.2%). Those gains are outside this package. DATE's improved source stays frozen at 71833d4e2 and ownership reserved for later recipe transfer; no concurrent DATE compiler.
