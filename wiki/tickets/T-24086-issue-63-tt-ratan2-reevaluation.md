---
id: T-24086
title: Re-evaluate Issue 63 (TT func_8014D260 ratan2) toolchain, provenance, and matching route
status: In Progress
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Issue #63 (`TT:func_8014D260` in `src/ovl/TT/8014D260.c`, 388 B) was previously left as `INCLUDE_ASM` under [[tickets/T-11890-tt-ratan2-aspsx]], which concluded that the three differences between `gcc 2.7.2-psx` + `maspsx` output and the target binary (`div` with early `mflo`, `mflo` hazard nops before branches/labels, and `lh rd, sym(rs)` expanded through `rd` instead of `$at`) did not come from the tested Win32/DOS `ASPSX.EXE` binaries.

However:
1. Why is this single function in `TT` compiled with `gcc` rather than IDO like the rest of `TT`? (Is `TT/8014D260` a linked SDK library object `RATAN.OBJ` from `LIBGTE.LIB`, or game C, or can IDO / another compiler+assembler combination in our toolchain produce it?)
2. Those exact three habits appear systematically across the main executable's SDK libraries (`asm/lib*.s`: 290/290 `mflo` sites, 103/103 `l* rd, sym(rs)` with `rd != rs` sites, 63/67 `div` sites). Why do all SDK objects in `SLPM_86.053` and `TT/8014D260` share these exact three habits, what tool actually produced them (e.g. an older MIPS assembler / `as1` / SGI / Sony tool or `maspsx` configuration), and what would it take to match `func_8014D260` cleanly in our build process?

## Acceptance criteria

- [ ] Audit `asm/ovl/TT/nonmatchings/TT/8014D260/func_8014D260.s`, neighboring `TT` objects, `asm/libgte.s`, `tools/aspsx_survey.py`, and `tools/cc.py`
- [ ] Test whether `func_8014D260` can be matched with IDO, gcc + as1, gcc + maspsx flags, or a clean, well-evidenced toolchain step
- [ ] Report clear technical findings and actionable options to the user

## Notes

- Read-only / scratch investigation in `build/scratch/t24086/` (do not modify `src/ovl/TT/` without user confirmation).
