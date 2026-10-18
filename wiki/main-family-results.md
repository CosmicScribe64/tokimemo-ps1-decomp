---
type: research
updated: 2026-10-17
---

# MAIN source family recovery

Scope: [[tickets/T-14200-main-source-families]], branch codex/main-14200 from 0f117230. MAIN only; 8004F870.c and 8007C030.c reserved. Original checkout remains read only. Existing Docker image, serial compiler probes.

## Initializer checkpoint

func_800412E0 adds **676 literal linked bytes**. Full-TU default funcdiff reports MATCH; MAIN SHA1 passes (e823bd844a8f8fa4d05483b59c66bc54b8393b26). Evidence: build/main14200/412-proof.log and 412-build.log.

Canonical GameState fields and the actual byte/halfword views of unk_0F8 reproduce the original initialization. The word store at +0x1098 overlaps the menu byte array and uses its canonical address. Preserve mask 0xFFF0: shortening it to 0xF0 is behaviorally redundant for the byte but differs in one instruction. Direct fields and a local GameState pointer both score two rows before restoring the original mask. No new FAKE. Added only the shared void declaration of the existing func_80041840 definition, avoiding implicit-int conflict when the initializer becomes C.

## Rejected focused probes

All drafts below remain ignored in build/main14200; no production/header changes retained for them.

- Appointment func_80076FDC: two eleven-row, two-byte tables and direct switch stores compile, but produce 252 rows; shared value-local and original goto forms give 264/341. Not a match. Table declarations removed.
- Telephone func_80077900: bitfield tests at bits 1/2/18 and month bit 29 with canonical GameState fields give 161 rows, consistent aggregate pointer 152. Helper-result narrowing and allocation remain. Temporary views removed.
- Parameter parameter_up_down: signed nine-byte activity/club rows compile after canonical forward declaration, but give 177 rows; input-parameter reuse 167, broader exclusive-branch local reuse 220. No promotion or header changes retained.
- Setup func_8007437C: direct canonical day-array fields compile at 85 rows, aggregate pointer 99; widening result locals and return produces 240/234. The target uses word spill homes, unsigned byte reads and a word return. Temporary prototype correction removed pending a complete match. Update (T-17016): `func_8007437C`, `func_8007505C` and `func_800741B8` are matched ([[matching-notes]], section T-17016); `parameter_up_down` stays `INCLUDE_ASM`, best form 221 funcdiff lines (`drafts/T-17016/`).

These are full-TU diagnostic row counts, not literal matches. Historical evidence read first: [[matching-breakthroughs]], [[bunka-family-results]], [[etc-family-results]], [[broad-coverage-results]] and referenced MAIN tickets. Do not repeat the same source forms without new evidence.

Initializer guards pass: headers OK, globals OK, prototypes 0 new / 15 known, branch check OK (build/main14200/412-guards.log). Review remains pending while further families are investigated.

## Character initializer checkpoint

func_80041C2C adds **796 literal linked bytes**. Using one local GameState pointer for every access with existing CharFlags b1/b3/b6/b7/f9 stores changes the full-TU score from 366 rows to zero. A new month/day bitfield view also scores zero with the pointer but is unnecessary and was removed. No header layout change or new FAKE is needed. The explicit month/day byte/halfword masks retain the original stores. Evidence: build/main14200/41c-proof.log, 41c-build.log and 41c-guards.log. Both new functions pass default literal verification together; MAIN SHA1, headers and globals pass. Cumulative accepted gain: **2 functions / 1472 bytes**.

## Review correction to initializers

The first standards/spec reviews found the new prototype outside the include guard, unexplained pointer-to-s32 strncpy casts, and redundant literal mask spellings whose original-source plausibility was not established. These have been removed. The final initializer source uses GsDateBits month/day fields for the birthday word and Rec38 +0x10, and CharFlags.f20 for the initialized nibble. Existing word/byte/halfword views and record sizes remain unchanged. The prototype is inside MAIN_API_H; strncpy receives pointers directly. Ordinary complemented-mask probes failed (store scheduling and a widened AND), so they were rejected. Both final bitfield functions retain default literal MATCH and MAIN SHA1 OK (build/main14200/bitfield-proof.log and bitfield-build.log). No FAKE is needed for the initializer pair. Re-review and final guards follow.

