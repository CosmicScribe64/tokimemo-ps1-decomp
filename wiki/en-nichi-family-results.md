---
type: research
updated: 2026-10-17
---

# EN_NICHI family results

EN_NICHI loads at 0x80132000 ([[overlays]]) and has two C files: `src/ovl/EN_NICHI/80132000.c` and
`80136B10.c`. Earlier work on it is spread over [[matching-notes]] (T-0750 batch B, T-2100, T-4010, T-9710,
T-10020), [[data/t0018-cases]] and [[tickets/T-14000-ending-en-nichi-families]]. This page starts with the
types-first pass.

## Types-first pass (T-18200)

[[tickets/T-18200-types-first-en_nichi]], branch `w3-en-nichi-types` from `w2-good`, under
[[tickets/T-16990-wave-3-conveyor]]. Start: 19 unmatched functions (15,464 bytes), two of them guarded drafts
(`func_801330D0`, `func_80136A2C`). `tools/siblings.py` found no matched sibling above a 0.70 opcode ratio for
any of them, so the unit had to be won from its own types. End: 14 unmatched, 5 matched (2,500 bytes), about
one hour of agent time.

### Type map

Every change keeps the EN_NICHI sha1.

| Type / global | Replaces | Evidence |
| --- | --- | --- |
| `EnObj D_8013984C[10]` (0x30 records, ends at `D_80139A2C`) | `u8 D_8013984C[]` and `Rec30 D_80139868[]` (a 0x30-stride view of `unk_1C`) | type_recovery: stride 0x30 x10, 19 functions; splat labels `D_80139862`..`D_801398AC` are fields of records 0..2. Matched users now read `D_8013984C[i].unk_00` or `&D_8013984C[arg0]` (the review turned the three `(u8 *)D_8013984C` byte walks of `func_80133F00`, `func_801340CC` and `func_8013515C` into fields); all still match. |
| `s16 D_80139A34[16][6]` | undeclared | `func_80133CA8`: six halfword frames per row, row `func_80134008(i)` for kinds 1..3 and row 8 + `func_80134008(i)` for kind 4 (+0x60); one base register for both, so one object. |
| `u8 D_80139A2C[8]` | `u8 D_80139A2C[]` | the sprite flag per object kind, read as `D_80139A2C[unk_00 & 0x7F]`; also the end of the records. |
| `s16 D_80139228[10][4]`, `s32 D_80139278[10]` | unsized | the pop-up ring buffer of `func_80135A48` (ten slots up to `D_80139278` and `D_801392A0`). |
| `EnHalf22` x3, `EnBytes22`, `EnHalf64`, `EnHalf13` | undeclared | stack tables: `func_80136DEC` copies `D_80139C68`/`C94`/`CC0` (22 halfwords, one per display record 96..117) and `D_80139CEC` (22 bytes, lhu/sh tail); `func_80138CCC` copies `D_80139DA0` (64 halfwords, picked by `rand() & 0x3F`) and `D_80139E20` (13 halfwords, one per girl). |
| `s32 *D_80139204`; `D_80139200`, `D_80139208`, `D_8013920C`, `D_80139214`, `D_80139218` | undeclared | asset addresses of `func_80132450` and `func_80132834`; `D_80139204` points at three tpage addresses. |
| `u8 D_80139B08[12]`, `u8 D_80139C34[8]`, `D_80139C3C[8]`, `D_80139C44[8]`, `s16 D_80139C4C[6]` | unsized externs (the C file defines the sizes) | sized so the fold sees `D_80139C4E`, `D_80139B09` etc. as elements. |

`include/ovl/EN_NICHI.h` now includes `game.h` for `DisplayActor44`, the view of the `D_8011ECD0` display
records that the redraft prints. Not changed: `EnObj` field types (`unk_14`, `unk_18`, `unk_20` have no proven
access; +0x16/+0x1A may be the integer halves of 16.16 positions), `D_801391E4` and the other `s32` asset
pointers that matched code assigns as integers, and `EnResetSlot8 D_801392AC[15][12]` (already typed, T-9710).

