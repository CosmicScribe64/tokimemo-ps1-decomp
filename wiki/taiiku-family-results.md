---
type: research
updated: 2026-10-16
---

# TAIIKU family results

Scope: the TAIIKU overlay (`src/ovl/TAIIKU/`, `include/ovl/TAIIKU.h`). At the start of [[tickets/T-16550-taiiku-types-first]] it had 64 unmatched functions (55,708 bytes, census sizes) in nine C files, seven of them with a guarded draft in `src`. Few matched siblings exist: `tools/siblings.py --unit TAIIKU` finds one function at a ratio of 0.80 or more and one unmatched cluster (`func_80137EA8`, `func_80146D18`, 0.919).

Three objects hold the same update code with different record layouts. This page calls them game A (`80133C80.c`), game B (`80142240.c`) and game C (`801452D0.c`). Each has four 16.16-position records, a player and three others, so the records are called runners here. That name is a working hypothesis from the code shape (speed added to a position, timers, a "next free slot gets i + 2" table), not from game text.

## Types-first pass (T-16550)

The pilot recovered the overlay's shared types before any function matching, redrafted all 64 functions with them and cut the rest into four packages. It took about one hour of agent time for the types and the redraft.

### Type map

All conversions keep TAIIKU's sha1. The build decided between an array, a struct and separate objects; the "why" column is the evidence.

| Type | Symbol(s) | File | Replaces | Why this shape |
|---|---|---|---|---|
| `TaiRunnerA` (0x24) in `TaiRunnersA` (`player`, `cpu[3]`) | `D_80149208` | `80133C80.c` | `TaiikuSlots` (`f0C`, `pad24[i * 0x24 + k]` views), `TaiSlot24` | every access in the original uses `%lo(D_80149208)`; the matched loops index the other runners from 0 |
| `TaiRunnerB D_8014A13C[4]` (0x24) | `D_8014A13C`; `D_8014A144/148/14C/154/158/161/185/1A9` become fields | `80142240.c` | `u8 D_8014A13C[0x90]` and 8 scalars | a struct member breaks `func_80143D24`: the original keeps the array base in a register for its constant-index clamps, as IDO does for an array and not for a struct member |
| `TaiikuController` (0x1C, +1 byte, +2 halfword added) | `D_8014A3E4` (player) and `D_8014A400[3]` | `801452D0.c` | `D_8014A3EC/F0/F4/F8/FE`, `D_8014A401/41D/439`, `Tk1C`, `TaiikuTickController` | separate objects: as one `[4]` array, `D_8014A3E4[i + 1]` changes the code of `func_801470A0`, `func_80147120` and `func_801482BC` |
| `TaiikuSlot1C D_8014A454[4]` (0x1C) | `D_8014A454`, `D_8014A460` | `801452D0.c` | `u8 D_8014A454[0x70]`, `TaiikuTick1C` | `D_8014A454[i + 1].unk_0C` compiles like the old byte index |
| one-array structs (`TaiBytes10`, `TaiShorts12x/20/23`, `TaiWords6/7/8/10`, existing `TaiikuHalf10`, `TaiikuW5`, `TaiikuS11`, `TkS16x4`, `TaiShorts12`) | 28 tables, `D_80148F28`..`D_8014A5F4` | header only | undeclared | each is copied to the stack by an unmatched function as one struct (lw/sw runs plus an lh/sh tail); element type from the drafts' use of the copy |
| `TaiHalves` gets its size comment; `D_8014A0C0` declared | | header | | needed by the redraft driver and by `func_80141CDC` |

