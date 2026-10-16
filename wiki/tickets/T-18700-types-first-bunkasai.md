---
id: T-18700
title: "Types-first: BUNKASAI"
status: Done
assignee: claude-opus (w3-bunkasai-types)
created: 2026-10-16
updated: 2026-10-16
links: ["[[speedup-research-2026-10-16]]", "[[bunkasai-family-results]]", "[[bunka-family-results]]", "[[tickets/T-16990-wave-3-conveyor]]", "[[taiiku-family-results]]", "[[taco-family-results]]"]
---

## Goal

Recover the shared data types of overlay unit BUNKASAI before function matching (wave 3, [[tickets/T-16990-wave-3-conveyor]]), redraft every unmatched function with them using the pilot drivers (T-16550, T-16560 and their later adaptations), and cut the remaining work into matching packages.

## Acceptance criteria

- [x] Shared structs, arrays, stack tables and callback tables declared in `include/ovl/BUNKASAI.h`; globals carry their aggregate types; every matched BUNKASAI function still matches (unit sha1 OK).
- [x] Every unmatched BUNKASAI function has a fresh draft in `drafts/T-18700/BUNKASAI/<func>/` (`body.c`, `context.c`, `result.json` with score and census score).
- [x] Drafts at or near 0 rows matched and committed.
- [x] Packages built with the wave-3 inventory scripts.
- [x] "Types-first pass (T-18700)" section on the family page.
- [x] Clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch` pass.

## Notes

Branch `w3-bunkasai-types` from `w2-good`. Owns `src/ovl/BUNKASAI/`, `include/ovl/BUNKASAI.h`; additions only to `include/game.h` and `include/main_api.h`.

## Comments

### 2026-10-16 types, redraft, matches, packages (claude-opus, branch `w3-bunkasai-types`)

- Start: 11 unmatched functions (12,492 bytes, census sizes), one guarded (`func_80151364`).
- Types in `include/ovl/BUNKASAI.h` (additions only): `BunkasaiNameTable D_801606DC`, the text-pointer tables
  `BunkasaiTexts12/11/4`, `u8 D_80160B4C`, `BunkasaiGirlBytes D_80160520`, `BunkasaiColumns D_8016052C`,
  `BunkasaiGirlColumns` (three per-year tables), `BunkasaiGirlList D_80160AA4`, prototypes for
  `func_80147F8C` and `func_8015B828`. Type map on [[bunkasai-family-results]].
- Matched 2 functions, 2,376 bytes, no FAKE: `func_80147F8C` (name table copy, named pointer local) and
  `func_8015AF90` (BUNKAKEN `func_80152680` port; `func_80132410` called undeclared, implicit `int`, which
  fixed a v0/v1 swap; `MAIN_API_OVERRIDE_func_800847B8` as in BUNKAKEN).
- Redraft: `drafts/T-18700/redraft.py` (T-18400 driver, BUNKASAI paths) and `hand.py`; 10 drafts, 4 better
  than the census, 3 the same, 3 worse; sum 2,576 to 2,453, median 302 to 246. No draft scored 0.
- Packages (wave-3 scripts): T-18701 `w3-bunkasai-p01` (4 functions, 3,604 bytes), T-18702 `w3-bunkasai-p02`
  (2, 3,468), T-18703 `w3-bunkasai-p03` (3, 3,700).
- Checks: clean build 27/27, `check_headers.py`, `migrate_globals.py --check`, `sync_protos.py --check-branch`.

### 2026-10-16 code review (claude-opus, against CODING_STANDARDS.md section 13)

- Scope: `git diff w2-good...w3-bunkasai-types`. C changes only in `src/ovl/BUNKASAI/80146FA0.c` and
  `src/ovl/BUNKASAI/8015AF90.c`; `include/ovl/BUNKASAI.h` additions only; no `config/`, `tools/` or `include/main_api.h`
  change; `drafts/` has only the new `drafts/T-18700/` folder (no existing archive touched); no game data, asm or
  binaries.
- Matches: forced rebuild of both files and the header, BUNKASAI sha1 OK; `check_headers.py`, `migrate_globals.py --check`
  and `sync_protos.py --check-branch` pass (0 new disagreements). Both matches are plain C: no inline asm, no `.word`,
  no `volatile`, no `NON_MATCHING` guard added, no FAKE needed. `func_80147F8C` uses a named `u8 *name` local that
  is a plausible source variable (DATE2's clone needs a FAKE pad for the same slot). `func_8015AF90`'s undeclared
  `func_80132410` call and `MAIN_API_OVERRIDE_func_800847B8` repeat BUNKAKEN's `func_80132548` and T-9960 forms and are
  explained in comments. The call to `func_80132410` is C89 implicit int, as in BUNKAKEN.
- Header: struct sizes check (`0x129`, `0x30`, `0x2C`, `0x10`, `0x0B`, `0x06`, `0x4E`, `0x2C`), offsets documented, no
  existing declaration changed, each new symbol declared once.
- Finding 1 (fixed): `drafts/T-18700/README.md` gave 180 and 2,328 bytes for `func_80147F8C` and `func_8015AF90`;
  inventory, ticket, family page and link map say 176 and 2,200 (sum 2,376). README corrected.
- No other findings. Ticket status matches the kanban column (Done).

