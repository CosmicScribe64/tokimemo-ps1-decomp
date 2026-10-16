---
id: T-17118
title: Wave 3 package w3-rpg-bat-p18
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the one listed RPG_BAT function, `func_80133108` (4,192 B, `src/ovl/RPG_BAT/80132FE0.c`; T-14500 draft score 863). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] A matched function is plain C (FAKE statements commented), unit sha1 OK, committed at once
- [x] An unmatched draft under 30 differing lines stays under `#ifdef NON_MATCHING` with a T-17118 comment, otherwise it is restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17118)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p18`, branch `w3-rpg-bat-p18`, base `w2-good`. Owns `src/ovl/RPG_BAT/80132FE0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:29 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80133108` | 4,192 | `INCLUDE_ASM` (T-14500 draft, score 863) | matched, plain C: angle `ang` computed once before the three sine loops, one shared `tmp` local (sine of cases 11/13/15 and case 19 call result), case 9 records indexed `[i + 64]`, `RPG_BAT_OVERRIDE_func_8013F15C` |

1 of 1 matched, no guard left, no FAKE statement (about 20 minutes). Method and findings: [[matching-notes]] (section T-17118), two new rows in [[register-cheatsheet]], [[rpg-bat-family-results]]. No header change.

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p18`)

Started 21:29 UTC, matched by 21:44 UTC (draft 1,035 instructions to 1,017, then the registers). Clean rebuild 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK. Moved to In Review; code review against `CODING_STANDARDS.md` pending.
