---
type: research
updated: 2026-10-17
---

# ETC bounded source-family transfer

Scope: [[tickets/T-13100-etc-families]] and children [[tickets/T-13101-etc-career-family]], [[tickets/T-13102-etc-exam-family]], [[tickets/T-13103-etc-memory-card-family]], from aa2b3158 in codex/etc-13100. Original checkout read only; all compilers/analysis used existing Docker image, one CPU/compiler. Production changes limited to four ETC C files and include/ovl/ETC.h.

## Accepted evidence

Eleven functions add **5900 literal linked bytes**:

| Source file | New functions | Linked bytes |
| --- | ---: | ---: |
| src/ovl/ETC/8014ABD0.c | 3 | 2400 |
| src/ovl/ETC/801492C0.c | 4 | 1132 |
| src/ovl/ETC/80146400.c | 1 | 176 |
| src/ovl/ETC/80140F50.c | 3 | 2192 |

All eleven pass tools/funcdiff.py --resolve --version default; final ETC SHA1 check passes. SHA1: 681c93b6e57c885f86e074c885a8bf6c3cca2319. Proof: build/etc13100/final-proof.log and final-build.log. Header/global checks pass; prototype check result recorded in umbrella ticket. A function-body text audit against aa2b3158 preserves **91 pre-existing C bodies** across all touched source files (build/etc13100/body-audit.log). No new FAKE idioms, overrides, compiler changes, assembly edits or NON_MATCHING guards.

GameState pointer transfer is confirmed on four functions: 8014B1B4 improves 4 -> 0 full-TU rows; 80140FD8 improves 15 -> 0, 80141E10 and held-out 80142574 each improve 104 -> 0. All aggregate accesses use the pointer consistently. The other seven accepted functions match with direct fields; the pointer is not a universal improvement. Actual pointer arrays recover the dialogue/career table loads, and void forward declarations reflect existing callee definitions. Existing CharFlags f9/b1 accesses improve the correctness of rejected exam drafts but produce no accepted match here.

## Bounded negative results

- Career 8014A360: 26 -> 4 rows after initializing the actual selector local and assigning the final default value after its call; remaining two HI16 instructions are swapped. Width changes, assignment-in-argument and same-line scheduling probes do not match. Original assembly retained.
- Career 80149F48: original typed draft 176/166 rows; structured reload/load-order source 140 rows. Not promoted.
- Exam 80146D4C: existing EtcWork40 view 310 rows, pointer form 192. Ranking draw 80148BC8 stays 40 rows with array/CharFlags correction and bounded operand/direct-argument forms. SDK SPRT drawing 80148F7C stays 336/338 rows. The other large rendering drafts need buffer/layout recovery; guessed padding not promoted.
- Album interaction 80140AE4: 48 rows; ordinary switch 54/58. Album loader 8013FFAC: six-word texture-buffer recovery 521/467 rows. A temporary missing shared-byte declaration used to compile that failed draft was removed; main_api.h is unchanged.
- Related card initializer 80142B80: 240 rows for both aggregate forms; ordinary case stores replacing m2c shared goto worsen to 272. This Bunka idiom does not transfer here.

All rejected variants stay in ignored build/etc13100. Do not repeat these bounded source-form probes or the earlier broad default sweep without new evidence. The next ETC gains need focused loop, buffer or shared data-layout reconstruction rather than another uniform pointer rewrite.

## Census boundary

Source census after this pass: ETC 349/394 functions, 70936/107284 bytes (66.1%, baseline 65036 bytes). Whole game 5870/6958 functions, 1399964/2279368 bytes (61.4%). SDK assembly excluded. These census numbers are not a fresh 27-binary checksum claim; this lane rebuilt and verified ETC. The broad 80% objective remains ongoing.

## Review and final gates

Repository code-review skill, aa2b3158...aa1c51ec: GPT-6.1 Sol medium standards and spec reviews both pass without findings. The spec review documentation observation was resolved by recording the completed prototype guard in all four tickets: **0 new / 15 known disagreements**, branch check OK. Tickets moved to Done after both review axes and guards passed. No production edits followed review. Final branch is codex/etc-13100, ready for integration; no push or PR.

## T-14800 album dispatch follow-up

[[tickets/T-14800-etc-801447b0]] accepts func_801447B0 (648 literal linked bytes). The census direct-field draft had only a final case-4 status-byte register difference (t9 versus v0, six diagnostic rows). Using the existing GameState through one consistent local pointer across state/status/counter fields yields zero full-TU diagnostic rows, then passes explicit default linked-byte proof and the ETC SHA1. This is a focused transfer from the confirmed pointer family above; no broader sweep was needed. Original census source remains archived unchanged.

