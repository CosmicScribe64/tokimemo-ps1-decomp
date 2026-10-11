---
id: T-9110
title: Wave 6: list 1
status: Done
assignee: w6-1
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal
Match the functions of wave-6 list 1 (37 files, 151 functions, 57,844 bytes; leftovers of wave 5).

## Acceptance criteria
- [x] Clean build 27/27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK
- [x] Inline review against CODING_STANDARDS.md recorded

## Notes
Worktree w6-1 (branch w6-1), not merged.

Result: 29 functions, 6,688 bytes (progress grand line 4296/6958 at the end).
Matched: main `func_80048CF8`, `func_80048EB8`, `initCoordinate`; TT `func_8013BF9C`, `func_8013BE08`, `func_8013E290`, `func_8013A960`; ETC `func_801433E4`, `func_801436E4`, `func_80147F7C`; EVENT `func_800F7690`, `func_800F788C`, `func_800F8818`, `func_800F9508`, `func_800FE138`; GEKO `func_8013BDB0`; OLH `func_80133C4C`, `func_80134060`; RPG_BAT `func_801368B0`, `func_80142000`, `func_801420EC`, `func_80144B7C`, `func_80145180`, `func_801535E4`; SHOUGATU `func_80141780`, `func_80141850`; TAIIKU `func_80139550`, `func_8013B104`, `func_8013FEF0`.
Header changes: `TaiikuBig` and `D_80122CA0` moved to `include/main_api.h` (main `initCoordinate`), new overlay externs appended in `include/ovl/{ETC,EVENT,GEKO,OLH,RPG_BAT,SHOUGATU,TAIIKU,TT}.h`, prototype fixes (`s32`/`void`) for functions now defined.
New idioms and the 31 new rows of [[data/t0018-cases]]: [[matching-notes]], "Wave 6, list 1 (T-9110)".
Tracked fakematches: TT `func_8013BF9C` (one-word struct copy), TT `func_8013BE08` and main `initCoordinate` (same-line statements), ETC `func_801433E4` and OLH `func_80133C4C` (frame pads).
Tooling: no bug found. `tools/dupes.py` and `neardupes.py` found no twin in the 37 files (also run at the end).

## Comments
Inline review (CODING_STANDARDS checklist): C89 with declarations at block tops, no `//`; fakematches marked `FAKE` with a reason (see Notes); main-exe symbols declared once in `main_api.h` (`sync_protos.py --fix`, `--check-branch` OK); overlay symbols in the overlay headers; struct layouts commented where new; no game data staged (only `src/`, `include/`, `wiki/`); every commit message `T-9110: ...` with the co-author trailer. Findings: the TT one-word struct copy lacked its `FAKE` marker (fixed), three address-literal / cast idioms lacked an explanation (comments added). No open findings.
