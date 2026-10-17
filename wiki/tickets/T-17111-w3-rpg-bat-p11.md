---
id: T-17111
title: Wave 3 package w3-rpg-bat-p11
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the two listed RPG_BAT functions, best first: `func_80144010` (1,196 B, `80144010.c`, T-14500 draft score 333) and `func_801505B8` (1,376 B, `801504E0.c`, T-17100 draft score 233). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17111 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17111)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p11`, branch `w3-rpg-bat-p11`, base `w2-good`. Owns `src/ovl/RPG_BAT/801504E0.c` and `src/ovl/RPG_BAT/80144010.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:21 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80144010` | 1,196 | `INCLUDE_ASM` (T-14500 draft, score 333) | matched, plain C: club index kept in the global `D_8015EBA0`, `D_8015EB9C` chain without a local, `ScheduleClubFlags`/`GsRec66C` loop |
| `func_801505B8` | 1,376 | `INCLUDE_ASM` (T-17100 draft, score 233) | matched, plain C: `RPG_BAT_OVERRIDE_D_8015EDAC` (`s32` selector), display-record fields as `DisplayActor44` elements |

2 of 2 matched, no guard left, no FAKE statement (about 30 minutes). Method and findings: [[matching-notes]] (section T-17111), four new rows in [[register-cheatsheet]], [[rpg-bat-family-results]]. Header changes: `RPG_BAT.h` (prototypes `func_80150AA4`, `func_80151108`, `func_80151380`, `func_801513C8`, `func_801515F0`, `func_801447B8`; externs `D_8015EBA8`, `D_8015EBD8`; `#ifndef` wrapper for `D_8015EDAC`).

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p11`)

Started 20:21 UTC, both functions matched by 20:51 UTC. Clean rebuild 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK. Moved to In Review; code review against `CODING_STANDARDS.md` pending.

### 2026-10-16 code review (claude-opus)

Reviewed `w2-good...w3-rpg-bat-p11` against `CODING_STANDARDS.md`: no findings. Only the two owned C files (`src/ovl/RPG_BAT/80144010.c`, `src/ovl/RPG_BAT/801504E0.c`), `include/ovl/RPG_BAT.h` and wiki pages changed; no `drafts/`, `config/` or `asm/` change; no inline asm, `.word` or `NON_MATCHING` guard; no FAKE statement (none needed); both matches are plain C and the new `RPG_BAT_OVERRIDE_D_8015EDAC` is explained and wrapped in the header; the `ScheduleClubFlags` and `DisplayActor44` casts follow existing uses (DATE `80132000.c`, other RPG_BAT units); header additions are declared once. Forced rebuild: RPG_BAT unit sha1 OK, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK. Status In Review to Done, card moved to Done.
