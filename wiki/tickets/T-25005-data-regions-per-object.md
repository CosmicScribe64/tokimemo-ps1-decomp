---
id: T-25005
title: "Build: more than one .data region per C object"
status: In Review
assignee: agent (worktree data2)
created: 2026-10-20
updated: 2026-10-20
links: ["[[build-system]]", "[[matching-notes]]", "[[tickets/T-25001-class-c-main-sndisp-strcallback]]", tools/data_split.py, tools/data_pieces.py, tools/data_island.py, tools/object_layout.py, src/main/8004F870.c]
---

## Goal

Let one C object own two or more `.data` ranges, so its data lands at every original address. The trigger is main `sndisp` (T-25001, guarded at 2 rows). Its `u8 pos[5] = {0,...}` template is `D_800B40F8` in the `800B40F0` range, while the object's `.data` was already the `D_800E3680` island.

## Acceptance criteria

- [x] yaml, data_island.py, data_pieces.py, object_layout.py and the linker script support further `.data` islands of an object; the bytes of every other object stay the same.
- [x] sndisp in plain C, `funcdiff.py --resolve main:sndisp` MATCH.
- [x] Tests with synthetic input only (`tools/test_data_split.py`, `tools/test_data_islands.py`, `tools/test_object_layout.py`).
- [x] Docs in [[build-system]] ("Several ranges per object").
- [x] Clean build 27/27, check_headers, migrate_globals --check, sync_protos --check-branch.

## Notes

A further island is a yaml dict line with `dir: island_<VRAM>` and `linker_section: .data_<VRAM>`. splat (0.50) supports both keys, so the linker script and the island file need no splat change. `tools/data_split.py` cuts the compiled object; see [[build-system]] for the rule. The section name carries the address, which `funcdiff.py --resolve` already resolves.

## Comments

- 2026-10-20: done in worktree `data2` (branch data2, from w2-good). Cherry-picked the T-25001 guards (b5dee4aa5), applied `data_island.py main/8004F870`, removed the sndisp guard. MATCH (388 bytes), clean build 27/27. No other guarded or INCLUDE_ASM function has a NON_MATCHING note or matching note naming a second data range as its only gap. `tools/test_build_deps.py` fails the same 11 tests before and after this change: its temporary tree lacks `tools/ugen_pass.py`.
