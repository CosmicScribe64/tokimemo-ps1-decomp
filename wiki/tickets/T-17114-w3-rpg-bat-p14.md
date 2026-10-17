---
id: T-17114
title: Wave 3 package w3-rpg-bat-p14
status: Done
assignee: claude-opus
created: 2026-10-16
updated: 2026-10-16
links: ["[[tickets/T-17100-types-first-rpg_bat]]", "[[rpg-bat-family-results]]"]
---

## Goal

Match the four listed RPG_BAT functions of `src/ovl/RPG_BAT/8013F4F0.c` and `src/ovl/RPG_BAT/801368B0.c`: `func_80136A9C` (316 bytes), `func_80140E70` (496), `func_8013F82C` (1,600), `func_80140204` (2,180). Wave 3 package from the types-first pass ([[tickets/T-17100-types-first-rpg_bat]]); best drafts `drafts/T-14500/RPG_BAT/<func>` (scores 114, 77, 248, 508).

## Acceptance criteria

- [x] Matched functions are plain C (FAKE statements commented), unit sha1 OK, committed
- [x] Unmatched drafts under 30 differing lines stay under `#ifdef NON_MATCHING` with a T-17114 comment, others restored to `INCLUDE_ASM`
- [x] Notes in [[matching-notes]] (section T-17114); compiler-gap rows in [[data/t0018-cases]] if any
- [x] Clean build 27/27, `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK

## Notes

Worktree `tokimemo-wt/w3-rpg-bat-p14`, branch `w3-rpg-bat-p14`, base `w2-good`. Owns `src/ovl/RPG_BAT/8013F4F0.c` and `src/ovl/RPG_BAT/801368B0.c` (plus `include/`, `config/`, `wiki/` edits). Started 2026-10-16 20:49 UTC.

## Results

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80136A9C` | 316 | draft T-14500 (score 114), old T-0018 `selector` rows | matched in plain C with one FAKE: arm stores to `D_8015EC6C` itself, `switch (*(s32 *)&D_8015EBAC)` (found by `autotweak`) |
| `func_80140E70` | 496 | draft T-14500 (score 77) | matched in plain C: the counter updated and read as the global itself (no local), prototype in `RPG_BAT.h` |
| `func_8013F82C` | 1,376 | draft T-14500 (score 248) | matched in plain C: grey level as two `c + (c << 8)` steps, `RPG_BAT_OVERRIDE_func_8013E7C0`, `s16 h`, display-record index forms, `D_8012077C` s16 view |
| `func_80140204` | 2,132 | draft T-14500 (score 508) | matched in plain C: records 100..104 as `DisplayActor44` fields, the five-iteration record loop (IDO peels and unrolls it), `p[50]` walk in case 8 |

4 of 4 matched (4,320 bytes of code, about 70 minutes). One FAKE in the matcher's report (`func_80136A9C`, the address-taken selector view); review added FAKE marks to the two first-symbol sites of the slot table `D_8015EBAC` (see Comments). New main-exe externs in `main_api.h`: `D_801211E6`, `D_80121532`, `D_80121575`, `D_801215FE`, `D_80121614`, `D_8012077A`, `D_8012077C` (moved from `EVENT.h`, with `MAIN_API_OVERRIDE_D_8012077C` in `8013F4F0.c`). `RPG_BAT.h`: prototype `func_80140E70`. Overrides: `RPG_BAT_OVERRIDE_func_8013E7C0` (existing mechanism) in `8013F4F0.c`. Notes: [[matching-notes]] (section T-17114), [[register-cheatsheet]] (four rows), [[data/t0018-cases]] (update), [[rpg-bat-family-results]].

## Comments

### 2026-10-16 code review (claude-opus, reviewer)

Reviewed `w2-good...w3-rpg-bat-p14` against `CODING_STANDARDS.md`. One finding, fixed.

- Finding (section 8, first-symbol accesses): `func_8013F82C` (`(&D_8015EC74)[i]`, `(&D_8015EBAC)[i + 50]`) and `func_80140204` (`p = &D_8015EBAC; p[50]`) reach the slot table `D_8015EBAC` through its first symbol without a `FAKE` mark. Fixed: two comment-only `FAKE` marks naming T-17114 and the unrecovered aggregate (the idiom is the one the other RPG_BAT units already use; the aggregate is T-17100 follow-up work). Unit sha1 unchanged.
- Only `src/ovl/RPG_BAT/8013F4F0.c` and `src/ovl/RPG_BAT/801368B0.c` changed in `src`; no `drafts/`, asm, config or binary change. No `NON_MATCHING`, `INCLUDE_ASM` removal outside the four functions, inline asm or `.word`; all four functions are plain C.
- Headers: `main_api.h` gets seven externs (types from the access widths); `D_8012077C` moved from `EVENT.h` (same `s32` type there) with `MAIN_API_OVERRIDE_D_8012077C` and the reason in `8013F4F0.c`; `RPG_BAT_OVERRIDE_func_8013E7C0` follows section 8a with its reason; `RPG_BAT.h` gets the `func_80140E70` prototype. `check_headers` OK.
- C89 (declarations at block top, `/* */` only), ASCII comments, fixed-width types, typed `DisplayActor44` fields. The one added string literal is code text of the same kind as the neighbouring ones in this file. The `func_80136A9C` FAKE carries reason and ticket.
- Minor: the Goal sizes (1,600 and 2,180 bytes) are the draft-list sizes, the Results sizes (1,376 and 2,132) the linked sizes; left as written.
- Rebuild in the worktree after the fix: `ninja` OK (`build/ovl/RPG_BAT.bin: OK`), `check_headers` OK, `migrate_globals --check` OK, `sync_protos --check-branch` OK (0 new, 15 known). The merge script repeats a clean configure and full ninja.
- Ticket status matches the kanban column (Done).
