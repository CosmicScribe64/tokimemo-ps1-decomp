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
| `func_80138DF8` | 1,336 | matched in the closer pass (FAKE: `i = 0` between the call and its test, `goto` into the first loop) |
| `func_80138AA8` | 848 | matched in the closer pass (`func_80085E30` called as an implicit int function, local `p` for the case 2 buffer address) |
| `func_8013B608`, `func_8013B3B0`, `func_8013B78C`, `func_8013BCEC`, `func_80139AB0` | 2,700 | drafts of 33 to 111 lines, not kept (`INCLUDE_ASM`); rows in [[data/t0018-cases]] |
| `func_8013B94C` | 496 | draft 80 lines (IDO promotes the counter in $a0 as in `func_8013B3B0`); with a `volatile u16` view of the counter 60 lines, but then uopt keeps `0x801499A0` in $a1 and addresses `2(a1)`; not kept |

Also: `cd_read_sectors` gets a file-local `u32` view in `80138950.c` (`MAIN_API_OVERRIDE_cd_read_sectors`, guarded in `include/main_api.h`); the tree builds with `u32` in the definition too, which would remove the override (needs a main-exe owner). Notes: [[matching-notes]] "T-16552", [[taiiku-family-results]], [[register-cheatsheet]].

## Comments

### 2026-10-16, Opus closer pass

Matched both guarded near misses; 3,000 bytes of the package are now matched (`func_80139FE4`, `func_80138DF8`, `func_80138AA8`).

- `func_80138DF8` (3 lines): `ugen -S` showed the original order `move s0,zero; la a2; la s1` already, so the difference is as1's. A `move` in the fall-through block never lands first, whether `i = 0` sits on an earlier, the same or a later line than the loop. The original's `andi; beqz; move s0,zero` is a delay-slot fill from before the branch: `idx = cd_read_sync_status(); i = 0; if (idx & 1)`. With that, a `for`/`while` keeps an entry test and uopt leaves the pointer start as `sll t9,zero,2; addu a2,t9,t6`. A do-while folds it to `la a2` but puts the increments on the last source line, and as1 then schedules them after the stores (6 lines). `goto first;` before `for (; i < 18; i++) { first: ... }` has no entry test and keeps the increments on the `for` line: MATCH. FAKE, commented. `#line` gave the same bytes in a scratch compile but `funcdiff` preprocesses with `gcc -E -P` and drops it, so it was not used.
- `func_80138AA8` (22 lines, 8 of them frame lines): `idx` alone left the frame 8 bytes short; one more scalar fixes the frame (11 register lines left). `funcdiff --proto 's32 func_80085E30();'` matched: with an int result the loop pointer no longer takes `$v0` and the constant 8 does. Kept with `MAIN_API_OVERRIDE_func_80085E30` (guarded in `include/main_api.h`), as an implicit int call in the original. The extra scalar is a real local, `p = 0x8018F000;` in case 2, as in `func_80138DF8`. Four other real locals (`ret` for `Fade_Out_Main()` or `cd_read_sync_status()`, `j` for the palette index) matched too.
- Verification: clean rebuild 665/665, 27 binaries sha1 OK, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` OK.