### Idioms

- **Index form, not a pointer walk.** m2c prints the record loops as a pointer walk (`EnObj *p; p += 1`). IDO
  strength-reduces an index loop into the same walk, but the two compile differently. With `D_8013984C[i].f`
  IDO knows that the stores go to the global and not to an address-taken local or a promoted global. It then
  schedules loads above them and keeps globals in registers across the loop. `func_80134258`
  (reload of `m` above the store to the record), `func_80133CA8` and `func_80135A48` matched only in index form;
  `func_8013358C` and `func_80132EE8` reach register-only differences in index form and lose the promoted
  globals as pointer walks.
- **Display records by index.** `((DisplayActor44 *)D_8011ECD0)[128 + i].unk_01` for the per-object sprites
  (records 128..137), as in [[game-state]].
- **Peeled remainder of a six-trip loop.** `func_80132834` and `func_80136B74` clear
  `D_800E6280.unk_1228[5..10]` and `[37..42]`. The original stores the first two iterations through absolute
  addresses (one `addiu tX, zero, -1` each) and runs four more from `i = 2`. In `func_80136B74`, whose counter is
  the palette loop's `s0`, one natural `for (i = 0; i < 6; i++)` gives exactly that; in `func_80132834` the same
  loop unrolls the other way and only the first two iterations written out match (FAKE comment).
- **Switch with an explicit `case 0:`.** `func_80133CA8` switches on `unk_00 & 0x7F` through a five-entry jump
  table; `case 0: default:` brings it back (without it IDO emits a compare chain).
- **Register numbering from statement order.** In `func_80133CA8`, `unk_43 = D_80139A2C[...]` before
  `unk_26 = unk_16` and `unk_10 += 1` before `unk_01 = 7` give the original's t-register numbers; the stores
  keep their order.
- **Implicit int** again: `func_80135A48` differed by one `nop` after its second test until it became `s32`
  without a return value ([[matching-notes]], T-4090).

### Redraft

`drafts/T-18200/redraft.py` is `drafts/T-17900/redraft.py` (from T-16550 and T-16560) adapted to EN_NICHI: the
fold covers multi-dimensional arrays (`D_801392AC[15][12]`, `D_80139A34[16][6]`), the record rewrite also
handles a walk that starts at record 0 (`var_s0 = ((DisplayActor44 *)D_8011ECD0);`), and the NAME_ENT views were
dropped. Scores are the census's full-unit diff rows.

| | value |
| --- | --- |
| unguarded functions | 17 |
| better than the census | 6 (5,244 bytes): `func_80136DEC` 592 to 502, `func_80133CA8` 175 to 151, `func_801362A4` 177 to 165, `func_80138CCC` 376 to 368, `func_80132EE8` 249 to 245, `func_80135274` 174 to 170 |
| same / worse | 11 / 0 |
| median, sum | 177 to 170, 3,816 to 3,674 |
| headers alone (plain) | changed 6 scores; `func_80136DEC` and `func_80138CCC` stop compiling until the stack-table rewrite |
| new score 0 | none |
| guarded (census scored the hand body) | `func_801330D0` 60 to 96, `func_80136A2C` 6 to 162 |
| matched after the redraft | 5 functions, 2,500 bytes, all by hand: `func_80133CA8` (600), `func_80134258` (664), `func_80136B74` (632), `func_80132834` (472, FAKE), `func_80135A48` (132) |

As in TACO, the redraft scores did not predict the matches: `func_80133CA8` (151) matched in two edits, the
40-row `func_80134258` needed the index form. Hand drafts with register-only differences:
`drafts/T-18200/EN_NICHI/func_8013358C/hand.c` (loop invariants in t1/t2/t0 instead of t0/t1/t2, and a
`move v0,zero` on the early return; implicit int does not change it) and `func_80132EE8/hand.c`.

