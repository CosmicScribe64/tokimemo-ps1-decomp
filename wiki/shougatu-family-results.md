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

## T-24052 sibling closure (2026-10-18, first checkpoint)

func_80133208 now matches488 literal linked bytes as unguarded default C, ported from GYOZI func_80135280 with the skip/reset block, five-step selector and0x28 byte postincrement bound. Current D_800B3778[girl][column] is the sound byte table. The sibling's unchanged-byte |=0 store is retained and explicitly FAKE/T-24052; no header/config changes. Fresh Docker ninja -j1 passed SHOUGATU checksum/headers and literal default linked proof. func_801333F0 remains in progress (selector no-op store), so [[tickets/T-24052-shougatu-dialogue-siblings]] stays In Progress at this checkpoint.

T-24052 final checkpoint: func_801333F0 also matches544 literal linked bytes as unguarded default C. Both target-specific unchanged halfword selector stores survive as plain self-assignment and are explicitly FAKE/T-24052. The byte-store negative from GYOZI does not transfer to this width. Compound no-ops, signed/unsigned views, locals, return liveness and volatile stores failed; assignment-in-condition got14 rows but retained one phantom temporary. Full source/proof/ledger in drafts/T-24052/README.md. Fresh production build passes checksum/headers and literal default proofs for both488/544 bytes, total1032. No header/config edits. [[tickets/T-24052-shougatu-dialogue-siblings]] now In Review for root review.

## T-24056 short sibling package, no promotion (2026-10-18)

Three source-only ports from matched GYOZI handlers are preserved in drafts/T-24056: D82062 rows, D92C72 rows, DF1412 rows. Canonical CharFlags.f9 reads/updates replace stale word/byte reconstructions; SHOUGATU's signed threshold halves and two-half failed-gate increments are preserved. D92C uses the sibling's >=1 comparison, restoring the initially missing instruction; a named-limit diagnostic is marked FAKE. Prior D82096-variant threshold/permuter exhaustion was read and not repeated; a return-liveness view adds no gain.

DF14 is the concrete near lead: target109/candidate107 words. Its initial byte load chooses an extra absolute LUI; its clamp forwards the earlier byte rather than the target's fresh read and absolute store address. GameState pointer/global clamp, explicit byte update, forced reload, postincrement, explicit assignment and register-pointer forms fail to recover both choices together (12/64/36/100 rows). Scratch-only D_80145FDC s16 declaration is supported by LH/SH evidence and approved conditionally by root, but no header change is made without a match. README records exact reproduction and negatives. Production/header/config unchanged; independent SHOUGATU checksum control passes through ASM. Zero new C bytes. [[tickets/T-24056-shougatu-short-siblings]] is In Review.

## T-24061 bounded short control/copy pass (2026-10-18)

SHOUGATU func_801359FC source semantics repaired: actual girl byte+F5F, word seed+10F8, choice narrowing only after decrement, and signed+11E halfword in both bonus arms. Word selector gives56 rows/exact69 words but frame0x58 versus0x50; aligned pointer storage and separate byte lifetimes are negative. No production/header change or C bytes. Full source/reproduction/negative ledger: drafts/T-24061/README.md and [[tickets/T-24061-short-control-and-copy]] (In Review).
