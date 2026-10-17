---
type: concept
updated: 2026-10-17
sources: ["include/game.h", "tools/type_recovery.py", "tools/test_type_recovery.py", "include/main_api.h", "config/symbol_addrs_types.txt", "wiki/matching-notes.md", "tools/migrate_globals.py", "tools/aggregate_audit.py"]
---

# Data types recovered from access patterns (T-5000)

T-20006 follow-up: GYOZI's D_801317EB and GyoziRec44.unk_07 are u8. The original scene callbacks read both with lbu; declaring the record field s8 leaves an lb where the original has lbu, even with a value cast afterward. The shared unsigned types allow ordinary field assignments in the two newly matched callbacks. Their actual linked proofs and whole-overlay/shared-header checks are recorded in [[tickets/T-20006-gyozi-scene-callbacks]].

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
| stack tables `RpgDirTable`, `RpgStrTbl4`, `RpgWordTbl4`, `RpgWordTbl10`, `RpgHalfTbl12` | `D_8015E4F0`, `D_8015E530`..`D_8015E578`, `D_8015E720`, `D_8015EFA0`..`D_8015EFF0` | `include/ovl/RPG_BAT.h` (T-17100) | 12 undeclared tables | 0 |
| `RpgStat` pad named (`unk_0C`..`unk_34`) | `D_8015EBFC` | `include/ovl/RPG_BAT.h` (T-17100) | pad cast in `func_801447E0` | 0 |
| byte view of EVENT's 0x44-byte table | `D_800EAFA0` (new) | `main_api.h` | uses only | 1 |
| `TaiRunnersA` (`player`, `cpu[3]` of `TaiRunnerA`, 0x24) | `D_80149208` | `include/ovl/TAIIKU.h` (T-16550) | `TaiikuSlots`, `TaiSlot24` views | 0 |
| `TaiRunnerB[4]` (0x24) | `D_8014A13C` | `include/ovl/TAIIKU.h` (T-16550) | 8 globals | 0 (FAKE byte views kept in `func_801449A0`, `func_801448D4`, `func_80144C70`) |
| `TaiikuController` player + `D_8014A400[3]`, `TaiikuSlot1C[4]` (0x1C) | `D_8014A3E4`, `D_8014A400`, `D_8014A454` | `include/ovl/TAIIKU.h` (T-16550) | 9 globals, `Tk1C`, `TaiikuTick1C` | 0 |
| 28 stack-copied constant tables as one-array structs | `D_80148F28`..`D_8014A5F4` | `include/ovl/TAIIKU.h` (T-16550) | none (new declarations) | 0 |
| `FnTblNN` step tables (35 sizes, `void (*f[NN])()`) | 68 tables `D_8015BF50`..`D_80160BD0` | `include/ovl/DATE.h` (T-17200) | 39 local typedefs and externs of 16 DATE `.c` files | 0 |
| DATE stack-copied tables `DateBytes11x21`, `DateWords21`, `DateShorts8`, `DatePairs8`, `Tbl21` | `D_8015B7A0`, `D_8015B888`/`8DC`/`930`, `D_8015B984`, `D_8015F9B0`/`9C0`, `D_8015FB1C`/`B2C`/`B3C` | `include/ovl/DATE.h` (T-17200) | none (new declarations) | 0 |
| ETC overlay data: `EtcExamRank` (0x8C), ranking row positions `s16[14]`, per-girl tables `u8[13]`, `s32[13][2]`, `s32 *[13]`, CD file table `s32 *[22]`, `SVECTOR`, `s32[16]`, `EtcWork40` view moved to the header | `D_80150668`, `D_80150630`/`64C`, `D_80150728`/`738`, `D_8014E0A4`, `D_8014DB90`/`BF8`, `D_8014E070`, `D_8014EA18`, `D_8015013C`, `D_8015014C`, `D_801230D0` | `include/ovl/ETC.h` (T-17300) | `D_80150150`, `D_80150188` (into `D_8015014C`); `D_8015013E`, `D_80150140` and `D_80150668`'s 18 splat labels stay data labels | 0 |
| `FnTblNN` step tables (36 sizes, `void (*f[NN])()`) | 90 tables `D_801207B8`..`D_8012514C` | `include/ovl/EVENT.h` (T-17400) | 63 local typedefs and externs of 15 EVENT `.c` files, `Blob80` of `D_801218E8` | 0 |
| EVENT stack-copied tables `U16x21`, `EvPairs8`, `EvShorts82`, `EvShorts10`, `EvShorts11`, `EvShorts5`; views `Ev44`, `EvTbl` | `D_80125454`..`D_801254D8`, `D_80122990`, `D_80121A34`, `D_80123B7C`/`B90`/`BA4`, `D_80125330`/`348`, `D_80125360`, `D_80120A74`, `D_80121AD8` | `include/ovl/EVENT.h` (T-17400) | local `U16x21`, `Ev44`, `EvEnt`/`EvTbl` typedefs | 0 |
| GEKO stack-copied tables `GekoMonthDays`, `GekoNames12`, `GekoWords11`/`GekoWords2`, `GekoFnTbl42`, `GekoShorts10`, `GekoShorts6`, `GekoBytes21`/`24`/`36`/`132`/`209`, `GekoText626`; existing `GekoThresholds` for `D_80144CF8`; `s16 D_801463E0[12]`, `s16 D_80146400[16]` | `D_80144CF8`..`D_80144D8C`, `D_801451B8`..`D_801451F4`, `D_80146630`, `D_801472C4`/`D8`/`EC`, `D_8014760C`/`618`, `D_80147764`..`D_80147C6C`, `D_801463E0`, `D_80146400` | `include/ovl/GEKO.h` (T-17700) | none (new declarations) | 0 |
| `ShFnTblNN` callback tables (8 sizes, `void (*f[NN])()`); `u8 D_8013C2E4`; `void D_80085368(void)` (main_api.h, address of `func_80085368`) | 10 tables `D_8013B094`..`D_8013CC20` | `include/ovl/SHUGAKU.h` (T-18000) | 8 local typedefs and externs of 7 SHUGAKU `.c` files; `Blk44`/`Tb44`/`Words17` copies now `DisplayActor44` | 0 |
| GYOZI overlay data: step-callback tables `GzCallbacks68/90/BC`, dumpling arrays `s32[50]` x3 and `s16[50]`, `s16[8]` x2, `Rec24 D_8012CA40[]`, `GyoziGirl.unk_14`, `GyoziWork.unk_10B8`, `u8 D_80148828[4]`; `GYOZI.h` includes `game.h` | `D_801476F4`, `D_80147C6C`, `D_80148880`, `D_8014901C`, `D_80148BF0`/`CB8`/`D80`/`E48`, `D_8014875C`/`76C`, `D_8012CA40`, `D_800F53A0` | `include/ovl/GYOZI.h` (T-17600); `D_800F62F0` added to `main_api.h` | `D_800F6458` written as `D_800F53A0.unk_10B8` in `func_8013DC2C` only; the other users keep the scalar | 0 |
| NAME_ENT tables: flag counters `u8 D_8014D060[16]` and `u8 D_8014D158[16]`, digit sprite (u, v) `u8 D_8014D0F4[10][2]`, name-entry buffers `D_8014CC74[8]`/`D_8014CC7C[8]` sized; main-exe records read through views in the redraft (`NameMetricWork` at `D_800E7D10`, `NameRecordF00[]` at `D_80123120`, `DisplayActor44[]` at `D_8011ECD0`) | `D_8014D060`, `D_8014D158`, `D_8014D0F4`, `D_8014CC74`, `D_8014CC7C` | `include/ovl/NAME_ENT.h` (T-17900) | none (new declarations) | 0 |
| EN_NICHI objects `EnObj D_8013984C[10]` (0x30), sprite frames `s16 D_80139A34[16][6]`, ring buffer `s16 D_80139228[10][4]`/`s32 D_80139278[10]`, stack tables `EnHalf22` x3, `EnBytes22`, `EnHalf64`, `EnHalf13`, asset pointer `s32 *D_80139204`; status and sprite bytes sized | `D_8013984C`, `D_80139A34`, `D_80139A2C`, `D_80139228`, `D_80139278`, `D_80139C68`/`C94`/`CC0`/`CEC`, `D_80139DA0`, `D_80139E20`, `D_80139200`..`D_80139218` | `include/ovl/EN_NICHI.h` (T-18200) | `u8 D_8013984C[]`, `Rec30 D_80139868[]` | 1 (`func_80132834`, peeled loop) |
| MASTER stack tables `MasterWords13` (`s32 v[13]`) x9 and `MasterHalfwords13 D_8013C6A4`; step tables `FnTbl4/7/30`, `PtrTbl3` moved into the header; second mini-event counters `u8 D_8013C788[5]`, `D_8013C780`, `s16 D_8013C794`; the local `MasterSlot` copies unified as `MasterClubBits` | `D_8013C2F4`..`D_8013C460`, `D_8013C7B4`, `D_8013C6A4`, `D_8013C2E4`, `D_8013C5C8`, `D_8013C700`, `D_8013C798`, `D_8013C788` | `include/ovl/MASTER.h` (T-18600) | `Tbl13`, two `MasterSlot` typedefs, `D_8013C78C` | 0 |
| OPTION BG cell map `SpriteMap10 D_80122120[2]` (GsMAP layout), cells `SpriteCell8 D_80122140[2][64]` (GsCELL), cell index `SpriteIndex80 D_80122540[2]`; prototype `void func_80134210(void)` | `D_80122120`, `D_80122140`, `D_80122540` | `include/ovl/OPTION.h` (T-20910) | none (new declarations of main bss read only by OPTION) | 0 |

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

