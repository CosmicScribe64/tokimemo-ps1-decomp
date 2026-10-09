---
id: T-0017
title: Reconcile -Wo,-no_const_in_reg with loop hoisting in the original
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[matching-notes]]", "[[toolchain]]", "[[tickets/T-0014-find-exact-ucode-compiler]]", "[[tickets/T-0016-frame-layout-emulation-pass]]"]
---

## Goal

The project flag `-Wo,-no_const_in_reg` (T-0014) is needed by `func_80042400` (a global read-modify-write must reload `%hi/%lo` per access) but prevents matching every function whose original keeps loop-invariant integer constants or global addresses in registers (`li v1,1` before a loop, `lui/addiu` before a loop, `multu` by a register holding 0x44). Find a uniform setting that reproduces both.

## Acceptance criteria

- [x] A toolchain setting (flag combination or a documented emulation step) that keeps `func_80042400` matching and matches `func_8013815C`, `func_801446A0`, `func_80146FA0` (TAIIKU), and ideally `SD_DetectCDPeak`, `strSync`. Done: `-Wo,-nokpicopt` replaces `-Wo,-no_const_in_reg`. `strSync` is down to one delay slot; `SD_DetectCDPeak` not attempted.
- [x] If none exists: decision recorded. Not needed.

## Notes

Evidence from T-0016: without the flag, every C function in `src/game.c` matches except `func_80042400`; the three TAIIKU leaf functions match byte for byte once the flag is removed (compile `src/ovl/TAIIKU.c` with `-DNON_MATCHING`). None of the 19 uopt options tried one at a time (`no_const_in_reg`, `do_opt_saved_regs`, `noheurAB`, `norlodrstropt`, `noprecolor`, `doassoc`, `docopy`, `nogenvreg`, `norecur`, `docodehoist`, `notail`, `nordstore`, `createbb`, `moremotion`, `noPalias`, `static`, `varref`, `nokpicopt`, `loopunroll`) gives both. Pairs and ugen options are untested.

## Comments

2026-10-09 (branch t0018-regs): `-Wo,-nokpicopt` is the setting. Without `-no_const_in_reg`, stock IDO keeps both integer constants and global addresses in registers; `-nokpicopt` stops only the address of a directly accessed scalar global, which is exactly the original's split (scans and evidence in [[matching-notes]], section "Constants in registers"). The T-0750 `* 0x44` -> `multu` gap has the same cause (constant multiplier kept in a register, so ugen never strength-reduces it), not an older ugen. Every IDO 5.3/7.1 per-pass mix gives the same code. Regression check over all C: only `check_end_k` (operand order) and `func_80042400` (now plain `D += 0x377; return D;`) needed source edits. Unit tests: `tools/test_cc.py` class `IdoFlags` (fail under the old flag and under stock flags). 13 functions newly matched; `ninja progress` 151 -> 164, 8132 -> 9580 bytes; clean rebuild 27/27 sha1 OK. Follow-up batch: [[tickets/T-0950-match-nokpicopt-unblocked-functions]].

2026-10-09 code-review gate (inline, CODING_STANDARDS checklist) on commits 95e334f..HEAD of t0018-regs: matches verified by clean rebuild (27/27 sha1 OK, `ninja progress` 164/6962); TAIIKU leaves keep their `FAKE` comment; `strSync` `NON_MATCHING` comment updated with ticket ids; C89, fixed-width types, new overlay headers have guards; tests run in Docker (`test_cc.py` 6 OK, `test_frame_pass.py` 20 OK). Findings: (1) the flag change broke the `indexed` IDO snippet of `tools/test_frame_pass.py` (uopt now hoists the local array base) - fixed, snippet rewritten plus a `hoisted` snippet; (2) `check_end_k`'s operand order needed a comment (section 9) - added. No open findings. Done.
