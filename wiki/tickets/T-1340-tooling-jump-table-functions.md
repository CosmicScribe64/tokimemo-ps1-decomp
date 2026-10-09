---
id: T-1340
title: Tooling: jump-table functions
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[build-system]]", "[[decompile-workflow]]", "[[matching-notes]]", "[[source-files]]", "[[tickets/T-0500-per-file-game-rodata-data-bss-split]]", "[[tickets/T-0018-ugen-temp-register-order]]"]
---

## Goal

No batch had matched a function with a `switch` jump table: IDO writes the table into the C object's `.rodata`, but rodata is one asm blob in both the main exe and the overlays. Find out whether the pipeline can link such a function byte-identically, and make it work uniformly.

## Acceptance criteria

- [x] Mechanism chosen and implemented for main exe and overlays (rodata islands: `.rodata` sibling subsegment, `tools/rodata_pieces.py`, `tools/rodata_island.py`, `configure.py`).
- [x] At least 5 jump-table functions matched across main and overlays (8: main `func_80053DDC`, `func_8007A254`; RENSYU `func_80133E5C`, ETC `func_8014A2C4`, DATE `func_8013EB1C`, DATE2 `func_80133620`, GYOZI `func_8013AE80`, EVENT `func_8011A874`).
- [x] Clean build (`rm -rf asm build`, configure, `ninja` without `-k`): 27 of 27 sha1 OK; `ninja progress` grand total 714 -> 722 of 6962.
- [x] Documented in [[build-system]] and [[decompile-workflow]] ("How to decompile a switch"); unit tests `tools/test_rodata_tools.py` (12), `tools/test_cc.py` (7).

## Notes

Findings. 902 jump-table functions exist (66 main, 836 in 25 overlays). (1) The splat route (`migrate_rodata_to_functions`, compiler IDO) was tried and rejected: it moves symbols into function files only partly, changes the `matchings`/`nonmatchings` layout `tools/progress.py` counts, and cannot place a compiler-emitted table between asm ones. (2) IDO emits an object's rodata as [strings and constants][all jump tables][padding to 16]; asm-processor's `.late_rodata` reproduces the table order, so asm functions and C functions can share one object. (3) Object boundaries show in the splat output as table blocks that end in zero words up to a 16-byte boundary; an overlay or `src/main` file with several objects has several chunks but one `.rodata`, so one island (one chunk) per C file. More chunks need a per-object split of the C file, the T-0500 work (the padding witnesses are new evidence for it). (4) The dummy zero word after a table is object padding, not alignment of every table: the `.align 3` splat prints is just the address.

Not done / limits: floats and doubles (late rodata too); several chunks per file; strings shared between converted and asm functions; `funcdiff.py` shows relocation-name noise for jump-table functions (the sha1 decides). Skipped T-0018 compare-chain functions (none of the eight is one).

## Comments

- 2026-10-09 inline code review against CODING_STANDARDS.md: C89 and `/* */` only; new externs in `game.h` (`D_800CA2FC`, `func_8007E390`, `_card_status`) and overlay headers, no duplicates (`tools/check_headers.py` OK; DATE.h dropped a duplicate of a `game.h` declaration); `main_only.h` for the file-local prototypes; no `FAKE` and no `NON_MATCHING`; `format((u8 *)"bu00:")` replaces the extern string symbol, as the original literal; scripts are Python 3 with docstrings, run in Docker, tests added; `INCLUDE_RODATA` rule added to section 6; wiki, log, index and kanban updated; no game data staged (`asm/`, `build/`, `expected/`, `disc/` ignored). No open findings.
