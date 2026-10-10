---
id: T-0500
title: Split game rodata, data and bss per source file
status: Done
assignee: claude
created: 2026-10-09
updated: 2026-10-09
links: ["[[source-files]]", "[[build-system]]", "[[decompile-workflow]]", "[[tickets/T-0012-game-file-boundaries-and-shift-jis]]", "[[tickets/T-0301-sdk-rodata-data-split]]", "[[tickets/T-1340-tooling-jump-table-functions]]", "[[tickets/T-3050-run-per-object-migration-after-wave-2]]", "[[tickets/T-3051-review-low-confidence-object-boundaries]]", "[[tickets/T-3052-per-object-data-bss-split]]"]
---

## Goal

The game code is split into 29 files ([[source-files]]) but `.rodata`, `.data` and `.bss` are still one subsegment each. Split them to match the files, so that string literals and statics can move into the C files when functions are decompiled.

Widened by the orchestrator (2026-10-09): make every function with a jump table, and every function using rodata (strings, constants), matchable from C with one documented mechanism, for the main exe and all overlays: find the original object boundaries of text and rodata, choose the build model, implement it with a migration tool that wave-2 trees can be put through later, and prove it on a few units. `.data`/`.bss` per object moved to [[tickets/T-3052-per-object-data-bss-split]].

## Acceptance criteria

- [x] Object boundaries of every overlay's text and rodata and of the main exe derived by a tool, with evidence and confidence in the wiki (`tools/object_boundaries.py`, `config/objects/*.txt`, [[source-files]] "Original objects").
- [x] Build model chosen and implemented: one C file per original object with its own `.rodata` island ([[build-system]] "Per-object C files"); `configure.py`, `progress.py`, `queue.py`, `dupes.py`, `m2c.py`, `funcdiff.py`, `permute.py`, `trailing_pad.py`, `check_headers.py` (unchanged, walks subfolders), `objdiff.json`, CI updated.
- [x] Migration tool `tools/split_objects.py`, idempotent, tested (`tools/test_objects.py`); not run in bulk on `src/` (wave 2); proven on RENSYU, OMIMAI, TEL, OLH and `src/main/80062CD0.c`; full trial on a copy: 27 of 27 OK. Bulk run: [[tickets/T-3050-run-per-object-migration-after-wave-2]].
- [x] At least 15 jump-table or rodata functions matched in the proof files, several of 512 bytes or more: 18 (4 of them 624-676 bytes).
- [x] Clean build (`rm -rf asm build`, configure, `ninja` without `-k`): 27 of 27 OK; progress at least start plus gains.
- [x] [[build-system]], [[decompile-workflow]], [[source-files]], [[toolchain]], [[overlays]] updated.

## Notes

Why T-0012 did not split rodata even for the clear cases: (1) of the 8 rodata object starts that code references exactly, 6 coincide with a file boundary and 2 do not (see [[source-files]]), so a rodata split would give 6 cuts for 28 files and the pieces between would still be unattributed; (2) only about 80 game functions reference `.rodata` directly and most of its 0x3EE0 bytes (strings reached through `.data` pointer tables, tables in the 0x800B2000-0x800B2300 region) have no code reference to attribute them to a file; (3) the rodata/data/bss of the game and of the SDK libs interleave from 0x800B23B0 on, which needs [[tickets/T-0301-sdk-rodata-data-split]] first; (4) a wrong cut would still match the sha1, so there would be no build signal to catch a bad ownership claim. Naming a partial split would therefore assert unproven ownership.

Why T-0012 did not do it: only about 80 game functions reference `.rodata` directly (strings are mostly reached through `.data` pointer tables), and the per-file `.data`/`.bss` blocks interleave with shared globals defined in other files, so a cut needs symbol-level ownership (who defines a symbol), not just who uses it.

Starting evidence (`tools/game_boundaries.py`): rodata objects start 16-aligned after a zero run of at least 5 bytes: `800AF370`, `800AF390`, `800AFB00`, `800AFB70`, `800AFE00`, `800AFE10`, `800B1590`, `800B2000` map onto the file boundaries; `.data`/`.bss` symbols used by a single file are 94% in file order. The libpress/libcd/libsnd/libgpu strings from about `0x800B23B0` (see [[tickets/T-0301-sdk-rodata-data-split]]) must be separated first. Pointer tables in `.data` that point into rodata give string ownership. Lines in splat's own output (`.asciz` comment hex) make string extents exact.

### Decision: per-object C files (2026-10-09)
Options weighed. (a) One C file per original object, each with its own `.rodata` island (chosen). (b) Keep one C file per overlay and get several rodata chunks out of it: a single IDO object has one `.rodata` and orders it [all strings][all tables], so chunks of several objects cannot be reproduced without an object-file post-pass that splits and reorders `.rodata` by original object: a per-file mechanism with its own failure modes and no source fidelity. (c) asm-processor `late_rodata` / `INCLUDE_RODATA` alone: already in use (T-1340) and kept, but it only places rodata inside one object; it does not solve several objects per file. (a) costs more files (440 instead of 54) and a migration, but needs no new build pass: the T-1340 island mechanism applies per object unchanged, static scope matches the original, and relocations and order are byte-identical by construction (checked: 27 of 27 on the full trial).

