---
id: T-11890
title: Match TT func_8014D260 (gcc ratan2) through an ASPSX pass on the gcc path
status: In Progress
assignee: diman8 (claim #56)
created: 2026-10-18
updated: 2026-10-18
links: [[gcc-objects]], [[tickets/T-9200-gcc-built-sdk-objects-in-overlays]], [[toolchain]], tools/cc.py, config/toolchains.txt
---

## Goal

Match TT `func_8014D260` (object `TT/8014D260`, PsyQ gcc `ratan2`), the last TT function. Model the three ASPSX behaviours that maspsx lacks ([[gcc-objects]]): the `div`/`mflo` order, the `mflo` hazard nops, and `lh rd,sym(rs)` expanded through rd. Scope: the gcc/maspsx path for SDK objects only.

## Acceptance criteria

- [ ] Each modelled behaviour measured over the ASPSX-built SDK asm of the main exe; rule and exceptions written down (CODING_STANDARDS 7a)
- [ ] Pass is uniform, fails loudly, has synthetic unit tests, and is documented in a module docstring, [[toolchain]] and [[matching-notes]]
- [ ] `TT/8014D260` listed in `config/toolchains.txt`; `func_8014D260` matches
- [ ] Every other function unchanged: clean rebuild 27/27, `TAIIKU/801488F0` still matches
- [ ] Code review against CODING_STANDARDS.md

## Notes

## Comments
