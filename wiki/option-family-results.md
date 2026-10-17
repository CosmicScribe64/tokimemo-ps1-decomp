---
type: research
updated: 2026-10-17
sources: ["include/ovl/OPTION.h", "drafts/T-17800/README.md", "drafts/T-17800/redraft.py", "drafts/T-20910/README.md", "drafts/T-20910/redraft.py", "tools/type_recovery.py", "tools/siblings.py", "tools/redraft.py"]
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

## Second type pass (T-20910)

Targeted second pass ([[tickets/T-20910-types-option]], [[data-types]]), branch `ty-option` from `w2-good`.
Start: 32 unmatched functions (2 guarded). About two hours of agent time.

### Survey

- OPTION's own data is typed: after T-17800 the overlay's `.data`/`.bss` labels are strings, the word and
  row tables of the type map above, and byte/halfword scalars (`D_8013D3A0` .. `D_8013D3F4`) with no
  stride, walk or ordered-pair evidence (`tools/type_recovery.py --unit OPTION` proposes only jump tables).
- What the unmatched functions read beyond that are main-exe objects: the display pool (already
  `DisplayActor44`), GameState, main's BG slot 0 at `D_801220D0` (`SpriteSlot24`, GsBG layout, declared as
  scalars in `main_api.h`) with its cell map, cells and cell index (`D_80122120`, `D_80122140`,
  `D_80122540`, undeclared), the pair table `D_80125C10` (`u8[]` in `main_api.h`; the matched
  `func_80134210` reads it as `*(s16 *)&D_80125C10[i * 4]`), and the work area `D_800E7D10`.
- Ten of the 32 are menu pages built on one template (below); their drafts were far off because m2c
  prints the rows loop as pointer walks with byte offsets, not because a type was missing.

### Types (all byte-identical, 27/27)

