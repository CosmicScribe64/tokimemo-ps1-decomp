---
id: T-0017
title: Reconcile -Wo,-no_const_in_reg with loop hoisting in the original
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[tickets/T-0014-find-exact-ucode-compiler]]", "[[tickets/T-0016-frame-layout-emulation-pass]]"]
---

## Goal

The project flag `-Wo,-no_const_in_reg` (T-0014) is needed by `func_80042400` (a global read-modify-write must reload `%hi/%lo` per access) but prevents matching every function whose original keeps loop-invariant integer constants or global addresses in registers (`li v1,1` before a loop, `lui/addiu` before a loop, `multu` by a register holding 0x44). Find a uniform setting that reproduces both.

## Acceptance criteria

- [ ] A toolchain setting (flag combination or a documented emulation step) that keeps `func_80042400` matching and matches `func_8013815C`, `func_801446A0`, `func_80146FA0` (TAIIKU), and ideally `SD_DetectCDPeak`, `strSync`.
- [ ] If none exists: decision recorded (drop the flag and make `func_80042400` `NON_MATCHING`, or keep it).

## Notes

Evidence from T-0016: without the flag, every C function in `src/game.c` matches except `func_80042400`; the three TAIIKU leaf functions match byte for byte once the flag is removed (compile `src/ovl/TAIIKU.c` with `-DNON_MATCHING`). None of the 19 uopt options tried one at a time (`no_const_in_reg`, `do_opt_saved_regs`, `noheurAB`, `norlodrstropt`, `noprecolor`, `doassoc`, `docopy`, `nogenvreg`, `norecur`, `docodehoist`, `notail`, `nordstore`, `createbb`, `moremotion`, `noPalias`, `static`, `varref`, `nokpicopt`, `loopunroll`) gives both. Pairs and ugen options are untested.

## Comments
