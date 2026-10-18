---
type: research
updated: 2026-10-17
---

# BUNKASAI family results

BUNKASAI is the school-festival overlay; its early family work with BUNKAKEN (T-12700) is on
[[bunka-family-results]]. This page holds the types-first pass of wave 3.

## Types-first pass (T-18700)

T-20900 update: `BunkaReturnFlags` and `BunkasaiModeFlags` are `Flags7D4F` (main_api.h; `mode` is `unk_2_4`); `BunkSelectWords` and `BunkModePair` are `BunkaDispatch8` (game.h) ([[data-types]], "Shared-type unification").

[[tickets/T-18700-types-first-bunkasai]], branch `w3-bunkasai-types` from `w2-good`. Method of
[[speedup-research-2026-10-16]]; driver and drafts in `drafts/T-18700/` (README there). At the start 11
functions were unmatched (12,492 bytes, census sizes), one of them guarded (`func_80151364`, 2 lines). Two
matched during the pass (2,376 bytes): `func_80147F8C` and `func_8015AF90`. The other nine are cut into three
packages. Agent time: about 45 minutes for the types, the redraft and the two matches.

### Survey

- `tools/siblings.py --unit BUNKASAI`: `func_8015AF90` (2,200 B) has the matched BUNKAKEN clone
  `func_80152680` (0.976); `func_8013B25C` is close to `func_80135914` (0.867) and `func_80151364` to
  `func_801537BC` (0.822); the rest are below 0.80 or have no sibling (`func_80142700`, `func_80142BE8`,
  `func_801433C0`, `func_801509A4`, `func_80150B7C`).
- `tools/type_recovery.py --unit BUNKASAI`: mostly jump tables. Its 0xC-stride "arrays" over `D_80160808`,
  `D_80160A9C`, `D_8016052C` and `D_801606D4` are artefacts: they span the 165-entry dispatch table
  `D_80160810` (`FnTbl165` in `80150930.c`), the per-girl byte tables and the name table below.
- Census drafts: few byte-offset casts (12 in `func_80142700`, Rec38 fields read through a walking pointer;
  11 in `func_8015AF90`). The distance came from main-exe GameState fields read through splat names and from
  stack tables typed as the wrong shape (`ByteTable297`, `UnalignedBytes11`, `RepairBytes6`/`RepairBytes78`,
  an m2c `M2cBlock2C`).

### Type map (`include/ovl/BUNKASAI.h`)

All additions keep BUNKASAI's sha1; no existing declaration changed.

| Type | Symbol(s) | Replaces | Evidence |
|---|---|---|---|
| `BunkasaiNameTable` (`u8 s[3][3][33]`) | `D_801606DC` | undeclared (`ByteTable297` in the census) | `func_80147F8C` copies 0x129 bytes to the stack and indexes `year * 99 + D_80122CDC * 33 - 95 * 99`; DATE2 `func_80137A2C` form |
| `BunkasaiTexts12`, `BunkasaiTexts11`, `BunkasaiTexts4` (`u8 *text[N]`) | `D_80160AE0`, `D_80160B10`, `D_80160B3C` | undeclared | 12, 11 and 4 string pointers; copied to the stack by `func_8015AF90` as BUNKAKEN copies `D_80156470`.. |
| `u8` | `D_80160B4C` | undeclared | frame counter of `func_8015AF90` (`lbu`/`sb`) |
| `BunkasaiGirlBytes` (`u8 v[11]`) | `D_80160520` | `UnalignedBytes11` | one byte (0..2) per girl, copied to the stack by `func_80142700` |
| `BunkasaiColumns` (`u8 v[6]`) | `D_8016052C` | `RepairBytes6` | `lw` + `lhu` copy; indexed by `D_800E6280.unk_0F4.h >> 12` |
| `BunkasaiGirlColumns` (`u8 v[13][6]`) | `D_80160534`, `D_80160584`, `D_801605D4` | `RepairBytes78` | three 0x4E copies in `func_80142BE8`, one per school year; rows stepped by 6 |
| `BunkasaiGirlList` (`s32 v[11]`) | `D_80160AA4` | `M2cBlock2C` | eleven words of 0x80 copied to `sp+0x30` by `func_801509A4`, then filled with girl indices |
| prototypes | `func_80147F8C`, `s32 func_8015B828` | implicit | called before their definitions |

Left alone: `D_80160640`..`D_801606D8` (39 pointer words set one by one in `func_80146FA0` and read through
`D_800CA160`..; no reader indexes them), `D_80160810` (declared in its `.c` file), and the three views of
`D_80122EB8` (`BunkSelectWords`, `BunkModePair`, `BunkaDispatch8`) and two of `D_800E7D4F`
(`BunkasaiModeFlags`, `BunkaReturnFlags`): matched code uses each, and no draft needs them merged.

