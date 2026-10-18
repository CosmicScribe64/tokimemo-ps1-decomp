---
id: T-24084
title: Match SHOUGATU and GYOZI sibling batch B (func_801359FC, func_8013C118, func_8013E824)
status: Done
priority: high
created: 2026-10-18
updated: 2026-10-18
depends_on: []
---

## Context

Following the completion of [[tickets/T-24080-gyozi-shougatu-twins]] (4 functions, 1,840 B matched across `GYOZI` and `SHOUGATU`), target the next short/medium functions in `SHOUGATU` and `GYOZI`:

- `SHOUGATU:func_801359FC` (276 B in `src/ovl/SHOUGATU/80134890.c`, sibling of `GYOZI:func_80137188`)
- `GYOZI:func_8013C118` (504 B in `src/ovl/GYOZI/8013BD50.c`, sibling of `SHOUGATU:func_8013960C`)
- `GYOZI:func_8013E824` (748 B in `src/ovl/GYOZI/8013E5B0.c`, sibling of `SHOUGATU:func_8013BDE0` and `GEKO:func_80133598`)

## Acceptance criteria

- [x] Match `SHOUGATU:func_801359FC` (276 B), `GYOZI:func_8013C118` (504 B), and `GYOZI:func_8013E824` (748 B) to `score=0`
- [x] Promote all matched functions to unguarded default C and verify `tools/funcdiff.py --resolve`, `ninja`, `tools/check_headers.py`, `tools/migrate_globals.py --check`, and `tools/sync_protos.py --check-branch`
- [x] Review against `CODING_STANDARDS.md` and record findings in `wiki/log.md`

## Notes

- All 3 functions matched (1,528 B total):
  - `SHOUGATU:func_801359FC` (276 B in `src/ovl/SHOUGATU/80134890.c`): `ShoStr7x5 labels = D_80143E80;` with `s32 pad;` declared before `labels` (T-3330 stack offset `+0x28`) and direct operations on `u8` global `D_80143DD8` (`score=0`).
  - `GYOZI:func_8013C118` (504 B in `src/ovl/GYOZI/8013BD50.c`): T-9020 indirect byte-store and reload `*(u8 *)&D_800F5ACD = *(u8 *)&D_800F62CF = 7; func_8008F618(*(u8 *)&D_800F62CF);` (with `FAKE` annotation) shares `$a0` across both stores and the call (`score=0`).
  - `GYOZI:func_8013E824` (748 B in `src/ovl/GYOZI/8013E5B0.c`): `s32 r; u8 s; r = func_8005E0E0(D_800F62CF); s = r & 0x7F;` (matching `GEKO:func_80133598`, zero `FAKE` tricks) produces the exact `move $v1, $v0; andi $t6, $v1, 0x7F` prologue and case register numbering (`score=0`).
