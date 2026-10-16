---
type: research
updated: 2026-10-16
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
`func_801362A4` in `80132000.c`.
