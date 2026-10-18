---
id: T-24008
title: Recover TAIIKU runner family
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: []
---

## Goal

Bounded types/sibling pass on src/ovl/TAIIKU/80133C80.c: func_80137544, func_80135480, func_80137C44, func_80137EA8, func_801365C8. Necessary include/ovl/TAIIKU.h changes only. Base 8f88afff607eb20c4e143239e28b94cc7f1c7392; preserve all earlier work.

## Acceptance criteria

Fresh default linked literal proof and TAIIKU SHA1 for wins. Docker only, serial compiler/ninja -j1. Source-only useful hypotheses, review against CODING_STANDARDS.md before Done.

## Notes

Read [[taiiku-family-results]], T-16550 and recent family history before probes.

## Comments

## Checkpoint: func_80137EA8 matched

692 verified bytes, no new FAKE or header edits. Matched sibling port from func_80146D18 with canonical runner/display fields, local C result array definition, explicit final else, and element-zero-first assignment. Fresh Docker default linked literal proof and TAIIKU SHA1 306d1013b9be465819525d1fb4dc0e86b422a846 pass. See [[taiiku-family-results]]. Other four targets remain open within bounded pass.
