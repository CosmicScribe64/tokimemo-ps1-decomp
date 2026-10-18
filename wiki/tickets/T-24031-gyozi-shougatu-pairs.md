---
id: T-24031
title: Recover paired GYOZI and SHOUGATU particle handlers
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[event-family-results]]", "[[shougatu-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Match exact opcode-structure pairs using the stronger recovered source and transfer:
- GYOZI func_80144B70 and SHOUGATU func_8014288C, each 1,300 literal bytes.
- GYOZI func_80144018 and SHOUGATU func_80141D20, each 1,204 literal bytes.
Exclusive C files src/ovl/GYOZI/80143AD0.c and src/ovl/SHOUGATU/80141780.c.

## Acceptance criteria
- Read prior type maps, drafts and negative evidence first; bounded novel pass near 25 minutes.
- Reuse built /private/tmp/tokimemo-complete-etc, codex/complete-gyozi-shougatu-pairs, preserving verified BUNKASAI ancestor 72cefc5d5 and all history.
- One GPT-6.1 Sol medium compiler total, Docker only, ninja -j1. No other source files or main_api.h edits without notification.
- Verified C removes target guard and INCLUDE_ASM and passes explicit normal-default linked proof, affected checksum and header/global/prototype guards.
- Preserve matched bodies and useful source-only drafts; send pinned wins promptly. Root owns independent review, all-27 checksums, integration and push.

## Prior evidence
T-17500/T-17600 typed arrays identify three 50-word work arrays, a short array and the shared Rec24/display records. Correct indexed array accesses are required before allocation experiments. T-24004 preserves repaired SHOUGATU func_80141D20 at 491 literal rows / 387 aligned edits; do not repeat uniform old m2c/redraft probes. No matched sibling exists yet in either target pair. Read current sibling setup handlers and faithful direct global mutations before choosing an iterative rewrite.

## Comments
Both target files are explicitly unclaimed by other active workers. Other GYOZI helper assignments remain excluded. No new worktree, history reset, host install, inline assembly or per-function compiler exception.
