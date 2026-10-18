---
id: T-24002
title: Complete OPTION matching package
status: In Review
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

2026-10-17: Package result: 1 of 5 matched (860 bytes), func_80139F8C improved behind its guard from 11 to 5 differing rows. func_80139E3C stays guarded at 60 rows; larger functions stay INCLUDE_ASM. Current typed source drafts and precise remaining register/scheduling blockers are preserved in drafts/T-24002/README.md and OPTION subdirectories. Header check passed; aggregate-global check passed after generating TACO assembly needed by existing keep proofs. Final branch prototype check pending below. Source meets the C89/guard/FAKE review checklist on self-review; independent review is required before Done.

2026-10-17 final verification: `sync_protos.py --check-branch` passed (0 new disagreements, 15 known); `migrate_globals.py --check` passed; `check_headers.py` passed. Official funcdiff confirmed the retained `func_80139F8C` guard has exactly five differing rows (missing argument-register copy and its branch/OR consequences). Default linked proof for func_80139198 and OPTION checksum passed again after that edit.
