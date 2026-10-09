---
id: T-0950
title: Match functions unblocked by -Wo,-nokpicopt
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-0017-const-in-reg-loop-hoisting]]", "[[matching-notes]]"]
---

## Goal

Decompile the functions that were blocked only by `-Wo,-no_const_in_reg` and now compile like the original with `-Wo,-nokpicopt` (T-0017): loops whose bound or multiplier the original keeps in a register (scan: 1020 functions with a hoisted loop bound, 241 with `multu` by a `li`-loaded register, main exe and overlays).

## Acceptance criteria

- [ ] Batches of matched functions committed per overlay/file, 27 of 27 sha1 OK after each.
- [ ] Functions that still fail are listed in [[matching-notes]] with the reason.

## Notes

Find candidates: the original has `addiu rX, $zero, N` before a loop and `bne/beq` against `rX`, or `multu` by such a register. Small examples already matched in T-0017: `func_80048E78`, `get_h_tokimeki_table`, `func_80140788` (DATE). Loops indexed by `s16` keep the `multu` (no induction-variable strength reduction). Functions that also read a scalar global in several blocks hit the T-0018 gap.

## Comments
