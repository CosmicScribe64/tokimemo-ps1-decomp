---
type: research
updated: 2026-10-17
sources: ["include/ovl/DATE.h", "drafts/T-17200/README.md", "drafts/T-17200/redraft.py", "tools/type_recovery.py", "tools/siblings.py"]
---

# DATE family results

The DATE overlay (date scenes: one handler per scene step, each scene a `.c` file). Earlier DATE work is in
[[matching-notes]] (waves 2 and 3, T-2010, T-4060, T-11580) and [[data-types]] (`Rec38`, the load-hoist
retries). DATE2 is a different overlay ([[scenes-family-results]]).

## Types-first pass (T-17200)

T-20900 update: `DateParticleView44`, `DateMover44` and `DateMotion44` are `DisplayFixActor44`; `DateScene44`, `DisplayRecord44` and the local `DateWords44` are `DisplayActor44`; `DateFix32` is `Fix32`; `DateFlag17` and `DateFlags0F4` are `ScheduleFlags` ([[data-types]], "Shared-type unification").

Wave-3 types-first pass ([[tickets/T-17200-types-first-date]], [[speedup-research-2026-10-16]]), branch
`w3-date-types` from `w2-good`. Start: 49 unmatched functions (40,656 bytes), 15 of them guarded. About one
hour of agent time for the survey, the types and the redraft.

### Survey

- `tools/type_recovery.py --unit DATE`: the top 30 "high" proposals are 0xC-stride arrays. They are not data
  records: IDO copies a struct in a loop of three words, so every step-table copy reads as a stride-0xC walk.
  The other proposals with evidence: `D_8015E108` (0x104 bytes of resource addresses, 37 ordered pairs, used by
  50 functions of which one is unmatched), `D_8015D030` (0x54; really the tail of three 13-word tables
  `D_8015CFD0`, `D_8015D004`, `D_8015D038` that `func_801496C4` reads with constant indexes), `D_8015DDD0`
  (already the `SVECTOR` view of `func_8014AD9C`).
- Census drafts (`drafts/T-14500/DATE`): the byte-offset casts sit on m2c temporaries holding a `Rec24`
  record (`D_801217D0 + i * 0x24`) or a `D_8011ECD0` display record (`+ i * 0x44`, offsets 6531..6712 are
  records 96..98), on the stack copies of constant tables (`sp + (i * 4) + off`, flagged `M2cBlock` and
  synthesized stack locals), and on old `GameState` names (`D_800E738A` is `unk_110A`, `D_800E71DF` is
  `unk_F5F`).
- `tools/siblings.py --unit DATE`: `func_8014AD9C` is a clone of the matched TACO `func_80147488`;
  `func_8013B208` of EVENT `func_80117D80` (0.99); `func_80132000` of EVENT `func_8010FE10` (1.0, but DATE's
  copy starts with a one-`nop` pad, not portable, [[matching-notes]]); `func_80139834` of EVENT
  `func_80116AA8` (0.93); the step-table handlers have `func_80150ED4` (0.70-0.85). `func_8015A640`,
  `func_8015A7E0`, `func_8015A980` are one 0.99 cluster.

### Type map

All changes keep DATE's sha1; header only, no `src` view changed except the moved declarations.