### Packages

From the orchestrator's `inventory2.py`/`mklists3.py`; lists in its scratchpad.

| Package | Ticket | Owned C file | Functions | Bytes |
| --- | --- | --- | ---: | ---: |
| w3-en-nichi-p01 | T-18201 | `80132000.c` | 5 (`func_80136A2C`, `func_801330D0`, `func_8013358C`, `func_80132EE8`, `func_80135274`) | 2,700 |
| w3-en-nichi-p02 | T-18202 | `80136B10.c` | 3 (`func_80138CCC`, `func_80136DEC`, `func_8013735C`) | 4,808 |

Left for later waves: `func_80132450`, `func_801339D4`, `func_80134800`, `func_80135ACC`, `func_80135FBC`,
`func_801362A4` in `80132000.c`. Since T-20920 the first four are matched (see "Targeted type pass (T-20920)" below);
`func_80135FBC` and `func_801362A4` are still open.

## Targeted type pass (T-20920)

[[tickets/T-20920-types-en-nichi]], branch `ty-en-nichi` from `w2-good`. Start: 14 unmatched functions (12,964 bytes),
two of them guarded (`func_801330D0`, `func_80136A2C`). End: 6 unmatched (6,636 bytes), 8 matched (6,328 bytes), all
by hand on the new types, every step EN_NICHI sha1 OK and the clean build 27/27.

### Type map

| Type / global | Replaces | Evidence |
| --- | --- | --- |
| `EnObj.unk_14`, `EnObj.unk_18` as `Fix32` (16.16 x, y), `EnObj.unk_20` as `s32` | `u8 unk_14[2]; s16 unk_16;`, `u8 unk_18[2]; s16 unk_1A;`, `u8 unk_20[4]` | `func_801339D4` reads and writes +0x14, +0x18 and +0x20 as words (`lw`/`sw`), every other user reads the halves at +0x16/+0x1A (`lh`/`sh`): `.unk_16` is now `.unk_14.h[1]`, `.unk_1A` is `.unk_18.h[1]` (26 matched uses). |
| `EnFixPos` (`Fix32 unk_00; Fix32 unk_04;`, 8 bytes): `D_801392AC[15][12]` (the point grid) and `D_80139228[10]` (the pop-up centres) | `EnResetSlot8` (four `s16`, T-9710) and `s16 D_80139228[10][4]` | `func_80135ACC` adds 16.16 words to the grid points (`lw`/`sw` at +0/+4) and subtracts the centre words; `func_80135FBC`, `func_801354E0`, `func_80135600` and `func_80135A48` use the integer halves (+2, +6). |
| local `Fix32 x, y` in `func_80135ACC` | m2c's `*(s16 *)((u8 *)&sp68 + 2)` | only the integer half is set, then the whole word is read (`sh 0x2(s6)` .. `lw 0x68(sp)`). |
| local `s32 p[4][2]` and `POLY_FT4 *` in `func_80135FBC` | eight spilled scalars, byte-offset casts | the corner positions sit in memory at sp+0x54.. in x, y order (an array, not spills); the primitive is a 0x28-byte `POLY_FT4` (`GetWorkBase(0x28)`, uv bytes at +0x0C/+0x14/+0x1C/+0x24, tpage +0x16, clut +0x0E). |
| `EnHalf64` copy in `func_80138CCC` with entries 32..47 rewritten | m2c's separate `s32 spB8[]` | the fill loop walks sp+0xB8..0xD8, which is `tbl.value[32..47]` of the copy at sp+0x78. |
| `DisplayFixActor44` for display record 0 in `func_801330D0` and records 105..116 in `func_80138CCC` | `(&D_8011ECF4)[1]` (FAKE), `volatile` pointers (FAKE) | the two words of record 0 are one object (the second load stays after the first store); rows 109.. and 113.. add 16.16 steps to +0x24. |
| `D_800E7D10.flags[11]`, `.flags[12]` | `D_800E7D2F`, `D_800E7D30` (undeclared) | `Work800E7D10` (T-20900); the base is now a link name in `config/symbol_addrs_types.txt`. |