Tried and dropped: `u8 D_80148EC0[4]` (game A's runner list; `func_80132000` also reads and writes it as a word, so the drafts got worse), `u8 D_8014A120[4]` and `u8 D_8014A3D8[4]` (m2c then prints `*(s32 *)D_8014A120 = 0` for a byte store; the unsized arrays stay).

`tools/migrate_globals.py` cannot hold these aggregates: it rewrites every `src` file and every overlay loads at 0x80132000, so a TAIIKU range would claim other overlays' `D_` names. The absorbed names were rewritten by hand in the three TAIIKU files, and none is left (checked with a scan of `src/ovl/TAIIKU` and the header).

### Idioms found on the way

- Array or struct or two objects decides code, not only names. For the same bytes, game A matches as a struct (`player`, `cpu[3]`), game B only as an array, and game C only as two objects. Try all three before writing a FAKE.
- `arr[i + 1].f` in a loop can compile differently from a separate `cpu[i].f` (game C), while `D_8014A454[i + 1].unk_0C` in the same function compiles like the byte index.
- `func_80141CDC` matched from its redraft by writing the flag loop over display records 96 and 97 as an index loop (`for (i = 0; i < 2; i++) ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 |= 8;`). The pointer loop that m2c prints derives the end address from the base register; the original loads the end address separately.
- `func_801449A0` keeps its byte view of `D_8014A13C` (FAKE): typed `D_8014A13C[i + 1]` fields add a second induction register and 8 bytes of frame.
- The review (T-16550) tried the field form for every other function that still walked a typed record array with byte offsets. `func_80147210` (game C, `TaiikuController *v` stepped with `v++`), `func_80148194` and `func_80148228` (`TaiikuSlot1C *`) match as fields and were converted. `func_801448D4` and `func_80144C70` (game B, `r = &D_8014A13C[i + 1]` or the byte base plus `0x24 * i` cast to `TaiRunnerB *`) and `func_8013732C` (game A, `p = &D_80149208.cpu[i]` or the cast form) change the code, so they keep the byte walk with a FAKE comment. `func_80138680` reaches the four runners as `(&D_80149208.player)[arg0]` (FAKE comment): the object is `player` + `cpu[3]`, not one array.
- `func_80132970` keeps three address registers for `0x80148EC0`, `0x80148EC2` and `0x80148EC3`, from `lui`/`addiu` pairs that splat did not name. `&D_80148EC0[2]` and `(u8 *)&D_80148EC0 + 2` are both rebuilt from the base register by IDO; the literal-address draft is closer (30 rows).
- The near misses of the guarded drafts did not move with the types: `func_801346FC` 8, `func_801368B8` 4, `func_80136BB4` 4, `func_8014666C` 4, `func_80138AA8` 22, `func_80138DF8` 30, `func_80139FE4` 66 rows, the same as with the old header. `func_801368B8` differs by one `lui` that the original schedules above the store to `player.unk_1C`; separate player and cpu objects in game A are an untested explanation. Update (T-16551): all three are matched now, `func_801368B8` as a loop over the four runners, `func_801346FC` with a `void` callee prototype, `func_80136BB4` with a `u16` copy of the counter ([[matching-notes]]).

### Redraft

`drafts/T-16550/redraft.py` runs `tools/exhaustive_drafts.py`'s `process()` on every unmatched TAIIKU function, so the scores are the census's full-translation-unit diff lines. Variants:

| Variant | What it adds | Notes |
|---|---|---|
| types | nothing: m2c with `game.h` + the new `TAIIKU.h` | m2c still prints the splat label (`D_80149214`) for a field of a typed object, so most drafts do not see the struct |
| fold | rewrites the asm given to m2c: a label inside a typed TAIIKU object, `GameState D_800E6280` or the `D_8011ECD0` display records becomes `base + offset` (the `%hi` keeps the bare base, else m2c reads every paired `%lo` as the `%hi`'s address); m2c sees `D_8011ECD0` as `DisplayActor44[]` | post-fixes: `((DisplayActor44 *)D_8011ECD0)`, byte offsets on it kept as bytes, typed pointer steps (`p += 0x1C` to `p += 1`), stack-table copies written as one struct assignment |
| fold-mainonly | fold limited to `GameState` and the display records | separates the TAIIKU types from the main-exe aggregates |
| fold+fixes | `tools/draft_fixes.py` rewrites | changed no score |
| guarded | the guarded drafts in `src`, scored with the new header | unchanged from the old header |

Every distinct candidate body was then rescored with the final header, and the best one is in `drafts/T-16550/TAIIKU/<func>/` (body, full translation unit, `result.json` with score, census score and provenance).

Results (64 functions; baseline: the census score from `drafts/T-14500/best-quality-catalog.jsonl`, or the guarded draft where it is lower):

| | value |
|---|---|
| median score, census catalog | 241.5 |
| median score, census or guarded | 234.5 |
| median score of the 58 new drafts | 183 |
| median of the best starting point per function | 203 |
| better than the baseline | 27 functions, 21,632 bytes |
| worse (the census draft stays the starting point) | 10 |
| equal | 21 |
| no compiling new draft (`M2C_LWL`, stack block copies) | 6 |
| new score 0 | none |
| matched after the redraft | `func_80141CDC`, 1,120 bytes, from an 18-row fold draft and one hand edit |

Largest gains: `func_80145C28` 280 to 120, `func_80142FB8` 439 to 282, `func_80132970` 166 to 30, `func_80140A90` 228 to 118, `func_80138224` 350 to 240, `func_80137C44` 352 to 245, `func_80147F58` 246 to 149, `func_80141CDC` 98 to 18. Most of the gain comes from the fold, not from the header alone: fold-mainonly beat headers-only in 19 functions; the TAIIKU types beat fold-mainonly in 13 (worse in 1: `func_8013B200`, 131 to 139). The plain types variant scores the same as the census wherever a function does not touch a changed type.

### Packages

Four file-disjoint work packages, best expected bytes first (expected bytes: size / (1 + score / 20); time: 12 + bytes / 90 minutes per function). Lists in the orchestrator's scratchpad, `pt-taiiku-p1-list.txt` to `pt-taiiku-p4-list.txt`.

| Package | Ticket | Owned C files | Functions | Bytes | Minutes |
|---|---|---|---:|---:|---:|
| pt-taiiku-p1 | T-16551 | `80133C80.c` | 8 | 7,012 | 174 |
| pt-taiiku-p2 | T-16552 | `80138950.c` | 9 | 6,196 | 177 |
| pt-taiiku-p3 | T-16553 | `801452D0.c` | 9 | 5,412 | 168 |
| pt-taiiku-p4 | T-16554 | `80140810.c`, `80132000.c`, `80142240.c` | 7 | 6,884 | 160 |

Left for later waves: the rest of `80133C80.c` (8), `80138950.c` (8), `801452D0.c` (5), `80142240.c` (4), `80132000.c` (`func_80132000`), `80132BD0.c` (3) and `80141280.c` (1).

### Package p4 results (T-16554)

`80132000.c`, `80140810.c`, `80142240.c`, seven functions from the redraft (6,884 bytes). Four matched in about 70 minutes of agent time: `func_80132970` (456 B), `func_80143DD0` (384 B), `func_80143F50` (956 B), `func_8014280C` (1,412 B). Guarded drafts: `func_80140A90` (2,020 B, 33 instructions differ in register numbers only) and `func_8014357C` (664 B, 20); `func_80142FB8` (992 B) left as `INCLUDE_ASM`. The redraft scores (30, 107, 230, 260) did not predict the order of difficulty: the 260-score `func_8014280C` matched after fixing three m2c artifacts (chain assignment, folded loop start, spill slots), the 118-score `func_80140A90` stops at a ugen temporary count. Method, idioms and the compiler-gap rows are in [[matching-notes]] ("T-16554") and [[data/t0018-cases]]. Type changes: `D_80148EC0[]`, `D_8014A064`, `D_8014A124`, `D_8014A1E8[]`, `D_8014A208[8]`, `D_8014A228[11]` declared; `func_80145044`, `func_801450C0`, `func_80145148` prototyped; `MAIN_API_OVERRIDE_func_80085368` and `func_800853FC` (`s32` view of two `void` callees) in `80140810.c`.

### Package w3-d-p01 results (T-19001)

`func_80140A90` (2,020 B) re-worked for about 50 minutes in wave 3: not matched, 33 instructions still differ. The block copy's temporary count was measured as a two-state rule of ugen and no code-neutral C reaches the original's state; the original disagrees with the same rule in SHUGAKU, MASTER and ENDING, so the gap looks like a ugen difference. Evidence and the variants tried: [[matching-notes]], "T-19001".

## Package pt-taiiku-p2 (T-16552): `80138950.c`

Worktree `pt-taiiku-p2`, 9 functions from the redrafts, about 2 hours. Result: `func_80139FE4` matched (816 bytes, FAKE dead assignments, see below); `func_80138DF8` (1,336 bytes) from 30 to 3 differing lines; `func_80138AA8` stays at 22 lines but loses both FAKE pads; the Opus closer pass then matched both (`func_80138DF8` with a FAKE `goto` into its first loop, `func_80138AA8` by calling `func_80085E30` as an implicit int function); `func_8013B3B0`, `func_8013B608`, `func_8013BCEC`, `func_8013B78C`, `func_8013B94C` and `func_80139AB0` stay `INCLUDE_ASM` (drafts of 33 to 111 lines, rows in [[data/t0018-cases]]). Details and evidence: [[matching-notes]], "T-16552".

| Function | Bytes | Start | End |
|---|---:|---|---|
| `func_80139FE4` | 816 | guarded 33 lines | matched |
| `func_80138DF8` | 1,336 | guarded 30 lines | matched (closer: FAKE `goto` into the loop) |
| `func_80138AA8` | 848 | guarded 22 lines | matched (closer: implicit int `func_80085E30`, local `p`) |
| `func_8013B608` | 388 | redraft 81 | draft 33 register + 9 structure lines, not kept; matched in T-19008 (closer) |
| `func_8013B3B0` | 528 | redraft 99 | draft 111 lines, not kept |
| `func_8013B78C` | 448 | redraft 136 | draft 45 lines, not kept |
| `func_8013BCEC` | 524 | redraft 113 | draft 49 lines, not kept; T-19008: guarded, 2 instructions missing |
| `func_80139AB0` | 812 | redraft 237 | draft 99 lines, not kept |
| `func_8013B94C` | 496 | redraft 149 | draft 80 lines (60 with a volatile counter view, address kept in a register), not kept |

What the types did and did not do here: the new `TaiikuBall`, `TaiikuA0`, `TaiikuCamera0C` and `TaiHalves` fields made the hand drafts of the counter family short, but none of the failures there was a type problem: they are promotion and ugen-temporary gaps (the original reloads the counter at every test; IDO promotes it). The wins came from compiler behaviour found with `regdiff` and a frame probe: bit-order colouring of leaf functions, the phantom spill slot of `A[f()] = v`, and the parameter type of a callee.
## pt-taiiku-p3 results (T-16553, `801452D0.c`)

[[tickets/T-16553-pt-taiiku-p3]] matched 9 of the 14 remaining functions of game C (5,244 bytes): `func_8014666C`, `func_80145C28`, `func_80147318`, `func_80147928`, `func_80146A60`, `func_80147F58` (the list), `func_80148370`, `func_80146338`, `func_80147594`. Four stayed guarded in the main pass (`func_80145640` 8 lines, `func_8014607C` 20, `func_80145370` 22, `func_80146D18` 31) and `func_80147B14` is untouched. The Opus closer pass then matched `func_80145640` (1,080 bytes, call result in a declared local), which brings the package to 10 functions (6,324 bytes); `func_8014607C` stays guarded at 20 lines. The idioms (index loops over `DISP(n)` and `D_8014A454[i + 1]`, pointer-compare loop bounds against the next table symbol, an alias symbol for a loop end, declared locals above a stack table, two index variables for two pointer ranges, `order_search` on the store lines of a record-init loop) are in [[matching-notes]], "T-16553". Types added to `include/ovl/TAIIKU.h`: `D_8014A4F8[12]`, `D_8014A528[9]` (the tpage id tables; `D_8014A4F4` is now `s32`), `D_8014A4D4[8]`, `D_8014A4E8[6]`, `TaiWords9`/`TaiWords12` with `D_8014A56C`, `D_8014A570`, `D_8014A5A0` (stack-table sources of `func_80145370`), `D_8014A668` and prototypes of the game-C helpers.

## Package pt-taiiku-p1 (T-16551, `80133C80.c`)

[[tickets/T-16551-pt-taiiku-p1]]: 5 of the 8 listed functions matched (`func_80138224`, `func_80133CFC`, `func_801368B8`, then `func_801346FC` and `func_80136BB4` in the closer pass; 4,480 bytes), `func_80137544` (26 lines) and `func_80135480` (22 lines) are guarded drafts, `func_80134060` stays `INCLUDE_ASM`. Details and idioms in [[matching-notes]], section T-16551.

- Display-record initialisers (`func_80138224`): the fold draft was one store-order permutation and two frame words away from the match; the best starting scores (240) did not predict that, the structure did.
- A table that the original defines in the unit has to be defined in the unit (`s32 D_801492F8[10] = {...}` in `80133C80.c`, three `INCLUDE_RODATA` pieces replaced) for two stores to share one `lui at`.
- A long run of identical statements over the four runners is a loop that IDO unrolls with the first iteration peeled (`func_801368B8`, `(&D_80149208.player)[i]`, FAKE). The "untested explanation" of T-16550 (separate player and cpu objects) was tested and is wrong: it hoists the loads of the next group above the stores.
- `D_801492C4` is a word (`s32`); the byte view made the three-word copy in `func_80133CFC` an `sb`.

## Package w3-d-p04 (T-19004, `80133C80.c`)

[[tickets/T-19004-w3-d-p04]], wave 3: `func_80137228` matched (260 bytes) once `D_80120A72` and `D_80120AA8` were read as fields of display records 111 and 112 (the loop's record stores then force the reload the original has); `func_80135830` (672 bytes) matched in the closer pass (index loop plus a FAKE dead `i = 0;`), `func_80137544` (26 lines) and `func_80135480` (22) unchanged, `func_80137C44` left `INCLUDE_ASM`. Details in [[matching-notes]], section T-19004.

### Package w3-d-p08 results (T-19008)

`80138950.c`, the rest of the D_801499A2 counter family (five functions, 2,288 bytes, about 130 minutes of agent time with the closer pass): `func_8013B200` (432 B) matched in the first pass; the closer pass matched `func_8013B608` (388 B). `func_8013BCEC` (524 B) is a guarded draft (2 instructions missing); `func_8013B78C` (448 B, 79 rows) and `func_8013B94C` (496 B) stay `INCLUDE_ASM`. The first match came from removing a local, ordering two stores and reading the flag through a u16 view; the gap of `func_8013B608` and `func_8013BCEC` was located in the first pass (one extra ugen temporary between the condition and the chain in the original) and closed by the closer pass. Details, the `ugen -S` method and the probes tried: [[matching-notes]], "T-19008"; rows in [[data/t0018-cases]].

Closer pass: `func_8013B608` matched by splitting the result assignment (`v = x >> 3; v += (x >> 3) * 80 / 48;`), which gives the extra ugen temporary with no instruction; the same split leaves `func_8013BCEC` 2 instructions short (the counter write-back before `func_8013C5C8`). Package total: 2 of 5 functions, 820 bytes.
