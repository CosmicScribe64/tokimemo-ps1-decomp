---
id: T-17115
title: Wave 3 package w3-rpg-bat-p15
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the four listed RPG_BAT functions, best first: `func_801328C4` (464 B, `801321D0.c`, T-17100 draft score 97), `func_8014A3DC` (672 B, `801498C0.c`, T-14500 draft score 66), `func_80132B34` (452 B, `801321D0.c`, draft score 154, a known T-0018 constant-folding family) and `func_80149C5C` (1,296 B, `801498C0.c`, draft score 567). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17115 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17115)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p15`, branch `w3-rpg-bat-p15`, base `w2-good`. Owns `src/ovl/RPG_BAT/801498C0.c` and `src/ovl/RPG_BAT/801321D0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:00 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_801328C4` | 464 | `INCLUDE_ASM` (T-17100 draft, score 97) | matched, plain C: loop counter declared before the stack table (`s32 i; RpgDirTable dirs;`), prototype in `RPG_BAT.h` |
| `func_8014A3DC` | 672 | `INCLUDE_ASM` (T-14500 draft, score 66, T-17100 hand edit one instruction off) | matched, plain C: `s32` return type (implicit int in the original), prototype `s32 func_8014A3DC(void);` in `RPG_BAT.h` |
| `func_80132B34` | 452 | `INCLUDE_ASM` (draft 154 rows) | not matched, `INCLUDE_ASM` restored; phi form keeps the adds unfolded but IDO shares the store tail ([[data/t0018-cases]]) |
| `func_80149C5C` | 1,296 | `INCLUDE_ASM` (draft 567 rows) | not matched, `INCLUDE_ASM` restored; natural C 252 rows, only register choice (`$a1`/`$v1` against `$v1`/`$v0`), saved as `drafts/T-17115/RPG_BAT/func_80149C5C/body.c` ([[data/t0018-cases]]) |

2 of 4 matched (1,136 bytes), no FAKE statement, no guard left. Notes: [[matching-notes]] (section T-17115), two new rows in [[register-cheatsheet]], [[rpg-bat-family-results]].

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p15`)

Started 21:00 UTC. `func_801328C4` matched 21:08, `func_8014A3DC` 21:19, the other two attempted until about 21:45 and restored to `INCLUDE_ASM`.