## Types-first pass (T-17300)

T-20900 update: `EtcRec44` is `DisplayActor44`; `EtcMat`, `EtcMat2`, `EtcVec`, `EtcVec2` are `MATRIX` and `VECTOR`; `EtcSlot` is `ScheduleClubFlags` (`grade` is `unk_25`, `flag` is `b30`); `FlagsWork24` is `Work800E7D10` (the `EtcConfig8` records stay a cast view) ([[data-types]], "Shared-type unification").

Wave-3 types-first pass ([[tickets/T-17300-types-first-etc]], [[speedup-research-2026-10-16]]), branch
`w3-etc-types` from `w2-good`. Start: 43 unmatched functions (34,724 bytes), 2 of them guarded. About 80 minutes
of agent time for the survey, the types, the redraft and the near-zero matches.

### Survey

- `tools/type_recovery.py --unit ETC`: most "high" proposals are jump tables. The useful ones were the 0x8C
  block at `D_80150668` (stride 2 then stride 4, four users), the 0x2D4 word table `D_8014FA84` (already
  `s32[]`), `D_8015014C` (indexed by `D_80150148`), the three shorts at `D_8015013C` and the 8-byte stride of
  `D_8014DB90`.
- Census drafts (`drafts/T-14500` catalog paths): the byte-offset casts sit on GetWorkBase primitives
  (`GetWorkBase(0x28, ...)` is a `POLY_FT4`, 0x14 a `SPRT`, 0x18 a `POLY_F4`; 80 casts in `func_80132C3C`
  alone, also through a stack array of primitive pointers in `func_8013D32C` and `func_8013D570`), on a local
  set to the `D_8011ECD0` display records themselves (`func_8014790C`, `func_80143758`), on `D_80121818`
  (`&D_801217D0[2]`, `func_8014407C`) and on the shared work area `D_801230D0` (`func_80146D4C`). No
  stack-copied constant tables (unlike DATE and TAIIKU).
- `tools/siblings.py --unit ETC`: `func_80144AC0` is a clone of main `func_8005C5BC` (1.0);
  `func_80145318` of NAME_ENT `func_80142028` (0.855); `func_8013E524` of TACO `func_80153E78` (0.856);
  `func_80144F74`, `func_80136448` (NAME_ENT), `func_8013BFFC` (ETC `func_8013C330`) at 0.75-0.77.

### Type map

All in `include/ovl/ETC.h`; ETC's sha1 stayed OK after each step. Matched code changed only where a
declaration changed (`D_8015014C[0]` in `func_80143240`; `EtcWork40` moved out of `80146400.c`).

| Type | Symbols | Evidence |
| --- | --- | --- |
| Exam ranking, three symbols since T-22029 (was `EtcExamRank`, 0x8C; a struct view made uopt hoist a base register): `s16 D_80150668[14]` value, `s32 D_80150684[14]` order, `s32 D_801506BC[14]` rank, scalar names for rows read by constant index | `D_80150668` (`D_801506F0` is `rank[13]`) | `func_801480D0` fills it (debug prints `[%d:%d]` of value, order, rank), `func_8014790C` and `func_80148BC8` draw it; the layout is the one the T-14500 repair draft of `func_801480D0` found |
| `s16 [14]` | `D_80150630`, `D_8015064C` | x and y of the 14 ranking rows (`func_8014790C`, `func_80148BC8`) |
| `u8 [13]` | `D_80150728`, `D_80150738`, `D_8014E0A4` | per-girl base score and spread of the ranking; per-girl pick counts of `func_8013C8EC` |
| `s32 [13][2]` | `D_8014DB90`, `D_8014DBF8` | two words per girl passed to `func_80046290(s32, s32, s32)` (`lw 0` / `lw 4` at `unk_F5F * 8`) |
| `s32 *[13]`, `s32 *[22]` | `D_8014E070`, `D_8014EA18` | per-girl pointers to the same word pairs; CD file tables per scene (`dec_bg_cd_read(D_8014EA18[a][b])`) |
| `SVECTOR` | `D_8015013C` (was `s16`) | RotMatrix input of the matched `func_801433E4`; `vy`, `vz` move in `func_80143A88` |
| `s32 [16]` | `D_8015014C` (was `s32`; absorbs `D_80150150`, `D_80150188`) | word 0 is a flag, words 1..15 the order of girls stepped through with `D_80150148` |
| `EtcWork40` (moved from `80146400.c`) | view of main's `D_801230D0` | `func_80146D4C` writes the day's score to `unk_28[subject]` (total in `unk_28[5]`) and the grade to `unk_10[subject]` |
| scalar widths | 14 overlay scalars (`D_8014DB80`, `D_80150078`, `D_801500C0`, ...) | load/store widths of their only (unmatched) users |
| `#include "psx_types.h"` | | the GetWorkBase primitives for the redraft and the matches |

