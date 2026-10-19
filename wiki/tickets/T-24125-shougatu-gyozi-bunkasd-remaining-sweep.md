---
id: T-24125
title: Sweep remaining SHOUGATU, GYOZI, and BUNKA_SD functions
status: In Progress
priority: high
type: code
created: 2026-10-19
updated: 2026-10-19
depends_on:
  - T-24114
  - T-24116
---

## Summary

Attack and close remaining `INCLUDE_ASM` functions in `SHOUGATU`, `GYOZI`, and `BUNKA_SD`:
- **`BUNKA_SD`**:
  - `src/ovl/BUNKA_SD/801388D0.c`: `func_80139290` (1,516 B — reached exact 378/378 instruction count in `T-24114` with `((DisplayActor44 *)D_80120650)[5].unk_18` and `switch (D_800E6280.unk_1104.w)`, only 2 regalloc diffs remaining!)
  - `src/ovl/BUNKA_SD/80134540.c`: `func_80134D94` (960 B)
- **`SHOUGATU`**:
  - `src/ovl/SHOUGATU/8013D660.c`: `func_8013D820` (`score=13`), `func_8013D92C`, `func_8013DAAC`
  - `src/ovl/SHOUGATU/801390C0.c`: `func_80139334` (`score=17`)
  - `src/ovl/SHOUGATU/8013BA00.c`: `func_8013BA7C` (`score=17`)
  - `src/ovl/SHOUGATU/8013E330.c`: `func_8013E50C`, `func_8013E6E4`, `func_8013EA98`
  - `src/ovl/SHOUGATU/80134120.c` (`func_801347E0`), `80134890.c` (`func_801350F8`), `80137FF0.c` (`func_80138240`), `8013F190.c` (`func_8013FB28`)
- **`GYOZI`**:
  - `src/ovl/GYOZI/80136220.c` (`func_801368B8`), `80139920.c` (`func_80139E4C`), `80140C90.c` (`func_80140E0C`, `func_80140F90`, `func_801412A4`), `801418B0.c` (`func_801421B4`)
