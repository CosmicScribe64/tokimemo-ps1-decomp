---
type: research
updated: 2026-10-17
---

# GYOZI and SHOUGATU event families

## Scene callbacks (T-20006)

Issue #43 claims `func_80141944` in `801418B0.c` and `func_80145384` in `80143AD0.c` before implementation. Both pass actual default linked comparisons: 336 + 372 = 708 new bytes. Existing callback tables and display records replace the invalid scalar calls in the archived drafts. Meaningful index locals, direct s16 counter increments, shared s32 return types and corrected unsigned byte fields suffice; no new fakematches or compiler changes. These supersede the T-17600 noncompiling redraft fallbacks. Evidence, whole-build checks and review are in [[tickets/T-20006-gyozi-scene-callbacks]].

## GYOZI dialogue follow-up (T-20004)

Issue #39 claims `func_80135118` / `func_80135280` in `80134FD0.c` before work. Both now pass actual default linked comparisons: 360 + 416 = 776 new bytes. Recover `D_800C3370` as a byte array instead of the archived word-scalar context, use direct timer updates and byte postincrement tests, and reread the selector after calls. The setup callee uses the same shared s32 return type as PR #37. Two marked FAKE sites retain the original unchanged-byte stores before the sound calls. Verification, limitations and local review: [[tickets/T-20004-gyozi-dialogue-followup]].
## Display/dispatch follow-up (T-20005)

Issue #41 claims `func_8014206C` / `func_8013C774` before work. Both now match 524 actual default linked bytes (160 + 364), with full GYOZI SHA1 passing. The display helper uses a signed local selector for compare operand order; the dispatcher uses a shared s32 return type to preserve its original schedule and byte extent. Existing DisplayActor44/GyoziWork/GzCallbacks90 types and inline literals suffice. No new fakematches or compiler changes. These supersede the T-17600 hand-draft near misses; see [[tickets/T-20005-gyozi-display-dispatch]] for verification and local review.

SHOUGATU types-first pass (T-17500): [[shougatu-family-results]].

Scope: [[tickets/T-13200-event-family-transfer]], isolated codex/events-13200 from aa2b3158. The original checkout stayed read-only; ignored inputs came from integration, and all analysis/compilation ran in the existing Docker image. No generated assembly edits or metrics changes.

## Accepted C

| Unit | Function | Literal linked bytes | Source form |
| --- | --- | ---: | --- |
| SHOUGATU | func_8013BDE0 | 656 | Direct case stores remove m2c's artificial shared temporary/goto |
| SHOUGATU | func_8013F494 | 1328 | Existing DisplayActor44 and chained structure assignments |
| GYOZI | func_80141ACC | 1440 | Held-out transfer of the same structure-copy form |

Total: **3 functions / 3,424 new literal linked bytes**. Two GYOZI selector entries in switch-proof.txt are unchanged ASM controls and contribute no gain. Literal proof is build/events13200/switch-proof.txt and init-proof.txt; affected-unit SHA1 evidence is switch-build.log and init-build.log. The selector progress-size convention says652, while its full literal span includes656 bytes; use3424 for this lane's linked-byte gain.

The initializer source needs one marked FAKE unused scalar. Natural loop, scoped-local, register, parameter, explicit-copy and meaningful argument-local forms failed. Without the extra scalar, both implicit structure-copy temporary addresses are4 bytes too high although frame and all other instructions match. The same4->0 result repeats in the held-out overlay. Track this as a source-layout workaround, not recovered author intent.

## Failed probes

- GYOZI39E4C: base196 rows, direct stores247, split flag stores33; a temporary recovery of the known mode byte inside GyoziWork reaches27 but still fails. Header restored. Bitfields and pointer views do not match.
- GYOZIE824: base141, direct stores188, narrow result plus split mask24. Declaration order and narrow casts do not close the remaining value-register/copy differences.
- SHOUGATU421D4: four indexed-array/loop/pointer probes give126–128 rows. A corrected word-copy interpretation remains128 and chained repeated stores worsen to156. No production source or array declarations accepted. This is a bounded negative result, not a compiler-impossibility claim.
- SHOUGATUF494 raw m2c fails on unresolved stack-copy types; using the existing aggregate exposes the small real layout gap instead.

