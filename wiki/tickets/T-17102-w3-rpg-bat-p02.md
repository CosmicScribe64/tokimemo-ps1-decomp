---
id: T-17102
title: Wave 3 package w3-rpg-bat-p02
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the three listed RPG_BAT functions of `src/ovl/RPG_BAT/80151C10.c` and `src/ovl/RPG_BAT/80156CC0.c`: `func_80151C10`, `func_80156D78`, `func_80157158`. Package list from the T-14500 drafts (see [[rpg-bat-family-results]], section "Types-first pass").

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under about 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17102 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]]; notes in [[matching-notes]] (section T-17102)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p02`, branch `w3-rpg-bat-p02`, base `w2-good`. Owns the two C files above (plus `include/`, `config/`, `wiki/` edits). Started 18:58 UTC, `func_80151C10` matched at 19:28 UTC, `func_80156D78` at 19:53, `func_80157158` at 20:07.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80151C10` | 808 | `INCLUDE_ASM` (draft score 62, no sibling above 0.70) | matched: `void func_80151F38(void)` called without arguments, `D_8015EE00.unk_04`/`.unk_0C`, `*(u32 *)&D_8015EC74` (FAKE), `s32 func_8013E7C0();` file override (`$v1` selector) |
| `func_80156D78` | 992 | `INCLUDE_ASM` (draft score 91) | matched: re-reads of `D_8015EDD8`, `D_8015EDE0 += 2`, `D_8015EDC4 &= ..; |= 3`, `func_80049A40` promoted-word override, `s32 unused;` (FAKE), `s32 func_8013F15C();` file override |
| `func_80157158` | 1,256 | `INCLUDE_ASM` (draft score 127) | matched: same idioms as `func_80156D78`, display-record view for `D_80121128`/`D_80121136`, `s32 unused[2];` (FAKE) |

3 of 3 matched (3,056 bytes). Notes in [[matching-notes]] (section T-17102), seven rows in [[register-cheatsheet]], an update line in [[data/t0018-cases]] (the `$v1`-selector rows can be retried with an int-returning callee view).

## Comments

Verification (2026-10-16, 20:13 UTC): clean build `rm -rf build && python3 configure.py && ninja` exit 0 (667/667, every `.bin: OK`, 27 of 27 units), `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (0 new, 15 known). Matches verified with `funcdiff --resolve` on the default build (`MATCH (linked function bytes equal target)`) for `func_80151C10` (808 bytes), `func_80156D78` (992) and `func_80157158` (1,256). FAKEs, each commented in the source: `*(u32 *)&D_8015EC74` (`func_80151C10`), `s32 unused;` (`func_80156D78`), `s32 unused[2];` (`func_80157158`). Overrides: `RPG_BAT_OVERRIDE_func_8013E7C0` (`80151C10.c`), `RPG_BAT_OVERRIDE_func_8013F15C` and `MAIN_API_OVERRIDE_func_80049A40` (`80156CC0.c`). Awaiting code review against `CODING_STANDARDS.md`.

Code review (2026-10-16), against `CODING_STANDARDS.md` and the ticket: no findings. Checked: only the two owned C files and `include/ovl/RPG_BAT.h` changed, no `drafts/` change, no inline asm or `.word`, no `NON_MATCHING` block left in the two files, all three matches are plain C, the three FAKEs carry `FAKE` and T-17102, the overrides (`RPG_BAT_OVERRIDE_func_8013E7C0`, `RPG_BAT_OVERRIDE_func_8013F15C`, `MAIN_API_OVERRIDE_func_80049A40`) are defined with a reason before the includes, declared next to the file declarations and wrapped in the headers (section 8a), `func_80151F38(void)` declared once in `include/ovl/RPG_BAT.h`, C89 and ASCII comments (the Japanese text is string literals, as in the other RPG_BAT files), no game data or binaries, wiki edits append-only. Rebuild in the worktree: up to date, unit sha1 OK.
