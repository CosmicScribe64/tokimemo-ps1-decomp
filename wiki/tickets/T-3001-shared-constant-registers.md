---
id: T-3001
title: Constants reused across stores and compare/store types
status: Done
assignee: r3-consts
created: 2026-10-09
updated: 2026-10-10
links: ["[[tickets/T-1321-register-promotion-build-step]]", "[[matching-notes]]"]
---

## Goal

Find the rule for two constant-register differences that `tools/cvt_pass.py` (T-1321) does not cover.

## Notes

- Straight-line stores of repeated constants (DATE `func_8013DCC0`: 3,3,4,4,2,2,3,4,2 into `s16` globals): the original loads each constant once into `$t6/$t7/$t8` and reuses it; IDO 5.3 with `-Wo,-nokpicopt` emits one `li` per store. The ucode constants already have one type (`LDC J len=2`), so type unification does not help; `-Wo,-zmovc:0` does not either.
- Loop constants shared between a compare and a store of different width (TT `func_80149108`: `0x40` compared against a `u16` field and stored to it; the original uses one saved register). cfe types the compare constant `LDC J len=4` and the store constant `LDC L len=2`, so uopt sees two constants. Rewriting every integer `LDC` to `J len=4` before uopt (scratch experiment) makes IDO share the register and changes none of the matched functions, but the saved-register order of the other loop constants (8, 0x20, 0x80) still differs.

## Acceptance criteria

- [x] A rule for each case or a written verdict.

## Comments

- 2026-10-10 (r3-consts, with [[tickets/T-5020-loop-unrolling-and-lui-sharing]]): verdict toolchain, no fix. Over the corpus the original re-loads a repeated straight-line store constant 1052 times and reuses a register 124 times; IDO with `-Wo,-nokpicopt` always re-loads, stock IDO always reuses (in `$v0/$v1`), and the same function can do both in the original (`func_800419FC` re-loads 0x28 four times; DATE `func_8013DCC0` reuses 3/4/2). Locals, `register` locals, chain assignment and 40 uopt option settings do not change IDO's output. The `li at,1` per compare and the straight-line `li 0x38; multu` are the same family. Details: [[matching-notes]], section (a). Review: documentation only, no code; done.
