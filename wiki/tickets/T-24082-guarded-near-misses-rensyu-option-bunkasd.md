---
id: T-24082
title: Close guarded 4-8 row near misses in RENSYU, OPTION, and BUNKA_SD
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Three `#ifdef NON_MATCHING` guarded functions in non-reserved overlays sit within 4 to 8 diff rows of a byte match:

- `RENSYU:func_80133628` (920 B in `src/ovl/RENSYU/80132040.c`, 4 diff rows)
- `OPTION:func_8013394C` (396 B in `src/ovl/OPTION/80132000.c`, 5 diff rows)
- `BUNKA_SD:func_80134418` (144 B in `src/ovl/BUNKA_SD/80134260.c`, 4 diff rows)

## Acceptance criteria

- [x] Close `OPTION:func_8013394C` (396 B) to `score=0`
- [x] Close `BUNKA_SD:func_80134418` (144 B) to `score=0`
- [x] Diagnose `RENSYU:func_80133628` (4 rows) and document exact `uopt`/`ugen` `I_IALIAS` binasm barrier in `NON_MATCHING` comment
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- Matched 2 of 3 functions (540 B total):
  - `BUNKA_SD:func_80134418` (144 B in `src/ovl/BUNKA_SD/80134260.c`): single-expression `*(u8 *)&D_800E6280.unk_110B = *(u8 *)&D_800E6280.unk_110B * 0 + 0x80;` folds the cap constant and its narrowing in one `uopt` pass while keeping `unk_110B` in the single promoted `$v1` register range (`score=0`).
  - `OPTION:func_8013394C` (396 B in `src/ovl/OPTION/80132000.c`): reading `g->unk_1094` through `GameState *g = &D_800E6280;` in both the loop test and `switch (g->unk_1094)` makes `uopt` CSE the `ILOD` into `$a2` while preserving `move $v0, $a2` in the first switch branch delay slot (`score=0`).
- `RENSYU:func_80133628` remains guarded at 4 rows; documented the `uopt` `UNAL(0,$4,$gp)` / `ugen` `.alias $4,$gp` (`I_IALIAS`) binasm boundary before the `case 0:` label.