## Background sprite checkpoint

Three related renderers add **5152 default literal linked bytes**: func_800578F4 (1064), func_80057D28 (1648), func_80058398 (2440). MAIN SHA1 passes. Proof: build/main14200/bg-proof.log and bg-build2.log.

The decisive source repair is one reused SPRT pointer across successive packets, rather than m2c's separate short-lived packet values. Shared table-pointer comparisons in the draft were actually comparisons of the incoming mode with 1/2; those now use the argument directly. The repository already defines SPRT in include/psx_types.h; it is reused, with a PsyQ-compatible setWH comma-expression helper. Paired size assignment and x/y-before-UV store order close the remaining scheduling differences.

**Two functions are FAKE:** 57D28 and 58398 each need one unused s32 word before the table-pointer local to reproduce the target's +0x3C spill home and 0x50-byte frame. Without it all non-frame instructions match, but 24/32 frame/home rows differ. Their source comments explicitly label this T-14200 workaround; the real extra local remains unknown. The 1064-byte 578F4 function uses no FAKE. Cumulative gain: **5 functions / 6624 literal bytes**, comprising **2536 bytes without new FAKE and 4088 bytes in two labeled frame-workaround functions**.

An initial duplicate SPRT declaration in libgpu.h was rejected by the complete MAIN build; it was removed in favor of the existing shared type. The subsequent build and literal proofs pass. A source audit confirms all 22 pre-existing C bodies in the two touched files remain byte-for-byte identical as source (build/main14200/body-audit.log).

## Additional bounded negative evidence

- Neighboring initializer 419FC improves 36 -> 22 -> 14 rows by correcting its byte clear, one-based index and loop increment in the body. Return-type and consistent-pointer probes do not close the remaining aggregate-base register and clear/store scheduling gap. No promotion.
- Scheduler 6D138: existing CharFlags casts 347 rows, consistent GameState pointer 262, aggregate flag-copy forms 329/394. Entry speculation/control-flow remains unresolved. No header change retained.
- Input 42CB0: packet bytes through existing controller arrays and canonical GameState fields 330 rows, pointer 324. Still broad allocation/control-flow differences.
- Numeric 4AE54: abs argument/texture byte-pair repairs identified, but initial scratch compile failed due an uncorrected void-pointer texture local. No match claim. The prior spill-order evidence remains applicable.

## Final review and validation

Both code-review skill axes (GPT-6.1 Sol medium) pass after removing redundant signed RGB casts. The final source passes all five full-TU default literal proofs (build/main14200/final-proof.log) and **27/27 freshly rerun binary SHA1 checks** (final-confirm-build.log). The complete shared-header build also passes (final-build.log). Headers/globals pass on final source; prototypes report 0 new / 15 known disagreements and branch check OK. All 22 old C bodies remain verbatim. No reserved-file, overlay-source, generated-ASM, compiler or coverage changes; no push/PR.

Final isolated-branch census: 5887/6958 game functions, 1414152/2279368 game bytes (62.0%). Accepted gain is 6624 literal bytes: 2536 without new FAKE and 4088 with the two labeled frame-home locals. This bounded checkpoint is complete; the broader 80% goal and remaining MAIN work are ongoing. Integrate the entire T-14200 commit chain, including the final review corrections, before relying on the source-plausibility result.

## Types-first pass (T-17000)

T-20900 update: `DisplayRecord44` (game.h and the local copy in `800674B0.c`) and `TextureRecord44` are `DisplayActor44`; `TelEntryFlags`, `EntryFlags54C`, `EntryFlags29` are `ScheduleEntryFlags`; `FlagsWork24` and `NameMetricWork` are `Work800E7D10`, the declared type of `D_800E7D10` (main_api.h) ([[data-types]], "Shared-type unification").