| Type | Symbols | Evidence |
| --- | --- | --- |
| `FnTblNN` (`void (*f[NN])()`), 35 sizes from 15 to 104 | 68 step tables `D_8015BF50`..`D_80160BD0` | Each handler copies its table to the stack whole and calls `tbl.f[D_800E6280.unk_110A](0x80)` (T-3330). The 39 local typedefs (one per use, some repeated) and the externs of 16 `.c` files moved to `include/ovl/DATE.h`; 27 tables of unmatched handlers (21 of them `func_80144E20`'s, one per date spot) got declarations. `NN` is the run of code pointers in the data, equal to every existing declaration. |
| `DateBytes11x21` (`u8 v[11][21]`, 0xE7) | `D_8015B7A0` | `func_8013C410` copies 0xE4 + an `lwl` tail and reads `[unk_F5F][D_800CA2F8]` |
| `DateWords21` (`s32 v[21]`, 0x54) | `D_8015B888`, `D_8015B8DC`, `D_8015B930` | `func_8013EB8C` copies all three, reads `[D_800CA2F8]` with `lw` |
| `Tbl21` (existing, `u16 v[21]`) | `D_8015B984` | same function, `lhu` at `0x34 + 2 * D_800CA2F8` |
| `DateShorts8` (`s16 v[8]`, 0x10) | `D_8015F9B0`, `D_8015F9C0`, `D_8015FB1C`, `D_8015FB2C` | screen x and y lists copied by `func_801556D8` and `func_801560E0` |
| `DatePairs8` (`s16 v[8][2]`, 0x20) | `D_8015FB3C` | `func_801560E0` reads `lh 0x6A + 4 * i` |
| prototype `void func_8015A170(void)` | | needed by `func_80159FAC`'s match |

Not changed: `D_8015E108`..`D_8015E1C4` (one unmatched user; 50 matched functions would need rewriting),
the three 13-word tables at `D_8015CFD0` (no indexed access, their words are defined one by one in
`80149310.c`), `DateDispatch*` and `DateCallbacks39` (same shape as `FnTblNN`, kept to leave matched code
alone).

### Redraft

`drafts/T-17200/redraft.py` adapts the T-16560 driver (plain and typed runs through the census path,
`tools/draft_fixes.py`, finish) and adds the T-16550 fold plus:

- **stack-table**: m2c's `M2C_MEMCPY_ALIGNED(&spXX, &D_X, n)` or word-by-word copy of a header table is one
  assignment `spXX = D_X;` with `spXX` of the table's type (declared when m2c printed none), and the element
  reads m2c prints relative to an unnamed `sp` (`*(T *)((unsigned char *)(sp + (i * 4)) + (44))`) become
  `spXX.f[i]` / `spXX.v[i][1]`.
- **fptab**: the T-3330 idiom, `s32 idx;` above the table and `idx = ...; tbl.f[idx](0x80);`.
- **record-temp**: a temporary set to `&D_801217D0[i]` (or the byte form when m2c increments `i` between the
  product and the sum) is a `Rec24 *`, a display record a `DisplayActor44 *`; byte offsets on it become fields.
- **display-records**: `D_8011ECD0 + (i * 0x44)` and fixed offsets on the display view become records.

Results (scores are full-unit diff rows, as in the census):

| | value |
| --- | --- |
| unguarded functions | 34 (2 matched from their typed drafts, below) |
| compiling drafts of the other 32 | 28 (census: 32, some after manual repair) |
| better than the census | 7 functions, 8,468 bytes |
| same / worse | 16 / 5 |
| sum of diff rows | 7,498 to 7,230 |
| typed vs plain run | typed better in 13 of 35, worse in 5 |
| `tools/draft_fixes.py` | no score changed |
| score 0 | none |
| guarded functions (census = hand guard body) | fresh drafts worse in 11 of 12 |

Largest gains: `func_801556D8` 338 to 232 (stack tables plus the declaration m2c left out), `func_801462B0`
1492 to 1388, `func_8014718C` 747 to 672 (record-temp), `func_801560E0` 257 to 229, `func_80155004` 66 to 50,
`func_80159FAC` 55 to 48, `func_80158338` 39 to 24. Four drafts still do not compile (`func_8013C190`,
`func_8013FD6C`, `func_801449D4`, `func_80144E20`: calls through a pointer into the table copy and byte
arithmetic m2c leaves on `?` types).

Matched during the pass (1,760 bytes):

| Function | Bytes | How |
| --- | ---: | --- |
| `func_8014AD9C` | 492 | port of the TACO clone `func_80147488`: index `D_801227A0[arg0]` at each use, no saved record pointer (the old guard kept five stack rows) |
| `func_8013B208` | 640 | EVENT twin `func_80117D80`: `GameState *g` declared before the two stack shorts moves them to 0x2A/0x28; the twin's FAKE entry assignment (`sp2A = D_800CA2FC;`) fixes the first compare's operand order |
| `func_80159FAC` | 360 | typed draft rewritten in the `func_80150ED4` shape: `GameState *g = &D_800E6280;` above `FnTbl46 tbl;`, `tbl.f[g->unk_110A](0x80)` |
| `func_80158338` | 268 | the same shape from a 24-row typed draft |

### Idioms and near misses

- Step-table handlers: the scalar above the table is either `s32 idx` (no other code) or a
  `GameState *g = &D_800E6280` used for every field (handlers with the three-place condition). Both give the 4
  bytes above the table; the `g` form matched where the `idx` form did not.
- `func_80154118` (FnTbl15, 0x3C table): with `idx` or `g` the frame is 0x70 and the table at 0x30 (original
  0x68 / 0x28); with no scalar the frame is right and the table 4 bytes high. Not solved; the compare chain
  also has the CVT operand-order rows.
- `func_80155E40` (FnTbl45): two `s32` counters above the table give the original frame and table offset
  (two `s16` counters pack into one word); the compare chain registers remain (`hand.c`).
- `func_80155004` (FnTbl53, T-3330 "two words before"): `s16 i; u8 c; s32 idx;` above the table gives the
  frame; 49 rows of registers left (the colour local is `$a3` in the original, `hand.c`).
- `func_801518B8`: 28 rows, all one register: the record copy `[97] = [96]` after a call takes `t6` for its
  end pointer where the original has `t7` ([[register-cheatsheet]], struct copy after a call; `hand.c`).
- `func_80139834`: the EVENT twin's form (no `u8` local, dead `if ((s32)unk_110A) {}` before `+= 0xC`) fixes
  the first branch but the else branch's `unk_F5F` load then takes `t5` for `v0` (18 rows, worse than the
  guard's 7); left guarded.
- `func_801449D4` and `func_80144E20` (T-17207, `80141EE0.c`): neither matched. The first reaches the original's structure (71 rows) with the global as the table index and a `u8 sel` copy of `D_800CA2F8`, but the shape reserves one slot more under the table than the original; the second has the exact frame and table offsets (one `void (**base)()` under the 21 tables, one unused scalar above) and about 180 register rows from the colour order of `base`. Details: [[matching-notes]], "DATE `80141EE0.c`: the two step-table dispatchers (T-17207)".
- m2c on folded asm printed `D_800E6280.unk0` for a `%lo(D_800E6280+0x10F8)` paired with a bare `%hi`
  (`func_80155004`); the field is `unk_10F8`. Check fold output for `.unk0` on `GameState`.

### Packages

Eight file-disjoint packages from the orchestrator's `mklists3.py` (lists in its scratchpad,
`w3-date-p01-list.txt`..`w3-date-p08-list.txt`):

