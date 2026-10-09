---
id: T-0006
title: Identify PsyQ libs/SDK version
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[psyq-sdk]]"]
---

## Goal

Identify the PsyQ SDK era and the set of libraries linked into the executable, as the basis for matching library code. Pinning the exact release and object-level boundaries is split off to [[tickets/T-0010-sdk-lib-object-boundaries]].

## Acceptance criteria

- [x] SDK era and library list recorded in [[psyq-sdk]] (libpress, libcd, libsnd, libspu, libgpu, libapi).
- [x] Follow-up work (exact release, object boundaries) re-scoped into [[tickets/T-0010-sdk-lib-object-boundaries]].

## Notes

Library code is currently asm-only (`sdk_libs`, `libapi_stubs` segments). Re-scoped after review so the acceptance criteria match the delivered scope.

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
- 2026-10-09: code review (vs CODING_STANDARDS.md, range 060900b..HEAD) run. Findings: Acceptance criteria re-scoped to match delivered work; remainder in T-0010. Fixed. Build re-verified from a clean tree after fixes: `build/SLPM_86.053.bin: OK`. Moved to Done.
