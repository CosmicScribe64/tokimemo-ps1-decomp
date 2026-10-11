---
id: T-9510
title: "Wave 8: list 1"
status: Done
assignee: w8-1
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Work through wave8-list-1.txt in order, matching as many of its 156 functions (84,972 bytes) in 35 owned files as possible. Time-box unsuccessful functions and restore INCLUDE_ASM.

## Acceptance criteria

- [x] Every listed function matched or time-boxed.
- [x] Function diffs and all 27 binary checks pass.
- [x] Prototype/global guards and inline standards review pass.
- [x] Results, blockers, and T-0018 rows recorded; no commits.

## Notes

Shared brief read from the orchestrator scratchpad. All analysis/build/dependencies use tools/docker.sh. No edits to tooling or unowned C files.

## Comments

- 2026-10-11: Claimed In Progress; read wiki index, standards, local instructions, workflow and register-shape findings. Preparing this new worktree's initial build before drafts.

- Initial NAME_ENT func_80147F78 attempt: repaired ring-buffer type and renamed getCDlevel; real diff remains in frame, branch/phi allocation, and load order. Temporary source/header edits restored.

- TACO func_80147444: 68-byte verified match using RECT and marked frame padding. All first 30 functions attempted; failures restored. Continuing the remaining list.

- NAME_ENT func_80148F08: 528-byte verified match. Through item 95: two verified matches totaling 596 bytes; other candidates restored.

- Main func_80054704: 136-byte verified FAKE volatile-counter match; preserves the original uninitialized stack read. Total 3 matches / 732 bytes before the final 61-function batch. The bounded permuter reports KeyError: delete; see matching notes.

- RPG_BAT func_8014DF60: 580-byte verified dispatcher match, two append-only overlay forward declarations. Total 4 matches / 1,312 bytes after the retry pass. All 156 listed functions attempted.

- Inline standards/spec review started against HEAD/worktree, per the wave brief. Four functions pass resolved diffs and unit SHA1; final duplicate pass and 27-unit checks pending. SDK RECT and the marked frame/volatile artifacts reviewed. The original uninitialized retry-counter read remains intentional.


- Final inline review against CODING_STANDARDS.md and the shared brief passed with no unresolved findings. Checked C89, SDK RECT, fixed-width types, placeholder names, allowed file scope, assembly fallbacks, FAKE annotations and append-only overlay forwards. The workspace-offset finding was resolved with generated D_80120E49; the original uninitialized counter read is documented. No tooling/config/raw data edits or commits.

- Final verification: 4 resolved function MATCH results, all 27 binary SHA1 checks OK, build/headers.ok and build/globals.ok present, sync_protos --check-branch and migrate_globals --check pass, git diff --check clean. One transient ninja deps-log error in the first main diff was cleared by a successful resolved rerun. Scoped dupes applied 0/1 candidates (failed SHA1 reverted); near-duplicate report found no fillable candidates.

- Final result: 4/156 functions, 1,312/84,972 bytes matched; all 152 remaining list entries time-boxed and restored. Grand progress 4738/6958 functions, 730924/2279368 bytes (32.1%). Nine T-0018 symptom rows added. Top other blocker: unrecovered table/stack layouts and mixed-width views. TAIIKU own-data island needs a separately authorized YAML task. Tooling bugs: bounded permuter KeyError: delete; register_shapes positional CLI in brief unsupported. Evidence remains in orchestrator scratchpad w8-1.

## Function audit

All 156 list entries are covered below. Matched bodies pass resolved function diffs and unit SHA1; final whole-repository verification is recorded in Comments. Full drafts, bounded variants, diffs and build logs remain in the orchestrator scratchpad w8-1 directory.