Not changed: `D_8014FA84` (already `s32[]`), the `D_801500A0` byte/short table (mixed widths, one user),
`D_80150190` (CLUT data of `func_80142B80`), the 0x50-byte `D_801227A0` records (shared `TaiikuBig`
declaration in `main_api.h`).

### Redraft

`drafts/T-17300/redraft.py` is a copy of the T-17200 driver (plain and typed runs through the census path,
the T-16550 fold, the T-16560 rewrites, `tools/draft_fixes.py`, finish) with these additions:

- **work-prim**: a local set from `GetWorkBase(size, ...)` is a pointer to the libgpu primitive of that size
  (0x28 `POLY_FT4`, 0x14 `SPRT`, 0x18 `POLY_F4`, ...); byte offsets on it become fields, also through a
  pointer-to-pointer that stores it (`(*var_s0)->x0`) and the stack array it walks.
- **display-base**: a local set to `D_8011ECD0` itself is a `DisplayActor44 *` (`var_s0[97].unk_01`).
- **work-view**: m2c sees main's `D_801230D0` as `EtcWork40`; fields are written through
  `((EtcWork40 *)&D_801230D0)->`.
- **Rec24 fold**: labels inside `D_801217D0[12]` fold to the table (`D_80121818` is `&D_801217D0[2]`).
- **lui pairing** (`typed`): a `lui %hi(X)` whose `%lo` uses all name X keeps the full offset. With the bare
  base (T-17200 fold, kept as the `typed1` run) m2c printed `D_800E6280.unk0` for a `%lo` behind a branch,
  which broke 10 typed drafts.
- Driver fixes: cfe warnings are dropped from the 12-line compile tail `tools/funcdiff_verify.py` keeps (the
  warnings pushed out the "redeclaration" line that `exhaustive_drafts` repairs on), and a function called
  earlier in its file without a prototype is drafted as `s32`.

Results (scores are full-unit diff rows, as in the census):

| | value |
| --- | --- |
| functions redrafted | 40 (43 minus the 3 matched below) |
| unguarded functions with a compiling draft | 36 of 38 (census: 38); `func_80141BE8` and `func_801480D0` compile in no run |
| better than the census | 11 functions, 8,316 bytes |
| same / worse | 20 / 5 |
| sum of diff rows (36 functions) | 6,856 to 6,532 |
| headers alone (plain run vs census) | 7 better, 22 same, 5 worse |
| typed (best of `typed`, `typed1`) vs plain | typed better in 7 of 25, worse in 15 |
| `tools/draft_fixes.py` | no score changed |
| score 0 | none |
| guarded functions (census = hand guard body) | `func_80143F24` 23 to 51, `func_80146400` 14 to 67 |

Largest gains: `func_8013C8EC` 244 to 92 (plain: the `D_8014E0A4` and `D_80150078`/`D_801500C0` widths),
`func_80146D4C` 309 to 180 (work-view), `func_80143A88` 422 to 345 (`SVECTOR`, `D_8015014C[16]`),
`func_80148BC8` 116 to 64 (plain: `EtcExamRank` and the row arrays), `func_8014A360` 117 to 84,
`func_8013CF2C` 173 to 149, `func_8013D32C` 74 to 63 (work-prim). The typed rewrites lost more often than they
won in ETC: byte arithmetic on the new `s16[14]` arrays (`&D_80150630 + i * 2`) and selector unions stop
several typed drafts from compiling (`func_80148BC8`, `func_8014790C`, `func_80144224`, `func_80143758`).

Matched during the pass (1,572 bytes), all sibling ports from `tools/siblings.py`:

