---
type: research
updated: 2026-10-17
---

# SHUGAKU family results

SHUGAKU is the school-trip overlay (load address 0x80132000). Its code is a set of location scenes. Each scene
has a callback table that a dispatcher copies to the stack, and pointer tables filled at run time. Three
13-word location tables feed three "current" pointers, chosen by a switch on `D_800E6280.unk_F5F`.
It shares templates with GEKO, GYOZI and EVENT (siblings in `tools/siblings.py`).

## Types-first pass (T-18000)

T-20900 update: the copy of record 96 to 98 is a `DisplayActor44` assignment and its FAKE comment is gone; the byte views of `D_80120650` are record fields ([[data-types]], "Shared-type unification").

[[tickets/T-18000-types-first-shugaku]] recovered SHUGAKU's shared types before function matching, redrafted
the 25 unmatched functions (9,908 bytes, 3 guarded) and cut the rest into four packages (branch
`w3-shugaku-types`, from `w2-good`). Drafts and drivers: `drafts/T-18000/` (README there). About 60 minutes of
agent time for types, redraft and the five matches.

### Type map

| Type | Symbol(s) | Used by (unmatched) | Evidence |
| --- | --- | --- | --- |
| `ShFnTbl16/18/25/38/41/43/44/70` (callback tables, `void (*f[N])()`) | `D_8013B094`, `D_8013BF20`, `D_8013C008`, `D_8013C0EC`, `D_8013C1C4`, `D_8013C228`, `D_8013C884`, `D_8013C930`, `D_8013CAA0`, `D_8013CC20` | `func_80133E10` (`D_8013C008`, 44), `func_80134B94` (`D_8013C228`, 41) | the dispatchers copy each table whole to the stack (12-byte `lw/sw` loop plus a tail) and call one entry; eight were local `FnTblNN` typedefs in the `.c` files, now in `include/ovl/SHUGAKU.h` (block `OVL_SHUGAKU_FNTBL_T18000`) |
| `DisplayActor44` (game.h) | the three 0x44-byte copies between display records in `80133D60.c`, `80134880.c`, `80135430.c` | none | replaces local `Blk44`, `Tb44` and four duplicate `Words17` typedefs; same code |
| `u8 D_8013C2E4` (was `s8`) | | `func_80135480` | the only load in the overlay's asm is an `lbu`; the stores are `sb` |
| `void D_80085368(void)` in `main_api.h` | address of `func_80085368` | `func_80134B94`, `func_80133E10`, `func_80135480` | the callback tables are compared with it; the overlay's asm takes the address with `%hi/%lo`, so splat names it `D_80085368`, and the overlay link resolves only that name (`func_80085368` is undefined there). Precedent: `D_800896E0` |

All conversions keep SHUGAKU's sha1. Not changed, with the reason:

- **Location tables.** `D_8013AFD0`/`D_8013B004`/`D_8013B038` (current `D_8013B06C`/`70`/`74`, `80132000.c`),
  `D_8013BE70`/`D_8013BEA4`/`D_8013BED8` (current `D_8013BF0C`/`10`/`14`, `80133430.c`),
  `D_8013C420`/`D_8013C454`/`D_8013C488` (current `D_800CA2D0`/`D4`/`D8`, `80138A60.c`) are three 13-word tables
  each, and `D_8013C508`/`D_8013C644`/`D_8013C780` three 0x13C-byte tables (current `D_8013BFFC`/`C000`/`C004`).
  The asm reads them only at constant offsets (no stride walk), about 400 matched lines use the scalar names, and the
  GEKO and GYOZI clones match with scalars, so they stay scalars.
- **type_recovery's 0xC-stride arrays** (`D_8013C1BC` x50, `D_8013C87C` x45, `D_8013CA98` x32 and smaller): they
  span unrelated scalars (pointer tables, flags, callback tables) that happen to sit 12 bytes apart.
