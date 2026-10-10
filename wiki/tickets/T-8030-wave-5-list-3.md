---
id: T-8030
title: Wave 5: list 3
status: Done
assignee: agent (w5-3)
created: 2026-10-10
updated: 2026-10-10
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[game-state]]", "[[data-types]]"]
---

## Goal

Match the functions of wave-5 work list 3 (146 functions, 48476 bytes, 37 files: main 80042A00 .. 80075320 and 17 overlays) with plain C where possible.

## Acceptance criteria

- [x] Work list time-boxed out; every match verified by ninja and funcdiff (30 functions).
- [x] T-0018 blocked cases recorded in [[data/t0018-cases]] (20 rows).
- [x] Clean build 27 of 27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK.
- [x] Inline review against CODING_STANDARDS.md recorded below.

## Notes

Branch w5-3, not merged. Result: 30 of 146 functions, 7512 bytes; progress 3967 -> 3997 of 6958 (521724 of 2279368 bytes after the branch). Idioms and unsolved shapes: [[matching-notes]], section "Wave 5, list 3 (T-8030)".

Matched: main `parameter_change`, `func_800732F8`, `func_800733D8`, `func_800734C4`, `func_800735B8`, `func_800736AC`; BUNKA_SD `func_80133540`; EVENT `func_80106150`; ETC `func_80143240`, `func_801446E4`, `func_8013E364`, `func_8013F320`; GYOZI `func_8014210C`, `func_8013AA48`; OLH `func_80133EA4`, `func_80133F90`, `func_80134184`, `func_80134254`, `func_80134324`; OPTION `func_8013A334`; TACO `func_8014B2A4`, `func_8014BC80`, `func_8014D3B4`, `func_8014D918`, `func_8014DA6C`, `func_8014E048`, `func_8014EC4C`, `func_8014F044`; TT `func_80148210`, `func_80148714`.

58 listed functions are in the shared-`lui $at` groups ([[data/shared-at-groups]]) and were skipped up front. Top blocker after that: the constant-in-`$a0` shared between a store and a call argument (4 functions) and the `$v0`/`$v1` selector choice.

## Comments
- 2026-10-10 inline review against CODING_STANDARDS.md (sections 1 to 13). Matches: each verified with `funcdiff.py --resolve` and a full `ninja` before its commit; the final clean rebuild (`rm -rf asm build`, configure, ninja) is 27 of 27 OK. No `NON_MATCHING` added; non-matching attempts were returned to `INCLUDE_ASM`. C89: declarations at block top, `/* */` comments. Types: fixed-width; `GameState` fields only through `D_800E6280.unk_XXXX` (`migrate_globals.py --check` OK); TACO records through `D_8015EDB4[n]`. Headers: new main-exe symbols in `include/main_api.h` (`func_80072FE8`, `func_80052000`, `func_8005BD20`, ... and the `MAIN_API_OVERRIDE_func_80066104` guard written by `sync_protos.py --write`), overlay symbols in `include/ovl/<NAME>.h`; the OLH prototypes of five functions changed from `void` to `s32` because the implicit-int form is what matches; `tools/check_headers.py` and `sync_protos.py --check-branch` OK. Fakematches (all marked `FAKE`): ETC `func_80143240` and TACO `func_8014BC80` (source-line layout of the pointer init), TACO `func_8014F044` (`t ^ 0` operand swap found by the permuter), TACO `func_8014EC4C` (`s32 pad[3]` frame size). Unused helper declarations from reverted attempts were removed. No game data or `asm/` staged. Findings: none open.
- Tooling notes: no tool bug found. `tools/docker.sh ninja` rebuilds every overlay when `include/main_api.h` changes (about 500 objects, several minutes), so header edits are expensive; `funcdiff.py` builds only the one object, which hides header-check failures until the next full `ninja` (it reported MATCH while `build/headers.ok` was failing for an unguarded `MAIN_API_OVERRIDE_`; `sync_protos.py --write` fixes that).
