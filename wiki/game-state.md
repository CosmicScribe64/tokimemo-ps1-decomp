---
type: concept
updated: 2026-10-10
sources: ["include/main_api.h", "tools/migrate_globals.py", "config/migrate_globals.txt", "tools/type_recovery.py", "tools/aggregate_audit.py", "asm/**/*.s"]
---

# GameState: the main game-state struct (T-5100)

`GameState D_800E6280` in `include/main_api.h` is the object of the original that holds the calendar, the player's parameters, the per-girl records, the event log and the system tables. It covers the main bss from 0x800E6280 to 0x800E7D10 (0x1A90 bytes). Almost every overlay reads it. C code reaches it only through fields (`D_800E6280.unk_F5F`, `D_800E6280.unk_1BC[i].unk_06`); the old splat names (`D_800E71DF`, ...) are rejected by ninja (`build/globals.ok`, `tools/migrate_globals.py --check`). Ticket: [[tickets/T-5100-game-state-struct]]. Method for other types: [[data-types]].

## Where it starts and ends

- **Base 0x800E6280** (high confidence). Loops over arrays far inside the block keep 0x800E6280 in the base register and reach the arrays with large immediate offsets: `search_tpage` (`lw 0x1228(v0)`, `sw 0x1128(v0)`, 64 words), OPTION `func_801326BC` (`sw 0x1638(v0)` .. `0x17BC(v0)`), ETC `func_80141ABC` (`lbu 0x760(s0)`, stride 8), `func_80067F04` (`lw 0x1C8(s2)`), the save routine `func_8005557C` (`lbu 0x53C(v1)`, `0x728`, `0x72C`, `0x73C`, `0x74C`). A compiler can only do this when the arrays are fields of one symbol at 0x800E6280. Matched C written before T-5100 already used `D_800E6280` as a byte view with these offsets (`D_800E6280[0x73C + i]`).
- **Not 0x800E6248** (the guess of [[tickets/T-5000-type-recovery-arrays-structs]]): 0x800E6260 and 0x800E6270 belong to libgte and libetc (`asm/libgte_patchgte.s`, `asm/libetc.s`), and `D_800E6248` appears only in `yuukou_down` as a strength-reduced compare constant (`p == &rec[k]` folded to an address below the table).
- **End 0x800E7D10** (high confidence for the next object, medium for the last bytes). ETC `func_80141ABC` uses `D_800E7D10` as its own base (`0x58(t3)` with t3 = `D_800E7D10 + 8i`) and main `func_800420D0` clears it with `bzero(&D_800E7D10, 0xEE0)`: another object starts there. The largest base offset seen is +0x183C (TACO, NAME_ENT); NAME_ENT stores addresses at +0x1A3C..+0x1A84.
- **O.BIN name** (hypothesis, not applied): the developer build has `sys` (0x1A70 bytes) followed by `sys0`; ours is 0x1A90 bytes followed by the `D_800E7D10` object. Size and order fit; [[obin]] has no data alignment for this area, so the name stays a comment.

## Layout

Offsets are from 0x800E6280. Types come from the access widths of all functions (lb/lh signed, lbu/lhu unsigned; stores give no sign), from the old C declarations, and from what the build accepted. `tools/migrate_globals.py --layout` prints the full field list, `--at D_800E71DF` the path for an address. 161 top-level fields; 6041 of 6800 bytes (88.8%) are typed fields, records or arrays, 759 bytes are unknown byte arrays (`unk_006`, `unk_120`, `unk_183C`, ...) where 164 of the 16817 accesses of the block fall (42 addresses).

