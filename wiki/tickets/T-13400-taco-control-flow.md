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

## Experiment evidence
Current full-TU baseline reproduces89rows. Eight bounded candidates combine natural boolean conditions (remove two cross-block gotos), consistent GameState pointer and signed/unsigned canonical flag view. Natural directaggregate reduces89to57; pointer forms regress to175 (natural) or159(seed); signedness changes no score. Remaining differences include local stack homes and duplicated mask-value copies across calls, so the next useful step is local-lifetime reconstruction, not further aggregate-pointer sweeps. No production source changed. Scratch: build/t13400/forms.log, natural.diff and candidate directories.
