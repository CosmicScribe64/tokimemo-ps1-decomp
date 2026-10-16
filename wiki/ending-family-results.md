---
type: research
updated: 2026-10-16
---

# ENDING family results

ENDING is the ending overlay (load address 0x80132000). Most of its code is scene scripts: one function per
ending scene, each a chain of `if (D_8013C35C == step)` arms that start messages (`func_801396A4`), switch the
two portraits (display records 30 and 31 of `D_8011ECD0`, read in matched code as the scalars `D_8011F4DE`,
`D_8011F4E0`, `D_8011F522` or as `*(s16 *)&D_8011ECD0[0x852]`) and count frames in `D_8013C360`. Around them
sit pointer-table initialisers (13- to 17-word location records, one function per scene), fade handlers on
display records 96 and 97, and two large dialogue functions (`func_80132420`, `func_8013A2E8`). Earlier
family work: [[tickets/T-14000-ending-en-nichi-families]], [[tickets/T-1040-overlay-batch-h-option-ending]].

## Types-first pass (T-18300)

[[tickets/T-18300-types-first-ending]] recovered ENDING's shared types before function matching, redrafted
the 13 unmatched functions (about 16 KB, 1 guarded) and cut them into five packages (branch
`w3-ending-types`, from `w2-good`). Drafts and drivers: `drafts/T-18300/` (README there). About 50 minutes
of agent time for types, redraft, one match and four hand drafts.

### Type map

| Type | Symbol(s) | Used by (unmatched) | Evidence |
| --- | --- | --- | --- |
| `EdShorts18` (`s16 v[18]`, 0x24) | `D_8013C3B0` | `func_801330E4` | copied whole to the stack (9 `lw/sw`), then read with `lh` at `sp30 + level * 2`; data is 18 rising halfwords (1 to 0x100), compared with `rand() % 256` |
| `EdShorts14` (`s16 v[14]`, 0x1C) | `D_8013CA60`, `D_8013CA7C` | `func_80135F4C` | both copied to the stack (7 `lw/sw`), read with `lh` at `sp + i * 2`: the x and y positions of the 14 sprites placed in display records 96 to 109 |
| `s32 **D_8013CAF0` (was `s32`) | | `func_8013A2E8` | read as `lw`, `lw 0(...)`, `lw k(...)`: a pointer to a block whose first word is a message pointer table; m2c now prints `(*D_8013CAF0)[k]`. Its matched initialiser in `801396E0.c` casts the address. |
| `dialogue_render` returns `s32` in ENDING (`MAIN_API_OVERRIDE_dialogue_render`) | | `func_8013A2E8` | the asm stores `v0` after the call into `D_80122CD0`, as main `k_disp_start` does (`src/main/8007C030.c` has the same override); no matched ENDING function calls it |
| `game.h` in every ENDING file, after `ovl/ENDING.h` | | all | the drafts read `DisplayActor44`, `Entry8` and `Rec24`; `ovl/ENDING.h` must come first so its `MAIN_API_OVERRIDE_` defines are seen before `main_api.h` |

All changes keep ENDING's sha1. Not changed, with the reason:

- **Location records.** `D_8013C280`, `D_8013C2C4`, `D_8013C308` and the run from `D_8013C5F0` to
  `D_8013C83C` are 0x30- to 0x44-byte records of TIM/TMD addresses (a halfword at +0xC). The pointer-table
  initialisers in `80134D20.c`, `80132000.c` and `80133C10.c` write them as scalars (about 300 matched
  lines), and the unmatched functions
  read them only at constant offsets. Same decision as SHUGAKU ([[shugaku-family-results]]).
- **type_recovery's 0xC-stride arrays** (`D_8013C368` x43, `D_8013CA34` x44): unrelated scalars that sit 12
  bytes apart, as in DATE and SHUGAKU.
