---
id: T-0301
title: Split SDK rodata and data per library and object
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[psyq-sdk]]", "[[tickets/T-0010-sdk-lib-object-boundaries]]"]
---

## Goal

The `rodata` and `data` segments still mix game and SDK data. Split off the SDK libs' rodata/data (strings, switch tables, RCS ids, lib state) per lib, then per object, in link order.

## Acceptance criteria

- [ ] `config/SLPM_86.053.yaml` has rodata/data subsegments named after the lib segments they belong to.
- [ ] Boundaries recorded in [[psyq-sdk]] with the xref evidence.
- [ ] `ninja` ends with `build/SLPM_86.053.bin: OK`.

## Notes

Starting evidence is in [[psyq-sdk]]: libpress strings 0x800B23B0-0x800B2460, libcd 0x800B2480-0x800B2924, libsnd from 0x800B2930, libgpu RCS ids 0x800B2E50/0x800B3170, data refs 0x800CA4xx (libpress), 0x800DC4F0-0x800DC9A0 (libcd), 0x800DCBD0 up (libspu).

## Comments
