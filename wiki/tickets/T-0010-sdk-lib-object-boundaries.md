---
id: T-0010
title: Pin SDK version and lib object boundaries
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Follow-up of [[tickets/T-0006-identify-psyq-libs-sdk-version]]: find the exact start of libpress and the object-level boundaries of libpress/libcd/libsnd/libspu/libgpu/libapi, and pin the PsyQ version.

## Acceptance criteria

- [ ] Each lib object in `sdk_libs` becomes its own asm subsegment with its rodata/data.
- [ ] SDK release pinned by matching an SDK function against a known library object.
- [ ] Findings recorded in [[psyq-sdk]].

## Notes

Current heuristic start is 0x80086810; switch-table evidence suggests libpress starts earlier (see [[psyq-sdk]]).

## Comments
