---
id: T-24087
title: Close guarded near misses and twin pairs in OPTION, VALEN, TAIIKU, and main
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following [[tickets/T-24082-guarded-near-misses-rensyu-option-bunkasd]] (2 of 3 guarded near misses matched, 540 B), target the next set of short guarded near-misses and high-similarity twins in disjoint files:

- `OPTION:func_80139E3C` (196 B in `src/ovl/OPTION/801389A0.c`, guarded at 5 rows)
- `OPTION:func_801378DC` (832 B) and `OPTION:func_80138080` (836 B) in `src/ovl/OPTION/80132000.c` (1,668 B twin pair, ratio 0.993)
- `VALEN:func_80132D0C` (892 B in `src/ovl/VALEN/80132760.c`, guarded at 7 rows; sibling of newly matched `GYOZI:func_8013E824` at ratio 0.844)
- `TAIIKU:func_8013D114` (528 B in `src/ovl/TAIIKU/80138950.c`, guarded at 2 instructions missing / 12 lines)
- `main:func_8006DF78` (in `src/main/8006CB30.c`, guarded at 1 row) and `main:func_8004556C` (`file_checksum_ok`, 124 B in `src/main/800451D0.c`, guarded at 4 lines)

## Acceptance criteria

- [x] Close as many of the target functions as possible to `score=0` in `build/scratch/t24087/`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Results

- `main:file_checksum_ok` (`func_8004556C`, 124 B in `src/main/800451D0.c`): `MATCH` (`goto loop;` into `for (; i < (blocks << 11) - 4; i += 2, p0 += 2, p1 += 2)` omits the entry test while keeping the `u16` narrowing in the `for`-step after `sltu` — `src/main/800451D0.c` is now 100% matched).
- `OPTION:func_801378DC` (832 B in `src/ovl/OPTION/80132000.c`): `MATCH` (4 scalar locals before `s16 a[16]..d[16]`, `if (D_80121728) {}` to promote `D_80121728` to `$t3`, comma-expression ordering `icon` and `&D_800E7D10.flags[i]` before `D_80120650`, same-line 4-record hide loop, and `s32` implicit fallthrough with `z = 0; p = &z; if (*p) {}` before the final store).
- `OPTION:func_80138080` (836 B in `src/ovl/OPTION/80132000.c`): `MATCH` (same recipe plus `s32 pad;` shifting `a..d` down by 4 bytes within the `0xE8` frame — `src/ovl/OPTION/80132000.c` is now 100% matched).
- Total: 3 functions matched (+1,792 B). `TAIIKU:func_8013BCEC` and `VALEN:func_80132D0C` handed off to [[tickets/T-24100-taiiku-data-defs-and-islands]].