Types-first pass of [[speedup-research-2026-10-16]] on the main executable ([[tickets/T-17000-types-first-main]],
branch `w3-main-types` from `w2-good`): recover the shared types, redraft all 163 unmatched `src/main`
functions (140 unguarded, 23 guarded; 139 KB), then cut packages T-17001..T-17027. Drafts and driver:
`drafts/T-17000/` (README there). About 1.5 hours of agent time to the packages.

Main is the hub of the game's data. Its large aggregates (the `D_800E7D10` object, the `D_80125C00` table,
`D_801230D0`, the 0x50-byte coordinate records) are declared in `main_api.h` as scalars or byte arrays that
overlay C files read under their own views. The ticket could change only `src/main` and `main_api.h`, so
those declarations stay; they are listed below for a cross-unit ticket. What changed are main-only tables
(no C users, no overlay declarations) and the draft-time views of the aggregates that are already typed.

### Type map

| Type / global | Evidence | Users |
| --- | --- | --- |
| `u8 D_800B6890[12][2]`, `u8 D_800B68A8[12][2]` | an 11-case switch reads `lbu %lo(D_800B6892 + 2k)` after adding the column; the next table starts at `D_800B68C0` (`func_80073CB0`) | `func_80076FDC`, `func_80073CB0` |
| `s8 D_800B68E0[8][9]`, `s8 D_800B6928[][9]` | `lb` rows, row index times 9; eight rows end where `D_800B6928` starts | `parameter_up_down` (408 to 177 rows) |
| `s32 D_800C6200[8]`, `s32 D_800C62C0[32]` | word walks that end at the next symbol (`D_800C6220`, `D_800C6340`) | `func_80059EF0`, `func_8006509C` |
| `s32 D_80125DA0[][16]`, `s16 D_80125E20[][16]` | rows indexed by `channel << 6` and `channel << 5` | `func_8007BBE8`, `func_8007AF0C` |
| `s16 D_800B5AA4[16][2]`, `s16 D_800B5AE4[15][2]` | (x, y) halfword pairs copied pair by pair into the pair table at `D_80125C10`; the copy from pair 3 runs to `D_800B5B20` | `func_8006509C` |
| `Flags7D4F` bit-field view of `D_800E7D4F` | `lui rA; lbu rB; ori; sb` with rB != rA is a bit-field store (T-7010); BUNKASAI's `BunkaReturnFlags` is the same view | `func_8007959C` (matched) |
| `u8 *D_800B6CF8[]` | message table indexed by `func_80077CA8()`, passed to `sndisp` | `func_80077E30` (matched) |
| `SaveBank D_80123320[2]` (`s32 checksum; u8 data[0xEFC]`), header externs `D_80123122`..`D_801231A0` | memory-card block of 0x2000 bytes at `D_80123120`: 0x200-byte header, two 0xF00-byte banks; a 12-bytes-per-iteration copy loop of 0xF00 bytes is `D_80123320[1] = D_80123320[0]`; one `lui at` per header store means separate symbols (T-17007) | `func_80054C74` (guarded), `func_80055A38` |
| seven `void` prototypes (`func_80075468` ..) and `s32 holiday_club(void)` | defined later in `80075320.c`, called earlier by `func_80075320` | `func_80075320` (matched) |
| draft-time only: `GameState D_800E6280`, `Rec34`, `Rec24`, `Ring18`, `Work80125D10`, `MATRIX D_80129C40`, every sized array of `main_api.h`, `D_8011ECD0` as `DisplayActor44[160]` | the T-16550 fold: a label inside one of them becomes base + offset in the asm handed to m2c | all redrafts |

Left for a cross-unit ticket (each needs overlay C edits):

- `D_800E7D10` object, 0xEE0 bytes (`func_800420D0` clears it with `bzero`): 30 of its labels are read by 14
  unmatched main functions and by C in ETC, TACO, NAME_ENT, BUNKAKEN, BUNKASAI, DATE, ENDING, OLH, OPTION, TT.
- `D_80125C00`: 13 records of 0x10 bytes; `D_80125C10` is a table of (x, y) halfword pairs. 22 unmatched
  main functions, C users in 13 overlays.
