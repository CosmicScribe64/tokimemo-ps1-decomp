---
id: T-18400
title: "Types-first: KANGEI"
status: Done
assignee: claude-opus (w3-kangei-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[kangei-family-results]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit KANGEI before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their later adaptations), and cut the remaining work into matching packages.

## Acceptance criteria

- [x] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/KANGEI.h`; globals carry their aggregate types; every matched KANGEI function still matches (unit sha1 OK).
- [x] Every unmatched KANGEI function has a fresh draft in `drafts/T-18400/KANGEI/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [x] Drafts at or near 0 rows matched and committed.
- [x] Packages built with the wave-3 inventory scripts.
- [x] "Types-first pass (T-18400)" section on the family page.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-kangei-types` from `w2-good`. Owns `src/ovl/KANGEI/`, `include/ovl/KANGEI.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 types, redraft, matches, packages (claude-opus, branch `w3-kangei-types`)

- Types (`include/ovl/KANGEI.h`): picture-set tables `D_80139950[3][8]`, `D_80139A50[3]`, `D_80139A5C[3][8]`, `D_8013A170[2][3][13]` (129 scalars absorbed, matched code rewritten to elements); `KangeiSprite44` extended to MASTER's display-record layout (`KangeiFixed32`); `KangeiPoint2 D_80139F10[10]`, `s16 D_80139F38[10]`; `KangeiWords11`/`KangeiWords2` tables at `D_80139A08`..`D_80139A44`; club name tables `KangeiNames20 D_80139FAC`, `KangeiNames21 D_8013A088`; `KangeiImage10 D_80139D10[2]` (eight scalars); `u8` fade colours `D_80139E00/04/08`; prototypes of 12 overlay callees. No `game.h` or `main_api.h` change. KANGEI sha1 OK after each step.
- Redraft: `drafts/T-18400/redraft.py` (T-18000 driver plus a multi-dimensional fold and the `gamestate-words` rewrite) and `hand.py` on the 11 functions unmatched after the two ports: 7 better than the census (4,448 bytes), 3 the same, 1 worse (`func_8013769C`); sum 1,809 to 1,478 rows, median 142 to 130; no score 0.
- Matched (4 functions, 2,596 bytes): `func_801370A0` (MASTER clone port), `func_80132C24` (`func_8013454C` form), `func_80135838` (FAKE: date checks through `GameState *g`), `func_80136630`.
- Packages: T-18401..T-18403 (`w3-kangei-p01`..`p03`), 9 functions, 6,836 bytes.
- Checks: clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` (0 new) pass.
- Details: [[kangei-family-results]], "Types-first pass (T-18400)".

### 2026-10-16 code review (claude-opus, against CODING_STANDARDS.md and the ticket)

- Scope: `git diff w2-good...w3-kangei-types`. Changed C and header files are only `src/ovl/KANGEI/*.c` and `include/ovl/KANGEI.h` (no `game.h`, `main_api.h` or config change); `drafts/` has new files under `drafts/T-18400/` only (no existing archive touched, no asm, no binaries); the rest is wiki.
- Matches: the 4 reported functions (`func_801370A0`, `func_80132C24`, `func_80135838`, `func_80136630`) are plain C (no inline asm, no `.word`). Rebuilt in the worktree: `ninja` has no work left, KANGEI sha1 OK, `headers OK`, `globals OK`; `sync_protos.py --check-branch` reports 0 new disagreements (15 known).
- Fakematches: `func_80135838` carries a `FAKE:` comment with reason and T-18400 (date checks through `GameState *g`); `func_801370A0` carries `FAKE:` on the wide `temp_a0` local with T-10630 (the MASTER original of the same code). The `*(s32 *)&D_800E6280.unk_0FC` word view of a `GsRec0FC` record follows the existing `unk_10F8` and `unk_1095` forms in DATE, NAME_ENT, ETC and TACO. No new `NON_MATCHING` block and none in `src/ovl/KANGEI/`.
- Headers: one type per symbol; the 129 absorbed scalars are gone from `KANGEI.h` and replaced by arrays (struct sizes and offsets documented, `KangeiSprite44` sums to 0x44); the 12 new callee prototypes agree with their definitions in `src/ovl/KANGEI/`. C89, ASCII, `/* */` comments only in the added code; no double blank lines.
- Finding 1 (fixed, wiki only): the type-map row on the family page said "11 overlay callees"; the header has 12 (11 called by `func_80132FF8`, plus `func_80136810`). Corrected.
- Observation (no change): the package sizes equal the instruction bytes of the nine `asm/ovl/KANGEI/nonmatchings/` files (6,836 in total), while `progress` leaves 6,588 bytes unmatched in KANGEI (30,148 - 23,560; the start figure of 9,184 minus the 2,596 matched is the same 6,588). The 248-byte gap between the two measures was not investigated.
- No other findings. Status Done.
