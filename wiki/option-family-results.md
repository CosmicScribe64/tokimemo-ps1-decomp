---
type: research
updated: 2026-10-17
sources: ["include/ovl/OPTION.h", "drafts/T-17800/README.md", "drafts/T-17800/redraft.py", "tools/type_recovery.py", "tools/siblings.py"]
---

# OPTION family results

OPTION is the options menu overlay (settings pages, a sound test, player and birthday data). Its 0x44-byte menu
records are the `D_8011ECD0` display records 96 to 113 (`D_80120650` is record 96), and every page hands four
`s16` row arrays to `menu_set` (`func_8004F870`). Earlier notes: [[matching-notes]] (batch H, T-1040; the
four-array setup; the `x * 0x44` linear-function test replacement; the `D_80120912` pair).

## Types-first pass (T-17800)

T-20900 update: `OptActor44`, `OptDraw44`, `OptSpr44` are `DisplayActor44`; `OptionScheduleBits` is `ScheduleEntryFlags`, `OptionPlayerBits` is `ScheduleFlags`; `D_800E7D10` needs no override (it is `Work800E7D10` in main_api.h) ([[data-types]], "Shared-type unification").

Wave-3 types-first pass ([[tickets/T-17800-types-first-option]], [[speedup-research-2026-10-16]]), branch
`w3-option-types` from `w2-good`. Start: 36 unmatched functions (22,648 bytes) in three files, 2 of them guarded.
About 75 minutes of agent time for the survey, the types, the redraft, four matches and the packages.

### Survey

- `tools/type_recovery.py --unit OPTION`: the top proposals are jump tables. The useful ones are the
  `stride 0x4 x1` arrays read by `func_8013A09C` and `func_8013A1BC` (`D_8013D798`, `D_8013E148`, ...), which turned
  out to be word tables of 58 and 63 entries (each ends at the next label).
- Census drafts (`drafts/T-14500/OPTION`, `drafts/T-15800`, `drafts/T-16510`): the frequent globals are GameState
  fields under old names (`D_800E738D` = `unk_110D`, 58 uses, `D_800E7314` = `unk_1094`, `D_800E7208` =
  `unk_F88`) and labels inside the display records (`D_80120912`/`D_80120914` = record 106 +0x1A/+0x1C,
  `D_801208FB`, `D_80120650`, `D_80121726`). 13 of the 36 drafts pass four `&spXX` locals to `menu_set`, which m2c
  declares as `s16 spXX` or `s32 spXX[1]`.
- `tools/siblings.py --unit OPTION`: `func_801342A8` <- `func_80136A50` (0.863), `func_8013394C` <- `func_801368FC`
  (0.880), `func_80137040` <- `func_80134804` (0.804), `func_8013493C` <- `func_80136A50` (0.786), `func_80136434`
  <- `func_80133AD8` (0.781); `func_801378DC` and `func_80138080` form one unmatched cluster (0.993).
- `menu_set`, `menu_check`, `menu_bar_color`, `k_speed_set` are the obin names of `func_8004F870`, `func_8004F984`,
  `func_80050B00`, `func_8004EBEC`; m2c prints the obin names, matched OPTION C uses the `func_` names.

### Type map

All changes keep OPTION's sha1.

| Type / declaration | Symbols | Evidence |
| --- | --- | --- |
| `s16 [7]` x4 | `D_8013D364`, `D_8013D372`, `D_8013D380`, `D_8013D38E` | the 0x38 bytes from `D_8013D364` (`.data` of `801389A0.c`) are four 7-entry halfword rows (x, y, w = 0x80, h = 0x14) that `func_80139198` passes to `menu_set`; splat had split them at `D_8013D366`.. |
| `s32 [58]` x2, `s32 [63]` x6 | `D_8013D798`, `D_8013D880`; `D_8013E148`..`D_8013E634` | `lw` indexed by `D_8013D3DC % 58` and `D_8013D3A0 % 63` / `D_8013D3A4 % 63`; 0xE8 and 0xFC bytes to the next label |
| `s32` x3 | `D_8013D300`, `D_8013D304`, `D_8013D308` | scroll state of `func_801326BC`; the census declared them itself |
| `u8 *[4]` | `D_8013D350` | four pointers (`D_8013D30C`..`D_8013D334`) walked up to `D_8013D360` by `func_8013554C` |
| `OptActor44` (0x44) | view of `D_8011ECD0` | `DisplayActor44` (game.h) with `s16` fields at its padding: +0x1C is the partner of +0x1A (`D_80120914`), +0x2C..+0x32 are set from `func_800AE0D0` in `func_80139198`. The drafts write records as `((OptActor44 *)D_8011ECD0)[k]` |
| `OptSpr44`, `Arrs4x8`, `void func_801325A0(Arrs4x8 *)` | | moved from `80132000.c`; with the prototype m2c types the row local of the five `func_801325A0` callers as `Arrs4x8` by itself |
| `OptionCharBits`, `OptionPlayerBits`, `OptionScheduleBits` | | bit-field views moved from `8013A550.c` |
| `OptionDateBits` | GameState +0xF8, Rec38 +0x10 | `GsDateBits` plus bits 9-10 (`<< 0x15 >> 0x1E`), read by `func_8013A7EC` |
| `#include "game.h"`, one include guard | | an early `#endif` left the second half of `OPTION.h` unguarded |
| `void func_80132BA8(void);` | | called before its definition; drafts of it did not compile |
| `extern u8 D_800E7D37;` (main_api.h, addition) | | read by `func_80137040` |