- `D_801230D0` (`s32[17]` by type_recovery; TEL and NAME_ENT read it as a counter, ETC as `EtcWork40`).
- `TaiikuBig` (0x50) has the GsCOORDINATE2 layout of TACO's `TcObj50`; `D_80122750` is a table of 17 of them.
- `D_80125D60`..`D_80125D9C`: four `s32[4]` channel arrays. Main-only, but matched `80079B10.c` reads the
  scalars and one `(u8 *) D_80125D60 + off` view; not converted.

### Idioms found on the way

- Switch on a `u8` GameState field: the original keeps the selector in `$v1`, IDO copies it (`move v0,v1`).
  The byte view `switch (*(u8 *)&D_800E6280.unk_1109)` (FAKE, the TEL idiom of T-10630) matched
  `func_80075320`, `func_80053F04` and `func_80077E30`. It did not help the compare-chain functions
  (`func_80077CA8`, 34 rows; `func_80044C98`, 35 rows, a global kept in `$v1`).
- A jump table needs every case: `func_80077E30` switches through five entries; its draft had `case 4`
  folded into `default`.
- m2c indexes a word table in bytes (`*(&D_800B6CF8 + (i * 4))`); declare the table and write `D[i]`.
- `GameState *p; p += 4;` (19 census drafts): m2c types a byte walk over the game state with the struct
  pointer. The new `struct-walk` rewrite gives a byte pointer; `func_80075AF8` matched as the 11-girl loop
  over `unk_66C` with the `s8` flag `unk_F6C` read unsigned through its address, as `8005A0B0.c` does.
- `func_8007866C`: a 72-byte name buffer and the `& 0xFF` argument mask give 14 rows
  (`main/func_8007866C/hand.c`); the rest is a temporary-register offset in the club flag update.
- Not closed in the time box: `Fade_In_Main` and `Fade_Out_Main` (the original sets the return value 1
  in `$v0` before the compare and keeps the byte in `$v1`; six return/flag spellings gave 20 to 40 rows). Both matched later in T-17022 (a `u8 temp` read after the timer test; [[matching-notes]], section T-17022).

### Redraft

`drafts/T-17000/redraft.py` scores two drafts per function through the T-14500 census path: `plain` (m2c with
the current headers) and `typed` (the T-16550 fold over all typed main aggregates and sized arrays, the
`DisplayActor44` view, and the T-16560 rewrites plus `struct-walk`), then `tools/draft_fixes.py`. Baseline:
the census score of `drafts/T-14500/best-quality-catalog.jsonl`.

| | value |
| --- | --- |
| functions redrafted | 163 (140 unguarded, 23 guarded) |
| unguarded with a compiling draft | 126 (14 failed: m2c errors, `M2C_LWL`, invalid casts; the census had hand repairs for some) |
| against the census (unguarded) | 45 better (34,208 bytes), 72 same, 9 worse |
| median over the 126 | 153.5 to 151; sum 33,099 to 31,966 rows |
| typed against plain | 50 better, 18 worse, 70 same; the typed draft was chosen for 108 of 126 |
| rewrites in the chosen typed drafts | pointer-step 28, actor-offset 13, struct-walk 4, matrix-copy 2 |
| guarded (regenerated m2c against the hand drafts) | 16 worse, 5 same, 1 better |
| score 0 | none |
| matched from the redrafts | 5 functions, 1,948 bytes: `func_80075320` (7 rows), `func_80053F04` (12), `func_80075AF8` (27), `func_8007959C` (30), `func_80077E30` (37) |

Largest gains: `parameter_show` 362 to 61, `parameter_up_down` 408 to 177, `func_80076000` 354 to 231,
`func_80073CB0` 488 to 380, `get_h_tokimeki` 844 to 742, `func_8008647C` 206 to 128, `func_8005CA94` 122 to 61.
The fold helps main less than TAIIKU: most census drafts already compiled the GameState labels as separate
scalars, which compile like members except for the load-after-store order, and the large shared objects
above could not be typed.

