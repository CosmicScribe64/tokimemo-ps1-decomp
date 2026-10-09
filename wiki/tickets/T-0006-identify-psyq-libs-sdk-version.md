---
id: T-0006
title: Identify PsyQ libs/SDK version
status: In Review
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[psyq-sdk]]"]
---

## Goal

Identify which PsyQ SDK version and library objects are linked in, for matching library code.

## Acceptance criteria

- [x] SDK era and library list recorded in [[psyq-sdk]] (libpress, libcd, libsnd, libspu, libgpu, libapi).
- [ ] Exact SDK release and object-level boundaries: moved to [[tickets/T-0010-sdk-lib-object-boundaries]].

## Notes

Library code is currently asm-only (`sdk_libs`, `libapi_stubs` segments). The second criterion is explicitly deferred to the follow-up ticket.

## Comments

- 2026-10-09: moved to In Review. Awaiting code-review against CODING_STANDARDS.md.
