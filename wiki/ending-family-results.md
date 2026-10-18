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

## Targeted type pass (T-20930)

[[tickets/T-20930-types-ending]] took a second look at the 13 functions T-18300 left unmatched (branch
`ty-ending`, from `w2-good`). The data tables became aggregates, the dialogue context got typed views, and
three functions matched on the way: `func_80132420` (1,764 bytes), `func_80132B04` (660) and `func_80139B48`
(476), 2,900 bytes in all. Drafts, the fold script and the hand-draft scorer are in `drafts/T-20930/`.

### Type map

| Type (include/ovl/ENDING.h) | Symbol | Evidence |
| --- | --- | --- |
| `s32 D_8013C280[3][17]`, `s32 D_8013CAC0[3][15]` | message scripts | the initialisers store one word per element in address order; a scene's three entries (row 0, 1, 2 at one column) are always copied together, into `D_800CA160/164/168` or `D_8013C34C` |
| `EdScriptSet` (3 words) | `D_8013C34C` | the running scene's text, kind and voice tables, copied whole to `D_800CA13C/140/144` (`func_80132D98`) |
| `EdScene30`, `EdScene44` (`s32` x3, `s16`, then texture-page words) | `D_8013C5F0`, `D_8013CBB0`; `D_8013C620[15]` | one initialiser per record; the first four fields are always passed or stored as (+0x04, +0x08, +0x00, +0x0C) to `func_80137160`, `func_800850D4` and a display record's `unk_0C`, `unk_10`, `unk_34`, `unk_14`; 15 records of 0x44 fill 0x8013C620 to `D_8013CA1C` |
| `EdPortraitTbl` (`unk_00`, `unk_38`, `unk_70`: `s32[14]`; `unk_A8`: `EdHalf4[14]`) | `D_8013C4C0` | the same four values as columns of 14 (`func_80134270` case k reads entry k + 1); `unk_A8` is zero data read with `lh` at a 4-byte stride |
| `s32 D_8013C5A0[7]` | texture pages | `func_80133C10` |
| `EdFnTbl6/9/13/14/16/25/43`, `EdWords4`, `EdShorts13` | the callback tables and two stack tables | moved from local typedefs in five C files (`FnTbl9` was defined twice) |
| `u8 D_8013C364` (was `s8`) | wait counter | 9 `lbu`, 8 `sb`, no `lb` in the original; three `(u8)` casts went away |
| `s32 func_80132B04(...)` (was `void`) | | returns 1 when a message is done (`li v0,1` / `move v0,zero` at the exits) |
| `s32 func_80132420();` (no prototype) | | the callee narrows its `u8`/`u16` parameters from the argument slots and its matched caller passes them unconverted |
| `set_kanji_string` override removed | | main_api.h's `(s16, s16, u8 col, u8 *, s32)` is the view the original used: with ENDING's old `s32 col` view the colour argument of `func_80132420` became a spilled register value |

The dialogue context of main (`D_800CA134` .. `D_800CA144`, `u8 *`/`s32` in main_api.h) is read through casts
in the ENDING code: `*(s16 *)D_800CA134` and `*(s16 *)D_800CA138` are the page and step halfwords (`D_8013C358`,
`D_8013C35C`), `((u8 ***)D_800CA13C)[page][step]` the text, `((u8 **)D_800CA140)[page][step]` the kind byte,
`D_800CA144` the voice table (`s32 ***`: `voices[page][step][0]` and `[1]` are the arguments of
`func_80046290`). main_api.h was not changed.

All changes keep the 27 hashes. 427 scalar declarations went away and 449 matched uses became element or field
accesses (`drafts/T-20930/fold_ending.py`, scoped to ENDING: `tools/migrate_globals.py` rewrites every source
file and overlay addresses repeat across overlays, so it is not used for overlay aggregates).

### Matches

- `func_80139B48` (476 B): a `POLY_G4` port of main `_sprite_set_box_shade_tarao` (T-17001) with one colour;
  it keeps the sibling's `FAKE` note (explicit `& 0xFF` masks, one source line per colour trio).
