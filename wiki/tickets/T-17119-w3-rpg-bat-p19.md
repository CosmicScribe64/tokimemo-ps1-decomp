---
id: T-17119
title: Wave 3 package w3-rpg-bat-p19
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the two listed RPG_BAT functions: `func_80148F70` (1,756 B, `80148F70.c`, T-14500 draft, score 521) and `func_80141060` (2,668 B, `80141060.c`, T-17100 typed draft, score 1,829). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] A matched function is plain C (FAKE statements commented), unit sha1 OK, committed at once
- [x] An unmatched draft under 30 differing lines stays under `#ifdef NON_MATCHING` with a T-17119 comment, otherwise restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17119)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p19`, branch `w3-rpg-bat-p19`, base `w2-good`. Owns `src/ovl/RPG_BAT/80141060.c` and `src/ovl/RPG_BAT/80148F70.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 21:39 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80148F70` | 1,644 | `INCLUDE_ASM` (T-14500 m2c draft, 521 rows) | matched, plain C, no FAKE: m2c's `sp2C` pseudo-locals and `goto block_64` join dropped (per-arm `D_8015EBB0 = ...`, promoted to `$a3` by uopt), arms in asm address order, 3-way clamp `if / else if / else`, one `base` variable, the `9999` store repeated in both arms of the last `if (flag)`, frame word from the named local `flag` |
| `func_80141060` | 2,564 | `INCLUDE_ASM` (T-17100 typed draft, 1,829 rows) | matched, plain C, no FAKE: `DisplayActor44` records for the call loops, byte-array index loops for the unrolled 40..63 groups (`D_8011ECD0[0x198C + i * 0x44]`), `i` shared with the count of the inner switch, frame word from the named local `sel` |

2 of 2 matched, no guard left (about 33 minutes). Method: [[matching-notes]] (section T-17119); five new rows in [[register-cheatsheet]]; [[rpg-bat-family-results]]. Header change: `RPG_BAT.h` gets `void func_801495DC(void);`. No compiler-gap rows (nothing unmatched).

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p19`)

Started 21:39 UTC; `func_80148F70` matched 22:01 UTC (commit `T-17119: match RPG_BAT:func_80148F70`), `func_80141060` matched 22:09 UTC. Both first matched with a FAKE `u8 pad[4]` (frame 8 bytes short, spill one word low) and then re-matched with a real named local each (`flag`, `sel`), so no FAKE remains. Clean rebuild 27/27 and the header and global checks: see the log. Moved to In Review; code review against `CODING_STANDARDS.md` pending.