The character-selection EA98 draft was inspected but not compiled: it needs a currently missing shared main global declaration and two14-byte table views; this is a follow-up lead, not a failed measured family or coverage gain.

## Preservation and review

An exact text audit against aa2b3158 checks **744 pre-existing C bodies** across both owned overlays and finds0 changes (build/events13200/existing-c-audit.txt). All new C retains full translation-unit context.

Two GPT6.1Sol medium reviewers independently checked standards and ticket scope. Both confirmed literal-byte counts. Their findings were resolved: new overlay extern blocks have dedicated guards, and the previously implicit main function func_800BCE40 now has the required central old-style s32 declaration in main_api.h. The declaration preserves the original implicit-int ABI and leaves semantics unknown. This minimal shared-header repair requires all27 checks, unlike the earlier overlay-only checkpoints.

Final all27 Docker build passes (build/events13200/all27-build.log). Independent hashes verify27/27 binaries (final-hashes.txt). Final default C proofs verify all three functions (final-proof.txt). Headers/globals pass; final-protos.txt reports0 new /15 known disagreements. The completed progress.txt census reports5,862/6,958 game functions and1,397,484/2,279,368 game bytes (61.3%). GYOZI is70.4% and SHOUGATU69.4%. The broader80% integration objective remains open. An earlier progress scan raced splat regeneration; its successful rerun is the final census. The progress convention increases by3,420 bytes, while literal function spans increase by3,424.

Probe ledger: 47 full-TU scoring attempts across6 distinct targets; 44 nonzero/error outcomes and3 zero-score outcomes. These include repeated diagnostic controls and exclude draft-only inspections. Accepted gain remains the three unique functions above. Detailed source forms and scores are in build/events13200/scores.jsonl.

Final handoff: codex/events-13200. Source gains are21df4fe7 and63d90035; final declaration/guard review repairs and this evidence are committed separately. Both review axes clear the source changes. Closure review confirms the negative particle result and excluded ASM controls; its stale pending-validation wording is corrected here. All744 baseline C bodies are unchanged.

## Types-first pass (T-17400)

T-20900 update: `EventParticleView44` and `EventMotion44` are `DisplayFixActor44`; `EventFlagRecord44`, `Ev44` and the local `Words17` copies are `DisplayActor44` ([[data-types]], "Shared-type unification").

This section is about the EVENT overlay (`src/ovl/EVENT`), not the GYOZI/SHOUGATU event scenes above. Wave-3
types-first pass ([[tickets/T-17400-types-first-event]], [[speedup-research-2026-10-16]]), branch `w3-event-types`
from `w2-good`. Start: 40 unmatched functions (39,712 bytes), 4 of them guarded. About one hour of agent time for
the survey, the types and the redraft.

### Survey

- `tools/type_recovery.py --unit EVENT`: as in DATE, the top "high" proposals are 0xC-stride arrays that are really
  step-table copies (IDO copies a struct in a loop of three words). `D_801205A8` (0xD4 struct, 75 functions) is the
  run of scalar resource addresses the scene setup handlers write; not converted.
- Census drafts (`drafts/T-14500/EVENT`): the byte-offset casts sit on m2c temporaries holding a `Rec24` record
  (`D_800EC190 + i * 0x24`), a display record (`D_800E9620 + i * 0x44`; `D_800EAFA0` is record 96), the
  `D_800B0860 + D_800B1746 * 0x34` records, and on the stack copies of step tables and data tables (`M2cBlock`
  copies, `sp + i * 4` calls). The step index is `D_800B1AF6` (DATE uses `D_800E6280.unk_110A`).
- `tools/siblings.py --unit EVENT`: two clones of matched DATE functions (`func_800FF320` of `func_80152684`,
  `func_80104C3C` of `func_801572E4`), and close siblings `func_800FD00C` (DATE `func_801511B4`, 0.94),
  `func_8010F340` (DATE `func_80144720`, 0.91), `func_8010AE7C` (GEKO `func_80141760`, 0.98),
  `func_8011EDFC` (DATE `func_801489D0`, 0.83).

### Type map

All changes keep EVENT's sha1.

