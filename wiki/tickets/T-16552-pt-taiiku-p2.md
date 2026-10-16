---
id: T-16552
title: "Types-first pilot package pt-taiiku-p2"
status: In Review
assignee: claude
created: 2026-10-16
updated: 2026-10-16
links: [[tickets/T-16550-taiiku-types-first]], [[taiiku-family-results]]
---

## Goal

Match the TAIIKU functions of `src/ovl/TAIIKU/80138950.c` from the types-first redrafts of [[tickets/T-16550-taiiku-types-first]]: `func_80138DF8`, `func_80138AA8`, `func_80139FE4`, `func_8013B3B0`, `func_8013B608`, `func_8013BCEC`, `func_8013B78C`, `func_80139AB0`, `func_8013B94C` (9 functions, 6,196 bytes), in that order, with the new types in `include/ovl/TAIIKU.h` instead of byte-offset casts.

## Acceptance criteria

- [x] Each function is matched (unit sha1 OK, no guard) or left under `#ifdef NON_MATCHING` (under about 30 differing lines) or as plain `INCLUDE_ASM`.
- [x] Compiler-gap rows in `wiki/data/t0018-cases.md`.
- [x] Clean rebuild 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` OK.
- [x] Section T-16552 in `wiki/matching-notes.md`.

## Notes

Worktree `tokimemo-wt/pt-taiiku-p2`, branch `pt-taiiku-p2` from `w2-good`. Ownership: `src/ovl/TAIIKU/80138950.c`; `include/`, `config/`, `wiki/` where needed.

## Results

| Function | Bytes | Result |
|---|---:|---|
| `func_80139FE4` | 816 | matched (FAKE: two dead assignments renumber the uopt ranges) |
| `func_80138DF8` | 1,336 | `NON_MATCHING`, 3 differing lines (was 30) |
| `func_80138AA8` | 848 | `NON_MATCHING`, 22 differing lines, both FAKE pads replaced by `idx` |
| `func_8013B608`, `func_8013B3B0`, `func_8013B78C`, `func_8013BCEC`, `func_80139AB0` | 2,700 | drafts of 33 to 111 lines, not kept (`INCLUDE_ASM`); rows in [[data/t0018-cases]] |
| `func_8013B94C` | 496 | not tried beyond the asm (same family as `func_8013B3B0`) |

Also: `cd_read_sectors` gets a file-local `u32` view in `80138950.c` (`MAIN_API_OVERRIDE_cd_read_sectors`, guarded in `include/main_api.h`); the tree builds with `u32` in the definition too, which would remove the override (needs a main-exe owner). Notes: [[matching-notes]] "T-16552", [[taiiku-family-results]], [[register-cheatsheet]].

## Comments