## Targeted type pass (T-20940)

Second, targeted pass on `src/main/80075320.c` (holiday menu, telephone, appointments; 28.7% matched before)
in [[tickets/T-20940-types-main-80075320]]. Types in [[data-types]] ("Targeted type pass: main `80075320.c`"):
the key ring `KeyRing D_800B6990` (one object: `pos` is reloaded after the store into `keys[pos]`), the telephone
numbers `u8 D_800B3778[13][7]`, the name table `u8 *D_800B3538[16]`, the unit's static tables, single-bit fields
`CharFlags.b15 .. b19` and `ScheduleClubFlags.b29`, and the missing `func_80075AF8` prototype. Matched sibling:
`func_8007437C` in `800737A0.c` (the weekday twin of `func_80077694`, same display-record setup).

| function | before | after | how |
| --- | ---: | ---: | --- |
| `func_80077694` | 159 | matched | written from the sibling; FAKE unused record pointer for the spill slot |
| `telephone_class_init` | 144 | matched | name table entries `[i - 1]`, `[12]`, `[13]`; four `s16[14]` row arrays filled x, y, w, h; FAKE second local for the frame |
| `holiday_tel_exit` | 85 | matched | `D_800B3778[F5F][k]` (row 10 for girl 13); `gs-local` |
| `func_80077900` | 163 | matched | bit-field tests `b1`, `b2`, `b18`, club `b29`; `gs-local`; club passed to `main_state_set` as a second argument |
| `appointment_launch` | 89 | 7 | `u32 tmp[3]`, the `GsWord` byte view of GameState +0x724 |
| `func_800755B4` | 205 | 10 | `KeyRing` clear loop on one line (FAKE), 0x104-byte message buffer, the typed fold's `unk0` fixed; the last rows are the shared `lui $at` of the unrolled stores (as1, [[build-step-gaps]]) |
| `func_80075C84` | 269 | 53 | `KeyRing`, `D_800B3778[i][j]`, the 16.16 word at Rec38 +0, `b3` read as a shift; frame 8 bytes large with `gs-local` |
| `func_80076630` | 84 | 18 | schedule-entry bit-fields `unk_00`, `unk_25`, `b24` |
| `func_8007739C` | 191 | 110 | `gs-local`; F68 nibble store; left: temporary numbering (t3 skipped by the original) |
| `func_80076000` | 231 | 218 | the typed fold's `unk0` fixed (it read +0 for `unk_1093`); frame 16 bytes small |
| `func_80076FDC` | 371 | 361 | header types only (`D_800B69D0`, `D_800B69D8`) |
| `func_800754C0`, `func_800764AC`, `func_800767A4`, `func_80077CA8` | 9, 54, 317, 26 | same | not worked on |

Sum of the best scores 2,397 to 1,183 rows; 2,568 bytes matched. The header types alone moved one score up and
one down and made `func_80075C84` stop compiling in every variant (m2c reads the new 2-D table and the struct
through byte offsets); the gains came from drafts that use the typed objects, as in T-16550 / T-16560. Idioms:

- `gs-local` (`GameState *gs = &D_800E6280;` with `gs->` everywhere) changes IDO's register choices without
  changing the addresses: two of the four matches needed it, two of the hand drafts got worse with it.
- The typed fold of `tools/redraft.py` still prints `D_800E6280.unk0` for a `%lo` whose `%hi` sits in a branch
  delay slot (`func_800755B4`, `func_80076000`, `func_80076FDC`); the `typed-late` variant avoids it, but the
  chosen `typed` drafts kept the wrong field.
- A local that only exists for the frame (`func_80077694`, `telephone_class_init`) is marked `FAKE`: IDO gives
  every local a slot once one local is in memory (T-3330).

Left for a cross-unit ticket: GameState +0x724 as `u8 unk_724[4]` (users in DATE, OPTION, `80041000.c`), the
three 16.16 words at the head of `Rec38`, and the `D_801230D0` and `D_80125CA0` blocks.

## Wave 3 package w3-main-p01 (T-17001)

