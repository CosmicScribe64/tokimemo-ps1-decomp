---
id: T-24002
title: Complete OPTION matching package
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[option-family-results]]", "[[matching-notes]]"]
---

## Goal

Recover verified C for OPTION `801389A0.c`: func_80139198, func_80139F8C, func_80139E3C, func_801394F4, and func_80139A30.

## Acceptance criteria

- [ ] Default linked proof and OPTION checksum for each recovered function.
- [ ] Header/global/prototype checks pass.
- [ ] Review against CODING_STANDARDS.md before Done.

## Notes

Bounded follow-up of archived T-17800/T-20910 drafts. Preserve default assembly for unresolved functions.

## Comments

2026-10-17: Created isolated branch codex/complete-option from w2-good; package begins with two guarded functions and three INCLUDE_ASM functions.

2026-10-17: `func_80139198` promoted from the T-17800 hand draft, with current DisplayActor44 type. A comma expression in menu_set's row argument delays the existing GameState flag clear until after address setup; marked FAKE because evaluation order is used for instruction scheduling and original source is unknown. All four row argument positions scored zero. Docker `ninja -j2 build/ovl/OPTION.ok` passed; `python3 tools/funcdiff.py --unit OPTION --resolve --version default func_80139198` reports `MATCH (linked function bytes equal target (860 bytes))`. Awaiting review; remaining package probes continue.