- **The 27-byte string copy in `func_8013A2E8`** (`D_8013C0D0`, in `801396E0.c`'s rodata): an automatic
  string initialiser (`u8 s[] = "..."`, the T-15100 form) is the likely source; it needs the literal, not a
  data symbol, so the header does not declare it. m2c prints `M2C_LWL` for the copy, so the redraft of
  `func_8013A2E8` does not compile; the census repair is kept as its hand draft.

### Idioms found on the way

- **`check_k_scroll` port** (`func_80132F18`, matched): `tools/siblings.py` found main `check_k_scroll` at
  0.978. The port needs the `Entry8` view of `get_k_work`, an unsigned compare `(u32) v == 0x48U` for the
  operand order of the `bne`, and an unused `sp40 = get_k_work(...)` local (FAKE): without it the frame is 8
  bytes smaller and both spill homes move.
- **Display records 96 and 97 through the view** (`func_80134B18`, guarded): writing the fade fields as
  `((DisplayActor44 *)D_8011ECD0)[96].unk_07` puts the promoted byte in `a1` as in the original, but then the
  GameState byte `unk_036` moves from `v1` to `v0`; still 9 lines, the guard is unchanged.
- **Flag times two** (`func_80137E40`): the original computes `(b14 != 0) * 2` as `sltiu; xori`, which
  `(b == 0) ^ 1` reproduces; `!= 0` and `!!b` give `sltu`, a ternary gives branches. Registers still differ
  (the original keeps the word in `a0` and the shifted word in `v0`).
- **Scene scripts** (`func_80137BB4`, `func_801387B0`): written like the matched siblings in `80137340.c`
  (scalar portrait fields, `*(s16 *)&D_8011ECD0[0x852]`, the T-9410 dead test before `% 240`) they are 58 and
  156 rows from a match. What is left is the register of the promoted `D_8011F4DE` (`a1` in the original) and
  of the shared constant `1`.

### Redraft

`drafts/T-18300/redraft.py` (from `drafts/T-18000/redraft.py`) scores two drafts per function through the
T-14500 census path: `plain` (m2c with the new headers) and `typed` (fold of GameState, display and Rec24
labels, stack tables, record temporaries, plus two new rewrites: a stack-table index scaled into its own
temporary, and `struct-bytes`, which turns m2c's `&D_800E6280 + i` into `(u8 *)&D_800E6280 + i`), then
`tools/draft_fixes.py`. `drafts/T-18300/hand.py` scores the hand drafts the same way.

| | value |
|---|---|
| functions | 13 (12 unguarded), after the `func_80132F18` match |
| compiling drafts | plain 12, typed 12 (`func_8013A2E8` neither: `M2C_LWL`) |
| better than the census (unguarded) | 7 functions, 7,268 bytes (4 of them from hand drafts) |
| same / worse (unguarded) | 5 / 0 |
| median, census to new (unguarded) | 161.5 to 142.5 rows; sum 4,250 to 4,113 |
| headers alone (plain vs census) | 3 of 11 scores changed, 2 better (`func_80132420` 364 to 336 from the `dialogue_render` override) |
| typed vs plain | 3 better, 5 worse, 4 the same |
| new score 0 | none |
| matched | `func_80132F18` (280 bytes), a sibling port, before the redraft |

As in the earlier passes, the types moved few drafts. The fold rewrites made two drafts worse
(`func_80134270` 501 to 890, from a shared-`%hi` store that became a pointer temporary, and `func_80135F4C`).
The driver keeps the better variant for each function, so neither loss reaches the packages. The useful
gains came from the sibling port and from hand drafts that follow the matched scene scripts.

### Packages

`inventory2.py` and `mklists3.py` cut the 13 functions into five file-disjoint packages: T-18301
`w3-ending-p01` (3 functions, 3,328 bytes: `80133030.c`, `80132000.c`), T-18302 `w3-ending-p02` (4, 2,044
bytes: `80137340.c`, the four scene scripts with hand drafts), T-18303 `w3-ending-p03` (3, 2,352 bytes:
`80133C10.c`, with the guarded `func_80134B18`), T-18304 `w3-ending-p04` (2, 5,248 bytes: `801396E0.c`),
T-18305 `w3-ending-p05` (1, 3,268 bytes: `80134D20.c`, `func_80135F4C`).