- **The `D_8013B06C`..`D_8013B090` scene block** (current pointers, saved copies, `s16` counters, `u8` offset):
  type_recovery proposes 0x24/0x54-byte structs from access order only; the guarded `func_80133038` might
  depend on it, untested.

### Idioms found on the way

- **Location-table switch** (`func_8013282C`, `func_801399F4`): one `break` per case, and the step-6 check as
  `if ((u8)(get_g_zyotai_s(6) & 0x7F) < 2)`. The m2c form `(u32)(... & 0x7F) < 2U` takes one temporary
  register less after the call and shifts every register of the rest of the function (188 rows to 0).
- **Callback-table dispatcher** (`func_80134B94`, EVENT `func_800FA6A0` idiom): `u8 index;` declared above the
  table copy (frame), `index = state; table.f[index](0x80); index = state;` and the compares on
  `table.f[index]`. Compare with `D_80085368`, not `func_80085368` (link).
- **One variable for two ranges** (`func_80139054`, guard of T-10850): the original keeps the selector in `a1`,
  because the same `u8` variable is reloaded and passed as the second argument of `func_80085B3C` later.
- **GEKO port with its FAKEs** (`func_80135630` from `func_8013B3C0`): the unused `s32 pad` above the RECT and
  the byte store of the state then member read are both needed here as well.
- Near misses: `func_80135480` (5 rows; the original keeps a dead `li v0,-0x81` after two masked stores),
  `func_80133E10` (13 rows; the byte of display record 96 sits in `a1` and is masked into `a0`).

### Redraft

`drafts/T-18000/redraft.py` (from `drafts/T-17700/redraft.py`) scores two drafts per function through the
T-14500 census path: `plain` (m2c with the new headers) and `typed` (fold of GameState, display and Rec24
labels, stack tables, `fptab`, record temporaries, and the new `rect-local` rewrite), then
`tools/draft_fixes.py`. `drafts/T-18000/hand.py` scores the hand drafts the same way.

| | value |
|---|---|
| functions | 25 (22 unguarded) |
| compiling drafts | plain 22, typed 24 (`func_80133E10` only by hand) |
| better than the census (unguarded) | 9 functions, 3,368 bytes |
| same / worse (unguarded) | 13 / 0 |
| median, census to new (unguarded) | 74 to 68 rows; sum 1,905 to 1,734 |
| headers alone (plain vs census) | 3 of 22 scores changed, 1 better |
| typed vs plain | 6 better, 5 worse |
| new score 0 | none |
| matched | 5 functions, 2,292 bytes: `func_8013282C` (804), `func_801399F4` (652), `func_80135630` (380), `func_80134B94` (260), `func_80139054` (196) |

As in the pilots, the types moved few drafts; the matches came from siblings (`tools/siblings.py`) and from
the callback-table idiom, which the header now makes visible to m2c.

### Packages

`inventory2.py` and `mklists3.py` cut the 20 remaining functions into four file-disjoint packages: T-18001
`w3-shugaku-p01` (5 functions, 2,328 bytes: `80133D60.c`, `80135430.c`, with both hand drafts), T-18002
`w3-shugaku-p02` (6, 2,316 bytes), T-18003 `w3-shugaku-p03` (5, 1,672 bytes), T-18004 `w3-shugaku-p04`
(4, 1,344 bytes).

## Exact history-copy clone (T-24017)

[[tickets/T-24017-clone-ports]] matched func_80133038 (176 literal linked bytes) in src/ovl/SHUGAKU/80132000.c by porting DATE func_80149F48. The two destination assignments, including the signed-halfword product narrowing, precede the last three pointer copies. This replaces the T-10510 guard with unguarded C; no INCLUDE_ASM remains. No data-island conversion, header change or FAKE needed. Fresh Docker ninja -j1 SHUGAKU checksum and default funcdiff --resolve --no-build pass. Existing pointer-type warnings in func_80132F44 / func_80132FC0 remain outside scope.
