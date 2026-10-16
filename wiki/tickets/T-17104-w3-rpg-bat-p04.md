---
id: T-17104
title: Wave 3 package w3-rpg-bat-p04
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the one listed RPG_BAT function of `src/ovl/RPG_BAT/801468B0.c`: `func_80146C30` (2,992 bytes of code, a 36-case switch of the Stampede / hermit battle script). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best draft `drafts/T-14500/RPG_BAT/func_80146C30` (score 329), typed variant `drafts/T-17100/RPG_BAT/func_80146C30` (329).

## Acceptance criteria

- [x] Matched function is plain C (FAKE statements commented), unit sha1 OK, committed
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17104 comment, others restored to `INCLUDE_ASM`
- [x] Notes in [[matching-notes]] (section T-17104); compiler-gap rows in [[data/t0018-cases]] if any
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p04`, branch `w3-rpg-bat-p04`, base `w2-good`. Owns `src/ovl/RPG_BAT/801468B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 19:19 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80146C30` | 2,992 | draft T-14500 / T-17100 (score 329, 764 against 745 instructions) | matched in plain C (one FAKE: `(&D_8015ECEC)[i]` first-symbol view in a rolled 3-trip loop): `RPG_BAT_OVERRIDE_func_8013E7C0` and `_func_8013E97C` implicit `s32` views move the `D_8015EDE0` web to `$v1`; m2c temporaries replaced by `D_8015EDE0 += n;` |

1 of 1 matched (2,992 bytes of code, about 15 minutes). Side result: the guarded `func_80145CC0` (T-12300) matches under `funcdiff --proto` with the same two overrides; `80145CC0.c` was outside this package and is not edited. Notes: [[matching-notes]] (section T-17104), [[register-cheatsheet]] (two rows), [[data/t0018-cases]] (update line), [[rpg-bat-family-results]].

## Comments
