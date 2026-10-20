---
id: T-24014
title: Close TACO func_8014A480 PRE operand order
status: Done
assignee: Codex
created: 2026-10-18
updated: 2026-10-20
links: ["[[tickets/T-22025-w4-p25]]", "[[matching-notes]]"]
---

## Goal

Bounded 15-minute source-only closer for func_8014A480 in src/ovl/TACO/80149F90.c. Test empty dot-form reads to seed the PRE expression representative, and paired dot forms at both real tests. Other functions remain unchanged.

## Acceptance criteria

- A win requires unguarded default linked literal proof, TACO SHA1 and header/global/prototype guards.
- Mark any nonplausible source FAKE; no toolchain changes, per-function flags or inline assembly.
- Otherwise preserve the existing guard and minimal reproducible evidence.
- Independent review before Done.

## Notes

T-22025 isolated one differing word: compiler-inserted reload address `addu t4,v1,t6`, target `addu t4,t6,v1`. Arrow pointer spellings retain it; a dot-form join alone breaks PRE and gives about 130 rows. Do not repeat those isolated forms.

## Comments

- First 16 probes: baseline weighted cost 2 (one word); empty dot reads produce 32 or 112. Paired byte tests cost 69, paired full conditions 65. Rewriting the entire state region to dot forms costs 4: the inserted reload is corrected, but the initial state address and increment address reverse operands (rows 129 and 149). Next test: reversed indexing and selective expression spellings within that dot-form PRE region.

- Completed 47 targeted probes without a match. All-dot preserves PRE and fixes row 156 but reverses two earlier additions; mixed arrow/dot, address-only seeds, reversed indexes, aggregate wrappers and dual-form joins do not produce all three required orders. Minimal three-case reproducer and results archived in drafts/T-24014/. Production source and all other TACO functions are unchanged; zero new C bytes.

- Preservation checks: minimal reproducer verifies baseline 2 / all-dot 4 / empty-dot-read 32. Explicit default linked proof matches the retained 796-byte assembly fallback; this is not a C match. Fresh TACO SHA1 comparison passes. `git diff --check` passes; production source has no diff. Submitted In Review for independent archive review.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
