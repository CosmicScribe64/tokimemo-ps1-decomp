---
id: T-17600
title: "Types-first: GYOZI"
status: Done
assignee: claude (w3-gyozi-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[event-family-results]]"]
---

## Goal

Types-first pass for the GYOZI overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover GYOZI's shared data
types into `include/ovl/GYOZI.h`, redraft every unmatched GYOZI function with them (drivers adapted from T-16550,
T-16560 and their descendants into `drafts/T-17600/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit GYOZI`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/GYOZI.h`; every matched GYOZI function still matches.
- [x] Every unmatched GYOZI function redrafted and scored full-TU; drafts in `drafts/T-17600/GYOZI/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-17600)" section on the GYOZI family page ([[event-family-results]]).
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/GYOZI/*.c`, `include/ovl/GYOZI.h`; additions only in `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 (claude, w3-gyozi-types)

Types in `include/ovl/GYOZI.h`: step-callback tables `GzCallbacks68/90/BC` (`D_80148880`, `D_801476F4`,
`D_80147C6C`, `D_8014901C`), `Rec24 D_8012CA40[]`, the dumpling arrays `s32 D_80148BF0/CB8/D80[50]`,
`s16 D_80148E48[50]`, `s32 D_80148EB0`, `s16 D_8014875C[8]`/`D_8014876C[8]`, `GyoziGirl.unk_14`,
`GyoziWork.unk_10B8` (`D_800F6458`), `u8 D_80148828[4]`; `GYOZI.h` includes `game.h`; `main_api.h` gains
`extern s32 D_800F62F0;` (addition only). GYOZI sha1 OK after each step.

Redraft of all 37 unmatched functions with `drafts/T-17600/redraft.py` (T-17300 driver adapted: display records
`D_80129F40`, Rec24 `D_8012CA40`, `func_80054B60` primitives, `GzCallback` stack tables, girl-only fold of
`GyoziWork`): 32 compile; over 29 unguarded, 7 better than the census (5,416 bytes), 19 the same, 3 worse; sum
4,710 to 4,715 rows; none 0. Five without a compiling redraft carry the census body and score.

Matched 3 functions / 908 bytes: `func_8013E308` (SHOUGATU clone port, FAKE T-9020 store idiom), `func_8013DC2C`
(GYOZI sibling port, `D_800F53A0.unk_10B8`), `func_8013B0C4` (redraft plus T-3330 index slot). Near misses with
`hand.c`: `func_8014206C` (6 rows), `func_8013C774` (14). Packages T-17601..T-17608 (`w3-gyozi-p01`..`p08`,
33 functions, 19,180 bytes). Section "Types-first pass (T-17600)" on [[event-family-results]]; rows in
[[data-types]] and [[matching-notes]].

Checks: clean build 27/27 OK (`ninja -j2`), `check_headers` OK, `migrate_globals --check` OK,
`sync_protos --check-branch` OK (0 new, 15 known). Moved to In Review.

### 2026-10-16 (claude, review)

Code review of `w2-good...w3-gyozi-types` against `CODING_STANDARDS.md`: one finding, fixed in a comment-only commit.
- Finding (FAKE marking, section 7): `func_8013DC2C` used three idioms without a `FAKE` comment: the `D_800F53A0.unk_10B8`
  member view of `D_800F6458`, the `GyoziBit3` bit-field view of the girl[10] flag byte, and the byte-typed chain
  store and reload of `D_800F5ACD`/`D_800F62CF` (the unmarked T-9020 idiom of its sibling `func_8013AC38`). Each now has a
  `FAKE ... T-17600` comment; the object is unchanged (GYOZI sha1 OK after the edit). The three fakematch idioms of the
  branch are therefore: `func_8013E308` byte-store-as-argument (T-9020 idiom, already marked), and those three in
  `func_8013DC2C`. `func_8013B0C4` is plain C (table copy plus the T-3330 index slot, commented).
- C files changed: `src/ovl/GYOZI/8013AE80.c`, `8013DB50.c`, `8013E1D0.c` only (all owned); headers `include/ovl/GYOZI.h`
  and `include/main_api.h` (one `extern s32 D_800F62F0;` addition, `lw` in `func_8013C774`, no other user). `drafts/` gained
  only the new folder `drafts/T-17600/` (no existing archive touched; same file kinds as `drafts/T-17300/`); no asm,
  `.word`, inline asm, disc data or binaries. No new `NON_MATCHING` guard in `src`; the two near misses
  (`func_8014206C`, `func_8013C774`) stay as `hand.c` drafts, `INCLUDE_ASM` in `src`.
- Matches (`func_8013E308` 372 B, `func_8013DC2C` 268 B, `func_8013B0C4` 268 B; 908 B) are C89 with declarations at
  block top, `/* */` comments only, fixed-width types; `GyoziGirl`/`GyoziWork` offsets documented, new tables declared once.
- Package totals (33 functions, 19,180 bytes), redraft split (7 better, 19 same, 3 worse of 29 unguarded) and match bytes
  agree between ticket, family page, catalog and log. Commits carry the ticket id and the trailer.
- Re-run on the branch: forced re-check of `globals`, `headers` and `build/ovl/GYOZI.bin: OK`, `build/SLPM_86.053.bin: OK`; after
  the comment edit GYOZI rebuilt, `build/ovl/GYOZI.bin: OK`; `check_headers` OK; `migrate_globals --check` OK;
  `sync_protos --check-branch` OK (0 new, 15 known).

Moved to Done.