| Package | Ticket | Files | Functions | Bytes |
| --- | --- | --- | ---: | ---: |
| w3-date-p01 | T-17201 | `801461B0.c` | 2 | 6,320 |
| w3-date-p02 | T-17202 | `80132000.c` | 6 | 1,972 |
| w3-date-p03 | T-17203 | `80154260.c`, `80152FC0.c`, `8014CC40.c` | 4 | 1,432 |
| w3-date-p04 | T-17204 | `80151460.c` | 4 | 2,328 |
| w3-date-p05 | T-17205 | `80149310.c`, `801552B0.c` | 5 | 3,180 |
| w3-date-p06 | T-17206 | `8014B1F0.c`, `80156600.c` | 2 | 1,136 |
| w3-date-p07 | T-17207 | `80141EE0.c` | 2 | 4,464 (none matched) |
| w3-date-p08 | T-17208 | `8014A1B0.c`, `8015A640.c`, `8015A7E0.c`, `8015A980.c` | 5 | 2,636 |

### Wave 3 result: w3-date-p01 (T-17201)

`func_80147958` (4,044 bytes) matched in plain C; the old guard was four fade rows, fixed by comparing the `u8` local `value` instead of its masked copy (two FAKE entries stay from T-12100: the `s32` type with bare returns, the unused header view). `func_8014718C` (1,996 bytes) was rewritten by hand in the shape of `func_80147958` (the same movie player with 19 steps) and stays guarded at 26 differing lines, down from 672 rows; its declaration in `DATE.h` is now `s32`. Details and the two left-over register gaps: [[matching-notes]], "T-17201".
Result of w3-date-p03 ([[tickets/T-17203-w3-date-p03]]): 3 of 4 matched (`func_8014EF1C`, `func_80154118`, `func_8014DCCC`, 1,064 bytes); `func_80155004` is back to `INCLUDE_ASM` with a draft in `drafts/T-17203/` (46 lines, register names only). Notes in [[matching-notes]], section T-17203.
### Package w3-date-p02 (T-17202)