`src/main/8007C030.c`, two long-standing guarded drafts ([[tickets/T-17001-w3-main-p01]]). `_sprite_set_box_shade_tarao` (484 B) is
matched: natural one-trio-per-line colour stores plus one local that holds the red byte of `arg5` and is read once (FAKE; it
numbers an invisible uopt range first, which shifts the register choices and the stack-argument load order, see
[[register-cheatsheet]]). `dialogue_speak_part` stays guarded at 16 lines; the as1 scheduler trace shows that the original needs
the first-argument constant and the narrowing on an earlier source line than the string stores ([[matching-notes]], T-17001).
### Wave 3 package w3-main-p04 (T-17004)

[[tickets/T-17004-w3-main-p04]] took five guarded drafts of `src/main/8006CB30.c` and matched four in plain C: `func_8006EB64`, `func_8006EEC4`, `func_80072998` and `func_800726F0` (2,544 bytes). The first three needed only a local `GameState *g = &D_800E6280;` for the reads (no FAKE left in the first two); `func_800726F0` also needs a marked `FAKE` local copy of the first `unk_03D` read, found by autotweak. The Opus closer matched `func_8006D6E0` (268 bytes; 5 of 5, 2,812 bytes): `s32` with no `return`, because as1 keeps the `move v1,v0` copy only when `$v0` is live at the exit. Details in [[matching-notes]], section T-17004.
### Wave 3 package w3-main-p08 (T-17008)

`func_80059EF0`, `func_8004F6B0` and `func_80059BE8` matched ([[matching-notes]], section T-17008). Type changes: `s32 D_800C6200[8]`, `s32 D_800C62C0[32]` are index loops now (the `func_80059EF0` entry of the type map above); `s16 D_80125BF0`/`D_80125BF2` declared (the cursor, promoted by uopt); `func_80098490` takes `u16` parameters; `D_800C9730` and `D_800C5FCC` have file-local array views in `80059B40.c` (`s32 D_800C9730[11]`, `s32 D_800C5FCC[2]`, `MAIN_API_OVERRIDE_*`), while `main_api.h` keeps `u8[]` and `s32` for their other users.

### Wave 3 package w3-main-p13 (T-17013)

`func_8004AE54` matched ([[matching-notes]], section T-17013); `func_8004B358`, `col2sepia` and `make_color_bar` not. Type notes: `D_800B3D84` is the table of sixteen (u, v) glyph pairs, so `D_800B3D96/97` and `D_800B3D98/99` are its elements 0x12/0x13 and 0x14/0x15 and no separate globals are declared; the sprites are `SPRT_8` (number printer) and `SPRT` (string printer) from `psx_types.h`.
### Wave 3 package w3-main-p14 (T-17014)

`src/main/800563F0.c`, the PsyQ movie player ([[tickets/T-17014-w3-main-p14]]). `func_8005715C` (subtitle picker, 564 B) is matched with one FAKE scalar that reserves the original's fourth stack slot. `strNext` (496 B) is matched in the closer pass with one FAKE (`w * 1` keeps the operand order of the width compare), `strCallback` and `func_80056BA8` stay `INCLUDE_ASM` with their drafts in `drafts/T-17014/`. Type changes: `StrDecEnv30` is now the sample's DECENV and lives in `main_api.h`; `StrCtx` (the state at `0x80180000` behind `D_80125C58`) and `StHEADER` are new; `SyncObj` and `MovieVlcState` are gone ([[matching-notes]], section T-17014).
### Wave 3 package w3-main-p17 (T-17017)

`80058D20.c`: no match of the three remaining T-0018 functions ([[matching-notes]], section T-17017). Type work: the model record walked by `func_8005938C` is a local 0x88-byte `ModelRec88` (active/flag bytes, object at +4, handle at +8, `MATRIX coord` at +0x18, `SVECTOR` rot/scale/pos/vel at +0x64..+0x7C) in `drafts/T-17017/main/func_8005938C/`; the word at `D_800E731C` is read as `*(s32 *)&D_800E6280.unk_1095[7]` (+0x109C, like the +0x1098 word other units read through the same array). `func_800590CC` (16 rows) and `func_800591D8` (32 rows) stay guarded with aliased width and height variables.

