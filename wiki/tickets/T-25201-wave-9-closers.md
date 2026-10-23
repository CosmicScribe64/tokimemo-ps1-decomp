---
id: T-25201
title: Wave 9 closers for wave 8 near misses
status: In Progress
assignee: wave-9 conveyor
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25200-wave-9-neighbor]], [[tickets/T-25100-wave-8-finish]], [[playbook]]]
---

## Goal

One fresh-eyes Opus pass on each guarded near miss (25 rows or fewer) that a merged wave 8 package left, one job per C file, starting from the wave 8 agent's NOTES.md in `drafts/T-251NN/`. The wave 8 conveyor skips these closers because of a function-name format mismatch in its script.

## Acceptance criteria

- [ ] Each job: match (literal default-C, 27/27) or an updated NON_MATCHING comment and NOTES.md with the next hypothesis.
- [ ] Review against CODING_STANDARDS.md before Done.

## Notes

Jobs are appended under Comments as they finish (file, functions, result).

## Comments

2026-10-23, TAIIKU `src/ovl/TAIIKU/80133C80.c`, `func_80137784`: no match, guard kept at 4 rows (NON_MATCHING comment now names T-25201). Measured why the compare operand order cannot be reached with the offset alias: uopt swaps `==` when its first operand is not a plain variable, and a constant addend on the second operand moves to the first (uopt or ugen). A control with undefined zero-offset alias symbols gets every instruction right (8 rows, all alias relocations). Next: `config/symbols_TAIIKU.txt` aliases at 0x8014984C and 0x8014981C, which need approval. Notes: `drafts/T-25106/TAIIKU/func_80137784/NOTES.md`, `alias-8rows.c`.
