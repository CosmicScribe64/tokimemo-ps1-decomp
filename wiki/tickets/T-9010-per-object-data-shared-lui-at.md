---
id: T-9010
title: Per-object .data/.bss/.rodata ownership (T-3052) to unlock shared lui $at
status: Done
assignee: r5-data agent
created: 2026-10-10
updated: 2026-10-10
links: ["[[tickets/T-3052-per-object-data-bss-split]]", "[[tickets/T-7000-shared-constants-lui-at-parameter-copies]]", "[[data/shared-at-groups]]", "[[matching-notes]]", "[[decompile-workflow]]", "[[build-system]]", "configure.py", "tools/split_objects.py", "tools/object_boundaries.py"]
---

## Goal

Tooling round 5. The original shares one `lui $at` across stores only for data defined in the same object file (T-7000). Give each C object its own `.data`/`.bss` so a C file can define its data at the original address, and model the shared `lui $at` for that data, so the shared-`$at` functions ([[data/shared-at-groups]]) can be matched.

## Acceptance criteria

- [x] Object boundaries for `.data` and `.bss` worked out (link order, alignment gaps, which functions use which symbols) and shipped as config, not game bytes.
- [x] A C file can define its own data (initialised data as C initialisers, `.bss` as plain definitions) and the build places it at the original address.
- [x] Proven on 2-3 shared-`$at` groups: data defined in the owning C file and the functions matched.
- [x] Workflow for matching agents in [[decompile-workflow]]; `queue.py` flag changes proposed in the report (r5-fixes owns `queue.py`).
- [x] Clean build 27/27 OK, headers OK, progress not lower than 4227/6958, no previously matched function changes bytes.

## Notes

- **Finding:** no as1 pass is needed. IDO 5.3 itself shares one `lui $at` between stores to different offsets of a symbol that the C file defines (initialised or static); it re-emits for externs, commons and different variables. That is the original's rule, so the work is data ownership: define the variable in its object's C file ([[matching-notes]], "Shared `lui $at`: data defined in the file").
- **Ranges:** `tools/object_boundaries.py --data --write` adds 248 `data`/`bss` lines to `config/objects/*.txt` from shared-`$at` groups (strong), single-object users and pointer words, in object order with 16-aligned starts ([[source-files]], "Data and bss ranges").
- **Build:** `tools/data_island.py UNIT/addr` (opt-in per object) cuts a `.data` island into the splat config and writes one `INCLUDE_RODATA` piece line per variable into the C file; `tools/data_pieces.py` (split rule) writes the pieces and `PROVIDE` lines for labels inside C-defined variables; `split_objects.py` keeps islands ([[build-system]], "Data islands").
- **Proof:** RPG_BAT `8014E780` (groups `D_8015EDB0`, `D_8015EDA8`, `D_8015EE00`, `D_8015EE30`, `D_8015EE44`, `D_8015EE50`) and main `800451D0` (`D_800B3D70`): 7 functions matched (`func_8014EBA8`, `func_8014EBD0`, `func_8014EB58`, `func_8014F230`, `func_8014F500`, `func_8014F524`, `func_80046290`). The old scalar names were migrated to struct members in 13 RPG_BAT files; all stay byte-identical. A struct is needed where other code switches on a member (an array element changes the selector register).
- **Workflow:** [[decompile-workflow]], "Data defined in the C file".
- **Proposals (not done here):** `tools/queue.py` flag `A` for shared-`$at` stores (owner known from `config/objects` `data` lines); `tools/funcdiff.py` to compare `sym+addend` so C-defined members do not show as relocation noise; `tools/cvt_pass.py` to leave variables defined in the file alone (entry selectors on own-object data: 22 `$v0` to 2 `$v1` in the original).
- **Not reproduced:** TACO `func_80143E80` (store order, see [[matching-notes]]).

## Comments

- 2026-10-10 review (inline, CODING_STANDARDS.md): matches verified by sha1 (clean build 27/27, headers OK, globals OK, `sync_protos.py --check-branch` OK; progress 4227 -> 4234). C89, fixed-width types, struct offsets documented (`RpgBatWords3/4`), neighbouring members declared as one struct (section 8), main-exe symbols only in `include/main_api.h` with the definition's type (8a: `D_800B3D70[4]`, `func_80046290(s32, s32, s32)`), no fakematch, no pass (7a not needed). Tools: Python 3 in Docker, docstrings, non-zero exit on failure, unit tests `tools/test_data_islands.py` (synthetic), all `tools/test_*.py` pass. No game bytes committed: data pieces and `PROVIDE` files are generated under `asm/` and `build/`; initialisers are C source. Findings: split_objects re-run on a unit with a C-defined global raised "used by objects" (fixed: only objects cut from the same file count) and inserted a blank line differently (fixed). No open findings.