### Results
- `tools/object_boundaries.py`: 440 objects (main 34, overlays 406), 357 rodata chunks, all island-capable; text starts 359 `pad`, 25 `rodata`, 29 `choice/N` (low confidence, [[tickets/T-3051-review-low-confidence-object-boundaries]]); 3 orphan chunks. 880 of 902 jump-table functions (782 of 810 KB) and 1495 of 1518 rodata-using functions are in island objects. Details: [[source-files]].
- `tools/split_objects.py` (migration), `config/labels/` (rodata labels that the split would lose), `tools/srcscan.py` C file list from the yaml files; `tools/cc.py` compiles UTF-8 string literals as Shift-JIS ([[toolchain]]).
- Migrated in this branch: RENSYU (2 objects), OMIMAI (4), TEL (3), OLH (8), `src/main/80062CD0.c` (2: `80062CD0`, `800674B0`). Full trial of `--all` on a copy: 27 of 27 OK, idempotent.
- splat mis-split fixed: `func_80066A2C` and `func_80066A84` had their case blocks cut into `func_80066A68/70/78`, `func_80066AC0`; sizes in `config/symbol_addrs_main.txt` (main game 834 -> 830 functions; the two "matched" case blocks were not functions).
- Matched (18): main `func_80066A2C`, `func_80066A84`; OLH dispatchers `func_80132420`, `func_80132FA0`, `func_80133B80`, `func_80134500`, `func_80134F40`, `func_80135AC0`, `func_801364E0` (204 bytes each) and menu handlers `func_80132614`, `func_801331C8`, `func_80133D74`, `func_801346F4`, `func_801366D4` (304 each); OMIMAI `func_8013285C` (628), `func_80132EB0` (624), `func_801340E4` (672), `func_80134384` (676, two switches, 14 string literals).

### Limits
- `.data`/`.bss` stay one blob ([[tickets/T-3052-per-object-data-bss-split]]); a C file cannot define initialised or static variables yet.
- `choice/N` text cuts are not checked by the build; 3 orphan chunks stay asm (22 jump-table functions).
- A string used by a C function and by an `INCLUDE_ASM` function of the same object is emitted twice (T-1340); the sha1 shows it.
- Main rodata after `0x800B23B0` (SDK) is not touched.

## Comments

- 2026-10-09 T-1340: rodata chunk islands for jump-table functions exist (`.rodata` subsegment named like a C file; [[build-system]]). New boundary evidence: a jump table that ends in zero words up to a 16-byte boundary ends an original object (70 cases in the overlays); IDO's per-object order is [strings][tables][padding]. A per-object split of the C files would allow several islands per file.
- 2026-10-09 verification: clean build (`rm -rf asm build`, `python3 configure.py && ninja`, no `-k`): 27 of 27 sha1 OK. `ninja progress`: main 271/830 (20736 bytes), overlays 913/6128, grand total 1184/6958 functions, 113072/2279368 bytes (start: 1168/6962, 107384 bytes; +18 matches, -2 case blocks that were not functions, -4 functions in the denominator). All `tools/test_*.py` OK; `split_objects.py --dry-run` on the migrated units: nothing to write.
- 2026-10-09 inline code review against CODING_STANDARDS.md (standards and spec). C89 and `/* */` only; no `NON_MATCHING`, no `FAKE`; `*(u8 *)&D_800E7313` for the u8 view of an s8 global (8a); new externs only in the overlay headers that use them (`OMIMAI.h`: the nine table arrays, `D_800E6374`, `func_800AE0F0` as in ENDING/KANGEI/SHUGAKU; `OLH.h`: the OLH-local callees), `check_headers.py` OK; placeholders kept, function sizes in a symbol file (`config/symbol_addrs_main.txt`), no renames; generated asm untouched (rodata labels through `config/labels/`, a splat input); scripts Python 3 with docstrings, Docker-only, tests added (`test_objects.py` 16, `test_cc.py` +2, `test_trailing_pad.py` +1, `test_queue.py` updated); commits small with the ticket id. Section 10: the C now holds 14 short Japanese place names as string literals of matched code (`func_80134384`), the same text the original code addresses; no dumps or assets, `asm/`, `build/`, `expected/`, `disc/` not staged. Findings fixed during the review: tagged-tuple/getattr state and an always-true parameter in `split_objects.py` (cleanup commit), CODING_STANDARDS sections 5 and 6 updated for per-object files and generated INCLUDE_RODATA lines. Spec: all points of the widened goal covered; `check_headers.py` needed no change (it walks `src/` recursively). Open items moved to T-3050 (bulk run after wave 2), T-3051 (low-confidence cuts, orphans), T-3052 (`.data`/`.bss`). No open findings.
