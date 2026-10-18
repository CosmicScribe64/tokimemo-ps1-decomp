---
type: research
updated: 2026-10-17
sources: ["include/ovl/SHOUGATU.h", "drafts/T-17500/README.md", "drafts/T-17500/redraft.py", "tools/type_recovery.py", "tools/siblings.py"]
---

# SHOUGATU family results

SHOUGATU (the New Year event overlay) shares its scene code with GYOZI; the earlier GYOZI and SHOUGATU
transfer work is in [[event-family-results]].

## Types-first pass (T-17500)

T-20900 update: `ShoSprite44` and `ShoCounter44` are `DisplayActor44` ([[data-types]], "Shared-type unification").

Wave-3 types-first pass ([[tickets/T-17500-types-first-shougatu]], [[speedup-research-2026-10-16]]), branch
`w3-shougatu-types` from `w2-good`. Start: 38 unmatched functions (21,124 bytes), 2 of them guarded. About 55
minutes of agent time for the survey, the types, the redraft and four matches.

### Survey

- `tools/type_recovery.py --unit SHOUGATU`: as in DATE ([[date-family-results]]), the top "high" proposals are
  0xC-stride arrays (`D_80143CB8`, `D_80146164`, `D_80144E18`, ...). They are the three-word loops of step-table
  copies, not data records.
- Census drafts (`drafts/T-14500/SHOUGATU`): the byte-offset casts sit on the `D_8011ECD0` display records (35 of
  them; `D_80120650`..`D_80120760` are records 96 to 99), on old `GameState` names (`D_800E71DF` is `unk_F5F`, in
  21 of the 38 drafts) and on the stack copies of constant tables (`M2cBlockNN`, flagged in 9 drafts).
- The data that the unmatched functions read, by label size (`dlabel`/`enddlabel`): four step tables of unmatched
  handlers, six 14-byte halfword tables, two 35-byte string tables, a colour table and a byte table, and the
  zero-filled work arrays of the `func_80141D20` family. The runs of `s32` scalars (`D_80143A44`..`D_80143AFC`,
  `D_801464B0`..`D_8014650C`, `D_80143DB0`..`D_80143DD4`) stay scalars: the matched GYOZI twins read them one by one.
- `tools/siblings.py --unit SHOUGATU`: `func_80132A34` and `func_80133760` have matched GYOZI twins at 0.964
  (`func_80134938`, `func_80135518`); `func_8013AFEC` has SHOUGATU `func_80137E54` at 0.910; `func_80133208` and
  `func_801333F0` form one unmatched cluster (0.946).

### Type map

All changes keep SHOUGATU's sha1.

| Type | Symbols | Evidence |
| --- | --- | --- |
| `FnTblNN` (`void (*f[NN])()`), 26 sizes from 10 to 62 | 49 step tables, `D_80143B28`..`D_80146970` | The local typedefs and externs of 21 `.c` files moved to `include/ovl/SHOUGATU.h` (as DATE did); four new: `D_80144FB4` (37), `D_8014616C` (27), `D_80146804` (40), `D_80146970` (48), the tables of the unmatched handlers `func_80138240`, `func_8013F224`, `func_801419D0`, `func_801430A0` (copy sizes 148, 108, 160, 192 bytes). |
| `ShoShorts7` (`s16 v[7]`, 0xE) | `D_801460CC`, `D_801460DC`, `D_801460EC`, `D_801460FC`, `D_8014610C`, `D_8014611C` | copied as `lw lw lw lhu` by `func_8013E50C`, `func_8013E6E4`, `func_8013EA98` (two each); values {0x46, 0x80, ..., 0x3E8} and {0, 3, 5, 7, 9, 0xB, 0x10} |
| `ShoStr7x5` (`u8 s[7][5]`, 0x23) | `D_80143E5C`, `D_80143E80` | 35-byte copy ending in `lwl`/`swl`; `func_801359FC` passes `str.s[i]` (`i * 5`) to `func_800AE0F0` |
| `ShoColors6x2` (`u32 v[6][2]`, 0x30), `ShoBytes3x12` (`u8 v[3][12]`, 0x24) | `D_801461D8`, `D_80146208` | both copied by `func_80140314`; the colour pairs go by value to `func_80140510`, the bytes are read `[D_800E62BE][...]` |
| `s32 [50]` x3, `s16 [50]` | `D_80146540`, `D_80146608`, `D_801466D0`, `D_80146798` | zero-filled work arrays of `func_80141D20`/`func_8014288C`/`func_801421D4`/`func_80142DA0`, counter `D_801467FC < 0x32`. `D_8014653C` stays a separate `u8` (matched code reads it with `lbu`/`sb`); the `sw` at `%lo(D_8014653C) + i * 4` is `D_80146540[i - 1]`. |
| `#include "game.h"` in `SHOUGATU.h` | | 24 of the 27 files did not include it, so every draft that wrote `DisplayActor44` failed to compile there |
| `func_80137664` prototype, `extern u8 D_800CA248[]` (main_api.h) | | called before its definition; the main text buffer of `func_801359FC` |