| Function | Bytes | How |
| --- | ---: | --- |
| `func_80144AC0` | 516 | main clone `func_8005C5BC` (1.0); only the `func_80042940` state code differs (2 for 4) |
| `func_8013E524` | 392 | TACO `func_80153E78` (0.856): `POLY_F4` bar plus tpage; colour stores in b, g, r order |
| `func_80145318` | 664 | NAME_ENT `func_80142028` (0.855): s32 with no return, so the incremented step stays in `v0`; ETC.h prototype now s32 |

### Near misses

- `func_8013C700` (4 rows, `hand.c`): the `func_8013C330` shape matches except the order of `or v0,zero,zero`
  and `sw zero` (of `D_80150070 = 0;` / `D_8015006C = 0;`) around the branch delay slot. In the original
  `D_80150070` is loaded into `v0`, in the sibling into `v1`; an explicit `return 0;` changes nothing.
- `func_8014354C` (`hand.c`): the TAIIKU `func_801406A8` loop shape gives the loop; left are the prologue
  order of the `0x801F9004` constant and the order of the three `SVECTOR` stores (IDO sorts stores to one
  struct by offset; the original has vz, vx, vy).
- `func_8013F980` (`hand.c`): the same loop with the `NOBJ %x` print; prologue scheduling rows left.
- `func_8013D32C` (63 rows, typed): a two-element `POLY_FT4 *` stack array walked by pointer; the frame is 0x78.

### Packages

Eight file-disjoint packages from the orchestrator's `mklists3.py` (lists in its scratchpad,
`w3-etc-p01-list.txt`..`w3-etc-p08-list.txt`):

| Package | Ticket | Files | Functions | Bytes |
| --- | --- | --- | ---: | ---: |
| w3-etc-p01 | T-17301 | `8013BEF0.c` | 5 | 2,816 |
| w3-etc-p02 | T-17302 | `8013B1F0.c` | 1 | 3,120 |
| w3-etc-p03 | T-17303 | `80146400.c` | 4 | 2,896 |
| w3-etc-p04 | T-17304 | `80132880.c` | 4 | 4,320 |
| w3-etc-p05 | T-17305 | `80142B80.c`, `80140F50.c` | 3 | 2,692 |
| w3-etc-p06 | T-17306 | `8013F980.c` | 2 | 1,048 |
| w3-etc-p07 | T-17307 | `80143240.c` | 6 | 2,072 |
| w3-etc-p08 | T-17308 | `801492C0.c`, `8013FF80.c` | 4 | 3,436 |

## T-24012 dispatcher checkpoint

[[tickets/T-24012-etc-render-family]] verifies `func_80136448`, 1,280 literal linked bytes, in `src/ovl/ETC/80132880.c`. The known NAME_ENT `func_80140044` sibling confirms the dispatch family. The void return view leaves one instruction out of the shared epilogue and shifts branch displacements; an `s32` view with no explicit return reproduces all 320 instruction words and passes explicit default linked-byte proof and the ETC checksum. The view is marked FAKE since the historical source declaration is unknown. Its existing void callees gain accurate overlay-header declarations. Header/global guards pass; prototype guard reports 0 new / 15 known disagreements. Proof logs: `build/etc24012/final-proof.log`, `final-build.log`, `header-guard.log`, `global-guard.log`, `proto-guard.log`. Other assigned rendering functions remain assembly; review pending.

### T-24012 remaining rendering drafts

The follow-up pass adds no verified bytes. `drafts/T-24012/` preserves three source-only compiling drafts, independently measured in the reviewed full TU: RGB filter `func_801328A4` cost 217, SDK quad renderer `func_80132C3C` 180, background selector `func_80132FB0` 150. Costs are aligned differing-instruction diagnostics after relocation, not match proofs. The RGB draft recovers a legal flat 822-record buffer, correct frame/RECT position, and remains blocked on the +24-byte row base and saved-register/arithmetic allocation. Quad frame/spill and colour loads differ. Background code uses the real 22-pointer table and legal absorbed-alias indices, but original address rematerialization differs from compiler base sharing. Draft README records the bounded rejected variants; no guessed frame padding or scalar aliases were added. Production remains at the reviewed dispatcher checkpoint and the other three functions remain assembly. [[tickets/T-24012-etc-render-family]] is In Review for the archive delta, with those targets pending.

## T-24038 signed-number renderer

[[tickets/T-24038-etc-master-siblings]] recovers `func_8013FC64` in `src/ovl/ETC/8013F980.c`: 796 default linked bytes match, and the ETC checksum passes. MAIN `func_8004AE54` supplies the digit-loop family, while this target uses full SDK `SPRT` packets with explicit 8x8 dimensions and no enable guard. Division precedes the digit increment. A real sign-offset local and explicit sign count increment reproduce the 0x78 frame without padding or new FAKE code. The approved 20-byte font declaration describes ten coordinate pairs and preserves the separate sign bytes. Proof and guard logs are under `build/etc-master24038/`; other production bodies are textually unchanged.