- The 0x44-byte table at `D_8011ECD0` (160 records; `D_80120650` is record 96) has one record type since T-20900 (`DisplayActor44`, fixed-point view `DisplayFixActor44`, section "Shared-type unification"), but the declaration is still `u8 D_8011ECD0[]` and its 381 splat labels (5,286 uses) are separate globals. Next step: `extern DisplayActor44 D_8011ECD0[160]` as a `config/migrate_globals.txt` aggregate, with the fixed-point labels and main `src/main/800674B0.c` (which declares the table as records) handled first.
- The main bss block is `GameState D_800E6280` (0x800E6280..0x800E7D10, T-5100): [[game-state]] has the layout, the evidence and what is still unknown. Its old `D_` names are rejected by ninja; `tools/migrate_globals.py --apply` rewrites them, also in new code. Every unit reaches it through the base symbol (T-7010, [[game-state]] "Views").
- `Rec38` +0x0C and +0x10 are `GsWord` unions (word and byte views of bit-field words); a bit-field struct that fits every user is still open.
- Main `D_80121818`, `D_8012183C`, `D_80121860`, `D_80121864`, `D_80121884` stay declared because EVENT has its own data at those addresses.
- Overlay aggregates (TAIIKU, T-16550) are not in `config/migrate_globals.txt`: the tool rewrites every `src` file and all overlays share the address 0x80132000 up, so an overlay range would claim other overlays' names. Their absorbed names are rewritten by hand; [[taiiku-family-results]] has the layouts and why each is an array, a struct or two objects.

