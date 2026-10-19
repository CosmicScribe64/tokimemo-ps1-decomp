---
id: T-24126
title: Sweep remaining GEKO, OPTION, TEL, BUNKAKEN, and OMIMAI functions
status: Done
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24115
  - T-24119
  - T-24121
---

## Summary

Attack and close as many of the remaining `INCLUDE_ASM` functions across `GEKO` (12), `OPTION` (12), `TEL` (3), `BUNKAKEN` (1), and `OMIMAI` (1) as possible:
- `src/ovl/BUNKAKEN/8013B510.c`: `func_8013B510` (only 1 function left in `BUNKAKEN`)
- `src/ovl/OMIMAI/801331B0.c`: `func_8013388C` (only 1 function left in `OMIMAI`)
- `src/ovl/TEL/80132000.c` & `8013CCD0.c`: `func_80134FE4`, `func_80136724`, `func_8013D88C` (only 3 functions left in `TEL`)
- `src/ovl/OPTION/80132000.c`, `801389A0.c`, `8013A550.c`: `func_80132000`, `func_801320C0`, `func_80132384`, `func_801326BC`, `func_80132BA8`, `func_80133010`, `func_801344E4`, `func_801394F4`, `func_80139A30`, `func_80139E3C`, `func_80139F8C`, `func_8013A7EC`
- `src/ovl/GEKO/*.c`: `func_80132B24`, `func_80133024`, `func_80133308`, `func_80133C50`, `func_80134808`, `func_801358CC`, `func_80135DC0`, `func_801367EC`, `func_80136C20`, `func_801420E4`, `func_80142C9C`, `func_80144198`

## Results

- Matched **3 functions (+2,480 bytes)** across `OPTION` and `TEL`:
  - `OPTION:func_801344E4` (800 B in `src/ovl/OPTION/80132000.c`)
  - `OPTION:func_801394F4` (900 B in `src/ovl/OPTION/801389A0.c`)
  - `TEL:func_80134FE4` (780 B in `src/ovl/TEL/80132000.c`)
- Added `#ifndef MAIN_API_OVERRIDE_set_c_girl` + `void set_c_girl(u8 arg0);` in `include/main_api.h` and `#define MAIN_API_OVERRIDE_set_c_girl` + `void set_c_girl();` at the top of `src/ovl/TEL/80132000.c` per `CODING_STANDARDS.md`.

