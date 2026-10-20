---
id: T-24502
title: Match remaining TACO functions
status: In Progress
assignee: codex
created: 2026-10-20
updated: 2026-10-20
links: [[[matching-notes]], [[playbook]]]
---

## Goal

Match remaining TACO functions using ordinary C and the existing Docker toolchain.

## Acceptance criteria

- [ ] Record function experiments and avoid repeated documented failures.
- [ ] Prove promoted C with literal default funcdiff and all 27 binary checksums.
- [ ] Review against CODING_STANDARDS.md before Done.

## Notes

Isolated branch codex/taco-24502 from 2c7293275. TACO files only.

## Comments

- 2026-10-20: claimed; inspect close func_8013B460 first, then tractable remaining functions.

- 2026-10-20: func_8013B460 comma-expression grouping leaves documented two moved stores unchanged; restored baseline. Fresh typed func_80140028 matches all 828 literal linked bytes in the default build. Parent review requested moving func_80140ADC's forward declaration to TACO.h; applied, full checks running. Details in [[matching-notes]].

- 2026-10-20 func_80140028 review: parent checked CODING_STANDARDS.md; sole finding was the local forward declaration, resolved by declaring func_80140ADC once in TACO.h. Docker verification after that change: default funcdiff MATCH (828 linked bytes), 27/27 independently recomputed binary SHA1 checks, headers OK, globals OK, prototype branch check OK (0 new disagreements). Campaign remains In Progress for additional TACO functions.
