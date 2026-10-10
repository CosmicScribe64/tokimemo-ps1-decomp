---
type: concept
updated: 2026-10-10
sources: ["tools/type_recovery.py", "tools/test_type_recovery.py", "include/main_api.h", "config/symbol_addrs_types.txt", "wiki/matching-notes.md"]
---

# Data types recovered from access patterns (T-5000)

splat names every address the original code touches, so one struct or array of the original source shows up as many `D_` globals. Declaring those globals separately is not only untidy: IDO treats separate symbols as non-aliasing and moves a later load above an earlier store, while the original compiler kept the order because the accesses went through one symbol. The wave-3 agents worked around this with "reach the later field through the first symbol" (`(&D_X)[n]`, marked `FAKE`). This page describes the tool that finds the real aggregates, the conversions made with it, and their limits. Ticket: [[tickets/T-5000-type-recovery-arrays-structs]].

## The rule behind it

- The original compiler hoists a load above a store to another symbol (most functions show it, for example `lw D_8015DFCC` above `sw D_8015E208` in DATE `func_80150460`), but keeps a load after a store when both go through one symbol. IDO 5.3 behaves the same way. So a load that the original leaves after an earlier store, with nothing else stopping it, says that both addresses belong to one object.
- T-5020 (r3-consts) measured a second, independent sign: the original's as1 shares one `lui $at` only between stores through the same symbol (320 shared address pairs, 10,669 re-emitted, 0 conflicts). Those groups are listed in `wiki/data/shared-at-groups.md` (branch r3-consts). IDO 5.3 does not reproduce the sharing, so a struct alone does not match those functions.
- Array or struct matters. RPG_BAT `D_8015EC58..D_8015EC6C` match as a struct of six words but not as `s32[6]`: IDO evaluates the sums in `func_801516E4` in another order for array elements. Try the struct first when the members are read in arithmetic.
- The base matters. The 0x24-byte table had to be based at `D_801217D0`, not `D_801217AC`: IDO folds `&T[1]` into the load offsets, while the original had the symbol at `D_801217D0` in the relocation (splat's `D_801217AC` is the code reading entry `i - 1`).

## tools/type_recovery.py

Reads the generated asm of all functions (`asm/**/matchings` and `nonmatchings`, main exe and 26 overlays; `disassemble_all` keeps both), follows the registers of each function in one linear pass, and records five kinds of evidence: indexed accesses with their stride (`sll`, `multu` by a constant, shift-and-add such as `x * 0x44`), pointer fields and walks with their end, ordered store-then-load pairs, stores through one `lui $at`, and co-access of neighbours. Symbols resolve per address space (main exe, or an overlay's own data labels; EVENT's data overlaps main bss addresses and is kept apart). Output: ranked proposals with base, size, stride, fields (offset, width, signedness from `lb/lbu/lh/lhu`), absorbed symbols, evidence counts and confidence; `--sym NAME` shows every proposal that holds a symbol, `--fake-sites` checks the `(&D_X)[n]` tricks in `src/`, `--json` dumps all. Tests: `tools/test_type_recovery.py` (13 cases, synthetic asm). Run time about 10 s in Docker.

Counts on the current tree: 1724 proposals, 654 high (544 arrays, 110 structs; 129 in the main exe, 525 in overlays; they absorb 11,593 symbols), 559 medium, 511 low (co-access only). Evidence in the high ones: index 525, walk 193, later-element field 188, loop end 186, ordered pairs 92, shared `$at` 76, pointer fields 39. Before conversion the `--fake-sites` check found a proposal for 70 of the 72 index-trick sites.

Limits: the pass is linear (no control flow), so a register state survives a label; proposals overlap (an outer array and a record view, or a 2-D index); the main-exe bss block `0x800E6248..0x800E7400` gives many large, conflicting proposals because it is very likely one game-state struct (see below). Treat the output as hypotheses and let the build decide.

## Conversions made (all byte-identical, clean build 27/27)

| type | base | where | absorbed | FAKEs removed |
|---|---|---|---|---|
| `Rec34[12]` (0x34 bytes, flag word as bit-field struct `Rec34Flags`) | `D_800B0A04` | `main_api.h` | 17 globals `D_800B0A06..D_800B0C16` | 13 (EVENT) |
| `Rec38[16]` (0x38 bytes, s16 at +2/+6/+A) | `D_800E643C` | `main_api.h` | 11 globals, `MAIN_API_OVERRIDE_D_800E6636`, the `s16[]` views of `D_800E643E`/`D_800E6476` | 9 (DATE) |
| `Rec24[]` (0x24 bytes) | `D_801217D0` | `main_api.h` | 19 globals | 2 (main) |
| `Work80125D10` (0x50 bytes, one work area) | `D_80125D10` | `main_api.h` | 33 globals of main `80079B10`/`8007C030` | 1 |
| `RpgRec18` (six words) | `D_8015EC58` | `include/ovl/RPG_BAT.h` | 5 globals | 1 |
| byte view of the 0x44-byte record table | `D_8011ECD0` | existing `u8 D_8011ECD0[]` | uses only | 6 (ENDING 2, DATE 1, RPG_BAT 2, main 1) |
| byte view of EVENT's 0x44-byte table | `D_800EAFA0` (new) | `main_api.h` | uses only | 1 |

33 `FAKE` comments of the "first symbol" family are gone (117 to 84 in `src/`). The `Work80125D10` conversion is the strongest single test: 96 uses in already-matched functions moved to one symbol and none changed bytes.

Linking: a base that no original instruction names has no label. Overlays get such bases from `config/symbol_addrs_types.txt`, which `configure.py` adds to `build/main_names.ld`. A main-exe base without a label would need a line in `config/symbol_addrs_main.txt` (splat then splits the bss label); none is needed now. No `size:` attributes were added to splat: the absorbed symbols stay as labels in the generated asm, which is harmless.

## Load-hoist functions retried (20)

| result | functions |
|---|---|
| matched with the recovered types (7) | GYOZI `func_80135C3C` (`GyoziWork.girl[5]`), KANGEI `func_80135C18`, SHUGAKU `func_80135B68`, NAME_ENT `func_8013B51C` (records 31/96/119 of `D_8011ECD0`), EVENT `func_800F87BC` (`D_800EAFA0`), DATE `func_8013C700`, `func_8013D9E8` (`Rec38[2]`) |
| match only through a base-symbol view whose real type is not recovered; left as asm (3) | SHUGAKU `func_80135994`, GEKO `func_8013B350` (`D_800E69A0` and `D_800E7388` are 0x9E8 apart in one object: the bss game-state block), EVENT `func_8010B678` (a second 0x44 table and a 0x24 table below `D_800EAFA0`) |
| load order fixed, another gap left (5) | GEKO `func_8013E1A0` (temp numbering, T-0018), DATE `func_80148924` and main `func_80079F00` (the original sign-extends `s16` arguments in the callee, IDO 5.3 does not), DATE `func_8015745C` (one `sb` scheduled earlier), main `func_8007A868` (load placement, 18 words) |
| not a one-object case (5) | DATE `func_80149F48` (the original hoists across these words: separate objects), TAIIKU `func_8013A63C` (shared `lui $at`, T-5020), EVENT `func_80100DD8` (one global read twice), GEKO `func_80140FD0` (bit-fields at `Rec38` +0x0C not modelled), main `func_8007A6AC` (T-0018 row, needs the callee's return struct) |

## Open work

- The 0x44-byte table at `D_8011ECD0` (160 records; `D_80120650` is record 96) is still a `u8[]` view: its users read the same offsets as `u8` and `s8` and some words as `s32` or pointers, so one struct does not fit all of them yet.
- The main bss block `0x800E6248..0x800E7400` behaves as one object (ordered pairs and the matches above span 0x9E8 bytes). Its layout (it contains `Rec38` at `0x800E643C` and bit-field words at +0x0C of each record) is the next big type to recover; the remaining FAKEs at `D_800E62B6`, `D_800E7395`, `D_800E699E`, `D_800E71DF`, `D_800B1746` wait for it.
- `Rec38` fields +0x0C..+0x37 keep their own names: their users read bit-fields through bytes and words.
- Main `D_80121818`, `D_8012183C`, `D_80121860`, `D_80121864`, `D_80121884` stay declared because EVENT has its own data at those addresses.