- `func_80132B04` (660 B): nested `if`s with `return 1` at the three "done" exits; the context is dereferenced
  inline at every use (`((u8 ***)D_800CA13C)[*(s16 *)D_800CA134][*(s16 *)D_800CA138]`). Locals for page and
  step changed the load order of the table pointers and swapped `v0`/`v1`.
- `func_80132420` (1,764 B): an old-style definition, `s32 ***voices`, the wait counter as
  `if (D_8013C364++ < 0x3C) return 0;` (the compare result in `v0`, the store only on the taken path), and
  main's `set_kanji_string` prototype. The frame needs a `FAKE` layout (`s16 x[6]` read as `x[1]`,
  `u8 buf[0x204]`, an unused `u8 pad[8]`): the original keeps `x` in memory at sp+0x256 across the call, which
  a plain `s16` does not do (it is constant-folded), and an `s16` array is 4-aligned.

### Paired scores

Diff rows of the census measure (`tools/redraft.py` and `drafts/T-20930/hand.py`). "Before" is `tools/redraft.py`
on the tree before the type changes (172ab7ce), "after" on b035184a (types done, before `func_80132420` matched).

| function | bytes | census | T-18300 best | before | after | final |
| --- | --- | --- | --- | --- | --- | --- |
| `func_80132420` | 1764 | 364 | 336 | 336 | 336 | matched |
| `func_80132B04` | 660 | 82 | 82 | 82 | (matched) | matched |
| `func_80139B48` | 476 | 194 | 194 | 182 | (matched) | matched |
| `func_801330E4` | 796 | 129 | 129 | 129 | 129 | 114 (hand) |
| `func_80134270` | 1848 | 468 | 468 | 487 | 468 | 468 |
| `func_80134B18` (guarded) | 208 | 24 | 24 | 24 | 24 | 24 |
| `func_80134C2C` | 240 | 56 | 52 | 46 | 46 | 40 (hand) |
| `func_80135F4C` | 3220 | 815 | 813 | 795 | 795 | 795 |
| `func_8013759C` | 352 | 99 | 94 | 99 | 99 | 66 (hand) |
| `func_80137BB4` | 652 | 100 | 58 | 62 | 62 | 58 (T-18300 hand) |
| `func_80137E40` | 292 | 67 | 64 | 66 | 66 | 64 (T-18300 hand) |
| `func_801387B0` | 748 | 209 | 156 | 174 | 174 | 156 (T-18300 hand) |
| `func_8013A2E8` | 4648 | 1667 | 1667 | none | none | 1667 (census hand) |

- Headers alone: of the 11 functions scored in both runs, 10 have the same best score. `func_80134270` went from
  487 to 468, a side effect, not a type effect: the archived T-18300 body names the folded scalars and no longer
  compiles, so the `draft_fixes` search ran on the `plain` draft instead and found 468 (the census score).
- The eight unguarded functions still unmatched (without `func_8013A2E8`): census 1,943 rows, T-18300 best
  1,834, now 1,761.