## Bounded renderer follow-up (T-24006)

[[tickets/T-24006-main-render-family]] reused complete-main at f70450137580e684e20f8ecd28e7a93062385ac7. Exclusive source scope: src/main/8004C3F0.c. No production C/header changes and **0 new verified bytes**. Source-only hypotheses and stable aligned scoring metadata are in drafts/T-24006/. Historical printable row counts are not directly comparable with the current stable insert/delete cost (a substitution costs two).

| Function / archived form | Aligned cost | Instructions built / target | Concrete blocker |
| --- | ---: | ---: | --- |
| mod_trans_vram, ModRec14 plus corrected word stride and byte buffer | 241 | 197 / 188 | frame 0x1A8 versus 0x1A0, buffer/divisor slots and selector allocation; natural loops change switch storage and loop register allocation |
| mod_set_gpu, ModRec14 plus SDK SPRT_8 | 225 | 204 / 201 | frame 0xB0 versus 0xA8; selector goes through v1, record/divisor spill offsets differ |
| mod_set, actual sprite request aggregate | 295 | 198 / 195 | frame 0xB8 versus 0xE8; original request at sp+0xB0, aggregate/selector allocation unresolved |
| get_fnt, natural byte-buffer loops | 251 | 266 / 259 | loop counter lifetime hoists the second loop counter into an extra saved register; buffer offset and horizontal mirror scheduling differ |
| srn_tpage_show, SDK POLY_FT4 / RECT | 1032 | 814 / 806 | first frame mismatch and repeated packet allocation/scheduling; no additional speculative variants |

New type hypothesis: `ModRec14`, size 0x14, follows func_8004E44C's writes and the three model consumers: pointer table +0, word map +4, format byte +8, halfword +0xA, four byte dimensions at +0xC..0xF, halfwords +0x10/+0x12. It stays local to archived drafts pending a verified migration. SDK SPRT_8 describes the 0x10-byte GPU packet in mod_set_gpu; SDK POLY_FT4 and RECT describe srn_tpage_show packets and input rectangle. The mod_set stack request has the observed 0x24-byte sprite layout; unrelated scalar locals in the old m2c draft lose most stores when only the address of the first is passed. Its original `var_s5 += 4` also advances a word pointer by sixteen bytes; the corrected hypothesis advances one word.

The T-17015 srn_set body remains unchanged. Its fresh aligned cost is 146 (149 / 149 instructions); yoff as s16 and unsigned cols both stay 146, wid/hgt as u16 gives 148, and xoff/yoff/y as s16 gives 138 with four extra instructions. No frame/register solution was found, so no redundant archive was added. get_fnt's m2c buffer was three words despite memcpy of 0x100 bytes, and its mirror operations contained incorrect word accesses; correcting bytes/strides alone leaves 338 cost versus 251 for the natural rewrite. The two unused pointer locals in that exploratory rewrite carry a FAKE frame note.

Verification in the existing Docker image: ninja -j1 for build/src/main/8004C3F0.o and build/SLPM_86.053.ok reports no work; fresh direct SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 equals config/SLPM_86.053.sha1. Fresh funcdiff --resolve --no-build --version default gives linked literal MATCH for all six assigned ASM-backed functions plus srn_vram_set, move_255_line, srn_init and func_8004E44C. These prove production preservation, not new C recovery. Literal sizes of mod_set_gpu/mod_set/mod_trans_vram are 804/780/752; the work-list sizes included 24 padding bytes each. check_headers: OK; migrate_globals --check: OK; sync_protos --check-branch: 0 new disagreements, 15 known, branch check OK. Logs remain ignored under /tmp/T24006-*.log. Ticket awaits integration review.

### Completion package T-24015

[[tickets/T-24015-complete-main-menu]] revisited five unstarted w4-p41 functions in 8005A0B0.c; source-only forms in drafts/T-24015/.

