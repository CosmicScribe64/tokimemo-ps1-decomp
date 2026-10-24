---
id: T-25500
title: Stubborn functions - multi-angle attack, toolchain changes allowed under 7a
status: Done
assignee: orchestrator
created: 2026-10-23
updated: 2026-10-24
links: [[[tickets/T-25400-wave-11-until-matched]], [[build-step-gaps]], [[original-compiler]], [[toolchain]]]
---

## Goal

User request (2026-10-23): pay extra effort to the functions that keep failing deep passes, think outside the box, and change the build tooling if that is what they need. Each target gets independent lenses in separate worktrees (clean-room reconstruction, compiler-internals trace, toolchain model, long search; for TAIIKU also a symbol-legitimacy check), then a synthesis pass, then two adversarial reviews (standards/fakematch and technical) before anything merges.

## Acceptance criteria

- [ ] Each target matched with literal default-C, 27/27, or a written verdict on what blocks it and what change would be needed.
- [ ] Any tools/ or config/ change meets CODING_STANDARDS.md 7a (uniform, documented, evidence-backed over a broad sample, fails loudly, unit-tested, replaceable) and leaves every other object byte-identical; the orchestrator reviews it before merge.

## Notes

Targets: [[tickets/T-25501-stubborn-shougatu-8013ea98]] (SHOUGATU func_8013EA98: three passes, empty-latch threading in uopt), [[tickets/T-25502-stubborn-taiiku-80137784]] (TAIIKU func_80137784: 4 rows, address-first compares; a full match exists only with two extra symbol names). Both files are reserved from wave 11 (local state/w11-active.txt).

## Comments
- 2026-10-24 (orchestrator): both targets matched: TAIIKU func_80137784 (plain C, uopt test replacement of index tests) and SHOUGATU func_8013EA98 (FAKE-marked C, one keep line). Phase 1 complete.
