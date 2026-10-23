---
id: T-24602
title: Match GEKO next-week date selector
status: Done
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24600-finish-matching]]", "[[tickets/T-23005-w5-big-p05]]"]
---

## Goal

Promote GEKO `func_80144198` after literal verification of its 216-byte date-selector reconstruction.

## Acceptance criteria

- [x] Default C matches all 216 literal linked bytes.
- [x] Full build checksums and declaration guards pass in Docker.
- [x] Standards and behavior reviews have no unresolved findings.

## Notes

The helper copies a twelve-month length table, applies February 29 for year 0x60, computes day minus weekday plus six, and carries an overflow into the following month/year. Selector zero returns year, one returns month, and other values return day.

Scalar result locals produce an extra expression range or two wrong register operands. A three-word year/month/day result array removes that conflict. Three initial dead stores and the first empty selector test establish the observed live-range order; the second test was removed after verification. The local table follows the scalar length and result array to reproduce its 0x18 stack offset and 0x40 frame.

## Comments

- 2026-10-20: Full translation-unit scratch compile gives zero resolved edit cost and 54 instructions. Literal linked proof, minimized source, full build, and reviews are pending; no coverage credited yet.
- 2026-10-20: Private-object and production `funcdiff.py --resolve` both report literal MATCH, 216 bytes. Docker `ninja -j 4` succeeds (GEKO hash and globals rerun; other binary/header targets remain current). Moved to In Review. Both independent reviewers report no findings against standards or assembly behavior.
- 2026-10-20: Fresh Docker hash verification passes for all 27 executables. Standards and spec reviews have no findings; all acceptance criteria met, In Review -> Done. Census is 6,818/6,952 functions, 2,092,820/2,276,920 bytes; 134 functions remain.