`80132000.c`, six guarded drafts, all 6 matched (1,972 bytes). The matcher pass matched `func_80139834`, `func_8013E6A4` and `func_80137DF4` (820 bytes). The Opus closer pass matched `func_801324DC`, `func_8013BEB8` and `func_80139664` (1,152 bytes). New idioms: the callee view `s32 func_80085A60();` (file override), `s32` without `return` for a flag read at the entry, the dead selector test in the else arm, the first mention of a promoted global in a long expression (`g * 0 +`, FAKE), the first chain's value kept in an `s32` local for stores placed after the next chain, and `return;` plus an element view for a byte that `$v0` carries at the exit. Details in [[matching-notes]] (T-17202) and [[data/t0018-cases]].

### w3-date-p04 (T-17204)

`80151460.c`, 4 functions (2,328 bytes): `func_80152060` matched (520 bytes: `DateWords2` stack table, a real element pointer local that moves the table 4 bytes, `s32` with no `return` so the last call's result stays in `$v0`); `func_80152268` matched (1,052 bytes: FAKE unread copies `j = v` keep the result variable live across the twelve `func_80152684` calls, `for` loop with a trailing `++i`); `func_801518B8` guarded at 14 rows (copy end pointer `t6` against `t7`, the old ugen-temporary gap); `func_80152E34` (POLY_G3 builder, colours of six uopt ranges) left as `INCLUDE_ASM` with a draft in `drafts/T-17204/DATE/`. Details: [[matching-notes]], section T-17204.

### Wave 3 result: w3-date-p08 (T-17208)

`func_8014AF88` (248 bytes) matched in plain C: the argument of `func_8009B3C0` and the loop calls is the pointer variable `dop` (`0x801F8000` plus steps), as in TACO `func_801471D0`. `func_8014A8FC` (1,140 bytes, a frame timeline with the light fade, the step loops and the ordering-table clear) was rewritten by hand and stays guarded at 34 differing lines, all of them register choices in the two light blocks (T-0018 row, same gap as TACO `func_80134A18`); two FAKE entries (byte view of `D_8012276C`, `s32` return with no `return`, and `func_8014A8FC` is `s32` in `DATE.h`). The three 416-byte rotation helpers stay `INCLUDE_ASM` (hand-written libgte, probe: 86 of 104 rows differ). Details: [[matching-notes]], "T-17208".
### Package w3-date-p05 (T-17205)

Result: 5 of 5 matched in plain C (`func_801496C4` 1,020 B, `func_801556D8` 744 B, `func_801560E0` 712 B, `func_80155E40` 484 B, and in the closer pass `func_80149F48` 176 B with `(s16)(D_8015D084 * 3) + D_8015D090`; 3,136 bytes). New in `DATE.h`: `DateFix32`, `DateMover44` (display records 98..105 as the eight-object scene uses them), `D_8015F9A0[8]` (state) and `D_8015F9A8[8]` (idle counters), `void func_801560E0(void);`, and `s32 func_80155E40(void);`. What fixed each function, and the register-cheatsheet rows it added, are in [[matching-notes]], "T-17205". The eight objects: `func_801556D8` sets them up, `func_801560E0` updates them and is called from the step-table handler `func_80155E40` when `unk_110A` is 0x11..0x13.

## T-24011 dispatcher pointer-view experiment (in progress)

The T-17207 small dispatcher draft compiled in isolation with `GameState *g` reads of `unk_110A` and `unk_03E/03F/040` fixes the shifted register allocation. The first promising source-only candidate has 23 added/removed resolved instruction rows (each replacement counts twice), including terminal padding; the T-17207 baseline has 115 by the same counter. It is not a verified match. The remaining table base is `sp+0x30`, original `sp+0x2C`, and the constant 16 is scheduled differently. Full translation-unit verification remains required.

T-24011 continuation: declaring the same recovered `g` pointer as `register` and retaining three unused pad words reproduces the small dispatcher frame and table offset exactly: 11 isolated resolved added/removed rows, including a terminal padding word. The remaining scheduling difference is the constant 16 hoisted before the selector-17 test rather than loaded separately on two paths. An `s32` no-return view did not change either dispatcher.

For `func_80144E20`, using `g->` for all GameState fields fixes the shifted allocation and preserves the exact frame and all 21 table offsets. A byte view of `D_8015BF44` then reduces isolated resolved rows from 426 (archived baseline under this counter) to 72. Remaining differences are missing redundant switch-arm instructions, resulting branch displacements, and symmetric compare operand order. Explicit empty default first/last and reversed source comparisons do not change 72; a pointer view of the switch selector worsens it to 88. The archived source also needs the missing `extern u8 D_8015BF48` declaration to compile.

### T-24011 retained checkpoint

Source-only bodies and the safe Docker harness are in `drafts/T-24011/`. Full-translation-unit resolved comparisons are 48 positional added/removed rows for func_801449D4 and 72 for func_80144E20. Isolated small-body 11 uses sequence alignment and includes terminal padding, so it is not interchangeable with 48. A bounded Astra follow-up found no improvement in 17 more probes. Its constant-local pre-as1 listing places 16 at the fallback entry, but the assembled object hoists it before the selector-17 test: this observed movement occurs in as1, without establishing an original assembler-version mismatch. `drafts/T-24011/stage.py` retains a source-only reproducer. No per-function compiler exceptions were added.

Production DATE checksum passes, headers and global migration guards pass, branch prototype guard reports zero new disagreements (15 known), and explicit default resolved linked comparisons match all three assembly fallbacks (1100, 3200, 416 bytes). These checks verify fallback integrity, not new C matches: accepted new bytes remain zero. T-24011 awaits review. Exploration outputs moved to ignored `build/date24011-scratch/`; the shared full-unit source is `build/date24011-scratch/probes/dispatchers-fulltu.c` with adjacent per-function diff files.

T-24011 review correction: the archived harnesses now validate funcdiff return status and the named diagnostic verdict before printing any score. Objdump failures and tracebacks cannot become false zero-row success. Five Docker contract tests (12 cases) pass, full-unit diagnostics remain 48/72, and the stage reproducer passes two consecutive reruns at 11 after removing an IDO -S output-flag collision. Production code and matching claims are unchanged.
## T-24018 source-based movie sibling follow-up

Matched DATE2 func_80133AF0 gives a much closer structural starting point for DATE func_801462B0 than the census. Adding DATE's callback branches and 300-frame timer, with an early break in case 14, restores the exact 0x68 frame and 951 instructions; first 442 instructions exact, 434 printable diff rows remain. Callback-save arrays, direct stage/record views and s32 return diagnostics do not solve later allocation. No production change or new C bytes. Source-only draft and reproduction in drafts/T-24018/, [[tickets/T-24018-large-sibling-state-machines]] In Review.

## T-24032 SDK rotation clone control

[[tickets/T-24032-date-taco-rotation-clones]] performs one fresh SDK MATRIX control on DATE A640, retaining all six DATE/TACO rotation helpers as ASM (zero new coverage). Source/control/provenance archived in drafts/T-24032/. Existing SDK-style classification remains; no per-function toolchain exception or impossibility claim.
