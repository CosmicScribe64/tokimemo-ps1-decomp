---
id: T-24011
title: Match assigned DATE dispatch family
status: In Review
assignee: Codex DATE worker
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24010-date-etc-coordination]]", "[[date-family-results]]"]
---

## Goal
Match func_80144E20 and func_801449D4 in src/ovl/DATE/80141EE0.c, and func_8015A640 in src/ovl/DATE/8015A640.c.

## Acceptance criteria
Follow T-24010 scope and verification gates; preserve matching siblings, useful drafts and concrete negative evidence. Review before Done.

## Comments
Assigned isolated worker; no other C files claimed.

2026-10-17: Docker source-only probes found exact-frame near misses for both dispatchers; details in [[date-family-results]]. No production changes or verified bytes yet. The small body improved to 11 isolated resolved +/- rows; the large to 72. Return-view probes unchanged. T-17208 already establishes that func_8015A640 is hand-written libgte assembly, so no repeated compiler sweep was attempted.

2026-10-17 checkpoint: retained both improved source-only dispatcher drafts and safe diagnostic/stage harnesses under `drafts/T-24011/`. Full-unit resolved diagnostics: small 48 positional +/- rows, large 72; isolated small 11 uses edit alignment and terminal padding. Astra follow-up unchanged; observed small constant hoisting happens after the cc-S stage during as1 optimization, with no proof of an assembler-version mismatch. Production src/include unchanged. Docker validation: DATE checksum OK; headers OK; globals OK; branch check OK (0 new, 15 known); default resolved linked fallback checks MATCH 1100/3200/416 bytes. Zero new C bytes. Required two-axis code review remains with the coordinator; status In Review.
