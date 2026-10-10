---
id: T-3001
title: Constants reused across stores and compare/store types
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-1321-register-promotion-build-step]]", "[[matching-notes]]"]
---

## Goal

Find the rule for two constant-register differences that `tools/cvt_pass.py` (T-1321) does not cover.

## Notes

- Straight-line stores of repeated constants (DATE `func_8013DCC0`: 3,3,4,4,2,2,3,4,2 into `s16` globals): the original loads each constant once into `$t6/$t7/$t8` and reuses it; IDO 5.3 with `-Wo,-nokpicopt` emits one `li` per store. The ucode constants already have one type (`LDC J len=2`), so type unification does not help; `-Wo,-zmovc:0` does not either.
- Loop constants shared between a compare and a store of different width (TT `func_80149108`: `0x40` compared against a `u16` field and stored to it; the original uses one saved register). cfe types the compare constant `LDC J len=4` and the store constant `LDC L len=2`, so uopt sees two constants. Rewriting every integer `LDC` to `J len=4` before uopt (scratch experiment) makes IDO share the register and changes none of the matched functions, but the saved-register order of the other loop constants (8, 0x20, 0x80) still differs.

## Acceptance criteria

- [ ] A rule for each case or a written verdict.

## Comments
