---
type: research
updated: 2026-10-17
---

# KANGEI family results

KANGEI is an overlay at load address 0x80132000 (`src/ovl/KANGEI/`, `include/ovl/KANGEI.h`). The club name
strings in `func_801327DC` and the name tables of `func_8013769C` suggest a club welcome scene (hypothesis
from the overlay name and those strings). Most of its
code is call sequences and scene switches on `D_800E6280.unk_F5F` (the current girl) and
`D_800E6280.unk_0F4.h >> 12` (the club). Picture sets are three pointers into loaded data; the scene code
copies one set into `D_80139AD0`, `D_80139AD4` and `D_80139AD8`. Its data shares layouts with SHUGAKU (the
`KangeiWords11`/`KangeiWords2` tables) and MASTER (the falling streak effect).

## Types-first pass (T-18400)

T-20900 update: `KangeiSprite44` is `DisplayFixActor44` (`unk_26` is `unk_24.h[1]`), `KangeiFixed32` is `Fix32`, `KgSlot` is `ScheduleClubFlags` (`grade` is `unk_25`) ([[data-types]], "Shared-type unification").

[[tickets/T-18400-types-first-kangei]], branch `w3-kangei-types` from `w2-good`. At the start 13 functions
were unmatched (9,184 bytes, census sizes), none of them guarded. Four matched during the pass (2,596 bytes):
`func_801370A0`, `func_80132C24`, `func_80135838`, `func_80136630`. The other nine are cut into three
packages. Agent time: about one hour for the types, the redraft and the four matches.

### Type map

All conversions keep KANGEI's sha1. Header: `include/ovl/KANGEI.h`.

