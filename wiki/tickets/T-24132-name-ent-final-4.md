---
id: T-24132
title: Close remaining 4 unmatched functions of NAME_ENT (func_80138C1C, func_801398EC, func_8013AF78, func_80143580)
status: In Review
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24112
  - T-24113
---

## Summary

Attack the final 4 remaining unmatched functions in `NAME_ENT` (`func_80138C1C` in `src/ovl/NAME_ENT/80132000.c`, `func_801398EC` and `func_8013AF78` in `src/ovl/NAME_ENT/80139740.c`, and `func_80143580` in `src/ovl/NAME_ENT/80142A80.c`).

## Results

0 of 4 matched (all regalloc/compiler-gap bound); 3 guarded `NON_MATCHING` drafts, production ASM unchanged:

- `NAME_ENT:func_8013AF78` (296 B): guarded (~99 resolved rows). Frame exact (0x30), s32 fallthrough exit, arg0 == 0 path exact. Open: sltu pointer zeroing loop (for gives bne) and raw byte loads in $a2/$a1/$v1/$v0 (draft spills; extra arg0 move). T-22023 draft.
- `NAME_ENT:func_801398EC` (492 B): guarded (~266 resolved rows, all regalloc). Shape follows the asm and matched sibling `func_80139F30`: `D_8014D0D4 % 3` selector, 13 `NameRecordF00.flags` bytes (stride 0xF00 by `D_800E8BEF`) driving `Rec24` records 0..12 two per trip, `(i/8)*0x30` signed plus `((unk_10F8>>2)%3)*0x10+0x80` sprite byte. Original keeps base/index/frame in $t0/$a3/$a2 across trips; draft reloads in-loop.
- `NAME_ENT:func_80138C1C` (740 B): guarded clean draft per the T-11650/T-22008 recipe (clear loops, three skip-space/zero copy loops, buf combine, month/day/blood nibbles). Blocked by the T-0018 promo row: no plain-C form hoists both `D_800B3644` bytes before the copy loops.
- `NAME_ENT:func_80143580` (4,308 B): stays `INCLUDE_ASM`; T-20008 archived the reconstruction, no new C form attempted (20-case state machine).
- `include/ovl/NAME_ENT.h`: added `void func_801398EC(void);` and `s32 func_8013AF78(s32 arg0);`.

Verification (Docker): `--version default` MATCH on all 4 (production bytes unchanged); `--version c` compiles all 3 guards (DIFF as documented); `check_headers` OK; `sync_protos --check-branch` OK. Full-tree `ninja` still stops in another lane's file (`src/ovl/SHOUGATU/80135B90.c`, untouched here).
