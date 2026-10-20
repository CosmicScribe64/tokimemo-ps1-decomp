---
id: T-24606
title: Match OPTION calendar controls
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]"]
---

## Goal

Complete OPTION `func_8013A7EC` using the recovered calendar fields, weekday loops and existing flag types.

## Acceptance criteria

- [x] Default C matches all 1,092 literal linked bytes.
- [x] Full Docker build, executable hashes and declaration guards pass.
- [x] Independent standards and behavior reviews have no unresolved findings.

## Notes

The minimized273-word candidate matches. Existing ScheduleFlags and OptionByteBf views recover field updates and selector allocation. An entry weekday read retains the original global lifetime; integer fallthrough preserves the original delay-slot NOP. Identity multiplication proved unnecessary and was removed. Shared pointer/month and row/loop-bound scalar storage remains necessary and carries FAKE explanations.

## Comments

- 2026-10-20: Created from T-24600 before production promotion. Private literal proof passes; cleanup, production gates and independent reviews remain pending.
- 2026-10-20: In Progress -> In Review. Root default literal proof matches1,092 bytes; full ninja, fresh27 hashes and declaration guards pass (zero new disagreements,15 known). Both independent reviews have no unresolved findings after correcting the explicit integer-return annotation. Omission controls: no entry read4/273; separate label pointer168/273; separate bound61/272; plain unsigned selector cast149/274; void return7/272. No shared header/data/compiler change.
- 2026-10-20: In Review -> Done after all gates and independent reviews passed. Adds1,092 verified bytes; total6,822 functions /2,097,048 bytes,130 functions remain.