| Type | Symbol(s) | Replaces | Evidence |
|---|---|---|---|
| `s32 D_80139950[3][8]`, `s32 D_80139A50[3]`, `s32 D_80139A5C[3][8]`, `s32 D_8013A170[2][3][13]` | picture-set tables | 129 `s32` scalars | the setters fill them in row order; every reader copies element `[k]` of rows 0, 1 and 2 (or 3, 4 and 5) into `D_80139AD0/AD4/AD8` |
| `KangeiSprite44` extended to the MASTER layout (`unk_08`, `unk_16`, `unk_18`, `unk_26`, `KangeiFixed32 unk_28`) | display records (`D_8011ECD0`, record 96 as `D_80120650`) | three fields and pads | `func_801370A0` is the MASTER clone `func_80137BF4` (same opcodes) |
| `KangeiPoint2 D_80139F10[10]`, `s16 D_80139F38[10]` | streak positions and speeds | undeclared | MASTER's `D_8013C58C`/`D_8013C5B4` |
| `KangeiWords11 D_80139A08`, `KangeiWords2 D_80139A34`, `D_80139A3C`, `D_80139A44` | stack tables of `func_80132C24` | undeclared | the `func_8013454C` tables at `D_80139BA8`.. |
| `KangeiNames20 D_80139FAC` (`u8 name[11][20]`), `KangeiNames21 D_8013A088` (`u8 name[11][21]`) | club name tables | undeclared | copied to the stack whole by `func_8013769C` (lw/sw loop, three-byte lwl/lwr tail as EVENT's `EventTableE7`); `name[club]` goes to `func_800AE0F0` (strcpy) |
| `KangeiImage10 D_80139D10[2]` | image records | eight scalars | `func_80135CE0` copies `unk_00`, `unk_04`, `unk_08`, `unk_0C` into display record 96 (+0x34, +0x0C, +0x10, +0x14) |
| `u8 D_80139E00`, `D_80139E04`, `D_80139E08` | fade colour | undeclared | byte loads and stores one word apart |
| prototypes | 12 overlay callees (11 called by `func_80132FF8`, plus `func_80136810`) | implicit | called before their definitions |

Left alone: `D_80139DCC`..`D_80139DEC` (pairs for display records 96 and 97 interleaved field by field, with
two `s16` one word apart; no clean struct), `D_80139DB0`..`D_80139DC8` (palette and six image pointers, read
only by matched code), `D_80139AD0`/`AD4`/`AD8` (the current set; matched code treats them as three objects).
`tools/type_recovery.py`'s 0xC-stride arrays at `D_801399A8` and `D_80139F44` span several of the tables
above and are not real.

### Idioms found on the way

- `tools/siblings.py --unit KANGEI` finds the two quick ports: `func_801370A0` is MASTER `func_80137BF4` with
  two other table symbols (ratio 1.000), `func_80132C24` is `func_8013454C` (0.925) with a month check
  (`D_800E6280.unk_03F < 6 || >= 10`) that overrides six table entries. Both matched on the first build.
- `func_80136630` (fade): the ugen temporaries follow the source order. The draft's `D_80139E04 += 0x10;
  D_80139E08 += 0x10; D_80139E00 += 0x10;` puts the `andi` of `E00` in `$t0`; with `E00` first it is `$t6`
  as in the original. The state stores (`unk_1104.w = 2`, `= 1`) then only differ in where as1 schedules the
  `li v0,k`; moving each store in the source fixed it (62 to 18 to 0 funcdiff rows).
- `func_80135838`: the typed draft (86 rows) needed four m2c fixes (byte flag `(u8) D_80139ABC`, the
  GsRec0FC word `*(s32 *)&D_800E6280.unk_0FC /= 2`, the Rec38 word at +8, `back_clear_switch(1)` without
  m2c's leftover registers), then the two date checks swapped `$v0`/`$v1` until their fields were read through
  `GameState *g` (FAKE comment; the register-cheatsheet date-check row).
- The Rec38 flag at bit 1 of `unk_1BC[i].unk_0C` is read with `lw; sll 30; bgez`: `.f.b1`. Bit 19
  (`func_801327DC`, `sll 12; bgez`) has no one-bit member in `CharFlags`.
- Near misses (`hand.c` in `drafts/T-18400/KANGEI/<func>/`): `func_801362F0` 22 rows (`func_800AE0D0() % 2` as
  a truth value keeps the signed fixup; the table byte and the `D_80139DF8` switch value sit one register
  lower than in the original), `func_80135004` 33 (a `u8 girl` local gives the frame and RECT slot; the
  original keeps the constant 1 in `$v0` for two compares, which a `s32 one` local reproduces at the cost of
  8 frame bytes), `func_8013852C` 36 (the frame matches; the original keeps `unk_F5F` in `$v0` as a uopt range
  and the year in `$v1`; ten variants tried, including the `GameState *g` idiom).

### Redraft

`drafts/T-18400/redraft.py` is the T-18000 driver with KANGEI paths, a fold that reads multi-dimensional array
globals, and a new `gamestate-words` rewrite (m2c prints `D_800E6280.unk_100 = (s32) D_800E6280.unk_100 +
0x18000` for a GsRec0FC record, which IDO rejects). `drafts/T-18400/hand.py` scores the hand drafts.

| | value |
|---|---|
| functions redrafted | 11 (7,716 bytes) |
| plain (headers only) | 10 compile, all equal to the census |
| typed (fold and rewrites) | better in 4 (`func_80132FF8` 441 to 351, `func_80134724` 238 to 150, `func_80135838` 145 to 86, `func_80135004` 44 to 43), worse in 3 (`func_80133A54` 79 to 140, `func_801327DC` 266 to 284, `func_8013769C` 142 to 154) |
| best per function (with hand drafts) | 7 better than the census (4,448 bytes), 3 the same, 1 worse (`func_8013769C`) |
| sum / median of scores | 1,809 to 1,478 / 142 to 130 |
| score 0 | none; two matched after hand edits (`func_80135838`, `func_80136630`) |

As in the pilots, the header alone changed no score; the fold and the m2c-defect rewrites gave the gains.
The fold makes `func_80133A54` worse: it reads display record 96 as `((DisplayActor44 *)D_8011ECD0)[96]`
fields, and the plain draft's separate symbols compile closer.

### Packages

Three file-disjoint packages from the wave-3 scripts (lists in the orchestrator's scratchpad,
`w3-kangei-p01-list.txt` to `w3-kangei-p03-list.txt`):

| Package | Ticket | Owned C files | Functions | Bytes |
|---|---|---|---:|---:|
| w3-kangei-p01 | T-18401 | `80135F10.c`, `80134120.c` | 4 | 2,624 |
| w3-kangei-p02 | T-18402 | `80132E40.c`, `80132090.c` | 3 | 3,188 |
| w3-kangei-p03 | T-18403 | `80137B90.c` | 2 | 1,024 |
