---
id: T-13400
title: Recover TACO flag handler control flow
status: In Progress
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[broad-coverage-results]]"]
---

## Goal
Test natural control-flow reconstruction for TACO func_80132FE8, a previously recorded 89-row near miss, and preserve reusable evidence.

## Acceptance criteria
- [ ] Compile candidates in full translation-unit context.
- [ ] Promote only literal verified C with overlay checksum proof.

## Notes
Root owns only TACO/80132220.c for this experiment. Prior cx7 work is completed and belongs to a different source file. Start from the saved broad-sweep draft; migrate existing aggregate names and remove decompiler-generated duplicate condition loads and cross-block goto where assembly supports ordinary conditions. Scratch-only until proven.