### Idioms found on the way

- An implicit `int` callee decides a v0/v1 pair far from the call. The BUNKAKEN port of `func_8015AF90` had
  identical instructions but swapped v0 and v1 between the `player_blood_types` base (`sp+0x3C`, live across
  the switch) and the uopt-promoted `D_80122EBC`. Struct views of `D_80122EBC`, dropping `GameState *g` per
  case, self-stores, pointer locals and a 7-minute permuter run did not move it. Bisecting in BUNKAKEN's own
  file found it: BUNKAKEN calls `func_80132548` undeclared; giving that call a `void` prototype swaps the same
  pair. Calling `func_80132410` undeclared in BUNKASAI matched (comment in `8015AF90.c`). Row added to
  [[register-cheatsheet]].
- IDO writes the promoted `D_80122EBC` back in the else arms of cases 8 and 12 (`sw v1,%lo(D_80122EBC)` with
  no store in the source). The BUNKAKEN clone shows the same; a self-assignment in C compiles to nothing.
- `func_800847B8(D_80122CDC)` passes the full word: the file uses BUNKAKEN's
  `MAIN_API_OVERRIDE_func_800847B8` and an unprototyped declaration (T-9960).
- `func_80147F8C`: the stack copy of the name table sits 4 bytes lower than a plain copy gives. A named
  `u8 *name` local for the strcpy source adds the slot without a FAKE (DATE2's clone uses a FAKE pad).
- `func_80142BE8` (hand draft, not matched): the target switches on the school year with three copies of the
  girl loop, not one loop over a chosen table; `(u8)(x >> 1)` gives its `andi`. Still 8 instructions short
  (the target indexes `unk_1BC[D_80122EBC]` with `multu` by a register 0x38) and three stack slots above the
  selector are missing (the amount is read uninitialized at `sp+0x11C` in the default arm).

### Redraft

`drafts/T-18700/redraft.py` is the T-18400 driver with BUNKASAI paths; `drafts/T-18700/hand.py` scores the
hand drafts.

| | value |
|---|---|
| functions redrafted | 10 (12,300 bytes, census sizes) |
| plain (headers only) | 8 compile, none better than the census (whose scores include the T-16510 and T-16100 improvements and hand repairs); `func_80142700` and `func_80142BE8` fail (m2c invents stack locals inside the new tables) |
| typed (fold and rewrites) | 9 compile; best variant for `func_80142700` (336 to 278), `func_80150B7C` (268 to 227) and `func_80158764` (342 to 332 after `draft_fixes`) |
| best per function (with hand drafts) | 4 better than the census (5,960 bytes, `func_8015AF90` 400 to 21), 3 the same, 3 worse (`func_80142BE8`, `func_801509A4`, `func_80151364`: their census starting points stay valid) |
| sum / median of scores | 2,576 to 2,453 / 302 to 246 |
| score 0 | none; `func_8015AF90` matched after the declaration fix |

As in the pilots, the header alone changed no score; the gains came from the typed rewrites and from the
sibling port.

### Packages

Three file-disjoint packages from the wave-3 scripts (lists in the orchestrator's scratchpad,
`w3-bunkasai-p01-list.txt` to `w3-bunkasai-p03-list.txt`):

| Package | Ticket | Owned C files | Functions | Bytes |
|---|---|---|---:|---:|
| w3-bunkasai-p01 | T-18701 | `801511E0.c`, `8013B190.c`, `80150930.c` | 4 | 3,604 |
| w3-bunkasai-p02 | T-18702 | `80154018.c`, `801354B0.c` | 2 | 3,468 |
| w3-bunkasai-p03 | T-18703 | `80142700.c` | 3 | 3,700 |

Starting points: `func_801509A4` (T-16100 draft, 59), `func_80151364` (guarded body in `src`, 2 lines),
`func_8013B25C` (T-16510 draft, 122: same instructions, the target keeps the case 5 and 14 constants and
`D_800CA148` in v0/v1 where the draft uses t registers; undeclared callees did not change it),
`func_80150B7C`, `func_80158764` and `func_80142700` (T-18700 typed drafts), `func_80142BE8` (census hand
repair 445, or the T-18700 hand draft for the switch shape).

## T-24016 state-machine follow-up

`func_80135C4C` matched in default unguarded C (1,508 literal bytes). Starting T-16510 draft: 264 printable diff rows. Replacing the redundant dialogue temporary with direct `D_800CA148` updates reduced it to 130; accessing GameState through a local pointer reduced it to zero. The correctly indexed assessment value is `unk_66C[1].unk_00` (0x800E68F0). No FAKE or header change. Normal Docker overlay checksum and literal default linked funcdiff both pass. Second target `func_80158764` remains in progress under [[tickets/T-24016-bunkasai-state-machines]].