- What moved scores were hand drafts that use the types: the record view of `D_8011ECD0` instead of main's
  scalars in `func_8013759C` (94 to 66; the first 37 rows now match, T-9020's finding again), the dialogue
  context and return value in `func_80132B04`, the `POLY_G4` sibling, the K&R and prototype views of
  `func_80132420`. The redraft driver alone moved nothing, as in T-16550, T-16560 and T-20900.

### Still open

- `func_80134B18` / `func_80134C2C` (24 / 40 rows): `D_80120657` (record 96 `unk_07`) is loaded into `a1` in the
  original; `tools/aggregate_audit.py --pairs` lists the load of `D_80120657` hoisted above the store to
  `D_8012069B` (record 97) in both, so the original reached records 96 and 97 through different symbols
  (`D_80120650` and `D_80120694`, or the scalars), not through `D_8011ECD0`.
- `func_801330E4` (114): the frame layout (original: table at sp+0x30, the spilled level at sp+0x54, points at
  sp+0x58, ranks at sp+0x78, frame 0xB0) and the load order of the three bonus flags.
- `func_8013759C` (66): the original keeps record 31's fade byte in `s1` across the whole function.
- Scene scripts (`func_80137BB4`, `func_80137E40`, `func_801387B0`): the record view and the scalar view give
  the same score; the register of the promoted portrait field is what is left.
- `func_8013A2E8`: no compiling fresh draft (`M2C_LWL` from the 27-byte string copy); the patterns of
  `func_80132420` (old-style definition, `u8 col` prototype, `D_8013CAC0` rows) should carry over.

## T-24047 initializer recovery

[[tickets/T-24047-ending-sprite-scene]] reconstructs `func_80135F4C` in
`src/ovl/ENDING/80134D20.c` from current main4496b0dc7. One signed-halfword counter,
one unsigned-halfword size and a signed-word quotient precede the two stack
aggregates. Direct record indexing removes machine-temporary pointer homes and
recovers the original160-byte frame with tables at sp+0x7C/sp+0x60.

Use GameState.unk_10A5[i+16] for the first three menu bytes and [i+4] for the
last twelve. The first loop needs a FAKE address roundtrip on that member to
retain the original alias dependency. Writing the width halfword through a
FAKE unsigned view lets its value share the size register with height/colour
and image-transfer arguments. The shared Rec24 declaration remains unchanged.
A named i/6 quotient immediately after the third loop's byte clear restores
its ten-store block; literal divisors naturally become register divisors.

Actor Y assignment before the fade-byte assignment restores the complete
second loop. The bit14 field view preserves the original shift/sign branch
and the register cycle of the scene switch. Preferred source still differs:
804/805 instructions, aligned cost43. Four words exchange the grid constants
128/499 between a3/t0. Two default-arm script-address LUIs move into dispatch.
Explicit case12/default prevents that motion but changes the bound and table,
so its five-word diagnostic is not accepted. Draft and replay: drafts/T-24047/.
No new verified C bytes; normal builds retain ASM.

The focused T-24047 continuation retained fourteen negative source probes.
Distinct default-cell symbol declarations leave the same residual, ruling out
the matrix declaration as the address-motion cause. Outer range checks retain
a second check; ordinary/register constant locals expand the frame; constant
casts and an early palette-store move are neutral. Preferred guarded source
remains unchanged at804/805 instructions and cost43, zero verified byte gain.

Eleven later probes also remain negative. Native AS1 DAG identifies a later
cross-block pass as the source of both default-arm LUI moves; line grouping
does not inhibit it. Partial stock uopt listing completes target allocation,
with adjacent ranges528/529 assigned colours6/7, before its ecvt runtime stub
aborts. Detailed priority/cost diagnosis needs the unavailable instrumented
uopt from [[uopt-register-allocation]]. No compiler rebuild was attempted in
this source lane; guarded source and verified coverage remain unchanged.

The allocator prerequisite was subsequently repaired in bounded Docker scratch;
[[ending-allocator-trace]] gives exact reuse/build/fidelity commands. Tracing
identifies an equal priority5/cost0 tie and explains the source coupling to
quotient priority. Moving UV before palette while spelling the later quotient
directly fixes the constant colours: intermediate closer804/805 instructions/cost37.
Only an early zero-halfword store and default-arm address speculation remain.
Source: drafts/T-24047/closer.c; no verified byte gain.

Moving the zero-halfword store after UV closes that final loop gap, advancing
the guarded production body and drafts/T-24047/body.c to804/805 instructions,
cost35. Every initializer loop, constant and scene body agrees. Only default-arm
cross-block LUI motion and its branch-offset cascade remain. The case12
diagnostic now differs only in the bound word and extra table entry, so remains
rejected. Direct volatile views, a named label and register/selector metadata
views do not fix the motion. Normal ENDING rebuild and tracer fidelity pass.
