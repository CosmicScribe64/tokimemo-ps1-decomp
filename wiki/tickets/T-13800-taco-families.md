---
id: T-13800
title: Recover remaining TACO actor families
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[tickets/T-13801-taco-actor-family]]", "[[tickets/T-12070-codex-cx7]]"]
---

## Goal
Recover verified TACO game bytes with bounded coherent actor/table family reconstruction from baseline 97ec697f. Own only src/ovl/TACO/*.c and include/ovl/TACO.h. Original checkout remains read only.

## Acceptance criteria
- Full-TU literal linked proof and TACO SHA1 for each accepted match.
- Preserve existing C, Docker tools only, no generated assembly edits.
- Standards review before Done; commit verified matches for integration.

## Notes
Prior T-12070 and T-13400 read. Do not repeat ACCC/BD30 broad searches or 32FE8 local lifetime permutations without new evidence. Prefer >=800-byte siblings.

## Comments
Setup: isolated codex/taco-13800 created from origin/main; copied ignored reference inputs from integration.

## Diagnostic handoff

No production function accepted: zero new matched bytes. Best 497EC has one commuted pointer-add row; 4A480 has three rows. 87A4 natural full-TU candidate has 62 rows. See [[taco-family-results]] for bounded experiments and ignored build/t13801/reprobe.py for reproduction. All production source/header trial edits restored; awaiting final default overlay verification and standards/spec review.