| offset | field | evidence | confidence |
|---|---|---|---|
| 0x000 | `u16 unk_000`, `u16 unk_002`, `u16 unk_004` | lhu/sh in main; ETC compares `unk_000` with 0x100, ENDING stores it | medium |
| 0x014 | `s16 unk_014[2]`, `s16 unk_018[2]` | indexed by `D_8011ECA0` (buffer 0/1), 400 lh in 15 units | high |
| 0x01C | `GsRec01C unk_01C[2]` (three words) | `SetWorkBase(arg0, buf)`: `p = base + buf * 12`, stores +0x1C/+0x20/+0x24 | high |
| 0x034 | bytes `unk_034`..`unk_041`, `s16 unk_042` | direct accesses; `unk_03E..unk_041` init 0x5F, 4, 4, 1 (`func_800418B0`): a date, compared with the birthday bits of Rec38 | high (widths), semantics hypothesis |
| 0x044 | `GsRec044 unk_044[2]` (two `u8[0x20]` arrays) | `bzero(+0x44, 0x80)`, index stride 0x40 with 0/1 (`func_80068898`), byte index by a day number | medium |
| 0x0C4 | `u8 unk_0C4[0x10]`, `unk_0D4[8]`, `unk_0DC[8]`, `unk_0E4[0x10]` | NAME_ENT text buffers, `gnsx(&unk_0D4)`, saved as 0x78 bytes | medium |
| 0x0F4 | `GsHalf unk_0F4`, `GsHalf unk_0F6` | lhu (230) and lbu of the bytes of the same halfwords | high |
| 0x0F8 | `s32 unk_0F8` | lw, birthday bits | high |
| 0x0FC | nine `GsRec0FC` members `unk_0FC`..`unk_11C` (`s16`, `s16`) | `parameter_show_init` reads the nine `+2` halves; KANGEI copies whole records. Separate members, not an array: ETC `func_8014A32C` sums two of them in the order of scalar members | medium |
| 0x120 | `u8 unk_120[0x9C]` | unknown: bytes stored by GEKO/main, s16 reads at +0x17A..+0x19A | low |
| 0x1BC | `Rec38 unk_1BC[16]` | one 0x38-byte record per girl; walked to +0x53C, indexed by `unk_F5F` everywhere; `unk_0C`/`unk_10` are bit-field words read as words and bytes (`GsWord`) | high |
| 0x53C | `u8 unk_53C[0x10]` | saved as 16 bytes, read byte by byte by ETC, NAME_ENT, OPTION | medium |
| 0x54C | `GsWord unk_54C[8]` | saved as 8 words; TEL tests bits 25..29 (`TelEntryFlags`) and clears bit 5 of byte 3 | high |
| 0x56C | `u8 unk_56C[0xFC]` | saved as 21 x 12 bytes; DATE indexes it by byte (`unk_56C[arg2] = 1`) | medium |
| 0x668 | `u8 unk_668[4]` | `get_k_speed` result and a copy of `D_800B6708` at +0x66A/+0x66B | low |
| 0x66C | `GsRec66C unk_66C[12]` | `bzero(+0x66C, 0x30)`; ETC tests a bit-field word per record, main clears bit 0 of byte 3 per girl | medium |
| 0x69C | `GsRec69C unk_69C[32]` | `func_8006CF38` clears 32 words and sets byte 0 to 0xFF; indexed by `unk_71C` | high |
| 0x71C | bytes `unk_71C`..`unk_727` | direct accesses; `unk_720..unk_722` hold the "return to" mode bytes (`func_80042878/908/940`) | high (widths) |
| 0x72C | `u8 unk_72C[0x10]`, `unk_73C[0x10]`, `unk_74C[0x10]` | loops of 11 (one byte per girl), saved as 3 x 16 bytes | high |
| 0x75C | `u8 unk_75C`, `u8 unk_75D` | direct accesses | high (widths) |
| 0x75E | `GsRec75E unk_75E[256]` (8 bytes) | saved as 256 x 8 bytes; MASTER writes record `unk_F5E`; code reads record `i - 1` (`unk_75E[unk_F5E - 1].unk_02`) | high |
| 0xF5E | `u8 unk_F5E` (record count of `unk_75E`), `u8 unk_F5F` (current girl, 1352 accesses) | direct accesses | high |
| 0xF60 | bytes and halves up to 0xF7E, `GsWord unk_F68` | direct accesses, lb/lbu per byte | high (widths) |
| 0xF80 | `s32 unk_F80`..`unk_F8C` | lw/sw; `unk_F88` is the pad/flag word (`& 0x20`, `& 0x40`) | high |
| 0xF90 | `GsRecF90 unk_F90[32]` (8 bytes) | lh/lbu with index stride 8 and 16 | low |
| 0x1090 | bytes, `s8 unk_1093`, `s8 unk_1094`, `s8 unk_1095[0xD]` | `menu_check` indexes +0x1093 by menu number, yet two matched switches need `unk_1093`/`unk_1094` as members (see below) | medium |
| 0x10A5 | `u8 unk_10A5[0x43]` | one byte per entry of the `Rec24` table at `D_801217D0` (`func_800415B4`) | medium |
| 0x10E8 | words `unk_10E8`..`unk_1100`, `GsWord unk_1104` | lw/sw; `unk_1104` counts frames of the current mode (`+= 1` at the top of most mode functions); BUNKA_SD reads it as `u32` (`.u`) | high |
| 0x1108 | bytes `unk_1108`..`unk_111F` | `unk_1109`/`unk_110A`/`unk_110D` are the mode selectors (`switch`, 1220 and 2668 accesses) | high (widths) |
| 0x1120 | `s32 unk_1120`, `u8 unk_1124` | direct accesses | high |
| 0x1128 | `s32 unk_1128[64]`, `s32 unk_1228[64]` | `search_tpage` loop (64, base offsets +0x1128/+0x1228), `tpage_buf_clear` | high |
| 0x1328 | `GsRec1328 unk_1328[32]` | `palette_load_vram` (+0x1328, stride 8) | medium |
| 0x142C | `GsRec142C unk_142C[33]` | `csr_load_vram` (stride 16) | low (count) |
| 0x163C | `s32 unk_163C[128]` | OPTION fills 128 words; main saves the first 0x40 bytes with `memcpy` and five words behind them | medium |
| 0x183C | `u8 unk_183C[0x200]` | unknown: TACO/NAME_ENT records with stride 16 around +0x182C | low |
| 0x1A3C | words `unk_1A3C`..`unk_1A84` | NAME_ENT stores VRAM/buffer addresses, TACO reads them | medium |

