---
id: T-9130
title: Wave 6: list 3
status: Done
assignee: wave-6 agent 3
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the functions on wave-6 work list 3 (36 files, 130 functions) in worktree w6-3.

## Acceptance criteria

- [x] Matched functions committed, build 27/27 OK
- [x] Inline code review against CODING_STANDARDS.md recorded

## Notes
37 functions matched, 10,736 bytes (4267 -> 4304 of 6958 functions). Matched: GYOZI `func_80140780`, `func_80140AFC`; DATE2 `func_801378A4`, `func_80137A2C`; EVENT `func_800FC56C`, `func_800FC1BC`, `func_80107A28/BB0/CEC/E20`, `func_801046F4`, `func_801041C4`, `func_80105C8C`, `func_801071D8`, `func_80103FF0`, `func_80105460`; OPTION `func_801387E4`, `func_80132AB8`; KANGEI `func_80138158`, `func_80138E64`, `func_80134F00`; TACO `func_8013587C`, `func_8013AFDC`, `func_80136024`; RPG_BAT `func_8014F790`, `func_801504E0`, `func_80151984`, `func_801513C8`, `func_80150F84`; BUNKAKEN `func_8013CF30`; TT `func_80142D40`, `func_80143580`, `func_8013CA0C`; SHOUGATU `func_8013BD2C`; GEKO `func_80139640`; SHUGAKU `func_80138044`, `func_8013799C`. Patterns and 27 skipped cases: [[matching-notes]] (section "Wave 6, list 3"), [[data/t0018-cases]].
Not buildable here: DATE `func_8015A980` and TT `func_8014D260` are gcc-compiled SDK code.
Tooling: none broken. `sed -i` in tool snippets needs `''` on macOS; `funcdiff` on the main exe needs `--unit main`.

## Code review (inline, CODING_STANDARDS.md)
- Section 4/7a: every trick is marked `FAKE` with its reason (8 frame pads, 1 `D_800B094E` table address, 1 RECT one-liner reused from the neighbour). No dead code, no debug output.
- Section 8a: new main-exe symbols went through `sync_protos.py --fix`; `check_headers`, `migrate_globals --check` and `sync_protos --check-branch` pass; no override added.
- No game data, `asm/`, `build/`, `expected/` or `disc/` staged. Strings are UTF-8 literals; rodata checked by the sha1 steps (27/27 OK).

## Comments