Not changed: the resource-address scalars `D_801464B0`..`D_8014650C` (two 0x34-byte records by layout, but the only
readers are unmatched and pass the fields one by one); the function-local rodata strings of `func_801350F8` and
`func_80133B44` (`D_80143640`.., `D_801435F8`), which belong in the functions.

### Redraft

`drafts/T-17500/redraft.py` is the T-17200 driver (plain and typed runs, fold, display records, stack tables,
`fptab`, record temporaries, `tools/draft_fixes.py`) with the `census` variant of T-17100, and two additions: the
stack-table rewrite also turns m2c's `&sp40[0]` and `((s16 *)sp30)[i]` into `sp40.v` and `sp30.v[i]`, and the census
variant moves a typedef of the census declarations into the function body. Numbers (36 unguarded functions):

| | value |
| --- | --- |
| plain (headers only) compiles | 28 of 36; better than the census in 4, worse in 1 |
| typed (fold + rewrites) compiles | 26 of 36; better than plain and census in 12 |
| census + rewrites compiles | 33 of 36; better than the census in 9 |
| chosen variant (32 still unmatched) | typed 13, census 10, plain 9 |
| against the census (32 still unmatched) | 15 better (10,560 bytes), 16 the same, 1 worse (`func_8013737C`, 194 to 202: the T-16510 body stays) |
| sum of diff rows | 5,854 to 5,567; median 111.5 both |
| score 0 | `func_801419D0` |

Largest gains: `func_8013E6E4` 375 to 317, `func_8013EA98` 385 to 334, `func_80134CB4` 101 to 53,
`func_8013E50C` 215 to 167, `func_80136204` 103 to 67 (stack tables and display records). The fold's lossy cases:
an indexed `GameState` array (`lw %lo(D_800E6448)(t9)` with the index in `t9`) loses its offset, and a `lui` shared
by two `%lo` gives `D_800E6280.unk0`; the census variant covers those functions.

### Matched during the pass (4 functions, 1,516 bytes)

- `func_801419D0` (240): the typed draft as it is: `FnTbl40 tbl = D_80146804`, `s32 idx` declared before the table
  (T-3330), record 96 through `((DisplayActor44 *)D_8011ECD0)[96]`.
- `func_80132A34` (704): port of GYOZI `func_80134938`; `switch (unk_F5F)` with `break` per case. The masked
  `get_g_zyotai_s(6) & 0x7F` goes through a `u8` local: compared inline it lands one temporary early (`t8` against
  the original's `t9`) and moves every later case's registers by one.
- `func_80133760` (276): port of GYOZI `func_80135518`; the flag is `unk_1BC[i].unk_0C.f.b3` (`sll 28; bltz`), and
  `func_800847B8` takes the full word at this call (`MAIN_API_OVERRIDE_func_800847B8` in `801330C0.c`, as in
  `80134120.c`); with the `u8` prototype the girl index moves from `a0` to `v0`.
- `func_8013AFEC` (296): typed draft plus the idioms of the matched sibling `func_80137E54`: the Rec38 flag set as
  a `ShouByteFlags` bit-field store (`|= 8` takes one temporary too few) and
  `*(u8 *)&unk_75D = *(u8 *)&unk_F5F = 0xA; func_800847B8(*(u8 *)&unk_F5F);` for the `li a0` shared by both stores
  and the call.

Tried and left: `func_801359FC` with `ShoStr7x5` (frame 0x58 against 0x50, the selector computed in other
registers); `func_801430A0`, the `func_801419D0` shape with a 48-entry table, where the original sets `v0 = 0`
after resetting the counter although the function's callees are `void`.

### Packages

Seven packages for wave 3, T-17501..T-17507 (`w3-shougatu-p01`..`p07`), 33 functions, 18,404 bytes, built with the
wave-3 inventory scripts; `func_8014288C` (1,300 bytes, 506 rows) is not in a package.

## T-24026 high-similarity sibling ports (2026-10-17)

Two unguarded C matches in src/ovl/SHOUGATU/8013F190.c add 708 bytes: func_8013F224 (316 B), from GYOZI func_80141944/T-20006, and func_80140510 (392 B), from DATE func_80152E34/T-20600. Existing FnTbl27, DisplayActor44 and GameState views suffice; the SDK POLY_G3 is reused. The callback retains the sibling fall-through s32 return, narrow counter increment and fresh selector reads. The builder retains the sibling colour comma lines and explicit masks, now explicitly marked FAKE. A local void forward declaration is needed for func_80140698; no shared headers changed.

func_80138240 remains guarded: typed FnTbl37 port matches all opcode/register choices but four stack frame/table-offset words (208/56 versus target 200/48). Source-only evidence and exhausted alternatives are in drafts/T-24026; avoid repeating return type, index declaration order, narrow index and direct-index variants. Default assembly fallback is retained, and no C win is claimed for it.

The full SHOUGATU checksum passes. Both claimed wins pass the source gate (no target INCLUDE_ASM) and normal linked literal proof with --resolve --no-build --version default. [[tickets/T-24026-shougatu-sibling-ports]] awaits independent review.
