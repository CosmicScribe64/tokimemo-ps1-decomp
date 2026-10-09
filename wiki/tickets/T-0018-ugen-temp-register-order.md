---
id: T-0018
title: ugen temporary register order differs from the original
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[tickets/T-0016-frame-layout-emulation-pass]]"]
---

## Goal

In `func_8004435C`, `func_800443F0` and `func_800443A0` the original loads the u16 parameter homes (`lhu`) into `t8,t9` and `t7,t8,t9`, where IDO 5.3 (with or without the frame pass) uses `t0,t1` and `t8,t9,t0`. Find what produces the original's register order (source form, option, or compiler difference).

## Acceptance criteria

- [ ] Cause identified or documented as a compiler difference; the three functions match or stay `NON_MATCHING` with a written reason.

## Notes

Not a frame issue: stock IDO without the pass gives the same registers. Also open: `func_8004111C` (original stores h and w before x and y), `func_80044700` (original frame has 8 more bytes of locals), `func_80056AA8` (no reload of the decremented volatile).

## Comments
