---
id: T-24086
title: Re-evaluate Issue 63 (TT func_8014D260 ratan2) toolchain, provenance, and matching route
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Issue #63 (`TT:func_8014D260` in `src/ovl/TT/8014D260.c`, 400 linked bytes / 388 unpadded bytes) was previously left as `INCLUDE_ASM` under [[tickets/T-11890-tt-ratan2-aspsx]], which concluded that the three differences between `gcc 2.7.2-psx` + `maspsx` output and the target binary (`div` with early `mflo`, `mflo` hazard nops before branches/labels, and `lh rd, sym(rs)` expanded through `rd` instead of `$at`) did not come from the tested Win32/DOS `ASPSX.EXE` binaries.

## Acceptance criteria

- [x] Audit `asm/ovl/TT/nonmatchings/TT/8014D260/func_8014D260.s`, neighboring `TT` objects, `asm/libgte.s`, `tools/aspsx_survey.py`, and `tools/cc.py`
- [x] Verify native MIPS `as0` + `as1` behavior on `gcc 2.7.2-psx` output and implement uniform Sony workstation assembler emulation pass `tools/sdk_aspsx_pass.py` + unit tests `tools/test_sdk_aspsx_pass.py` per `CODING_STANDARDS.md` section 7a
- [x] Wire `TT/8014D260 gcc 2.7.2-psx 2.56` into `config/toolchains.txt` and match `src/ovl/TT/8014D260.c` at 100% (`score=0`, 400 linked bytes, `303/303` C functions in `TT`)
- [x] Survey all remaining `INCLUDE_ASM` functions in `src/` and all 49 `asm/lib*.s` files in `SLPM_86.053` for other modules using this toolchain

## Notes

- **Provenance**: `TT/8014D260` (`0x8014D260..0x8014D3F0`) and `D_80159B20` (`ratan_tbl`, `0x80159B20..0x8015A322`, 2,050 B) are Sony's precompiled `LIBGTE.LIB(RATAN.OBJ)` linked into the tail of `TT.EXN` because `SLPM_86.053` did not reference `ratan2`.
- **Assembler Root Cause**: Assembling untouched `gcc 2.7.2-psx` `.s` output with native MIPS `as0` + `as1` (`/opt/ido/5.3/usr/lib/as0` + `as1`) directly reproduces `lh rd, sym(rs)` through `rd` (`103/103` in `asm/lib*.s`) and the two-slot `mflo` hazard window (`290/290` in `asm/lib*.s`). Moreover, `asm/libspu.s` mixes 25 MIPS ucode `cc` functions and 3 GCC `cc1` functions that both share all three habits, proving the habits come from Sony's NEWS-OS / MIPS workstation assembler (`as1` derivative) rather than `cc1`.
- **Broader Survey**:
  - Among the 424 remaining `INCLUDE_ASM` functions in `src/main/` and `src/ovl/`, **0** are GCC-compiled C (418 are Konami IDO 5.3 C; 6 are hand-written `LIBGTE` `RotMatrixX/Y/Z` assembly in `TACO` and `DATE`). All 2 GCC-compiled SDK functions in overlays (`TAIIKU:func_801488F0` and `TT:func_8014D260`) are now matched in C, and `TT` is 100% matched in C (`303/303` functions).
  - Across the 49 main-exe SDK files (`asm/lib*.s`, 722 functions), ~445 functions (`libgs`, `libsnd`, `libgpu`, `libcd`, `libetc`, `libpress`, `libcard`, `libapi_counter`, `libgte` C modules, and the second half of `libspu`) use this exact `gcc 2.7.2-psx` + `sdk_aspsx_pass.py` pipeline.