Unions: `GsWord` (`s32 w`, `u32 u`, `u16 h[2]`, `u8 b[4]`) and `GsHalf` (`u16 h`, `u8 b[2]`) model words that the original reads as a whole and through single bytes. IDO narrows a bit-field load to the byte that holds it (`lbu`, then a shift), so these are probably bit-field words in the original; the C keeps the views the matched code needs.

## What the build showed

- **The block is one object.** 1459 C uses moved to `D_800E6280` fields and every matched function stayed byte-identical except the cases below. The INCLUDE_ASM functions whose original keeps a load of the block after a store to another of its fields (191 functions with such a pair from `tools/type_recovery.py`) were tried with plain m2c C and `migrate_globals.py --apply`: 47 match, and the same C with the old separate symbols matches none of them (IDO hoists the load). 7 `FAKE` "first symbol" tricks became plain field accesses (EN_NICHI `func_80132B40`, `func_80132C4C`, SHUGAKU `80134880.c`, DATE2 `80132DE0.c`, DATE `80152FC0.c`, main `func_80053CC0`, MASTER `func_80138374`).
- **Array element versus member.** A constant-index array element is not the same as a struct member for IDO: a `switch` on `unk_1093[1]` gets `$v1` where the original (and a member `unk_1094`) has `$v0` (main `func_8005C414`, `func_8005E924`, ETC `func_8014979C`), and a sum of two elements of `GsRec0FC[9]` loads them in the other order (ETC `func_8014A32C`, as `RpgRec18` in [[data-types]]). So +0x1093/+0x1094 and the nine records at +0xFC are members.
- **A cast of the struct address is not the old array view.** `((u8 *)&D_800E6280)[0x10A5 + i]` gives another induction variable than the old `u8 D_800E6280[]` view; the field access `D_800E6280.unk_10A5[i]` matches. All old byte views were rewritten as field accesses; none was kept.
- **Kept old views**: none since T-7010. SHOUGATU `func_8013E0F0` kept `D_800E66E8` (`unk_1BC[12].unk_0C.w`) because the sum matched only on the scalar; written as a 3-bit field of the flag word (`D_80145F2C += f * 3 - 3`) it matches through `D_800E6280`.
- **Unit-private selectors** (T-5010 U0): the struct does not change them. ETC `func_80145960` with a direct `switch (D_800E6280.unk_110A)` still gets `$v1`; its `FAKE` local copy stays. Of the 26 U0 functions whose selector lies in the block, one matches with plain C (main `func_80074D28`, a compare chain on `unk_69C[0].unk_00`), but it matches with the old scalar name too: a detector false positive, not a struct effect.

