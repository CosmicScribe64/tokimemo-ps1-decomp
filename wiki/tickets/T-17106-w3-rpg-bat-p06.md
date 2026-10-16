---
id: T-17106
title: Wave 3 package w3-rpg-bat-p06
status: In Review
assignee: w3-rpg-bat-p06 agent
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the work list of package w3-rpg-bat-p06 in `src/ovl/RPG_BAT/80154430.c` (wave 3, from [[tickets/T-17100-types-first-rpg_bat]]): RPG_BAT `func_80154DD8` (1,204 B, draft score 113), `func_80155868` (520 B, 213), `func_80154760` (1,656 B, 319).

## Acceptance criteria

- [x] Each matched function is plain C, verified by the RPG_BAT sha1 in ninja, committed one per commit (`func_80155868`: fceabdae).
- [x] Near misses under about 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17106 comment; the rest go back to plain `INCLUDE_ASM` (`func_80154DD8` 216 rows and `func_80154760` 197 rows are plain `INCLUDE_ASM`, drafts in `drafts/T-17106/`).
- [x] Clean build 27/27 sha1 OK; check_headers, migrate_globals --check and sync_protos --check-branch pass (results in the log entry).
- [x] Notes in [[matching-notes]] (T-17106); compiler-gap rows in [[data/t0018-cases]]; cheat sheet rows in [[register-cheatsheet]].

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p06`, branch `w3-rpg-bat-p06` from `w2-good`. Owned C file: `src/ovl/RPG_BAT/80154430.c` only (plus `include/`, `config/`, `wiki/`). Started 19:23 UTC, last experiment about 20:45 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80155868` | 520 | draft 213 rows | matched in plain C: three FAKE marks (unused local for the frame slot, `0x2FU` to keep the second constant apart, four `(unused - unused)` references to move uopt's 20-reference block boundary) and `arg4`/`arg5` updates on one line |
| `func_80154760` | 1,656 | draft 319 rows | about 197 rows, restored to `INCLUDE_ASM` (opcodes 96 percent; `$v0`/`$v1` of the selector and the unfolded `li s3,2` loop-pointer block are open) |
| `func_80154DD8` | 1,204 | draft 113 rows | about 216 rows, restored to `INCLUDE_ASM` (opcodes 97 percent; inner selector in `$v1`, constant 1 in `$a3`, quotient webs) |

## Comments
