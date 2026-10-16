---
id: T-17112
title: Wave 3 package w3-rpg-bat-p12
status: In Review
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the four listed RPG_BAT functions in `src/ovl/RPG_BAT/801525B0.c`, best first: `func_80154220` (528 B, draft score 135), `func_80153784` (732 B, draft score 192), `func_80153F3C` (740 B, draft score 249), `func_801529B4` (1,460 B, draft score 183). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17112 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17112)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p12`, branch `w3-rpg-bat-p12`, base `w2-good`. Owns `src/ovl/RPG_BAT/801525B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:27 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80154220` | 528 | `INCLUDE_ASM` (T-14500 draft score 135) | matched; `s32 *xy`/`s32 *uv`, `u8 *p` with element stores, `y` computed first as `-D_8011ECA0 * 0xF0 + 0xF0` (unary minus reuses the operand register) |
| `func_80153F3C` | 740 | `INCLUDE_ASM` (T-14500 draft score 249) | matched; `s32 xy[8]`, `s32 uv[8]`, five `func_80154220` calls, the x pair of each point on one source line (FAKE comment), `D_80120E93` is `u8` in `main_api.h` |
| `func_801529B4` | 1404 | `INCLUDE_ASM` (T-14500 draft score 183) | matched; compare `switch` with stacked labels for the shared arm, jump table inside `if (D_8015EB9C != 0xE)` |
| `func_80153784` | 732 | `INCLUDE_ASM` (T-17100 draft score 192) | guarded draft, 19 differing rows (8 are branch offsets): frame (FAKE `pad[3]`), RECT stores (two per line), selector `move` (`s32` return) and the 48-word clear match; case 1 load schedule open |

3 of 4 matched (2,672 bytes; about 75 minutes), one guarded at 19 rows. Method and findings: [[matching-notes]] (section T-17112), [[register-cheatsheet]] (eight rows), [[data/t0018-cases]] (one row), [[rpg-bat-family-results]]. Header changes: `D_80120E93` `s8` to `u8` in `main_api.h` (all asm users are `lbu`/`sb`, the other units still match), prototypes for `func_80153A60`, `func_80153B08`, `func_80153D20`, `func_80154220` in `RPG_BAT.h`. Siblings: none above ratio 0.70 for any of the four.

## Comments