| Type | Symbols | Evidence |
| --- | --- | --- |
| `FnTblNN` (`void (*f[NN])()`), 36 sizes from 14 to 103 | 90 step tables `D_801207B8`..`D_8012514C` | each handler copies its table to the stack whole and calls `tbl.f[D_800B1AF6](0x80)`. The 63 local typedefs and externs of 15 `.c` files moved to `include/ovl/EVENT.h`; 27 tables of unmatched handlers got declarations (21 of them `func_8010F74C`'s); the `D_80122EC8` override moved from `80103B60.c` to the header. `NN` is the run of code pointers in the data (trailing zero words dropped). |
| `D_801218E8` as `FnTbl32` | was `Blob80` (`u8 b[0x80]`) | it is a step table; the copy compiles the same |
| `Ev44`, `EvEnt`/`EvTbl` | `D_80120A74`, `D_80121AD8` | local views moved to the header; `D_80121AD8` is the (a, b) pair table of `func_800FF320` |
| `U16x21` (moved), `EvPairs8` (`s16 v[8][2]`), `EvShorts82`, `EvShorts10`, `EvShorts11`, `EvShorts5` | `D_80125454`/`480`/`4AC`/`4D8`, `D_80122990`, `D_80121A34`, `D_80123B7C`/`B90`/`BA4`, `D_80125330`/`348`, `D_80125360` | data tables the unmatched handlers copy to the stack (lw/sw runs plus an lh/sh tail); element width from the reads of the copy (`func_80103774` reads `lh 0x6A + 4 * i`, the `[i][1]` of `EvPairs8`) |
| `void func_8010AFE4(void)` | | defined after its first caller in `8010A210.c` |

Not changed: `D_801220EC` (FnTbl42) and `D_8012277C` (FnTbl32) stay `main_api.h` scalars read through a cast,
because `main_api.h` has no `MAIN_API_OVERRIDE` guard for them (adding one changes an existing declaration);
`EvFnTbl44`, `EvFnTbl53` and `EventCallbackTable47` keep their names (same shape as `FnTblNN`, matched code
uses them); `D_800B0860` (`EventStateView`, cast views in matched code) and the 0x34-byte records in it.

### Redraft

`drafts/T-17400/redraft.py` is the T-17200 driver adapted to EVENT: the fold ranges and record views are
`D_800E9620` (display records, `DisplayActor44`) and `D_800EC190` (`Rec24`), both `u8[]` in `main_api.h`, so m2c
sees them typed and the draft writes `((DisplayActor44 *)D_800E9620)` and `((Rec24 *)D_800EC190)`.

| | value |
| --- | --- |
| unguarded functions | 34 |
| compiling drafts | 32 (`func_8010F74C`, `func_8011B848` do not; census: 34, some after manual repair) |
| better than the census | 9 functions, 7,560 bytes |
| same / worse | 21 / 2 (`func_800FD00C`, `func_800FEAE8`: hand-repaired census drafts) |
| sum of diff rows | 11,600 to 11,395 |
| typed vs plain run | typed better in 9 of 30, worse in 7; typed compiles 5 that plain does not |
| `tools/draft_fixes.py` | no score changed |
| score 0 | none |
| guarded functions | fresh drafts worse in 3 of 4 (census = hand guard body) |

Largest gains: `func_8011A8E4` 142 to 61 (the three `U16x21` stack tables), `func_80118B4C` 276 to 225,
`func_801022C0` 86 to 52, `func_8010F340` 38 to 13 (display records through one base), `func_800FF61C` 8 to 2,
`func_8011C750` 1285 to 1271.

Matched during the pass (4 functions, 1,416 bytes):

| Function | Bytes | How |
| --- | ---: | --- |
| `func_800FF320` | 708 | port of the DATE clone `func_80152684` (`EvTbl tbl`, the clone's FAKE `s32 pad[2]` above it) |
| `func_80104C3C` | 268 | port of the DATE clone `func_801572E4`; `D_800EB02A` is a byte store (`sb`), not the `s16` the record suggests |
| `func_800FF61C` | 244 | typed redraft in the `func_800FD1C0` shape: `s32 idx` above `FnTbl48 tbl`, both range checks in one `if` |
| `func_8010AE7C` | 196 | port of the GEKO sibling `func_80141760`: `u8 index` read twice, `FnTbl56 table` |

### Near misses (hand drafts in `drafts/T-17400/EVENT/<func>/hand.c`)

- `func_800FD00C` (6 rows): the DATE `func_801511B4` shape plus the EVENT `printf`; the frame (0xD0, table at
  0x30) comes right with an unused `u8` local declared after the table, and the store chain must be written
  `D_800EB02F = D_800EC30C = D_800EC30D = D_800EC30E = D_800F19AB;` to store `30E` first. Left: the `%hi` of
  `D_800F19AB` in `$v0` instead of `$v1`.
- `func_8010F340` (11 rows): the DATE `func_80144720` shape with the display records written through
  `D_800E9620` (`((DisplayActor44 *)D_800E9620)[31].unk_07 = 0;`, `*(s32 *)(D_800E9620 + 0x874) |= ...`) and the
  two `Rec24` flags; the census scalars cost 25 rows. Needs `D_800B086A`, `D_800B1888`, `D_800B18D0`,
  `D_80080714` in `main_api.h`. Left: `D_800B0F30` lives in `$v0` instead of `$a0`; a local for it does not help.
- `func_801022C0` (14 rows): `s16 i; u8 c; s32 idx;` above the table gives the frame and table offset (the T-3330
  two-words-before form). Left: the loaded `D_800F19AB` and `c` in `$t8` instead of `$a3`.
- `func_800FC040` (55 rows): the step-table shape does not fix the `var_v1` (`func_800458E0` result) flow.

### Packages

`inventory2.py` / `mklists3.py`: seven packages T-17401..T-17407 (27 functions, 18,224 bytes):
w3-event-p01 `8011C650.c` (2), p02 `8010BB90.c` + `800FBBF0.c` (5), p03 `8010FE10.c` (5), p04 `801011A0.c` +
`800FDC20.c` (5), p05 `800F9680.c` + `801050D0.c` (4), p06 `801024B0.c` + `80107FC0.c` (5), p07 `8010B360.c` (1).
## Types-first pass (T-17600)

Wave 3 types-first pass for GYOZI ([[tickets/T-17600-types-first-gyozi]], branch `w3-gyozi-types`, method of
[[speedup-research-2026-10-16]]). Types in `include/ovl/GYOZI.h`, redrafts in `drafts/T-17600/` (README there).
GYOZI sha1 OK after every step.

### Type map

| Type / global | Evidence |
| --- | --- |
| `GzCallbacks90` `D_801476F4`, `D_80147C6C`; `GzCallbacks68` `D_80148880`; `GzCallbacksBC` `D_8014901C` | step-callback tables, each copied whole to the stack by an lw/sw loop (`func_8013B0C4`, `func_8013C774`, `func_80141944`, `func_80145384`) and called through `D_800F647A`, as `D_80148904`. These copies are most of `type_recovery.py`'s stride-0xC proposals (as in DATE). |
| `Rec24 D_8012CA40[]` | 0x24-byte records indexed `i * 0x24` in the dumpling functions with the field widths of main's Rec24 table; `D_8012CA40` is `D_801217D0 + 0xB270`, the same shift as the display records `D_80129F40` (`D_8011ECD0 + 0xB270`). |
| `s32 D_80148BF0[50]`, `D_80148CB8[50]`, `D_80148D80[50]`, `s16 D_80148E48[50]`, `s32 D_80148EB0` | one entry per falling dumpling (`func_80144018`, `func_801444CC`, `func_80144B70`, `func_80145084`); the count `D_80148EAC` stops at 50 and each array ends where the next starts. Records `D_8012CA40[i]` start at `i = 4`, and the `[i - 4]` accesses fold into `%lo`, so splat names `D_80148BE0`, `D_80148D70`, `D_80148E40`. |
| `s16 D_8014875C[8]`, `D_8014876C[8]` | indexed with the same `i * 2` (`func_80140E0C`, `func_80140F90`, `func_801412A4`); m2c printed `*(&D_8014876C + i * 2)` on an undeclared symbol, a wrong scale. |
| `GyoziGirl.unk_14` (s32), `unk_10[4]`, `unk_18[0x20]` | `lw` at +0x14 in `func_801378D0` and `func_80140464`; matched code reads `unk_10[0..3]` only. |
| `GyoziWork.unk_10B8` (`D_800F6458`), size 0x10BC | `func_8013DC2C` matches only with the store as a member of `D_800F53A0`: then the following girl[10] bit-field load stays below it (SHOUGATU writes the same field as `D_800E6280.unk_10E8`). The other scalars past 0xF30 (`D_800F6474`, ...) stay `main_api.h` scalars. |
| `u8 D_80148828[4]`, `extern s32 D_800F62F0` (main_api.h) | sized so the fold reaches `D_80148829`..`2B`; `D_800F62F0` had no declaration (`func_8013C774`). |
| `GYOZI.h` includes `game.h` | `DisplayActor44` for the display records `D_80129F40`; drafts in files without `game.h` failed on the cast. |

Not changed: the pointer tables `D_80146220`..`D_80146354` and `D_801470F0`..`D_80147188` (rows of 13 words
selected by constant index, saved and restored through the triples `D_80146358`, `D_8014718C`, ...; 155 matched
lines use the scalar names and the [6][13] / [3][13] shape is not proven); the byte table read at
`D_801483E8 + i` (matched code stores a pointer at that address); `D_80147224` (read as `lh` and `lbu`).

### Redraft

`drafts/T-17600/redraft.py` (T-17300 driver adapted, see the README): `plain` (headers only), `typed` (fold,
stack tables, record-temp, display-base, work-prim with `func_80054B60`, pointer steps, field symbols) and
`typed1`, then `tools/draft_fixes.py`. 32 of 37 functions got a compiling redraft; over the 29 unguarded ones
7 are better than the census (5,416 bytes: `func_80144B70` 516 to 474, `func_801428EC` 176 to 138,
`func_801421B4` 407 to 393, ...), 19 the same, 3 worse (`func_80144018` 461 to 540, `func_80138C9C` 302 to
326, `func_80145084` 116 to 123); sum 4,710 to 4,715 rows, median 124 to 123. The headers alone changed 10
of 28 plain scores (5 better); the typed rewrites beat plain in 4 of 26 and lost in 5. As in the pilots, the
header types moved little; the dumpling arrays fix a wrong scale in the drafts but did not lower their rows.

### Matched (908 bytes)

- `func_8013E308` (372 B): port of SHOUGATU `func_8013B758` (`tools/siblings.py` ratio 1.0, clone); the
  byte-typed store as the call argument is the marked T-9020 FAKE idiom (`D_800F62CF = 0xC` as the argument
  gives other registers).
- `func_8013DC2C` (268 B): port of GYOZI `func_8013AC38`, with the bit-3 store `GyoziBit3` on
  `girl[10].unk_10[0]` and `D_800F53A0.unk_10B8 = 1` (see the type map).
- `func_8013B0C4` (268 B): the 29-row redraft with `GzCallbacks90 table` and an index local declared before it
  (T-3330: its slot sits above the table, moving the table from sp+0x38 to sp+0x34).

### Near misses and leads

- `func_8014206C` (6 rows, `drafts/T-17600/GYOZI/func_8014206C/hand.c`): only the three `beq v0,v1` come out
  as `beq v1,v0`; a local copy, `7 != x`, `!(x == 0 || x == 7)` and a switch did not change it.
- `func_8013C774` (14 rows, `hand.c`): the table slot matches with the index local; the literals `"S\n"`,
  `"T\n"` (rodata `D_80145BB0`/`BB4`) shift the data by 0x10, and the original keeps nops where IDO fills the
  delay slots with the `D_800F53DF` loads.
- Sibling leads for the packages: `func_8013E824`, `func_80139E4C`, `func_8013907C` against SHOUGATU
  `func_8013BDE0` (0.93, 0.94, 0.86; earlier probes reached 24-27 rows, see "Failed probes"),
  `func_80141944`/`func_80145384` against `func_80140780` (0.80/0.75).

### Packages

T-17601..T-17608 (`w3-gyozi-p01`..`p08`): 33 functions, 19,180 bytes.

## Legacy polygon clone (T-24017)

[[tickets/T-24017-clone-ports]] ported matched ENDING func_80139B48 to legacy EVENT func_80108B24 in src/ovl/EVENT/80107FC0.c. Fresh default linked literal proof confirms 476 bytes as unguarded C (no target INCLUDE_ASM), and EVENT SHA1 passes after rebuilding all its header-dependent objects. Keep legacy helper names: func_80019D00 returns the consumed packet pointer; call-site views for func_80067678, func_800675A8 and func_800674F4 have unused returns. The legacy word buffer index is D_800E9600; its ordering-table stride is 64 bytes, versus 1024 in the GEKO/ENDING siblings, and its address-only base is D_800B2580. No semantic rename was made. EventStateView names signed two-halfword coordinate pairs at +0x14 and +0x18 in previously padded bytes; all later offsets remain unchanged. Shared declarations are centralized in main_api.h except allocator 0x80019D00, outside the current main range, in EVENT.h. Inherited FAKE colour masks/source-line trios are retained and ticketed. Root coordinates all-27 shared-header integration checks.
## T-24019 GYOZI sibling completion

func_80142A8C matched 392 default linked bytes by porting DATE func_80152E34 with SDK POLY_G3 and existing GyoziWork. Local work pointer plus comma colour assignments match; direct global reads leave22 rows. No new FAKE or header change. Explicit target ASM replaced, GYOZI checksum and default literal no-build comparison pass; shared declaration checks pass. Other three assigned selector ports remain under bounded investigation; earlier T-13201 negatives retained. See [[tickets/T-24019-gyozi-sibling-ports]].

T-24019 final bounded outcome: one of four matched (392 bytes). Selectors remain ASM. E82424/24 and 39E4C27/27 reproduce the T-13201 negatives; source-line grouping and meaningful copy types do not close them. 3907C's natural control flow plus meaningful s32 result local recovers sp+0x2B, leaving56 rows/12 aligned edits (base allocation, LUI and switch-index scheduling). Its selected source corrects the byte-table index and needs a future header declaration. Unresolved forms and exact blockers are preserved in drafts/T-24019/; all loose probes retained in ignored build/T-24019-probes/. Ticket In Review, no compiler-limitation claim.
## T-24018 source-based movie siblings

DATE2 func_80133AF0 ports to EVENT func_8011C750 through existing movie/state/Rec24 views. After callback/resource substitutions and the 300-frame timer, a direct exit-state reload improves 317 to 167 printable diff rows (924 versus 925 target instructions); callback return views and page-byte access forms do not match. DATE func_80147958 ports to func_8011DDB8 only after adapting EVENT's sound/timer/resource-selector/callback cases; it remains 357 rows, frame 0x68 versus 0x60, and T-23004 remains the better source. Numbers use printable rows and are not directly comparable with T-23004's normalized metrics. No production source/header change or new matching C. Drafts and reproducible scratch-only return views: drafts/T-24018/ and [[tickets/T-24018-large-sibling-state-machines]] (In Review).

## T-24028 short movie source port

DATE/DATE2 source structure recovers EVENT func_8011D5C4 to seven differing instructions (14 printable rows), exact 509 instructions and 0x40 frame, versus T-22009's distant draft and wrong frame. All record loops, fades and callback spills match. Only timer narrowing has the same raw/masked range split as DATE func_8014718C's old T-17201 gap. The selected scratch body needs s32 own-return view; existing void header and production INCLUDE_ASM remain unchanged. No new C bytes or header change. Source-only near miss, tested reproduction and failed hypotheses: drafts/T-24028/; [[tickets/T-24028-date-event-short-movie-siblings]] In Review.


## T-24033 VALEN selector sibling retry

[[tickets/T-24033-valen-selector-ports]] recovers a seven-row guarded form of func_80132D0C: use direct girl accesses through a local GameState pointer and a meaningful s32 mode local. This fixes prior operand allocation; only early/late dispatch shift scheduling, one nop and branch offsets remain. func_80134030 reproduces the27-row default-block LUI gap shared with GYOZI39E4C. Zero matches or verified-byte gain; default VALEN checksum and header/global/prototype checks pass. Selected sources and bounded negative provenance are in drafts/T-24033/.
