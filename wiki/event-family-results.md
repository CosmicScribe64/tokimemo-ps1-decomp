---
type: research
updated: 2026-10-16
---

# GYOZI and SHOUGATU event families

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
