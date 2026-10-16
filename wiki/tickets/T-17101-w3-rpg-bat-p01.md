---
id: T-17101
title: Wave 3 package w3-rpg-bat-p01
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-17100-types-first-rpg_bat]], [[rpg-bat-family-results]]
---

## Goal

Match the four listed RPG_BAT functions, best first: `func_80152080` (812 B, `80151FC0.c`, draft score 32), `func_80145CC0` (2,372 B, `80145CC0.c`, guarded draft), `func_80152384` (556 B, `80151FC0.c`, draft score 93), `func_80146578` (824 B, `80145CC0.c`, draft score 277). Package list from the T-17100 types-first pass.

## Acceptance criteria

- [x] Each matched function is plain C (FAKE statements commented), unit sha1 OK, committed one by one
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17101 comment, others restored to `INCLUDE_ASM`
- [x] Compiler-gap rows in [[data/t0018-cases]] where the rules say so; notes in [[matching-notes]] (section T-17101)
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch`

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p01`, branch `w3-rpg-bat-p01`, base `w2-good`. Owns `src/ovl/RPG_BAT/80145CC0.c` and `src/ovl/RPG_BAT/80151FC0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 18:42 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80152080` | 812 | `INCLUDE_ASM` (draft score 32, frame 0x38 against 0x40) | matched, FAKE `home0`/`home1` unused locals before the `sp34` spill; `RpgObjectRecord44.unk_40` for the byte of `D_8012163C` |
| `func_80145CC0` | 2,372 | guarded draft (T-12300, 116 rows, `$v0`/`$v1`) | matched, `func_8013E7C0` and `func_8013E97C` as implicit-int views (`RPG_BAT_OVERRIDE_*`) |
| `func_80152384` | 556 | `INCLUDE_ASM` (draft score 93) | matched, `MAIN_API_OVERRIDE_func_800AE0F0` (`void *` return), stack tables in `RpgStrTbl4`/`RpgWordTbl4`, do-while for the first loop |
| `func_80146578` | 824 | `INCLUDE_ASM` (draft score 277) | matched, `func_8013F15C` implicit int (`RPG_BAT_OVERRIDE_func_8013F15C`), FAKE one-word `zero[1]` stored in the call argument; defined `s32` (implicit declaration in the caller) |

4 of 4 matched (4,564 bytes; about 40 minutes), no guard left. Method and findings: [[matching-notes]] (section T-17101), [[register-cheatsheet]] (six rows), [[rpg-bat-family-results]]. Header changes: `RPG_BAT.h` (`unk_40`, `#ifndef` wrapper for `func_8013F15C`), `main_api.h` (`#ifndef` wrapper for `func_800AE0F0`; all 27 units still match).

## Comments

### 2026-10-16 start and finish (claude-opus, branch `w3-rpg-bat-p01`)

- Siblings: none above ratio 0.70 for any of the four.
- The `--proto` option of `funcdiff.py` found the implicit-int views in seconds; see the matching notes.
- Awaiting code review against `CODING_STANDARDS.md`.

### 2026-10-16 code review (claude-opus, against `CODING_STANDARDS.md` section 13)

- Scope: only `80145CC0.c` and `80151FC0.c` changed among the C files; no `drafts/` change; no inline asm, `.word` or `//` comments; no game data or binaries; four of four matches are plain C, no `NON_MATCHING` block left.
- FAKEs: `zero[1]` and the `zero[0] = 0` argument store in `func_80146578`, `home0`/`home1` in `func_80152080`; each has a `FAKE` comment with reason and T-17101. The three `*_OVERRIDE_*` defines sit before the first include with a reason; the `#ifndef` wrappers match the declarations (`RpgObjectRecord44` still 0x44 bytes: `pad3C[4]`, `unk_40`, `pad41[3]`).
- Verified in the worktree: forced rebuild of all 27 units sha1 OK, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (0 new, 15 known).
- Finding (fixed): the comment above the `func_8013E7C0`/`func_8013E97C` wrappers in `RPG_BAT.h` named two prototypes; a one-line comment now covers the `func_8013F15C` wrapper too. Header comment only, build unchanged.
- Noted, not a finding: `((u8 **)D_8015EB70)[count + 4]` views the unrecovered text-window object as a pointer table, as `DATE2` and `SHOUGATU` do for their windows; the struct is still to be recovered.
- Result: no open findings, ticket Done.