| File | Function | Bytes | Outcome |
|---|---|---:|---|
| NAME_ENT/80144AD0 | `func_80147F78` | 360 | Time-boxed: resolved mismatch |
| EVENT/801050D0 | `func_80105DF8` | 856 | Time-boxed: build rejection |
| TACO/80146D60 | `func_80147444` | 68 | Matched |
| OPTION/801389A0 | `func_80139F8C` | 132 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013C5C8` | 204 | Time-boxed: resolved mismatch |
| GEKO/801323E0 | `func_80132A4C` | 216 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013AE14` | 232 | Time-boxed: resolved mismatch |
| ETC/80140F50 | `func_80142AE8` | 152 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_801478E0` | 112 | Time-boxed: draft needs recovery |
| ETC/80140F50 | `func_801428EC` | 136 | Time-boxed: resolved mismatch |
| 80053650 | `func_80055A38` | 196 | Time-boxed: draft needs recovery |
| OPTION/801389A0 | `func_80139878` | 148 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_80145EA4` | 208 | Time-boxed: resolved mismatch |
| BUNKASAI/8015AF90 | `func_8015B828` | 216 | Time-boxed: draft needs recovery |
| BUNKA_SD/80133540 | `func_80133D54` | 592 | Time-boxed: build rejection |
| TT/801478D0 | `func_801478D0` | 328 | Time-boxed: draft needs recovery |
| TT/801478D0 | `func_80147FD0` | 236 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013A63C` | 192 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013ACBC` | 344 | Time-boxed: draft needs recovery |
| OPTION/801389A0 | `func_80139E3C` | 200 | Time-boxed: resolved mismatch |
| TT/80142570 | `func_80142AF0` | 384 | Time-boxed: build rejection |
| SHOUGATU/80135B90 | `func_80136078` | 208 | Time-boxed: draft needs recovery |
| SHOUGATU/80135B90 | `func_80136340` | 228 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013C4E4` | 228 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013CF88` | 396 | Time-boxed: build rejection |
| TACO/80146D60 | `func_801472D4` | 300 | Time-boxed: draft needs recovery |
| TACO/8015D270 | `func_8015D270` | 416 | Time-boxed: resolved mismatch |
| TACO/80146D60 | `func_80147B24` | 372 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013C780` | 504 | Time-boxed: build rejection |
| 80053650 | `func_80054704` | 136 | Matched |
| TAIIKU/80138950 | `func_8013D114` | 320 | Time-boxed: draft needs recovery |
| TACO/80146D60 | `func_80146D60` | 220 | Time-boxed: draft needs recovery |
| BUNKASAI/801354B0 | `func_801354B0` | 164 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013F948` | 460 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80145378` | 220 | Time-boxed: resolved mismatch |
| TAIIKU/80140810 | `func_80140810` | 640 | Time-boxed: resolved mismatch |
| 8004AD60 | `func_8004B590` | 488 | Time-boxed: resolved mismatch |
| TT/80142570 | `func_80142570` | 384 | Time-boxed: resolved mismatch |
| TT/801478D0 | `func_80147D08` | 524 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013CD34` | 452 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_80139E34` | 432 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80147A80` | 364 | Time-boxed: build rejection |
| NAME_ENT/80144AD0 | `func_80149144` | 364 | Time-boxed: build rejection |
| ENDING/80137340 | `func_80137E40` | 292 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80145FB0` | 296 | Time-boxed: draft needs recovery |
| BUNKASAI/80146030 | `func_80146030` | 208 | Time-boxed: resolved mismatch |
| GEKO/801323E0 | `func_80133024` | 280 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013D788` | 556 | Time-boxed: build rejection |
| NAME_ENT/80144AD0 | `func_801469D0` | 364 | Time-boxed: build rejection |
| 80053650 | `func_80054E6C` | 340 | Time-boxed: draft needs recovery |
| 8004AD60 | `col2sepia` | 580 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_801400D8` | 608 | Time-boxed: build rejection |
| TACO/80146D60 | `func_801471D0` | 260 | Time-boxed: draft needs recovery |
| TACO/80146D60 | `func_80147928` | 508 | Time-boxed: draft needs recovery |
| SHUGAKU/801378B0 | `func_801388D8` | 392 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_801455E4` | 380 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_80139FE4` | 816 | Time-boxed: draft needs recovery |
| ETC/8014ABD0 | `func_8014B0D0` | 228 | Time-boxed: draft needs recovery |
| ETC/8014ABD0 | `func_8014BF2C` | 236 | Time-boxed: resolved mismatch |
| TT/801478D0 | `func_80147A18` | 712 | Time-boxed: build rejection |
| TACO/80146D60 | `func_80146F74` | 604 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80149B08` | 484 | Time-boxed: build rejection |
| TACO/80146D60 | `func_80147674` | 644 | Time-boxed: draft needs recovery |
| SHOUGATU/8013D660 | `func_8013D820` | 268 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013A314` | 808 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_80139AB0` | 812 | Time-boxed: draft needs recovery |
| DATE2/801326F0 | `func_801328D0` | 288 | Time-boxed: resolved mismatch |
| SHOUGATU/8013D660 | `func_8013D92C` | 304 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_801454CC` | 280 | Time-boxed: resolved mismatch |
| 8004AD60 | `make_color_bar` | 1072 | Time-boxed: resolved mismatch |
| ETC/80140F50 | `func_80141468` | 316 | Time-boxed: resolved mismatch |
| ETC/8014ABD0 | `func_8014AF28` | 424 | Time-boxed: draft needs recovery |
| SHOUGATU/80135B90 | `func_8013644C` | 324 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_801473AC` | 484 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_801486F8` | 484 | Time-boxed: resolved mismatch |
| SHUGAKU/8013AA70 | `func_8013ABAC` | 356 | Time-boxed: resolved mismatch |
| TACO/80146D60 | `func_80147C98` | 488 | Time-boxed: draft needs recovery |
| TEL/80132000 | `func_8013655C` | 456 | Time-boxed: build rejection |
| RPG_BAT/801421E0 | `func_80142544` | 364 | Time-boxed: resolved mismatch |
| RPG_BAT/801421E0 | `func_80142DF4` | 380 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013B608` | 388 | Time-boxed: build rejection |
| 80053650 | `func_8005478C` | 248 | Time-boxed: resolved mismatch |
| TACO/80143E80 | `func_801446D8` | 248 | Time-boxed: draft needs recovery |
| ETC/80140F50 | `func_80141ABC` | 300 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013B200` | 432 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013B78C` | 448 | Time-boxed: build rejection |
| GEKO/801323E0 | `func_80133598` | 452 | Time-boxed: resolved mismatch |
| 80053650 | `func_8005557C` | 1212 | Time-boxed: draft needs recovery |
| 8004AD60 | `make_color_bar16` | 1540 | Time-boxed: build rejection |
| 80053650 | `func_800536AC` | 324 | Time-boxed: resolved mismatch |
| TACO/80137F90 | `func_8013801C` | 324 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013B94C` | 496 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013FC5C` | 488 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80148F08` | 528 | Matched |
| TAIIKU/80138950 | `func_8013B3B0` | 528 | Time-boxed: resolved mismatch |
| GEKO/801323E0 | `func_80133308` | 656 | Time-boxed: resolved mismatch |
| RPG_BAT/8013AD50 | `func_8013B2F0` | 580 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013C0E4` | 600 | Time-boxed: build rejection |
| GEKO/80134040 | `func_80134808` | 432 | Time-boxed: resolved mismatch |
| ENDING/80137340 | `func_80137BB4` | 652 | Time-boxed: resolved mismatch |
| 80053650 | `func_800552DC` | 672 | Time-boxed: build rejection |
| OPTION/801389A0 | `func_80138DF8` | 456 | Time-boxed: build rejection |
| RPG_BAT/8014DF60 | `func_8014E300` | 692 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013C978` | 900 | Time-boxed: resolved mismatch |
| OPTION/801389A0 | `func_80138C20` | 472 | Time-boxed: build rejection |
| OPTION/801389A0 | `func_80138FC0` | 472 | Time-boxed: build rejection |
| NAME_ENT/80144AD0 | `func_80144B44` | 328 | Time-boxed: build rejection |
| ENDING/80137340 | `func_801387B0` | 748 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_80144C8C` | 780 | Time-boxed: draft needs recovery |
| GEKO/801323E0 | `func_80132D04` | 800 | Time-boxed: resolved mismatch |
| TACO/80146D60 | `func_80147E80` | 560 | Time-boxed: build rejection |
| 8004AD60 | `func_8004B358` | 568 | Time-boxed: draft needs recovery |
| ETC/80132880 | `func_80132C3C` | 884 | Time-boxed: resolved mismatch |
| SHOUGATU/8013DD90 | `func_8013DF14` | 436 | Time-boxed: draft needs recovery |
| TEL/80132000 | `func_8013639C` | 448 | Time-boxed: build rejection |
| TACO/80143E80 | `func_80144430` | 672 | Time-boxed: draft needs recovery |
| ETC/80140F50 | `func_801421AC` | 484 | Time-boxed: resolved mismatch |
| ETC/80140F50 | `func_80142390` | 484 | Time-boxed: resolved mismatch |
| GEKO/801323E0 | `func_801325C0` | 492 | Time-boxed: resolved mismatch |
| 80053650 | `func_80055AFC` | 1396 | Time-boxed: draft needs recovery |
| 80053650 | `func_80054FC0` | 796 | Time-boxed: build rejection |
| SHOUGATU/8013D660 | `func_8013DAAC` | 592 | Time-boxed: draft needs recovery |
| BUNKA_SD/80133540 | `func_80133984` | 832 | Time-boxed: resolved mismatch |
| 8004AD60 | `func_8004AE54` | 840 | Time-boxed: draft needs recovery |
| TT/80142570 | `func_801427A8` | 840 | Time-boxed: build rejection |
| BUNKA_SD/80137B40 | `func_8013867C` | 404 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_80138AA8` | 848 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013D2B4` | 1236 | Time-boxed: draft needs recovery |
| ENDING/80137340 | `func_80137730` | 888 | Time-boxed: draft needs recovery |
| OPTION/801389A0 | `func_801394F4` | 900 | Time-boxed: draft needs recovery |
| ETC/80132880 | `func_801328A4` | 920 | Time-boxed: draft needs recovery |
| TACO/80143E80 | `func_80144094` | 924 | Time-boxed: draft needs recovery |
| BUNKA_SD/80137B40 | `func_80137FEC` | 928 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013F3A4` | 1444 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013E894` | 972 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013D9B4` | 1028 | Time-boxed: draft needs recovery |
| 80053650 | `func_80054C74` | 504 | Time-boxed: draft needs recovery |
| BUNKA_SD/80133540 | `func_80133FA4` | 508 | Time-boxed: build rejection |
| RPG_BAT/8014DF60 | `func_8014DF60` | 580 | Matched |
| SHUGAKU/801378B0 | `func_80137CFC` | 520 | Time-boxed: draft needs recovery |
| RPG_BAT/801421E0 | `func_801427E4` | 588 | Time-boxed: resolved mismatch |
| NAME_ENT/80144AD0 | `func_80149840` | 668 | Time-boxed: resolved mismatch |
| BUNKASAI/80150930 | `func_801509A4` | 472 | Time-boxed: draft needs recovery |
| 80053650 | `func_80054884` | 624 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80146624` | 752 | Time-boxed: build rejection |
| ETC/80132880 | `func_80132FB0` | 832 | Time-boxed: draft needs recovery |
| TAIIKU/80138950 | `func_8013DDB8` | 1092 | Time-boxed: build rejection |
| TAIIKU/80138950 | `func_8013E1FC` | 1688 | Time-boxed: draft needs recovery |
| RPG_BAT/8013AD50 | `func_8013AD50` | 460 | Time-boxed: draft needs recovery |
| OPTION/801389A0 | `func_80139198` | 860 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80145AC4` | 992 | Time-boxed: resolved mismatch |
| TAIIKU/80138950 | `func_8013EC60` | 1788 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_80144FDC` | 924 | Time-boxed: draft needs recovery |
| NAME_ENT/80144AD0 | `func_8014826C` | 1120 | Time-boxed: resolved mismatch |
| ETC/80132880 | `func_80136448` | 1280 | Time-boxed: build rejection |
| ETC/80140F50 | `func_80141154` | 680 | Time-boxed: resolved mismatch |
