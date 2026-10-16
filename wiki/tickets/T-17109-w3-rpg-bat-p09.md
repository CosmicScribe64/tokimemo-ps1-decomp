---
id: T-17109
title: Wave 3 package w3-rpg-bat-p09
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the three listed RPG_BAT functions of `src/ovl/RPG_BAT/8013D780.c` and `src/ovl/RPG_BAT/801477E0.c`: `func_8013D834` (1,160 bytes), `func_80147930` (1,624 bytes), `func_8013DC60` (1,804 bytes). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best drafts `drafts/T-14500/RPG_BAT/<func>` (scores 158, 396, 319).

## Acceptance criteria

- [x] Matched functions are plain C (FAKE statements commented), unit sha1 OK, committed
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17109 comment, others restored to `INCLUDE_ASM`
- [x] Notes in [[matching-notes]] (section T-17109); compiler-gap rows in [[data/t0018-cases]] if any
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p09`, branch `w3-rpg-bat-p09`, base `w2-good`. Owns `src/ovl/RPG_BAT/801477E0.c` and `src/ovl/RPG_BAT/8013D780.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:46 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_8013D834` | 1,068 | draft T-14500 / T-17100 (score 158) | matched in plain C: the state byte through `D_800E6280.unk_110D` with a reload after each call (m2c's `temp_v1` spills a byte), the `+ 3` arm as three `+ 1` statements (ugen temporary per increment step) |
| `func_80147930` | 1,504 | draft T-14500 / T-17100 (score 396) | matched in plain C: return codes restored (0/1/2/3, `0x80000000`, `return D_8015EDF0`), `D_8015EBFC.unk_10`/`unk_14`, `case 14: break;`, `RPG_BAT_OVERRIDE_D_8015EBCC` scalar view, decrement and return on one line |
| `func_8013DC60` | 1,696 | draft T-14500 / T-17100 (score 319) | matched in plain C: as `func_8013D834` with the `+ 2` arm as two `+ 1` statements, `s32` without a return (the `lw ra` delay slot) |

3 of 3 matched (4,268 bytes of code, about 35 minutes). No FAKE statements; the `RPG_BAT_OVERRIDE_D_8015EBCC` scalar view of `D_8015EBCC[3]` is the one deliberate view override (header wrapper in `include/ovl/RPG_BAT.h`). New externs `D_8015EBD8`, `D_8015EBE0` in `RPG_BAT.h`. Notes: [[matching-notes]] (section T-17109), [[register-cheatsheet]] (four rows), [[rpg-bat-family-results]].

## Comments
