---
type: concept
updated: 2026-10-10
sources: ["tools/type_recovery.py", "tools/test_type_recovery.py", "include/main_api.h", "config/symbol_addrs_types.txt", "wiki/matching-notes.md", "tools/migrate_globals.py", "tools/aggregate_audit.py"]
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

Limits: the pass is linear (no control flow), so a register state survives a label; proposals overlap (an outer array and a record view, or a 2-D index); the main-exe bss block `0x800E6280..0x800E7D10` gave many large, conflicting proposals because it is one game-state struct, now `GameState` ([[game-state]]). Treat the output as hypotheses and let the build decide.

## Conversions made (all byte-identical, clean build 27/27)

| type | base | where | absorbed | FAKEs removed |
|---|---|---|---|---|
| `Rec34[12]` (0x34 bytes, flag word as bit-field struct `Rec34Flags`) | `D_800B0A04` | `main_api.h` | 17 globals `D_800B0A06..D_800B0C16` | 13 (EVENT) |
| `Rec38[16]` (0x38 bytes, s16 at +2/+6/+A) | `D_800E643C` (since T-5100: `GameState.unk_1BC`) | `main_api.h` | 11 globals, `MAIN_API_OVERRIDE_D_800E6636`, the `s16[]` views of `D_800E643E`/`D_800E6476` | 9 (DATE) |
| `GameState` (0x1A90 bytes, records `GsRec*`, unions `GsWord`/`GsHalf`, T-5100) | `D_800E6280` | `main_api.h` | 166 globals, 1459 uses rewritten by `tools/migrate_globals.py` | 7 |
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
| match only through a base-symbol view whose real type is not recovered; left as asm (3) | SHUGAKU `func_80135994`, GEKO `func_8013B350` (`D_800E69A0` and `D_800E7388` are 0x9E8 apart in one object: the bss game-state block), EVENT `func_8010B678` (a second 0x44 table and a 0x24 table below `D_800EAFA0`). Update T-5100: SHUGAKU `func_80135994` and GEKO `func_8013B350` match through `GameState` |
| load order fixed, another gap left (5) | GEKO `func_8013E1A0` (temp numbering, T-0018), DATE `func_80148924` and main `func_80079F00` (the original sign-extends `s16` arguments in the callee, IDO 5.3 does not), DATE `func_8015745C` (one `sb` scheduled earlier), main `func_8007A868` (load placement, 18 words) |
| not a one-object case (5) | DATE `func_80149F48` (the original hoists across these words: separate objects), TAIIKU `func_8013A63C` (shared `lui $at`, T-5020), EVENT `func_80100DD8` (one global read twice), GEKO `func_80140FD0` (bit-fields at `Rec38` +0x0C not modelled), main `func_8007A6AC` (T-0018 row, needs the callee's return struct) |

## Open work

- The 0x44-byte table at `D_8011ECD0` (160 records; `D_80120650` is record 96) is still a `u8[]` view: its users read the same offsets as `u8` and `s8` and some words as `s32` or pointers, so one struct does not fit all of them yet.
- The main bss block is `GameState D_800E6280` (0x800E6280..0x800E7D10, T-5100): [[game-state]] has the layout, the evidence and what is still unknown. Its old `D_` names are rejected by ninja; `tools/migrate_globals.py --apply` rewrites them, also in new code. Every unit reaches it through the base symbol (T-7010, [[game-state]] "Views").
- `Rec38` +0x0C and +0x10 are `GsWord` unions (word and byte views of bit-field words); a bit-field struct that fits every user is still open.
- Main `D_80121818`, `D_8012183C`, `D_80121860`, `D_80121864`, `D_80121884` stay declared because EVENT has its own data at those addresses.

## Aggregates and old names (T-5100)

Once an aggregate is declared, its fields must not be used through their splat names: separate symbols let IDO hoist loads that the original kept behind stores. `config/migrate_globals.txt` lists the aggregates (`aggregate <base> <type> <header>`) and the few uses that match only through the old symbol (`keep <file> <symbol> <reason>`). `tools/migrate_globals.py --apply` rewrites every C use of a `D_` name inside an aggregate into the field access (the old declaration picks the field: a byte of a union, an array, a record element) and deletes the old declarations; `--check` runs in ninja. Findings from the `GameState` migration that hold for any type here:
- A constant-index array element is not a struct member for IDO: a `switch` on it and a sum of two of them compile differently. Use separate members where the matched code needs them.
- `((u8 *)&S)[k + i]` is not the same as the old `u8 S[]` view: it changes the induction variable. Write the field access.
- Struct-array elements in a sum can swap the load order against a scalar extern. The one such case (SHOUGATU `func_8013E0F0`) was a bit-field and matches through the struct (T-7010); there is no `keep` line now.

## Member or separate symbol: what IDO changes (T-7010)

- IDO 5.3 compiles a constant-offset member of an extern struct like a scalar extern at that address: same instruction words after relocation, same registers, and uopt keeps members apart (a member value survives a store to another member). Only alias rules differ: as1 does not move a load above a store through the same symbol (it does for two symbols), and uopt reloads a member after a store through an indexed element or a pointer into the struct. A diff in registers or operand order is never fixed by switching between the two views; check with both before blaming the declaration.
- Evidence for a separate symbol in the original is therefore a load hoisted above a store between two fields of an aggregate (`tools/aggregate_audit.py --pairs`). For `GameState` there is none (407 pairs kept, 0 hoisted), and `tools/migrate_globals.py --check` accepts a `keep` line only with such a pair in that file's original object.
- `lui rA; lbu rB,%lo(D)(rA); ori/andi; sb` with rB different from rA is a bit-field store, not a separate symbol: IDO computes a bit-field member's address apart from the load. Write a bit-field view of the word (a local `typedef struct { u32 pad0 : 14; u32 f : 1; u32 pad1 : 17; }` cast on the member, LSB first, as the neighbouring files do); `tools/aggregate_audit.py --bitfields` lists the sites in the aggregates (56 in `GameState`), and the same shape appears on `Rec34` (`unk_0C.b14`) and EVENT/GYOZI flag bytes.

## T-9020 additions
- `Rec38Flags` / `CharFlags` (`include/main_api.h`): the flag word at +0x0C of a `Rec38` record as a union of `w`, `u`, `h[2]`, `b[4]` and the bit-field struct `f` (bits 0-8, `f9` = bits 9-11, bits 12-14). Evidence: `sll; bltz/bgez` tests of bits 1, 2, 3, 6, 14 and `sll 20; srl 29` reads of bits 9-11 in the original; it replaced thirteen file-local views without changing a byte.
- `D_800CA21C`, `D_800CA224`, `D_800CA22C`, `D_800CA234`: four `s16[4]` rows (were twelve `s16` scalars). DATE indexes each row from its own base and sets rows with chain assignments; `D_800CA232` is written, so the rows have four entries.
- `D_8011ECD0` (the 0x44-byte table): OPTION and ENDING functions match only when `D_8011ECF6`/`D_8011ECFA`/`D_801208FB` and `D_8011F4D0`..`D_8011F522` are read through it, more evidence that it is one object (record n field k = `D_8011ECD0[n * 0x44 + k]`). Still a `u8[]` view.
- ENDING: `D_8013C35C` is `s16`, `D_8013C360` is `u32`.

