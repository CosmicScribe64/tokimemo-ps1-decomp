---
id: T-18200
title: "Types-first: EN_NICHI"
status: Done
assignee: claude (w3-en-nichi-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[data-types]]", "[[en-nichi-family-results]]"]
---

## Goal

Types-first pass for the EN_NICHI overlay (wave 3, [[tickets/T-16990-wave-3-conveyor]]): recover EN_NICHI's shared
data types into `include/ovl/EN_NICHI.h`, redraft every unmatched EN_NICHI function with them (drivers adapted from
T-16550, T-16560 and later passes into `drafts/T-18200/redraft.py`), and cut the rest into work packages.

## Acceptance criteria

- [x] Survey: `tools/type_recovery.py --unit EN_NICHI`, siblings, census drafts.
- [x] Recovered structs, arrays and tables declared in `include/ovl/EN_NICHI.h`; every matched EN_NICHI function still matches.
- [x] Every unmatched EN_NICHI function redrafted and scored full-TU; drafts in `drafts/T-18200/EN_NICHI/<func>/`.
- [x] Score-0 or near-zero drafts finished and matched where possible.
- [x] Packages computed; "Types-first pass (T-18200)" section on the family page.
- [x] Clean build 27/27, `check_headers`, `migrate_globals --check`, `sync_protos --check-branch` pass.

## Notes

Ownership: `src/ovl/EN_NICHI/*.c`, `include/ovl/EN_NICHI.h`; additions only in `include/game.h` and `include/main_api.h`.
Start: 19 unmatched EN_NICHI functions (15,464 bytes) in 2 files, 2 of them guarded drafts; no matched sibling above 0.70.

## Comments

### 2026-10-16 claude (w3-en-nichi-types)

- Types (`include/ovl/EN_NICHI.h`): `EnObj D_8013984C[10]` replaces `u8 D_8013984C[]` and the `Rec30 D_80139868[]`
  view (matched users rewritten as fields or byte views); `s16 D_80139A34[16][6]`, ring buffer and status tables
  sized, stack tables `EnHalf22`/`EnBytes22`/`EnHalf64`/`EnHalf13`, `s32 *D_80139204`, `game.h` included for
  `DisplayActor44`; prototypes for `func_801344F0`, `func_80134784`, `func_80133F00` (K&R). EN_NICHI sha1 OK
  after each step.
- Redraft: `drafts/T-18200/redraft.py` (from T-17900). 17 unguarded: 6 better than the census (5,244 bytes),
  11 the same, 0 worse; median 177 to 170, sum 3,816 to 3,674. No score 0.
- Matched by hand (2,500 bytes): `func_80133CA8`, `func_80134258`, `func_80136B74`, `func_80135A48`, and
  `func_80132834` (FAKE: two peeled loop iterations written out). Hand drafts with register-only differences for
  `func_8013358C` and `func_80132EE8`. Idioms on [[en-nichi-family-results]] and in [[matching-notes]].
- Packages: w3-en-nichi-p01 (T-18201, `80132000.c`, 5 functions, 2,700 B) and w3-en-nichi-p02 (T-18202,
  `80136B10.c`, 3 functions, 4,808 B).
- In Review: waiting for the code review against `CODING_STANDARDS.md`.

### 2026-10-16 review (claude, `CODING_STANDARDS.md` checklist)

- Scope: only `src/ovl/EN_NICHI/80132000.c`, `src/ovl/EN_NICHI/80136B10.c` and `include/ovl/EN_NICHI.h` changed; `drafts/T-18200/` is new (no existing archive touched); no game data, asm or binaries; no inline asm, `.word` or `#pragma`; the five reported matches are plain C. The two `NON_MATCHING` blocks in `80132000.c` (T-10610, T-10850) are old and keep their ticket and remaining lines. The one new fakematch (`func_80132834`, peeled iterations) carries `FAKE` and `T-18200`. Header declarations agree (`check_headers` OK).
- Finding 1 (section 8, fixed): `func_80133F00`, `func_801340CC` and `func_8013515C` reached the records through `(u8 *)D_8013984C` byte offsets although `EnObj` is declared. They match as `EnObj *` / field accesses; rewritten, wiki line on the family page adjusted. EN_NICHI sha1 OK after the change.
- Finding 2 (style, fixed): a stray double blank line in `EN_NICHI.h` where `u8 D_8013984C[]` was removed.
- Checks after the fixes: EN_NICHI sha1 OK after a forced rebuild, `tools/check_headers.py` OK (in ninja), `tools/migrate_globals.py --check` OK, `tools/sync_protos.py --check-branch` OK (0 new, 15 known). Result: 5 functions matched, 2,500 bytes. Findings resolved, ticket Done.