Zero of five matched; no production C or header changed, zero verified bytes added. All five remain ASM. Current resolved rows/aligned: CA94 57/57, CC40 118/114, B43C 112/112, A410 41/41, F730 113/111.

A410 is the strongest new lead: signed coordinate halfwords plus a same-array 64-byte end bound remove IDO alignment peeling and recover frame/spill offsets. End-label address sharing, counter allocation and scheduling remain. The form visits sixteen coordinate pairs, two per iteration; an unrelated end-label bound scores 187 rows. This is a concrete retry lead, not proof of a match or compiler limitation.

CA94 repairs the lossy old GameState fold to unk_1118 and uses byte strides through Work800E7D10; selected-index spill remains four bytes high and narrowing/copy allocation differs. CC40 repairs GameState fields, byte strides and unsigned timer comparisons. Its D_800B39C4 table appears to hold two words per index, still an unresolved scratch scalar extern. Its s32 fallthrough definition preserves the existing implicit caller view; a void signature needs a deliberate canonical prototype decision before promotion.

B43C uses Rec38 unk_0C.f.b2 instead of a word mask; original frame and input-nibble allocation differ. F730 follows matched F26C/FDEC siblings with five-row halfword arrays. D_800E67C4 maps to GameState unk_53C[8]. Frame matches but all four array homes are eight bytes high. Coordinate variable names are inherited from T-16510; original menu_set order and values are retained. No FAKE padding was added to selected forms; a rejected padding experiment is preserved only in the ignored probe archive.

Baseline MAIN SHA-1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 matches config/SLPM_86.053.sha1. Ninja -j1 build/SLPM_86.053.ok was current. Headers/globals pass; prototype branch check reports zero new disagreements, fifteen known. Literal no-build win proof is not applicable because no function was promoted.
## T-24041 LINE_G2 renderer

[[tickets/T-24041-main-render-cd-siblings]] recovers func_80049FF0,348 B of unguarded C, using the actual two-colour line packet LINE_G2 (0x14 bytes), a meaningful GameState pointer, and the matched polygon siblings' explicit colour masks/comma grouping (FAKE marked). A K&R narrow definition keeps the existing unprototyped caller ABI; main_api.h stays unchanged. Root approved the missing shared packet declaration in psx_types.h. The allocation and individual sb/sh offsets establish its layout; no external SDK header was available locally or imported. Normal default linked literal proof and full MAIN checksum pass.
## Dialogue closure update (2026-10-18)

The historical guarded 16-row dialogue gap is resolved by [[tickets/T-24021-dialogue-scheduling]]. Default C matches 8,672 text bytes and main SHA1. Two early constant argument definitions use a marked zero-mask FAKE; prior negative experiments remain historical evidence. Replay is in drafts/T-24021/replay_closure.py.

## T-24041 CD dispatcher source checkpoint

func_800455C4 remains unchanged assembly. [[tickets/T-24041-main-render-cd-siblings]] archives repaired typed source in drafts/T-24041: normal full-file aligned cost512/frame104, then499/frame88 after removing artificial temporary homes; target frame80. Ring18 indices, word payload-header access and SDK-shaped stack locals are repaired, but broad branch/register differences remain. Exact negative hypotheses and scratch-only declarations are recorded in the archive; no speculative CD headers added. The separate348 B renderer win and final MAIN checksum/guard evidence stand.

## T-24046 screen page structural follow-up

[[tickets/T-24046-screen-page-renderer]] adds no production C. Source-only drafts/T-24046 fixes two concrete old-draft defects: signed-byte narrowing of a positive128 texture offset before texture-page arithmetic, and the missing unused argument5 slot that shifts all clut/depth stack inputs. Reused POLY_FT4 plus inlined common expressions produces the target120-byte frame, versus200 for the prior-style source. Fresh aligned costs1032 baseline,1026 pointer reuse,637 named derived values,562 automatic spills,524 ABI/local ordering,504 consecutive RGB initialization. Best812 vs806 instructions remains broad; named word-offset/global-state alternate523 is rejected. No shared headers changed and no raw instructions archived. Preserve these structural fixes before any closer pass.