## Aggregates and old names (T-5100)

Once an aggregate is declared, its fields must not be used through their splat names: separate symbols let IDO hoist loads that the original kept behind stores. `config/migrate_globals.txt` lists the aggregates (`aggregate <base> <type> <header>`) and the few uses that match only through the old symbol (`keep <file> <symbol> <reason>`). `tools/migrate_globals.py --apply` rewrites every C use of a `D_` name inside an aggregate into the field access (the old declaration picks the field: a byte of a union, an array, a record element) and deletes the old declarations; `--check` runs in ninja. Findings from the `GameState` migration that hold for any type here:
- A constant-index array element is not a struct member for IDO: a `switch` on it and a sum of two of them compile differently. Use separate members where the matched code needs them.
- `((u8 *)&S)[k + i]` is not the same as the old `u8 S[]` view: it changes the induction variable. Write the field access.
- Struct-array elements in a sum can swap the load order against a scalar extern. The one such case (SHOUGATU `func_8013E0F0`) was a bit-field and matches through the struct (T-7010); there is no `keep` line now.

## Member or separate symbol: what IDO changes (T-7010)

- IDO 5.3 compiles a constant-offset member of an extern struct like a scalar extern at that address: same instruction words after relocation, same registers, and uopt keeps members apart (a member value survives a store to another member). Only alias rules differ: as1 does not move a load above a store through the same symbol (it does for two symbols), and uopt reloads a member after a store through an indexed element or a pointer into the struct. A diff in registers or operand order is never fixed by switching between the two views; check with both before blaming the declaration.
- Evidence for a separate symbol in the original is therefore a load hoisted above a store between two fields of an aggregate (`tools/aggregate_audit.py --pairs`). For `GameState` there is none (407 pairs kept, 0 hoisted), and `tools/migrate_globals.py --check` accepts a `keep` line only with such a pair in that file's original object.
- `lui rA; lbu rB,%lo(D)(rA); ori/andi; sb` with rB different from rA is a bit-field store, not a separate symbol: IDO computes a bit-field member's address apart from the load. Write a bit-field view of the word (a local `typedef struct { u32 pad0 : 14; u32 f : 1; u32 pad1 : 17; }` cast on the member, LSB first, as the neighbouring files do); `tools/aggregate_audit.py --bitfields` lists the sites in the aggregates (56 in `GameState`), and the same shape appears on `Rec34` (`unk_0C.b14`) and EVENT/GYOZI flag bytes.

- Proposed second evidence (T-20820, [[keep-lines]]): a `keep` line with a byte-match necessity proof for functions where the original reads one field as two uopt variables (the frame counter in TACO). `tools/keepproof.py check` compiles the unit with the symbol folded and with it separate; the asm shape alone has a false-positive rate of 100% on matched code, the proof has none by construction.

## T-9020 additions
- `Rec38Flags` / `CharFlags` (`include/main_api.h`): the flag word at +0x0C of a `Rec38` record as a union of `w`, `u`, `h[2]`, `b[4]` and the bit-field struct `f` (bits 0-8, `f9` = bits 9-11, bits 12-14). Evidence: `sll; bltz/bgez` tests of bits 1, 2, 3, 6, 14 and `sll 20; srl 29` reads of bits 9-11 in the original; it replaced thirteen file-local views without changing a byte.
- `D_800CA21C`, `D_800CA224`, `D_800CA22C`, `D_800CA234`: four `s16[4]` rows (were twelve `s16` scalars). DATE indexes each row from its own base and sets rows with chain assignments; `D_800CA232` is written, so the rows have four entries.
- `D_8011ECD0` (the 0x44-byte table): OPTION and ENDING functions match only when `D_8011ECF6`/`D_8011ECFA`/`D_801208FB` and `D_8011F4D0`..`D_8011F522` are read through it, more evidence that it is one object (record n field k = `D_8011ECD0[n * 0x44 + k]`). Still a `u8[]` view.
- ENDING: `D_8013C35C` is `s16`, `D_8013C360` is `u32`.