Not changed: `D_80120912[]` stays an `s16` array of `main_api.h` (the matched siblings read the pair through it);
the `D_8011ECD0` declaration itself (`u8[]` in `main_api.h`, used byte-wise by matched OPTION code).

### Redraft

`drafts/T-17800/redraft.py` is the T-17500 driver with three changes. It writes the records through
`OptActor44`. A new `menu-arrays` rewrite declares the four `menu_set` rows as `s16 spXX[n]` in address order, with
`n` from the gap, or as one `Arrs4x8` when `func_801325A0` fills them, and turns `spXX = v` into `spXX[0] = v`. Two
repairs of m2c output were added: `raw-display` for a folded label printed as a number, and `return-type` for `void`
printed in an `s32 f(void)` of the header.
Numbers (30 unguarded functions still unmatched):

| | value |
| --- | --- |
| plain (headers only) compiles | 26 of 30; better than the census in 0, worse in 1 |
| typed (fold + rewrites) compiles | 30 of 30; better than plain and census in 14 |
| census + rewrites compiles | 28 of 30; better than the census in 11 |
| chosen variant | typed 18, census 9, plain 3 |
| against the census | 19 better (13,876 bytes), 10 the same, 1 worse (`func_80132BA8`, 192 to 200: its T-16510 body does not compile in the census variant) |
| sum of diff rows | 5,722 to 5,362; median 183.5 to 167.5 |
| score 0 | none |

Largest gains: `func_8013493C` 251 to 152, `func_80139A30` 302 to 225, `func_801344E4` 323 to 281,
`func_801326BC` 166 to 134, `func_801361FC` 120 to 96, `func_801376A8` 107 to 85. The headers alone changed
nothing; the gains come from the fold and the menu-arrays rewrite. Fold losses as in SHOUGATU: an indexed GameState
read loses its offset (`func_80139E3C` prints `(u8) D_800E6280`), and a `lui` shared by two `%lo` gives
`D_800E6280.unk0`; the census variant covers those.

### Matched during the pass (4 functions, 1,548 bytes)

- `func_8013A09C` (288) and `func_8013A1BC` (328): written with the new word tables
  (`D_8013E340[D_8013D3A0 % 63]`, ...). The `u8` selector must go through a local, `u8 t = D_8013D3B4; switch (t)`:
  switching on the global puts it in `$v1`, the original has `$v0`.
- `func_80136434` (488): port of `func_80133AD8` with five records,
  `for (i = 0; i < 5; i++) D_8011ECD0[0x1C6F + i * 0x44] &= ~0x80;` on the `for` line (the T-7020 FAKE of the
  sibling); IDO peels one iteration and unrolls the other four as the original.
- `func_80137040` (444): the `func_801368FC` shape (record loop with a separate counter `j += 2`, `switch (sel)`)
  with four records and the four-case `switch`.

### Near misses (hand drafts in `drafts/T-17800/OPTION/<func>/hand.c`)

- `func_8013394C` (396, 5 funcdiff rows): the `func_801368FC` port as an `s32` function; only the `move v0,a2` in
  the delay slot of the first switch compare is missing. `s8`/`s16`/`s32` selectors and switching on
  `unk_1094` itself give 7 rows; `u32` gives 5.
- `func_80139198` (860, 12 rows): the record loop matches with a stepping pointer, `p[96].field` over
  `OptActor44`, `s32 i`, and the `unk_14` store moved up; left is the `unk_03A = 0` store, which the original
  schedules after the `menu_set` argument setup and relocates against `D_800E62BA`.
- `func_80132000` (176, 52 rows): the selector part matches with a `u8` local; the original keeps `unk_03A` and the
  two record halfwords in `$v1`/`$v0`, IDO in `$t6`/`$t7`.
- `func_801342A8` (468, 116 rows): port of `func_80136A50` with an `Arrs4x8` local; the original frame is 0xC0
  (0x48 bytes more than the locals need) and it reads `D_80120914` as its own symbol.
- `func_8013A7EC` (1,156, 218 rows): the bit-field version in the style of the matched `func_8013AF10`; not close.

### Packages

Three packages for wave 3, T-17801..T-17803 (`w3-option-p01`..`p03`), 12 functions, 6,636 bytes, built with the
wave-3 inventory scripts. The p01 and p02 lists point `func_80139198`, `func_8013394C` and `func_80132000` at their
census drafts; the hand drafts above are closer.