## Views: one struct in every unit (T-7010)

Wave 4 (lists 3, 5 and 7) reported about 20 functions that "reach a field as a separate symbol" and called it the top blocker. [[tickets/T-7010-game-state-view-audit]] audited every access of the original to the block and tested the claim. Verdict: the original source had one object, reached through `D_800E6280` by every unit; no unit had separate externs, aliases or a second aggregate for these fields. No per-unit view is declared, and `config/migrate_globals.txt` has no `keep` line.

**What a separate symbol would change.** IDO 5.3 compiles a constant-offset member (`D_800E6280.unk_03A`) exactly like a scalar (`D_800E62BA`): the same `lui`/`%lo` words (the relocation `D_800E6280+0x3A` resolves to the field's address), the same registers, and uopt treats the members as distinct (`a = s.w; s.v = 5; b = s.w` keeps `a`). Two things differ, both alias rules:
- as1 moves a load above a store to another symbol, but not above a store through the same symbol (`X += 1; Y += 1;` on two scalars: `lw X; lw Y; sw X; sw Y`; on two members: `lw X; sw X; lw Y; sw Y`);
- uopt assumes a store through an indexed element or a pointer into the struct may change any member, so it reloads a member after such a store; a separate scalar stays in its register.
A "symbol-direct" access (`lui %hi(D_X); lbu %lo(D_X)`) is therefore no evidence for a separate symbol: it is what every constant-offset member looks like.

**Audit of the original** (`tools/aggregate_audit.py`, all 6958 functions, matched and unmatched):

| kind | accesses | meaning |
|---|---|---|
| direct | 13625 | own `%hi`/`%lo` at the field address (member or scalar, see above) |
| idx | 201 | `lui; addu index; l %lo(D_X)` (element of a member array, offset folded) |
| ptr | 1040 | immediate offset from a register holding an address of the block |
| ptridx | 1670 | the same after an index was added (strength-reduced loops) |
| sharedhi | 9 | one `lui` serving two fields |
| lo? | 272 | `%hi` set in another block (branch delay slot) |

- Base-relative accesses (ptr, ptridx, sharedhi) come from 129 of the 277 original objects and from 21 of the 24 units that touch the block (all but BUNKA_SD, OLH and OMIMAI, which only reach single fields); they reach 52 of the 144 top-level fields, from `unk_000` to `unk_183C`. 91 fields are only ever reached directly (scalars: `unk_03E`, `unk_110A`, `unk_110D`, ...), which says nothing either way.
- Read-modify-write pairs in one basic block (direct accesses, different words, independent values): inside the block 407 kept, **0 hoisted**; between globals outside every aggregate 954 kept, 27 hoisted (2.8%). At the outside rate about 11 hoisted pairs would be expected if the fields were separate symbols (chance of none about 1 in 85,000).
- Reloads after indexed stores: `load_palette`, `load_csr_ab`, `load_csr_tp` (store a record of `unk_1328`/`unk_142C` through the index `unk_1428`/`unk_1429`, then reload the index for `+= 1`) and KANGEI `func_80135438` (reloads `unk_F5F` after a store into `unk_1BC[unk_F5F]`) do what only one object gives.

**The wave-4 cases, re-diagnosed.** For 15 attributed functions an m2c draft was compiled twice, with the `GameState` members and with the old scalars: 13 give the same instruction words, `load_palette` and KANGEI `func_80135438` differ and the member form is the original's (reload). The real causes:
- bit-fields (DATE `func_8015522C`, KANGEI `func_80135438`, SHOUGATU `func_8013E0F0`; matched): a bit-field store loads the byte into another register than its `lui` (`lui t9; lbu t0,%lo(D)(t9); ori; sb`), a scalar `D |= 0x40` loads into the `lui` register. `tools/aggregate_audit.py --bitfields` lists 56 such sites in the block;
- line-based scheduling (DATE `func_8015745C`, matched with a marked `FAKE`);
- register allocation and constant sharing of the T-0018 / T-3001 families (OPTION `func_80132AB8`, `func_801387E4`, `func_8013C780`, TACO `func_80136C60`, ENDING `func_80134B18`, DATE `func_8014E38C`, `func_801378BC`, `func_801386C8`, `func_80137B78`, `func_801395B4`, KANGEI `func_80132354`, OMIMAI `func_801337EC`, main `get_weekly_bg_sector`, `load_palette`): the scalar view gives the same words, so no view fixes them.

**Rule for the future** (`tools/migrate_globals.py`): a `keep` line (a separate-symbol view for one file) is accepted only when the original object of that file has a hoisted read-modify-write pair on that address (`tools/aggregate_audit.py --pairs`); `--check` runs the audit on that object and rejects the line otherwise. A function that misses by registers, operand order or scheduling does not get a view.

**Update (T-9020).** The four list-1 functions of wave 5 said to need "separate scalars" were checked again: main `func_80042960` matches with the T-8080 selector view (`switch (*(u8 *)&D_800E6280.unk_1108)` in an `s32` function); VALEN `func_80133670`, BUNKASAI `func_80146030` and TACO `func_8013546C` miss on one register or one load position, which the scalar declaration does not change either. The Rec38 flag word `unk_1BC[i].unk_0C` is now the union `Rec38Flags` with the bit-field struct `CharFlags` (`.f.b1`, `.f.f9`, ...) in place of per-file views ([[matching-notes]], "Wave-5 codegen shapes (T-9020)").

## Tool and workflow

- `tools/migrate_globals.py --apply` rewrites `D_X` inside an aggregate to the subobject at `X - base` whose type equals the old declaration (own `extern`, the overlay header's override, else `main_api.h`), removes the absorbed declarations and their `MAIN_API_OVERRIDE_` guards, and leaves a use it cannot map for a hand rewrite (it keeps that symbol's declarations). Re-runnable; a migrated tree is a no-op. Without a declaration it rewrites only an offset where exactly one scalar starts (or one array, for an indexed use).
- `--check` (ninja, `build/globals.ok`): every use of an absorbed name with the field to use, every old view of the base (`D_800E6280[i]`), every declaration of an absorbed name, and every `keep` line without a hoisted pair in its original object (T-7010).
- `tools/aggregate_audit.py` (T-7010): per access kind, per top-level field (`--fields`), per original object (`--units`), hoisted pairs (`--pairs`) and bit-field sites (`--bitfields`); `--json` for everything.
- Decompiling against the block: keep the `extern` lines m2c prints for the old names above the function (or in the overlay header), run `tools/docker.sh python3 tools/migrate_globals.py --apply`, then build ([[decompile-workflow]] step 5a).

## Open

- Real semantics: calendar (`unk_03E..unk_041`, `unk_044`), parameters (`unk_0FC..unk_11C`), girl records (`Rec38`), event log (`unk_75E`), modes (`unk_1109`/`unk_110A`/`unk_110D`) are hypotheses from use; rename through a symbol file only when confirmed.
- The unknown arrays `unk_120`, `unk_183C` and the record counts of `GsRecF90`, `GsRec142C`.
- The bit-field words (`Rec38.unk_0C`, `unk_10`, `unk_54C`, `unk_66C`, `unk_F68`) as bit-field structs, once one layout reproduces every user.
- For the Godot port: the save routine `func_8005557C` copies `unk_03D..unk_042`, `unk_044` (2 x 0x3C), `unk_0C4` (0x78), the 16 `Rec38` records, `unk_53C`, `unk_54C`, `unk_56C`, `unk_668..`, `unk_69C`, `unk_728..unk_74C`, `unk_75E` (0x800 bytes) and a dozen bytes of `0xF6C..0xF75`, `unk_1092`, `unk_03C`, then sums 0xF00 bytes of the buffer as a checksum: the persistent part of the state.