## T-24067 focused album-copy follow-up

Source-only checkpoint drafts/T-24067/body.c reaches cost18/28 diagnostic rows with exact0x48 frame and75 instruction words. Reading the stored mode byte restores the final printf/store schedule. A FAKE format-pointer identity test retains the format range and restores s2=format/s3=end/s4=save. The end uses canonical GameState+0x800; no header/export change. Remaining differences are preheader order, two equal-value stack-argument stores and copy scratch t5/t6. Existing private tracing confirms an equal-priority first-use tie before the identity workaround, with code/data/relocations identical to stock. Production unchanged; [[tickets/T-24067-etc-album-copy]] remains In Progress.

T-24067 follow-up improves the checkpoint to cost16/20 rows by retaining initializer and do-header source-line grouping (explicitly FAKE scheduling purpose). The native listing ties this to loop-constant materialization source location. Source/body replay preserves exact grouping and canonical endpoint; no alias or interface changes. End/save HI and LO pairs, two stack stores and four copy words remain. Earlier cost18 frozen at eadad3134; production stays assembly and ticket In Progress.

T-24067's natural indexed GsRec75E loop reaches cost16/16 rows, reproducing argument-stack and preheader scheduling without the earlier format identity. Current drafts/T-24067/body.c retains only a FAKE count/for line group. Eight differing words remain, split equally between end/save saved assignment and second-copy t5/t6. Earlier complementary cursor16/20 source is cursor-body.c; safe replay selects indexed/default or --variant cursor. Production unchanged; target active.

Follow-up indexed controls retain that checkpoint: inclusive/do-loop16/16, while limit/narrow/top-exit forms lose induction elimination and split-copy/local-pointer forms add differences. Indexed tracing confirms an equal-priority format/save/end tie; save is numbered before end. The second-copy discrepancy is consistent with the existing T-20710 B/F state evidence in [[ido-52-evaluation]], without a new4.1 experiment. No source promotion or external-blocker claim.

The exact cursor form's explicit end entry store raises its priority2.75 above save2.5; indexed end/save remain tied3.33333325. Endpoint-derived index bounds do not fold cleanly. Source-only drafts/T-24067/capture.py validates unchanged stock code/data/relocations while retaining uopt input/output in ignored build storage. The next specific evidence gap is constant identity at bit creation, since current tracing shows only final numbering. Best source remains16/16; no production or compiler changes.

Parent-approved private creation logging answers that gap: allocation preparation first encounters format in block2, save in block7 and end in block8, assigning bits54/55/56. Whole object SHA and all code/data/relocations equal stock. Logger generator/provenance is retained in drafts/T-24067; shared tracer and production tools stay unchanged. A single derived goto-first loop form normalizes to16/16, so it is rejected. No source promotion.

Early rejection/continue supplies the next source gain: cost8/8 rows, all saved end/save allocation corrected, leaving only four second-copy t5/t6 instructions. Tagged full-TU explicit-object default replay confirms exact frame0x48/75 words. Current body.c holds this form; indexed-body.c preserves16/16 and cursor-body.c16/20. No production promotion; [[tickets/T-24067-etc-album-copy]] remains In Progress on the copy subgroup.

A separate FAKE store-control.c demonstrates current5.3 can select all expected copy registers after an extra direct stack store. Explicitly rejected for its added SW, frame0x58 versus0x48 and branch displacements (15 diagnostic rows); it proves no4.1 necessity. Required printf/global-tail stores cannot simply move to that point without changing argument/mutation semantics. Aggregate volatile-copy view is neutral8/8. Production remains unchanged; backend address/state classification is the next read-only question.

Read-only existing ugen reference distinguishes aggregate stores (eval_mov calls emit_rob directly) from scalar store classification. Its normal unaligned eight-byte path allocates/frees one scratch before selecting AT and the second scratch; actual offsets do not select the alternative large-offset paths. Input/generator/reference hashes and locations are pinned in drafts/T-24067/README.md; no regenerated compiler was built or run. A conditional wide-integer allocation diagnostic was rejected before compilation because the project has no C89 64-bit integer type. Four differing words remain, no production changes, ticket In Progress.