## Wave-8 aggregate recovery (T-9610)

The repeated shapes in the rejected drafts are (1) complete word/halfword/byte
array copies to stack locals, including final halfword or unaligned tails;
(2) eight-byte positions passed by value in two argument words; (3) 0x44 scene
records and 0x88 actor records; (4) GPU DMA packets; and (5) 0x20 matrices.
Examples: TAIIKU `func_80135DF8` copies an 11-by-10 halfword table (0xDC),
TACO `func_801472D4` fills a MATRIX and copies its eight words into a coordinate
object at +4, and EN_NICHI `func_80134F2C` has a pointer spill but no local copy.
Existing `tools/type_recovery.py` already combines stride/offset evidence from
all functions by symbol/address space. No new general pass is justified yet.

`include/psx_types.h` now supplies the eight POLY forms, SPRT, fixed sprites,
TILE forms and draw-control packet layouts; `include/libgte.h` supplies SVECTOR,
VECTOR and MATRIX. These are new fixed-width declarations, not imported SDK
headers. ABI facts were checked against the PsyQ Run-Time Library Reference
(4.7, structures introduced in 3.x), https://psx.arthus.net/sdk/Psy-Q/DOCS/LibRef47.pdf,
and original packet/copy instructions. Packet tags occupy four bytes; MATRIX
translations start at +0x14 and its size is 0x20. Existing private POLY_F4 and
SPRT copies use these common types. `EnObj` moves from its
C file to `include/ovl/EN_NICHI.h`.


### Recurring families in the eight lists

| Wave list | Representative evidence (includes already matched baseline work) |
|---|---|
| 1 | TACO matrix copy and 0x88 actor stride; ETC output buffers of 16 bytes/16 halfwords |
| 2 | RPG_BAT 96-record prefix plus 64-record group, each record 0x44; NAME_ENT 35-byte text buffers |
| 3 | TAIIKU 11x10 halfword copy; GYOZI 30 callbacks; 0x24 control-record stride |
| 4 | EVENT 39-callback copy; TT ordering-table tag/packet fields; RECT workspace |
| 5 | DATE2 30/51/39/81 halfword copies, including halfword tails; BUNKAKEN 4x4 word grid |
| 6 | TAIIKU 23/46-byte copies; TACO two-word by-value positions and 0x14 SPRT |
| 7 | 0x44 scene records and coordinate-table pairs; TT 0x54/0x58 records |
| 8 | EN_NICHI 0x30 object and 0x18 POLY_F4; larger local-copy and string workspaces |

New retained functions and rejected experiments are recorded in
[[matching-notes]] under T-9610. [[decompile-workflow]] contains measured frame
rules. The SDK reference dates DR_TPAGE to 3.5 and DR_STP to 4.1; their named
layouts are provided without asserting that every overlay uses those versions.

