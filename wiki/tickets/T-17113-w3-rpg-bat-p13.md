---
id: T-17113
title: Wave 3 package w3-rpg-bat-p13
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the four listed RPG_BAT functions of `src/ovl/RPG_BAT/801447E0.c`, `src/ovl/RPG_BAT/8013C890.c` and `src/ovl/RPG_BAT/801484E0.c`: `func_801447E0` (924 bytes), `func_80148714` (1,012), `func_8013C890` (1,072), `func_80144C84` (1,276). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best drafts `drafts/T-14500/RPG_BAT/<func>` and `drafts/T-17100/RPG_BAT/<func>` (scores 216, 182, 212, 360).

## Acceptance criteria

- [x] Matched functions are plain C (FAKE statements commented), unit sha1 OK, committed
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17113 comment, others restored to `INCLUDE_ASM`
- [x] Notes in [[matching-notes]] (section T-17113); compiler-gap rows in [[data/t0018-cases]] if any
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p13`, branch `w3-rpg-bat-p13`, base `w2-good`. Owns `src/ovl/RPG_BAT/801447E0.c`, `src/ovl/RPG_BAT/8013C890.c` and `src/ovl/RPG_BAT/801484E0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:31 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_8013C890` | 1,024 | draft T-14500 (score 212) | matched in plain C: two-level switch on `D_8015EDC4` (compare chain outside, jump tables for 7 and 5 labels), `D_8015EDC4 \|= func_80144B7C();`, `s32` without a return (`RPG_BAT_OVERRIDE_func_8013C890`, callers keep `void`) |
| `func_80148714` | 1,012 | draft T-17100 (score 182) | guarded `NON_MATCHING`, 28 differing lines: shift loop makes uopt store `D_8015EDB4` back around it, case 0 load hoisting; one FAKE (redundant `else { start = 0; }`) |
| `func_801447E0` | 924 | draft T-14500 (score 216) | `INCLUDE_ASM`, 154 rows: `D_8015EB98`/`D_8015ED98` colours (`$v1`/`$a1` against `$a1`/`$a0`), second spill slot; draft in `drafts/T-17113/` |
| `func_80144C84` | 1,276 | draft T-17100 (score 360) | `INCLUDE_ASM`, 252 rows: four promoted `D_8015EBFC` fields tie, frame 0x30 against 0x38; draft in `drafts/T-17113/` |

1 of 4 matched (1,024 bytes of code), 1 guarded draft, about 75 minutes. Notes: [[matching-notes]] (section T-17113), rows in [[data/t0018-cases]] (3), [[rpg-bat-family-results]].

## Comments