`include/ovl/EN_NICHI.h` also gained prototypes for `func_80136700`, `func_80136904` and the new definitions
(`func_801339D4`, `func_80134800`, `func_80135274`, `func_80135ACC` take `s32 arg0`).

### Matched (all by hand)

| Function | Bytes | Baseline score | What made it match |
| --- | ---: | ---: | --- |
| `func_80135ACC` | 1,160 | 268 | the `Fix32` locals; direct `D_80139228[arg0]` (a pointer local keeps the centre's y load below the grid store); band bounds `r0`..`r7` as declared locals, which with `x`, `y` in memory gives the 0x98 frame (T-3330 slot rule); one FAKE empty statement `};` from the permuter for the register order |
| `func_80134800` | 1,052 | 324 | plain logic in index form; a pointer local moves the spill slot by 4 |
| `func_801339D4` | 724 | 181 | `EnObj` words; index form with no locals (a pointer or a remainder local grows the frame by 8) |
| `func_80132450` | 780 | 144 | record fields in `DisplayActor44` index form, implicit int; the second loop by absolute record index 96..115 |
| `func_801330D0` | 736 | 60 (guarded) | record 0 words through `DisplayFixActor44`; the clamp through a pointer to the pool (reloads after each store, base in `$v1`) and the copy as `p[152]`; replaces the T-10610 guard |
| `func_80135274` | 620 | 170 | the result is unset on the default path, so IDO keeps it in memory; declaration order `dx, dy, dir` gives the slots |
| `func_80136A2C` | 228 | 6 (guarded) | a plain `for` loop, which IDO unrolls by two; replaces the T-10850 guard and its FAKE |
| `func_80138CCC` | 1,028 | 368 | the table entries 32..47, the u8 girl list and count, the neighbour read before the new frame is stored, one `rand` variable per row (frame) |

### Redraft and paired scores

`tools/redraft.py --unit EN_NICHI` (default variants), once on a snapshot of `f3933305` (before the types) and once on
`c2dabc21` (after them), each from its own copy under `build/ty/`. Drafts in `drafts/T-20920/EN_NICHI/<func>/`
(`result.json`: `scope`, `score`, `census_score`, `base_score`; the matched ones have `status: matched` and the body
as `hand.c`; `catalog.jsonl` has one line per function).

| | value |
| --- | --- |
| the 6 still unmatched | same best score before and after: `func_80132EE8` 242, `func_8013358C` 77, `func_80135FBC` 288, `func_801362A4` 65, `func_80136DEC` 480, `func_8013735C` 584 (sum 1,736) |
| the 8 matched | baseline 1,521 rows in all (144 + 60 + 181 + 324 + 170 + 268 + 6 + 368), now 0 |
| draft text | no m2c draft of the 6 uses the new fields; the casts are unchanged (`func_80135FBC` 8, `func_8013735C` 23) |
| hand drafts (instruction rows that differ, relocation-blind, not the census measure) | `func_80132EE8` 6 (two `lui` swapped), `func_801362A4` 11 (u8 load register, frame +8), `func_80135FBC` 14 (5 with a FAKE `u1 = arg0`), `func_8013358C` 46 (T-18200 hand) |

This repeats the lesson of T-16550/T-16560/T-20900: the header types alone do not move the m2c scores. What they
did here was make hand drafts short enough to finish: with the 16.16 fields and the right local aggregates
(a `Fix32` local, a point array, the table entries), the drafts of six functions were within a few rows on the first
compile, and the rest was frame and register order (declaration order of locals, index form against pointer
locals, implicit int).