Reference erratum: the 4.7 PDF's DR_TPAGE listing gives two command words,
but the actual packet is eight bytes (tag plus one command word), as recorded
in the [DIABPSX.SYM type dump](https://gist.github.com/mewmew/f4c94068e1462e40142b6cab112d43d2).
The shared declaration and ABI test use that eight-byte layout. DR_STP is
twelve bytes, with two command words.

## MASTER held-out table pilot (T-13001)

[[master-types-pilot]] verifies three additional 13-s16 copies at D_8013C7E8/C804/C820 using the existing MasterHalfwords13 type. Reusing MasterSprite44 preserves ordered accesses at +03/+07/+16 in the two later handlers. Three full-TU matches add 3,648 literal bytes; source-level register/stack idioms remain FAKE-labelled, distinct from the demonstrated layouts. No shared type migration.

## TACO types-first pass (T-16560)

`TcActor` (0x88) now has its object handle (`Tc10`, GsDOBJ2 layout) at +0x04 and its coordinate object
(`TcObj50`, GsCOORDINATE2 layout, complete) at +0x14; `Tc14` has no holes; `D_8015EC70` is a `Tc14[16]`
table; `D_80129C40` is a `MATRIX` in `main_api.h` (which now includes `libgte.h`). Map, evidence and the
draft rewrites built on them: [[taco-family-results]] ("Types-first pass").
- `type_recovery.py` reads every struct copy that IDO emits as a three-word loop as a stride-0xC array: in DATE
  the 30 highest proposals are step-table copies, not records ([[date-family-results]], T-17200).

## Main types-first pass (T-17000)

Main-only tables declared in `main_api.h` (`D_800B6890`/`D_800B68A8` two-byte rows, `D_800B68E0`/`D_800B6928`
signed nine-byte rows, `D_800C6200[8]`, `D_800C62C0[32]`, `D_80125DA0`/`D_80125E20` channel rows,
`D_800B5AA4`/`D_800B5AE4` halfword pairs, `u8 *D_800B6CF8[]`) and the `Flags7D4F` bit-field view of
`D_800E7D4F`. The shared main aggregates that need overlay edits (`D_800E7D10` object, `D_80125C00` table,
`D_801230D0`, the GsCOORDINATE2 layout of `TaiikuBig`) are listed in [[main-family-results]] ("Types-first
pass (T-17000)").
  the 30 highest proposals are step-table copies, not records ([[date-family-results]], T-17200); the same in EVENT
  ([[event-family-results]], T-17400).

## BUNKA_SD types-first pass (T-18100)

`BsdEvent` (frame, command word) and the three scenes' event-list tables (`BsdEvent *[]`) and text tables,
plus seven loaded picture sets typed as `s32 *` page table, two `u8 *` images and a palette word, all in
`include/ovl/BUNKA_SD.h`. The scene control words stay separate scalars (matched code). Evidence for a
main-exe aggregate around `D_80122EA8`: `func_80135F38` compares `D_80122EAC` with the constant first,
which IDO gives only for a non-plain operand. Details: [[bunka-sd-family-results]].

## BUNKASAI types-first pass (T-18700)

Stack-copied tables as one-array structs: `BunkasaiNameTable D_801606DC` (`u8 s[3][3][33]`, strings by school
year and `D_80122CDC`), the text-pointer tables `BunkasaiTexts12/11/4` (`D_80160AE0`, `D_80160B10`,
`D_80160B3C`, BUNKAKEN's `D_80156470`.. shape), per-girl bytes `BunkasaiGirlBytes D_80160520` (`u8 v[11]`),
the column selector `BunkasaiColumns D_8016052C` (`u8 v[6]`), the per-year `BunkasaiGirlColumns` tables
(`u8 v[13][6]` at `D_80160534`, `D_80160584`, `D_801605D4`) and `BunkasaiGirlList D_80160AA4` (`s32 v[11]`).
type_recovery's 0xC-stride arrays at `D_80160808`, `D_80160A9C`, `D_8016052C` and `D_801606D4` are artefacts
of these tables and of the dispatch table `D_80160810`. Details: [[bunkasai-family-results]].

## KANGEI types-first pass (T-18400)

Picture-set tables as arrays, one row per component of the current set (`D_80139AD0`/`AD4`/`AD8`):
`s32 D_80139950[3][8]`, `D_80139A50[3]`, `D_80139A5C[3][8]`, `D_8013A170[2][3][13]` (129 scalars absorbed);
`KangeiSprite44` extended to MASTER's display-record layout with `KangeiFixed32`; the MASTER streak tables
(`KangeiPoint2 D_80139F10[10]`, `s16 D_80139F38[10]`); club name tables `KangeiNames20`/`KangeiNames21`
(`u8 name[11][20]`, `[11][21]`, stack copies); `KangeiImage10 D_80139D10[2]`; the `KangeiWords11`/`KangeiWords2`
summer tables at `D_80139A08`. type_recovery's 0xC-stride arrays at `D_801399A8` and `D_80139F44` are artefacts
of these neighbouring tables. Details: [[kangei-family-results]].

## ENDING targeted type pass (T-20930)

A second pass over ENDING ([[tickets/T-20930-types-ending]], details and numbers in [[ending-family-results]],
"Targeted type pass"). New types in `include/ovl/ENDING.h`, all byte-identical (27/27):

- message scripts `s32 D_8013C280[3][17]` and `s32 D_8013CAC0[3][15]` (row = table, column = scene; the three
  rows of one scene are copied together), the running scene's set `EdScriptSet D_8013C34C`;
- scene image records `EdScene30` (`D_8013C5F0`, `D_8013CBB0`) and `EdScene44 D_8013C620[15]`: three TIM
  addresses, a halfword, then texture pages;
- the portrait columns `EdPortraitTbl D_8013C4C0` (three `s32[14]` and an `EdHalf4[14]` read with `lh` at a
  4-byte stride) and `s32 D_8013C5A0[7]`;
- the callback tables `EdFnTblN` and the stack tables `EdWords4`, `EdShorts13`, moved out of five C files;
- `D_8013C364` is `u8` (only `lbu`/`sb` in the original), `func_80132B04` returns `s32`, `func_80132420` has no
  prototype (K&R callee), and ENDING's `set_kanji_string` override is gone (main's `u8 col` view is the right one).

The 427 scalar labels were folded with a one-off script scoped to the overlay (`drafts/T-20930/fold_ending.py`),
not with `tools/migrate_globals.py`, which rewrites every source file and would touch the same `D_8013xxxx`
names in other overlays.

Draft scores: the header change alone moved none of the 11 redrafted functions (one 487 to 468 change is a
`draft_fixes` search-order effect). Hand drafts that use the types matched `func_80132420`, `func_80132B04` and
`func_80139B48` (2,900 bytes) and lowered three others (`func_8013759C` 99 to 66 with the `D_8011ECD0` record
view instead of main's scalars). Same lesson as T-16550 and T-20900: types pay off when a draft is rewritten
around them; the views of callees and of the context (prototype or K&R, return type, typed pointer casts of
main's `u8 *` slots) mattered more than the data aggregates.

## Shared-type unification (T-20900)

The types-first passes (T-16550 .. T-18800) could only add declarations, so one table had a view per unit.
T-20900 compared the views with the original accesses and folded them into one type per table; every step was
built byte-identical (27/27). Ticket: [[tickets/T-20900-shared-type-unification]].

### The display-record pools (0x44 bytes)

Main `D_8011ECD0` (160 records, `D_80120650` is record 96), EVENT `D_800E9620` (`D_800EAFA0` is record 96) and GYOZI
`D_80129F40` (`D_8012B8C0` is record 96) hold the same record (EVENT and GYOZI were linked against another main-exe
layout; the main and GYOZI pools both end 0x80 bytes before a `Rec24` table, `D_801217D0` and `D_8012CA40`). A scan of every function's asm with
`tools/type_recovery.py`'s register pass (accesses at pool base + n * 0x44 + k, also through a register that holds a
pool address) gives the widths:

| offset | original accesses | type |
|---|---|---|
| +0x00 .. +0x07, +0x40 .. +0x43 | `lbu`/`sb` only; no `lb` through the pool symbols in any unit | `u8 unk_00` .. `unk_07`, `unk_40` .. `unk_43` |
| +0x08, +0x0A, +0x14 .. +0x22, +0x3C, +0x3E | `lh`/`sh` (one `lhu` at +0x18: RPG_BAT `func_8014F88C` switches on it unsigned, through a `*(u16 *)&` view in its body) | `s16` |
| +0x0C, +0x10, +0x34 | `sw` of addresses, `lw` | `u8 *` |
| +0x38 | `sw`, `lw` | `s32` |
| +0x24, +0x28, +0x2C, +0x30 | `lw`/`sw` (16.16 arithmetic) next to `lh`/`sh` at +0x26, +0x2A, +0x2E, +0x32 | two views, below |

Other word accesses at every fourth offset are whole-record copies. C89 has no anonymous unions, so the record has two
views in `include/game.h`, identical except at +0x24 .. +0x33:

- `DisplayActor44`: every field, halfwords at +0x24 .. +0x32 (the coordinates most of the tree reads).
- `DisplayFixActor44`: `Fix32 unk_24`, `Fix32 unk_28` (16.16 words; the coordinate is `.h[1]`), `s32 unk_2C`,
  `s32 unk_30`. Used by MASTER, KANGEI, DATE, EVENT, NAME_ENT, TEL and one TAIIKU line.
- `Fix32` (`s32 w; s16 h[2]`): the 16.16 word, also used outside the pools (TAIIKU's controller records, `D_8014A1CC`).

| retired view (unit) | now |
|---|---|
| `DisplayRecord44` (game.h, and a local copy in main `800674B0.c`), `ShoSprite44`, `ShoCounter44` (SHOUGATU), `NameActorHead44`, `NameRecord44`, `NameAssetActor44` (`image`) (NAME_ENT), `EventFlagRecord44` (EVENT), `RpgObjectRecord44`, `RpgHealth44`, `RpgFlagRecord44`, `RpgObjectBlock`, `RpgHealthBlock` (RPG_BAT; `group[n]` is record 96 + n), `TaiValue44`, `TaiRecord44` (`active`, `flags`, `y`) (TAIIKU), `GyoziRec44`, `GyoziInitSprite44` (GYOZI), `OptActor44`, `OptDraw44` (`unk_00.b[3]` is `unk_03`), `OptSpr44` (OPTION), `DateScene44` (DATE), `TextureRecord44` (`pages`) (main) | `DisplayActor44` |
| record copies `Ev44` (EVENT, GEKO), `Words17` (EVENT, three copies), `DateWords44` (DATE), `EtcRec44` (ETC) | `DisplayActor44` assignment (same copy code) |
| `MasterSprite44`, `KangeiSprite44` (`unk_26` is `unk_24.h[1]`), `DateParticleView44`, `DateMover44`, `DateMotion44` (DATE), `EventParticleView44`, `EventMotion44` (EVENT), `NameMotionActor44` (`x`, `y`, `phase`, `flags`), `TaiikuScene44` (`position`), `TelRec44` | `DisplayFixActor44` |
| `MasterFixed32`, `KangeiFixed32`, `DateFix32`, `TaiikuPosition`, `NameMotionWord` (`.word`, `.half.high`) | `Fix32` |

Kept as a documented view: TAIIKU `TaiikuActor44`, the shared layout with +0x01 and +0x06 signed. `func_80138DF8`,
`func_80139610` and `func_801400D8` store constants to those bytes, and with `u8` fields uopt puts the constants in
other registers (measured: the TAIIKU sha1 fails with the shared type). `NameKanaPages44` is not a pool view (NAME_ENT's
kana table). The pointer fields keep the `u8 *` type of the shared record; a store of a constant or an `s32 *`
casts, an indexed word table reads `((s32 *)p->unk_34)[i]`.

The byte-offset uses of the pools in matched code are field accesses now, each with its old base symbol: 669
constant ones (`*(s16 *)&D_80120650[0x26]` is `((DisplayActor44 *)D_80120650)->unk_26`, `D_80120650[0x4B]` is
`((DisplayActor44 *)D_80120650)[1].unk_07`), 12 record copies (`((DisplayActor44 *)D_8011ECD0)[98] = ...[96]`) and 83
indexed ones (`D_8011ECD0[0x19C3 + i * 0x44]` is `((DisplayActor44 *)D_8011ECD0)[i + 96].unk_43`; IDO distributes the
record index, so the code is the same). All matched unchanged except EVENT `func_80108DD4`, whose loop takes one
more register with the fields; it keeps the byte view, marked `FAKE`. Two FAKE comments went away (TAIIKU
`func_80142240`, SHUGAKU's record copy). Not converted: `*(s32 *)&` views of the pointer fields, addresses
(`&D_8011ECD0[n]`), main `800674B0.c` (it declares the table as records) and `8007C030.c` (issue #45).

### Other tables with three or more views

| object | retired views | now |
|---|---|---|
| `D_800E7D4F` flag byte | `BunkaReturnFlags`, `BunkasaiModeFlags` (BUNKASAI), `BunkakenModeFlags` (BUNKAKEN; `mode` is `unk_2_4`) | `Flags7D4F` (main_api.h) |
| `D_80122EB8` word pair | `BunkaDispatch8` (two copies), `BunkSelectWords`, `BunkModePair` | `BunkaDispatch8` (game.h) |
| `D_80125C60` / `D_801227A4` matrix copies | `EtcMat`, `EtcMat2`, `TaiikuMat`; `EtcVec`, `EtcVec2`, `TaiikuVec` | `MATRIX`, `VECTOR` (libgte.h) |
| GameState +0x54C entries | main `TelEntryFlags`, `EntryFlags54C`, `EntryFlags29`, TEL `TelEntryFlags`, OPTION `OptionScheduleBits` | `ScheduleEntryFlags` (game.h: `unk_00:5`, `b24`, `unk_25:4`, `active` = bit 29) |
| GameState +0x66C club words | TEL `TelClubFlags`, KANGEI `KgSlot`, ETC `EtcSlot`, MASTER `MasterClubBits` | `ScheduleClubFlags` (game.h: `b24`, `unk_25:2`, `unk_27:3`, `b30`); RENSYU's `u16` `RensyuClubCount` stays |
| GameState +0xF4 word | DATE `DateFlag17`, `DateFlags0F4`, OPTION `OptionPlayerBits` | `ScheduleFlags` (game.h: `unk_08:4`, `unk_12:4`, `b16` .. `b20`, `unk_21:3`) |
| GameState +0xF8 date | NAME_ENT `NameDate` (its `day`/`month` were swapped against main's names) | `GsDateBits` (main_api.h) |

`D_800E7D10` is a 0xEE0-byte work area (main clears it with `bzero(&D_800E7D10, 0xEE0)`; +0 is the word main steps by
0x377 and NAME_ENT passes to `srand`). Its views: main's `FlagsWork24` (prefix), the album view `NameMetricWork` of main
and NAME_ENT (to +0xED8), TACO's `TcSetAll` (+0x40 .. +0x57: the settings bit pair, three names, three words) and
`TcSave` (a word/halfword/byte union over +0x40), and ETC's `EtcConfig8` (8-byte records indexed from the base, +0x58
on). `Work800E7D10` in `main_api.h` takes the first three (the bit pair is `WorkSetBits`), and `D_800E7D10` is declared
with it: the overrides in OPTION and TACO `80142820.c` and the casts in main and NAME_ENT are gone, main's word uses
are `.unk_000`. TACO's loader keeps `TcSave` (its word view overlaps the name bytes) and ETC `EtcConfig8`, as casts of
the base. Not merged: `D_801217D0`, `Rec24` and main's local `SprEnt` (two views; +0x0A `u16` against `s16`).

A bit-field view compiles from the accessed field's position, width and container type only: the views that differ
in their padding fields merged without a byte change; a different container (`u16` against `u32`) is a real
difference and stays. 68 type names are gone (702 typedefs to 631 in `include/` and `src/`, TT excluded, with four
new ones: `Fix32`, `DisplayFixActor44`, `Work800E7D10`, `WorkSetBits`).

### Draft scores

`tools/redraft.py` (variants `plain`, `typed`, `typed-late`, no `draft_fixes`) on the 166 unmatched functions whose asm
reads one of the pools (37 of them a field that the old `DisplayActor44` left as padding), once on a snapshot of
`w2-good` (dcfb746d) and once on the unified tree (the 37 on 2b4d091a; the other 129 on 7ad19d0c, which differs only in
matched source, comments and the `record-copy` fix below, with the 13 drafts that hold a record block copy rerun on
2b4d091a), each run from its own copy under `build/ty/`:

- 153 functions scored on both sides, with the same best score each (34,580 diff rows in all); 12 compile on neither
  (among them `DATE:func_8013FD6C`, `EVENT:func_8011B848`, `GYOZI:func_801368B8`, `SHOUGATU:func_801350F8`,
  `TAIIKU:func_80147B14`, `main:func_800476C0`).
- One function changed: `main:func_80077900` had no compiling fresh draft and now has one at 163 rows (census and best
  archive 211), kept in `drafts/T-20900/main/`. The cause is a tool effect of the unification: the T-18600 `club-bits`
  rewrite ran only where MASTER's `MasterClubBits` existed and now runs wherever `ScheduleClubFlags` does.
- Variant level, two changes: ETC `func_80143968` `typed`/`typed-late` 79 to 109 (the old draft stored through
  `pad_1C[temp_lo]`, a byte array, by luck closer; its best, `plain` 72, is unchanged) and the one above. A first run
  showed the `record-copy` rewrite writing `DisplayFixActor44` in every unit (mixed-type assignments that did not
  compile in EVENT, GEKO, KANGEI); it now uses the unit's record view, and those drafts score as before.
- Declaring `D_800E7D10` as `Work800E7D10` (6575fad7, measured separately) is the one change that moves scores. Of the 34
  unmatched functions that read the work area, 33 score on both sides and 13 change: 3 better (`main:func_8005CC40` 92
  to 15, `NAME_ENT:func_80139F30` 1,171 to 992, `ETC:func_80144F74` 115 to 46), 10 worse (sum 8,201 to 8,297). Scoring
  the archived bodies on the new headers too, 8 of the 13 beat their best archive and 3 fall behind it
  (`ETC:func_8013E6AC` 216 against 130, `ETC:func_8013F054` 249 against 238, `main:func_8005CA94` 65 against 61). Here
  the type changes the folds: m2c and the `typed` fold see the labels inside the work area (`D_800E7D14`, `D_800E7D36`,
  ETC's records) as fields of one symbol, which changes what the compiler may reorder, the label-fold effect of
  T-16550, in both directions. The three improved drafts are in `drafts/T-20900/`; `main:func_8005CC40` was tried by
  hand and stays at 15 rows (in case 1 the original keeps the switch value in `$v1` and loads `unk_F88` into `$v0`; the
  draft swaps them).
- The draft text changed in 51 of the 111 `typed`/`typed-late` bodies of the 37 (none of the `plain` ones): byte-offset
  casts `*(s16 *)((unsigned char *)D_8011ECD0 + (7236))` became fields `((DisplayActor44 *)D_8011ECD0)[106].unk_1C`
  (1,057 to 945 casts, 583 to 756 record-field references).

This repeats the T-16550 / T-16560 result with a cleaner test: IDO compiles a constant-offset member like a cast at
the same address (T-7010), so a better header makes drafts readable but does not move the score. Scores move when a
rewrite changes what the compiler sees as one object (the label fold of the `typed` variant) or the statement shape.
The unification pays off for people reading and editing drafts, for rewrites that were gated on one unit's private
type (`club-bits`, `record-copy`) and for later rewrites that need one field name per offset (`tools/redraft_rewrites.py` reads one record view per unit; `TYPE_ALIASES` maps the retired names in
archived bodies).

## OPTION second type pass (T-20910)

The tables behind main's two BG slots: `SpriteSlot24` (game.h, GsBG layout) at `D_801220D0 + n * 0x24` points
at the cell map `D_80122120[n]` (`SpriteMap10`, GsMAP layout), whose cells are `D_80122140[n]` (64 x
`SpriteCell8`, GsCELL layout) and whose index is `D_80122540[n]` (`SpriteIndex80`, 64 halfwords); main
`func_8004A1A0` (asm) sets up slot n through all four, OPTION `func_801328C0` fills slot 0. The types and the
three externs live in `include/ovl/OPTION.h` because OPTION is the only C reader (CODING_STANDARDS 8a: a main-bss
symbol may stay in an overlay header only through a type of that header; `check_headers` rejects a `game.h` type
there, and a plain `u16 [2][64]`, which is why the index is a one-array struct). When main matches
`func_8004A1A0`, the types move to `game.h` and the externs to `main_api.h`, and `D_801220D0` (now `u32` plus
fifteen scalar field labels in `main_api.h`) can become `SpriteSlot24 D_801220D0[2]`.

Score effect, the T-20900 lesson again: the header types moved two variant scores (`func_801326BC` 134 to 130
with a draft-side `SpriteSlot24` view of `D_801220D0`; `func_801328C0` `plain` 101 to 99, `typed` 101 to 126),
while hand drafts in the shape of the matched siblings took the OPTION sum from 5,184 to 3,504 diff rows and
matched `func_80135E48`. A measured non-change: `D_8013D3B8` stays `volatile u16` (dropping it helps one draft
and hurts two). Details: [[option-family-results]] ("Second type pass (T-20910)").