| Type / declaration | Where | Evidence |
| --- | --- | --- |
| `SpriteCell8` (0x8: `u`, `v`, `cba`, `flag`, `tpage`), `SpriteMap10` (0x10: `cellw`, `cellh`, `ncellw`, `ncellh`, `base`, `index`), `SpriteIndex80` (`u16 v[64]`) | `include/ovl/OPTION.h` | main `func_8004A1A0` (asm) sets up BG slot n: `SpriteSlot24` at `D_801220D0 + n * 0x24` with +0x14 = `&D_80122120[n]` (stride 0x10), cells `D_80122140 + n * 0x200`, index `D_80122540 + n * 0x80`; bss sizes 0x20, 0x400, 0x100 (two slots of 64 cells). Field widths from the `sb`/`sh` of `func_801328C0` and `func_8004A1A0`; the layouts are PsyQ libgs `GsMAP`/`GsCELL` (the field names are the SDK's). |
| `extern SpriteMap10 D_80122120[2]`, `SpriteCell8 D_80122140[2][64]`, `SpriteIndex80 D_80122540[2]` | `include/ovl/OPTION.h` | main-bss symbols that only OPTION's C reads, through types of the overlay header (CODING_STANDARDS 8a; `check_headers` rejects them in an overlay header with a `game.h` type or as a plain `u16[2][64]`, hence the one-array struct). |
| `void func_80134210(void);` | `include/ovl/OPTION.h` | called by `func_80133F50` before its definition; drafts of the caller did not compile. |
| object view `SpriteSlot24` of `D_801220D0` | `drafts/T-20910/redraft.py` only | the drafts read slot 0 as main `80041000.c` does, `((SpriteSlot24 *)&D_801220D0)->unk_10`; matched OPTION code keeps the `main_api.h` scalars. |

Not changed:
- `D_8013D3B8` stays `volatile u16`. Measured without `volatile`: the `plain` draft of `func_80139A30` 302 to
  219 rows (best 225 to 219), but the `typed` draft 225 to 370 and the guarded `func_80139E3C` 60 to 63; a
  `volatile` view inside the guarded body instead (pointer local 61, casts 82) does not keep its score. The
  original `func_80139A30` does not behave as if the halfword were volatile, so the qualifier is a draft aid
  of T-11340, not the type; left for the ticket that matches `func_80139E3C`.
- `D_80125C10`, `D_801220D0`: main-exe declarations (`main_api.h`), not in this ticket's scope. A pair-table
  type for `D_80125C10` (`s16 [16][2]`) and `SpriteSlot24 D_801220D0[2]` are the natural next step; the
  users are main, NAME_ENT, OPTION and ETC.

### The menu-page template

`func_80133718`, `func_80133CB8`, `func_801344E4`, `func_8013554C`, `func_80135E48`, `func_80136688`,
`func_80136C30`, `func_801373C8`, `func_801378DC`, `func_80138080` share one shape with the matched page
functions (`func_80133AD8`, `func_80134C00`, `func_80136434`, `func_80137040`):

```c
if (D_800E6280.unk_110D == 0) {
    ((DisplayActor44 *)D_8011ECD0)[106].unk_18 = PAGE;   /* also unk_26 = -0xA0, unk_2A = -0x78, unk_03 = 0 */
    D_80120912[1] = 0x7FFF;
    D_800E6280.unk_110D = 1;
    D_80120912[0] = 0x7FFF;
} else {
    D_80120912[0] = D_80120912[0] / 17 * 16;             /* record 106 zooms in */
    D_80120912[1] = D_80120912[1] / 17 * 16;
    if (D_80120912[0] <= 0x1000) {
        D_80120912[1] = 0x1000;
        D_80120912[0] = 0x1000;
        for (i = 0, j = FIRST; i < N; i++, j += 2) {    /* rows and records 107.. */
            a[i] = X; b[i] = i * DY + Y0; c[i] = W; d[i] = 0x10;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_18 = j;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_03 |= 0x80;
            ((DisplayActor44 *)D_80120650)[i + 11].unk_2A = b[i];
            ((DisplayActor44 *)D_80120650)[i + 11].unk_26 = a[i];
        }
        func_8004F870(1, N, a, b, c, d);
        switch (setting) { ... the current choice's record unk_18++ and unk_1094 ... }
        func_8004284C();
    }
}
```

The `func_801373C8`/`func_801378DC`/`func_80138080` group zooms record 159 (`D_80121726`, `D_80121728`)
under `if (unk_110D != 0)` and dispatches on `unk_110D` afterwards (0: set up record 159, 1: hide records
107..110 and shrink record 106, else: close).

- With five rows IDO peels and unrolls the loop as the original; the stored value is `i * 2 + FIRST`. With
  three rows the original keeps the value in its own counter (`j`, as in `func_80134804`).
- Residuals: the stack offsets of the four arrays (the original keeps 8 to 16 bytes above them; `s32 i;`
  then an unused word before the arrays reproduced `func_80135E48`'s frame, marked `FAKE` like the
  T-4070 pad), and in the three-row loops the register order of the `j` counter (the original allocates it
  after the `w`/`h` row pointers; no counter spelling tried here moves it).
- `func_801344E4` increments records 14..17 of the base pool (`D_8011F0A0`, ...) inside its later loops; the
  original reloads them in every iteration, which IDO does only when the loop's records go through
  `D_8011ECD0` too (through `D_80120650` it keeps them in registers).

### Redraft

`drafts/T-20910/redraft.py`, variants `typed`, `typed-late`, `plain`, `archived`, `guarded` and `hand`, then
`draft_fixes` and `gs-local` on the best. Paired with the baseline: `tools/redraft.py` with its default
variants on the tree before T-20910 (`26d45f25`), each run in its own copy.

| | baseline | T-20910 |
| --- | --- | --- |
| sum of diff rows (32 functions) | 5,184 | 3,504 (-32%; 3,484 with the match at 0) |
| median | 156 | 97 |
| better / same / worse than baseline | | 12 / 20 / 0 |
| score 0 | none | none (`func_80135E48` at 20 matched by hand) |

The header types alone changed two variant scores: `func_801326BC` `typed` 134 to 130 (the slot view) and
`func_801328C0` `plain` 101 to 99, `typed` 101 to 126 (the fold of the BG labels). The gains are the hand
drafts in the template shape and `gs-local` on them: `func_80135E48` 364 to 20, `func_80133CB8` 252 to 48,
`func_8013554C` 229 to 31, `func_80136688` 160 to 46, `func_80133718` 139 to 50, `func_80136C30` 208 to 4,
`func_80133F50` 315 to 93, `func_801344E4` 281 to 127, `func_801328C0` 101 to 12, `func_801373C8` 248 to 193,
`func_801378DC` 206 to 203, `func_801326BC` 134 to 130.

### Matched

- `func_80135E48` (948 bytes): the template with five rows and the `GameState *g` local of the gs-local
  form; `s32 i; s32 pad;` before the arrays gives the original frame (0x88).

### Near misses

- `func_801328C0` (460, 12 funcdiff rows): the BG slot through the new types. Left: the original creates the
  cell pointer before the index pointer (`addu s0` then `addu s2`, increments `s1, s0, s2`); every index
  spelling tried (`[j + i * 4]`, `[i * 4 + j]`, row pointers, declaration order, a cell pointer local)
  either keeps the order or breaks the `&D_80122540[j]` exit test.
- `func_80139F8C` (132, guarded, 5 rows with `gs-local`): only the `move a0,t1` that puts the byte into the
  argument register before `ori`/`andi`; ten local and cast spellings give 6 to 39 rows.
- `func_80136C30` (780, 4 rows): the template with four rows, `s32 i; s32 pad[3];` before the arrays (frame),
  and `D_80120912[0] = 0x7FFF;` before `unk_110D = 1;` (found by `tools/autotweak.py`, 8 to 4 rows). Left: the
  original hoists `lui %hi(D_8012093F)` (record 107's `unk_03`) above `li 0x34` (its `unk_18`); four
  spellings of the stored value and two statement orders keep it, and a second autotweak run (1,456
  candidates) found nothing.
- `func_8013554C` (1,112, 31 rows): `GameState *g` with `s32 i; u8 t;` declared before the arrays.
- `func_80133718`, `func_80133CB8`, `func_80136688` (46 to 50): the stack offsets and the register order of
  the `j` counter; the layout probe (scalars or arrays first, no pad to `pad[3]`) gives 46 to 60.

