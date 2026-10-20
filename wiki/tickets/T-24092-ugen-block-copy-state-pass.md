---
id: T-24092
title: Prototype and evaluate ugen block-copy state (F vs B) uniform pass
status: Done
priority: high
type: tooling
created: 2026-10-18
updated: 2026-10-18
---

## Summary

Investigated and implemented `tools/ugen_pass.py`, a uniform ucode-to-binasm shim for IDO 5.3's `ugen` block-copy direction state (`F` vs `B`) difference documented in `[[build-step-gaps]]`, `[[ido-52-evaluation]]`, `[[tickets/T-20710-true-gap-census]]`, and `[[tickets/T-24085-event-geko-twins-b]]`.

## Findings & Implementation

1. **Root Cause in IDO 5.3 `ugen` (`f_eval_mov` at `0x45f59c`):**
   - `f_eval_mov` reads an uninitialized byte on its own stack frame at `sp + 240` (`0xfffe0c0`), which holds the high halfword of the 32-bit word at `0xfffe0c0` (`< 0x7ff8` -> State `F` forward copy, `>= 0x7ff8` -> State `B` backward copy).
   - Between IDO 4.1 and IDO 5.2/5.3, SGI added 64-bit `long long` branch helpers `f_emit_branch_rill` (`0x44f4d4`) and `f_emit_branch_rrll` (`0x44fb60`) inside `f_jump`, shifting `f_jump`'s helper stack frames down by 16 bytes so that non-`zblt` branches (`if (x == 0)`, `Uild` loops, 2-register loops) overwrite `0xfffe0c0` with `0` (`< 0x7ff8`), flipping subsequent block copies to State `F`.
2. **Uniform Pass (`tools/ugen_pass.py` + `tools/test_ugen_pass.py`):**
   - Wraps `/opt/ido/5.3/ugen` via `tools/cc.py` (`SHIMS = {"as1": FRAME_PASS, "uopt": CVT_PASS, "ugen": UGEN_PASS}`).
   - For procedures containing `Umov`, tracks whether `0xfffe0c0` is in State `B` or State `F` (set to `F` only immediately after `Uistr`/`Uistv` or after a `Uistr`/`Uistv` followed by a signed-immediate `<` loop branch `zblt` with `imm > 0`), injects a zero-register ucode sentinel store (`Uldc 0; Ustr M -0x7777` for `B`, or `Uldc 0; Uldc 0; Uistr -0x7777` for `F`) immediately before `Umov`'s two address operands, and strips the resulting `zsb $zero` binasm record from `.G` before `frame_pass.py` / `as1` runs.
   - Zero regressions (`0 / 6,337` matched functions across all 431 compilation units) and 17/17 unit tests in `tools/test_ugen_pass.py` pass.
3. **12 Promoted Block-Copy Functions (+6,748 B verified default C, plus `BUNKASAI:func_801509A4` 472 B already promoted in `T-24094` = 13/13 total):**
   - `DATE:func_801518B8` (360 B in `src/ovl/DATE/80151460.c`): `MATCH`
   - `TACO:func_8013B2A0` (248 B in `src/ovl/TACO/8013B170.c`): `MATCH`
   - `TACO:func_801472D4` (300 B in `src/ovl/TACO/80146D60.c`): `MATCH`
   - `TACO:func_8015B1B8` (244 B in `src/ovl/TACO/8015ABF0.c`): `MATCH`
   - `main:func_80054C74` (504 B in `src/main/80053650.c`): `MATCH`
   - `TAIIKU:func_80140A90` (2,032 B in `src/ovl/TAIIKU/80140810.c` — **`src/ovl/TAIIKU/80140810.c` is now 100% matched!**): `MATCH`
   - `SHUGAKU:func_801357AC` (396 B in `src/ovl/SHUGAKU/80135430.c`): `MATCH`
   - `SHUGAKU:func_80135EA4` (524 B in `src/ovl/SHUGAKU/80135430.c` — **`src/ovl/SHUGAKU/80135430.c` is now 100% matched!**): `MATCH`
   - `GEKO:func_801406CC` (780 B in `src/ovl/GEKO/8013F360.c`): `MATCH`
   - `EVENT:func_80109404` (768 B in `src/ovl/EVENT/80107FC0.c` — **`src/ovl/EVENT/80107FC0.c` is now 100% matched!**): `MATCH`
   - `GYOZI:func_8014494C` (296 B in `src/ovl/GYOZI/80143AD0.c` — **`src/ovl/GYOZI/80143AD0.c` is now 100% matched!**): `MATCH`
   - `SHOUGATU:func_8014265C` (296 B in `src/ovl/SHOUGATU/80141780.c` — **`src/ovl/SHOUGATU/80141780.c` is now 100% matched!**): `MATCH`
