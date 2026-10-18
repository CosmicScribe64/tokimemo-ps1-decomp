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
## T-24049 exam ranking semantic reconstruction (2026-10-18)

func_801480D0 remains ASM. The T-14700 types repair is adapted to the current three arrays and existing EtcWork40 total-score field in drafts/T-24049/exam-ranking.c. Signed casts on byte spread divisors recover the target signed div operation. A substantive old-draft error is corrected: its final bonus updated15 records, but the target peels3 stores and loops over2 groups of4, updating exactly11. The malformed Shift-JIS debug spelling is restored to the source string. No header changes are needed.

Current full-TU default diagnostic is885 rows,709 words versus702 target, frame216 versus208. Locals before the array do not change it. Selector/noise lifetime split, a volatile noise spill, separate generation counter and explicit next tie indices all worsen (987/1004/1053/1071 rows); README records each. The target's four saved registers, noise stack spill and loop range lifetimes remain unrecovered, so this is not a register-only closer. Annotated source reproduces885 via drafts/T-24028/probe.py. Production/header unchanged; independent ETC SHA1 control passes through ASM. Zero new C bytes. [[tickets/T-24049-etc-exam-ranking]] is In Review for root review.
## T-24055 short model and scrolling helpers

[[tickets/T-24055-etc-short-helpers]] leaves all three targets as ASM, zero matches/bytes. Fresh selected model4354C reproduces historical13/13 with split model pointer and actual SVECTOR; prologue and vz/vx/vy grouping remain. Fill43758 corrects stale first-coordinate word reads to bytes, uses real actor fields and reaches111/109; index/single-base forms193/194 are worse. Scroll43968 corrects byte reads and reaches70/70; preserving its record index before the helper call changes to109, direct counter74/72, return liveness70. Original counter caching and multiply-index lifetime remain. No production/header/config change; baseline ETC checksum passes. Source bodies, reproducer and bounded evidence in drafts/T-24055/README.md; all instruction traces ignored.

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

Later parent-authorized logging-only ugen diagnostic passes same-input whole-object and all code/data/relocation equality. Its free-list trace isolates destination evaluation (head10→12), source neutral, dummy burn12→13 and second-copy scratch13=t5. 0x10019DAC is FP-free, not integer pending. Generator/audit/provenance retained in drafts/T-24067. One derived C89 block-local register destination pointer leaves the entire object identical and is rejected as neutral. Production unchanged, four-word source checkpoint retained, ticket active.

Separate audited comparison of the existing rejected store reveals identical incoming free lists but stale backend frame slot sp+232=9 versus0x10000. Original ugen branches over the only local assignment for indirect destinations, then reads that slot to select one versus two scratch burns. This explains t5/t6 in these two inputs without an older-compiler claim. No faithful required-expression route established; last-writer provenance of this reused backend stack address is the next precise gap. README retains hashes and exact read/write addresses; no production/toolchain change.

Narrow parent-authorized writer observations now match baseline9 to the required branch's f_jump→f_emit_branch_rrll a2 spill, and store0x10000 to f_unaligned_loadstore→f_loadstore saved gp. Static stack/dispatch analysis preceded a bounded native record of explicitly selected helper stack writes; no broad memory trace. Both inputs retain whole-object and code/data/relocation equality. Address evaluation does not affect the slot. This gives causal helper provenance but no established legal source transformation; no new speculative control or production change.

Required-comparison follow-up finds scalar immediate helper requires a constant RHS; both actual inputs are variable. Operand reversal only changes a spilled register number0..31, still below the copy's stale-value-plus8 threshold0x7FF9. Thus no unsupported flip probe is run. No distinct legal C89 predicate preserving all target words is established; four-word checkpoint remains unresolved, without an exhaustive impossibility/toolchain claim.

Existing52 ugen diagnostic (parent authorized, cached amd64 runtime) accepts the exact captured current53 O/T. Full-pipeline capture and same-stage stock replay fidelity pass first. Both backends then yield identical raw assembled object7211475ad0a8b8410ae27f588b8d5296fcfb0cdebae9e92009627fac6f735629 through current frame/as1, target75 words/frame0x48/cost8/8rows. Raw binasm differs but the same four copy words remain. Source-only ugen52.py/README preserve provenance and limits: not a full52 pipeline or production C proof, no toolchain change.

Parent closed this investigation pass as unresolved/In Review, with no C match or verified byte gain. Four-word source and compiler-mechanism packet remain for independent review; speculative41ABC probes stop and production INCLUDE_ASM remains. T-24067 is not Done.
## T-24064 focused model initialization loop

func_8013F980 in src/ovl/ETC/8013F980.c now matches252 actual default linked bytes. Matched MAIN func_8006190C/T-17010 supplies the integer TMD cursor to retain the header pointer across printf; corrected TaiikuBig/u32 pointer steps give the real16-record loop. Initializer order and FAKE initializer/do source-line grouping fix the final setup schedule (original line layout unknown). Plain shared prototype already matches; no approved-but-unnecessary local override/header/config change retained. ETC checksum, interface guards and exact other-body preservation audit pass, including matched func_8013FC64. [[tickets/T-24064-etc-count-trio]] In Review;40AE4 deferred,41ABC unresolved separately.
## T-24068 focused album interaction

Current full-TU baseline func_80140AE4 reproduces48 rows, correct56 words/frame0x28. Required-call comma sequencing and one FAKE neighboring selector D8+4 word read both remain48 and are rejected; no base/type sweep. Existing sibling report has no match>=0.70. Unchanged read-only allocator tracer passes whole-object fidelity and shows selector bit0 priority0.5/cost0/color3(a0), versus constant1 bit27 priority0.6667/cost1/color2(v1); no equal-priority tie. Specific unresolved question is v0 call-result/cost liveness, not record semantics. Source-only drafts/T-24068 retains controls/provenance; [[tickets/T-24068-etc-album-interaction]] In Progress and production unchanged.

One parent-approved target s32 fallthrough/no-return view, absent exact historical evidence of exhaustion, remains48 rows and is rejected with all callee prototypes/source choices fixed. FAKE diagnostic retained, no result/width sweep. Existing best-color trace cannot distinguish v0 forbidden interference versus call cost; source-level candidate-mask/cost sites identified for a scoped logging request. Production unchanged.

Narrow parent-approved bit0/colors1..3 diagnostic resolves that distinction: mask60000000/0 forbids v0/v1 before cost evaluation; a0 allowed/cost0. Same-input whole object and four code/data/relocation sections agree. Block-mask union over selector bb3/bb11 explains the restriction class, but logs do not identify its exact fixed-range contributor. ONE derived source hypothesis is explicit void discard of the final leading check call before bb3, retaining its s32 prototype/call sequence; no result claimed. Source-only generator/audit/provenance retained; production unchanged.

That one parent-approved explicit final-call discard is48 rows and whole-object neutral, rejected/retained in replay. Next evidence stays existing trace/source mapping of bb3/bb11's v0 mask contributor, without instrumentation widening or another source sweep. No production changes or verified gain.

Parent-approved helper-only bit0/bb3/11 logging, audited whole-object/four-section identical first, isolates the initial forbidden v0 to bb3 source40000000/0; bb11 contributes zero. Exact fixed-marker creation inside bb3 remains unknown. One label-only boundary after the final E80 call normalizes to the identical object/48 rows and is rejected without a label/goto sweep. Source-only trace_blocks.py/replay/provenance retained; no production or verified-byte gain. Further scope awaits parent direction on that precise mask-origin gap.

Read-only mask-creation inspection narrows static candidates to makelivranges initialization468BD4 and explicit register writers469748/46A230; their execution for bb3 remains unobserved. The live R0 offset2 web does not prove the discarded E80 result creates the mask. Missing evidence is writer PC and creating statement fields, rather than another final-mask trace or unsupported source spelling. No new compiler run or production change.

The single authorized writer diagnostic retains same-input whole-object/four-section fidelity. bb3 initializes to0/0, but neither candidate statement writer executes for it. Later localcolor has additional mask creation paths; their target execution/live-range provenance remains unobserved. No faithful C89 source route follows, so no speculative control or further instrumentation is run. Source-only trace_writers.py/README preserve this negative result; production remains assembly and the48-row baseline is unchanged.

Parent closes T-24068 as unresolved/In Review with packet bed9333e1. No C match, verified-byte gain or impossibility claim; production INCLUDE_ASM is retained and further tracing stops pending a later specific scope.

## T-24069 focused menu state machine

Initial diagnostics: func_8013BFFC baseline110 rows; target case3 retains original else-if despite sibling8013C330 using sequential ifs. Branch-specific clears improve103; private void return view is neutral. Consistent GameState pointer improves67. These scores are historical controls, not separate verified gains.

T-24069 verifies820 actual default linked C bytes. One word temporary joins required step/status load ranges (FAKE local-allocation rationale), improving67→5; leaving switch selector uncaptured removes the extra move and matches. Existing s32 caller view matches too, avoiding header changes. Normal-default ETC checksum, headers/globals and prototypes0 new/15 known pass. Exact other-source text and40 neighbor object bodies remain unchanged, including8013C330. Ticket In Review for parent review/integration. Deeper lookup finds T-14500 mechanical baseline110 already existed; fresh110 reproduces it, rather than establishing an unprecedented compiling target.

Parent independent standards review PASS/no findings and all27 hashes/interface/default-C/literal gates pass; published fd700bf3f. [[tickets/T-24069-etc-menu-state]] Done. Source-only controls/reproducer under drafts/T-24069 replay zero without production changes. Authoritative Docker progress at published primary:6514/6958,1865416/2279368 bytes(81.8%).
## T-24070 focused save-card state machine

Fresh current-type func_80144F74 baseline117 rows becomes47 with a consistent GameState pointer; all cases1–3 match and only case0's capped byte-array updates differ. Historical T-20900 scalar-alias46 is not a directly comparable fresh score. One shared-byte counter49 and its word promotion49 are rejected without a width sweep. Production unchanged; [[tickets/T-24070-etc-save-card-state]] In Progress.

Direct array cap/increment source improves39 and restores second cap's a0/call schedule. First-cap index/address/counter allocation and missing masked-index copy remain. Other bounded controls (actual pointer47, byte-index111, raw capture49, reused pointer103, argument capture49) add no improvement. Source-only drafts/T-24070 body/replay reproduce39 with exact unchanged production; no C match or interface/config change. Next evidence question is first-index v0 interference/priority, without more speculative controls.

Unchanged tracer on39 passes whole-object/four-section equality and observes mask-local16 priority3/v0 before retained-index22 priority1.5/v1, then first address76/a0 and counter77/a1. It reports best colors, not forbidden masks. Derived inline-mask105 and retained-element-pointer110 are rejected; removal of named local alone does not establish v0 availability. Source-only fidelity helper/provenance retained, best39/production unchanged. Next parent-authorized observation compares existing tracer on the already-tested105 input, without a new binary or width/color probe.

Approved105 unchanged-tracer comparison also passes same-input whole-object/four-section equality. Global mask expression80 retains priority3/v0, before retained index23 priority2/v1, explaining why removing the named local leaves competition. This does not expose per-range masks. The proposed conditional reload uses unsigned modulo16 versus initial &15, preserving domains; no result yet. Best39, production unchanged, no new binary.

The one approved conditional unsigned-modulo control is neutral105/whole-object identical to inline105 and rejected. FAKE diagnostic/replay retained. Best39 remains; no supported next legal source form identified. Existing trace cannot establish per-range mask/cost details, so further work needs a specific hypothesis/scope rather than another source sweep. Production unchanged, no impossibility claim.

Astra read-only review completed without a supported untried source lead. NAME_ENT's sibling lacks both caps and offers no allocation evidence for them; original t6 mask/indexv0 remains distinct from common-maskv0 in the draft. T-24070 closes this pass unresolved/In Review with39 source/negative controls preserved, no verified gain or impossibility claim. All44F74 tracing stops.
## T-24071 focused menu selection

Fresh func_80144DF0 baseline61 rows uses current fields, captured selection indexing across k_disp_inc, true helper calls and unsigned ready-byte load. Historical plain67/typed130 are separate contexts. GameState pointer65 and selected-byte comparison capture61 do not improve. Original frame38/spill28 versus current30/spill2C and per-arm narrowing remain. [[tickets/T-24071-etc-menu-selection]] In Progress; production/neighbors unchanged, no C proof or guessed spill padding.

T-24071 actual button snapshot reduces61 to57 fixes selection spill28 without guessed storage, but frame30/per-arm byte-value differences remain. Field-readback57 and status frame/spill tradeoffs57 rejected; real Work members61. drafts/T-24071 retains source-only forms/replay. Production unchanged, no actual C proof; In Progress.

T-24071 approved unchanged trace57 and stock-stage capture pass same-input whole-object/four-section fidelity. Masked ranges64/65 already receive v0; final uopt output retains AND255/CVT L/register store, but byte-store lowering removes the ANDI and narrows the call argument separately. This is not a demonstrated v0-interference blocker. No new binary or unsupported source control; drafts/T-24071 preserves source-only audit helpers.

T-24071 read-only ugen reference identifies unsigned CVTL8 argument-mask construction but not the exact earlier mask-removal rule. Proven boundary remains post-cvt uopt output versus final object across ugen/as1. No supported new source condition; no further probes or binary. Source57 unresolved, production ASM.

T-24071 subsequent stock backend capture resolves prior interval uncertainty: identical ugen output/as1 input retains per-arm AND masks, final object removes them, locating deletion in stock as1 processing. Same pinned57 input passes whole-object/four-section identity; backend_capture.py retained. No new source control/compiler binary; production ASM remains.

T-24071 bounded AS1 reference identifies a candidate needed-bit-width ANDI-to-copy gate (func41917c) plus mask-chain combination; runtime gate execution remains unobserved. No supported new expression beyond prior word/argument controls. Precise source gap is per-arm full-word mask retention without redundant narrowing/hoisting; no source sweep or assembler changes.

T-24071 final handoff is unresolved/In Review, not Done. Best57/source controls/fidelity/stage capture preserved; production ASM unchanged. Candidate AS1 width-rule attribution remains a hypothesis without runtime gate values. Stop44DF0 probes/tracing; no impossibility claim.

T-24072 fresh4354C baseline13; verified3F980 integer-cursor/do-loop adaptation5. Prologue/map cursor/loop now exact, only realSVECTOR tail base/store grouping remains. Production ASM unchanged; no header/config edit or actual C proof.

T-24072 retains new integer-recipe5 under drafts/T-24072; zero-identity/pair controls both neutral5. Residual is3 vector AT address groups versus original2 sharing vx/vy. Known SDK aggregate retained without scalar alias or interface/config edit. No actual C proof/verified gain; In Progress.

T-24072 one parent-approved post-vz SVECTOR pointer control is neutral5/full-TU whole-object identical to retained integer recipe. rotation-pointer.c/replay preserved; no further pointer variants/instrumentation, production unchanged.

T-24072 final status unresolved/In Review, not Done. Best5 baseline/controls/replay retained; production ASM unchanged. Stop4354C diagnostics; no verified gain or compiler-impossibility claim.

T-24073 fresh faithful Rec24[2] baseline129, replacing stale array scaling and raw field casts. Original incoming a0 requires one word argument; parent approves narrow unspecified-argument header declaration to preserve matched no-expression caller, no caller change. Private baseline still narrows m2c s16 locals and has frame50 versus48; production untouched.

T-24073 current Rec24 repair129 improves through word values/known MAIN unsigned width84 to early required y-store22 and postincrementID20. Best20 original frame/instruction count, with only counter increment across signedDIV guard unresolved; early IDstore30 rejected. Source-only packet/replay uses approved private header() view. Production/header unchanged, no C proof; In Progress.

T-24073 func8014407C now matches352 actual default linked C bytes at e89ce5f3. Real Rec24/current fields, word values, matched unsigned SDK width view, early required y-store and canonical ordered for-loop recover all instructions. Approved void() declaration preserves original incoming-a0 behavior and matched caller without argument expression. ETC checksum/interfaces/42 neighbor-word bodies pass; In Review pending parent independent all27/publication.

T-24073 parent independent all27/hash/interface/actual-C/literal352 checks pass, standards/spec review no findings. Published main c4847ad5d; ticket/card/index Done. Source/provenance retained.

T-24074 fresh faithful44224 baseline209/frame58 versus38, after real helper arguments/order/Rec24 and required frame reloads across calls. Old stale-frame cache was a semantic error. No production/helper/header/config edit; source-led lifetime cleanup begins.

T-24074 faithful baseline209 improves through ordinary disjoint-value/directlookup/inlinephase cleanup to166/exactframe38. Current source corrects actual helper arguments, real arrays and post-call frame reloads. Pointer226 and quotient type/order controls do not close it. Remaining copied-frame/quotient allocation, table bias/redundant stores and later fade scheduling; source-only drafts/T-24074 retained, production/helper/interfaces unchanged. In Progress, no C proof.

Parent-approved bounded post-review cursor control uses actual order pointer and
post-increment loads only in the interior table arm. Valid default-resolved
full-TU diagnostic241/frame40 is worse than166/frame38; orderindexv1 becomesa1,
quotient spillsp34 becomessp3C. Rejected and retained as source-only replay.
No other source choices, production or interfaces changed; ownership remains
active In Progress pending a concrete next lead, without further pointer sweep.

T-24074 final bounded handoff is unresolved In Review: best166/frame38,
post-increment cursor241/frame40 rejected. Required post-call frame reload
corrections and all source controls/replay retained. Production remains ASM;
no supported next faithful form or compiler-impossibility claim. Further
probes stopped under parent direction.

Fresh faithful full-TU corrected-pair baseline92/frame30. Actual object has six
DIVU, zero signedDIV: K&R u8 promotion already supplies unsigned modulo, so no
explicit unsigned cast control is justified. Joined disjoint remainder locals
worsen116; rejected. Baseline extra s1 constants and eliminated repeated global
stores motivate one ordinary global-assignment/short-circuit rejection-loop
control, preserving RNG call count and rejection sets. Production unchanged.

Ordinary global assignment plus short-circuit rejection loops improves92 to12
resolved rows, exact frame30/226words. All six RNG loops now agree, including
repeated global stores and at-immediates. Remaining six real words rotate
index/base/shift among t5/t6/t7 and reverse ADDU operands in the tail pair lookup.
Actual s32* pair loads and equivalent address operand reversal each remain12;
no sweep follows. Source-only checkpoint/replay in drafts/T-24075, production
ASM unchanged, ticket In Progress pending concrete source investigation.

Matched same-TU8013F320 uses direct arguments from the same s32 pair table.
Porting that source idiom closes all six tail words: private default-resolved
full-TU zero/904B, pending actual production proof. Sole caller8013BF34 ignores
result, original epilogue does not establish return; retain existing implicit
s32 matching view with conservative FAKE note, no header/config change.

T-24075 production5d083af5 passes actual default linked904B and ETC checksum.
Global rejection loops reproduce repeated stores and immediate comparison
constants; matched same-table8013F320 direct pair arguments close the six tail
words. Real GameState/word-pair types, no shared interface/config changes;
unproven s32 fallthrough view conservatively FAKE-tagged, sole caller ignores
result. All34 other function words and neighbor text unchanged. Headers/globals
OK, protos0new/15known; In Review pending parent all27/integration.

Parent independently verified T-24075 literal904B/actual-C gate, all27 checksums
and interfaces, with full standards/spec review no findings. Published
mainc7690151b; ticket/card/index Done. Exact production5d083af5 and source/replay
provenance remain preserved. Coverage measurement pending, no inferred totals.

Root Docker measurement at publishedc7690151b:6516/6958 functions,1866672/2279368 bytes (81.9%),412696 remaining; gain904 bytes. T-24075 is Done after independent review/publication.

Replay handoff correction: parent found the old helper required the removed
ASM fallback after promotion. It now exact-matches/replaces the pinned current
C body, strips only archive banners and asserts production unchanged. Fresh
Docker replay on matched checkout reports zero; verified production5d083af5
unchanged and Done status retained.

Fresh faithful current full-TU baseline174 rows, exact frame28. Record view uses
whole-GameState byte offsets with integer loop index through0, plus actual saved
album bytes, correct one-arg helper and no incomingarg2. This is new measurement,
not historical249/238. Original predecrement/unsigned address-bound loop differs
from generated biased-pointer induction, motivating one target-address cursor
control without forming a before-object C pointer. No production/header change.

Predecrement/unsigned target-address cursor improves174 to162; natural shared-
call short-circuit tail143; selected-code word local139 removes extra ANDI;
selected-event-type word local103 restores original constant2 sharing with mode.
Byte record fields remain unchanged, frame28 exact. All source-only controls and
replay retained in drafts/T-24076; production/header/config unchanged, In Progress.
Remaining index-load sinking, register/web/global reload and branch differences
are unresolved; no match or impossibility claim.

Consistent GameState pointer from matched same-TU E6AC restores final F5F
reloads but worsens103 to107; retained negative, no pointer sweep. Inline FAKE
now explains whole-GameState+756/index0 crossing known subarray and integer
address bound, with original C view unproven. Annotated best replay required
before pin; no production/header/type-layout redesign.

Annotated103 freshly replays103 with inline FAKE whole-GameState+756/integer
cursor rationale. Narrow scalar field-address views on only the two final F5F
conditions restore original independent reloads and improve to90, explicitly
FAKE-tagged. Signed raw-code comparison views worsen294 and are rejected; no
additional type alternatives/sweep. Best90 retains frame28 and177words versus
179 original; production/header/config unchanged, active source-only checkpoint.

Parent/Astra unsigned search-index/code reuse is neutral90 and whole-object
identical to void best90; retained, no further reuse forms. Separately authorized
no-arg s32 fallthrough on originalbest90 improves to71, restoring epilogue/join
placement. FAKE original-return uncertainty is inline; no value invented and
private forward matches actual selected signature. Best71/frame28/178words versus
179 original, early cached-index/search and scratch gaps remain. No production/
header/config changes; checkpoint active pending bounded coordination.

T-24076 final handoff is unresolved In Review, best71/frame28/178words versus
179 original. Production remains ASM, all source controls/replays and inline
FAKE boundary/reload/return caveats preserved. No helper/header/config or
neighbor changes, no matched/Done/impossibility claim. Further F054 probes stopped.

Read correction before baseline: retained PLAIN165 already selects the actual
D_8014DBF8[girl] row correctly; only typed archive has malformed whole-array
arithmetic. No untried overall pair-indexing fix is claimed. Original full VRAM
8013D118..8013D12C confirms eight-byte row/two words; B118 was ROM-offset shorthand.
Fresh baseline uses actual fields, correct row pointer/columns and real Rec24
flags64/65. Scratch-only actual one-word void E1DC/F320 forwards; no shared edit.

Fresh current full-TU row-pointer baseline132/frame28. It exposes a necessary
state-byte view: shared GameState.unk_110C is s8, so direct increments use LB,
while original target reads all state values with LBU. Next exact u8 address
view preserves original byte reads/writes without changing the shared header.

Unsigned state view reaches122; direct-pair call and natural default are neutral
122 and whole-object identical. Target-only s32 fallthrough126 rejected, no
helper/caller overrides; current ETC caller407F0 ignores result. Matched BFFC
word-join alone122, complete consistent-GameState plus word-join recipe102;
explicit FAKE graph purpose. Original206 words/frame28 versus best206/28.
Remaining girl/case2 statusv0-v1, later scratchrotation and early terminal
constant scheduling gaps; source-only packet/replay drafts/T-24077. Production/
headers/config untouched, active In Progress, no match/impossibility claim.

T-24077 final unresolved In Review preserves102/frame28/206words. Bounded Astra
read-only review found no supported untried form: shared-word recipe already
present; scratch rotation/equal count do not isolate missing op/lifetime; no
observed dependency delays early flag constant. No additional probe, no match/
Done/impossibility claim. Production ASM; active packet published9a522dac3.

Original full VRAM8013D5BC sets cursor tosp84, 8013DE64 steps4bytes and
8013DE4C/DE68 enforce two iterations, proving slots84/88 in frame90. Fresh
baseline repairs singleton to real POLY_FT4*poly[2], typed pointer step1, all16
GameState.unk_018 frame-index reads and exact two-argument AddPrim byte-ordering
table address. Old plain792/typed957 remain history. No production/header change.

Fresh valid current baseline800/frameC0; joining12 disjoint same-type aliases
preserves expressions/store order and reduces frame to98 with800 rows. Direct
SDK UV assignments remove inherited signed-byte casts, restore word loads and
reach790/frame98. Source-only controls/replay are drafts/T-24078; production,
headers/config and helpers remain unchanged. Original frame90; recovery active,
no match or linked C proof claimed. Initial field-substitution syntax error was
repaired before scoring and is not a baseline result.

Metric correction: printed +/- rows are diagnostic only, with different equal-
length alignment rules. Existing tools/funcdiff.py diff_score measures aligned
resolved-key cost. Original588words/frame90: baseline638cost/800rows/594words/C0;
joined638/800/594/98 is retained; scalar SDK UV678/790/582/98 is WORSE despite
fewer rows. UVpair638/800/596/98 restores LW/saved136 but adds2words; per-case
height638/800/596/A8, inlineheight678/790/582/98, namedwidth648/806/594/A0 are
rejected. drafts/T-24078 replay now prints alignedcost/words/frame evidence.
No supported form simultaneously restores original stackUV50/54, saved136/320
and DIV-before-MULTU scheduling. Active In Progress pending focused direction;
no further unsupported probes, no match/impossibility claim. Production ASM.

Parent-authorized bounded Astra read-only review returned one source-backed
D32C transfer: derive pageX as0x140 + var_s4*0x80 and remove only independent
var_fp declaration/init/increment. Single indexed-page control neutral638cost/
800rows/594words/frame98. Compiler reconstructs320 induction but spills atsp50
rather thanbaseline60; no targetfp320/frame90 gain. drafts/T-24078/indexed-page.c
and replay preserve exact source; measured ignored build/etc24078/indexed-page.
Astra idle, no further unsupported control/instrumentation; still active pending
direction, production unchanged.

Read-only original/source relationship audit: drafts/T-24078/relationships.md
maps all4 arms/shared tail to full VRAM formulas. Signed Q11/Q141/Q68/Q112 and
R=(A*i*68)/80, distinct upper/lower right expressions, repeated signed center
reloads, UV words and all persistent values agree; no missingop/address/formula
correction found. One structural question is full-word coordinate intermediates
until SDK SH versus3 inherited s16 locals; their later low16-only uses make early
narrowing observationally equivalent, NOT a proven semantic bug/original C type.
Proposed single3-local s32 control is source-backed but UNTESTED; no compiler,
new instrumentation, source sweep or production change in this read-only pass.

Exact parent-approved word-until-SDK-store control changes ONLY temp_a1,
temp_a2,temp_v1 froms16 tos32 againstjoined638. New best604 alignedcost/670
printedrows/592words/frameA0 (original588/90), frame8B larger thanjoined98.
First divergent arithmetic still MULTU i*136 before Q11 DIV; original DIV then
MULTU. UV LB and early160-A remain. drafts/T-24078/body.c/replay and full-VA
relationships.md preserve result plus all9 controls. No other width/control or
production edit; active ownership continues. This is structural evidence, not a
proven behavior bug or original C type.

With full-word coordinates fixed, UV-word-pair control606cost/682rows/594words/
A0 restores LW but is worse than604. Case-local arithmetic declarations keep
604/670/592 yet enlarge frame toD0: lexical scopes do not reuse these homes.
Both rejected as best; no declaration-order sweep. Next source-only check is
case0 Q11/P/R alias inlining into exact unchanged X0/X3 associations; all other
arms fixed. Inputs A/i do not mutate within the arm and no calls occur there.

Further one-at-a-time controls against604: original-backed UVwordpair606/682rows/
594words/A0 restores LW but is worse; true case-local arithmetic lifetimes604/
670/592/D0 reject enlarged lexical homes; case0-only compound Q11/P/R aliases
635/703/593/A0 reject, first arithmetic remains MULTU-before-DIV. All12 variants
and exact sources/replay retained drafts/T-24078. Best604 unchanged; source
math agrees, no wrongop or compiler-limitation proof. Active same-function
read-only investigation continues before any supported new compile; no sweep,
new instrumentation, production/helper/header/config change or target churn.

Broader read-only PRIMARY matched semantic search at9d0d6a8e0 found only scalar
TACO39BCC signed/80; matched TACO3D528/40ADC use projected word points and/127
depth, TAIIKU3D9B4 tiledUV shifts, RPG_BAT54220 incoming word arrays. No genuine
68/136 slope11/141/112 repeated-division quad analogue or new supported source
form. relationships.md records search/limits. Existing target evidence has only
final objects, NO compiler-stage inputs: cannot attribute earlyP/H to uopt vs
ugen/as1; previous 'uopt hoist' causal wording is unproven. Existing stock-capture
pattern read only, no new diagnostic scope executed. Same target active.

Stock ugen capture of saved604 source passed SAME-SOURCE WHOLEOBJECT identity
67fbefa6... before interpretation, all code/data/relocation sections equal.
phase.md/capture.py/capture-fidelity.json record exact provenance. P-before-Q11
alreadyugenINPUT128..137. H remainsnearY2 input208..211 andugenOUTPUT127after
x2SH124, so earlyHfinal movement is POST-ugen; prior joint'uopt hoist' claim is
superseded, no broaderphase attribution. Input183..188 also folds X0=B+24 and
X1=X0-Q68+136 intoB-Q68+160. One approved matching-only use-site narrow in X1
cases0/2 follows; no other type/barrier/instrumentation change.

Approved X1-only s16 use-site cast in cases0/2 changes final offset web back to
X0-Q68+136 but costs638/800rows/594words/A0, worse than604. X0storedfroma0 then
MOVEto t5 beforeX1 creates extra copies (2words); no explicit halfword extension.
Matching-only FAKE, notsemanticrepair; source/replay preserved x1-narrow.c.
No additional cast orstagecapture. Next narrow unresolved phase question is
frontend vsoptimizer origin ofP-before-Q11: current capture begins afterthat
boundary. Samefunctionactive, production unchanged.

Second exact stock capture locates P reversal: frontend target818records has
Q11DIV106 thenPMPY111; actualconvertedpre.B target BYTEIDENTICAL. Stockraw.O
762records hasPMPY130 thenQ11DIV136. Postconversion WHOLEFILEidentical toraw.O;
previousugeninput targetbytes identical. Wholeobject67fb... and sections pass
FIRST, unchanged options/default405=1. Thus P-order change is inside stock
optimizer, not frontend or conversion rewrite; internaltransform stillunknown,
no faithful new source dependency identified. phase.md/uopt_capture.py/manifest
preserve exactprovenance. H movement remains separately post-ugen, not this
phase. No further compile/probe/instrumentation or production edit; active.

Read-only existing stockuopt generatedreference: oneproc statically calls copy
propagation, findinduct/codemotion, coloring thenreemit. Firstarmactualgraph has
independentQ11 andP feedingSUB; no valueedgeQ11→P, so statementorder isn'tbarrier.
Earliergraphrewrite vsfinalDAGre-emission remain unproven internalcandidates.
phase.md records referencecalls/dependencytable andsmallestmissingobservation:
firstarmLOC450/label159 statement/DAG order atENTRYreemit. No suchsnapshot,
no supportednewsourceform, no logger/build/options/probe executed. Active.

One approved bounded native reemit observer PASS same604 WHOLEOBJECT67fb...
FIRST, allsections/options unchanged. ENTRY BB11 LOC450/451/452/453 arithmetic
assignments are alreadyNOP96; firstX0store DAG ADD(SUB(SUB(P,Q11),R),24).
Correspondingoutput emitsleftP beforeQ11. Thus priorassignmentelimination plus
operandfirstreemission, notswapofsurvivingassignments; exactearlierpassunknown.
reemit.md/patch/fidelity preserveminimalrecipe/hash/observations; rawbinary/logs
ignored. One faithfulsourceassociation lead isoriginfirst24-Q11+P-R mirroring
lowerX3origin160; no newsourceprobe yet. Samefunctionactive, noextra logger.


Origin-first cases0/2 control restores original Q11-DIV/P-MULTU/R-DIV order,
628cost/856 diagnostic rows/592words/frameA0. Keep604 numeric best and628 causal
frontier separately; neither matches. Actual caller A0..80/i0..1 supports exact
reassociation, no invented dependency. Source/replay and regional comparison
are in drafts/T-24078; cases1/3 unchanged. Remaining X1+160 versus original
X0+136 dependency needs read-only analysis before any further control. Active.


Correction after inspecting final words: origin-first628 ALREADY uses actual
X0-Q68+136 in cases0/2 without a narrow cast. The prior pending-X1 description
was an unverified carryover from604's folded160 expression. The one subsequent
origin-x1 control adds only X1-use (s16) casts, otherwise identical628 source.
It measures624 aligned cost /792 diagnostic rows /594 words /frameA0, but adds
MOVE a0-to-t5 in each arm. It preserves Q11/P order and changes allocation;
it does NOT restore a previously missing dependency or add a sign extension.
Retain604 numeric best and628 no-extra-copy causal frontier. Preserve624 only
as a measured added-copy control, not a semantic/structural improvement. Its
FAKE comments describe intent; final code shows the dependency already existed.
No further control follows from this result. Active same target, production ASM.


One private void(s32,s32) return-view control against628 is target-neutral:
identical resolved target,628cost/856rows/592words/A0. Private full-TU forward
matches definition exactly; production/header unchanged. Audit finds six actual
calls (three each C330/C700), all ignored results; original DE44..DE9C epilogue
sets no explicit return. Matched D32C is s32 fallthrough, so this evidence does
not prove original void. All35 full-TU function bodies were compared: C330
alone changes9 words (alignedcost18), from v1 to v0 for post-call F5F tests and
1104 increment. Other34 including target identical. Thus even ignored-result
callers can depend on return liveness for matching. Reject promotion of void;
retain conservative FAKE s32 fallthrough, no invented return value. The private
control remains reproducible via --variant origin-void, with its exact forward.
No new structural source lead follows from this neutral result. Same target active.


One current628 word-UV-pair transfer follows explicit verification that628
still uses LB at UV accesses. Geometry, coordinate types, source ordering and
s32 signature remain fixed. The existing FAKE aggregate view is unproven as
original C but represents the two observed persistent stack words. Measured
634cost/862 diagnostic rows/594words/frameA0, worse628 and two extra words.
It restores LW atsp94/sp90 (originalsp54/sp50), with repeated reloads before
u2/u3 rather than original word reuse. Mode stays s7,136 staysfp,160 stayss6,
page320 remainssp64; no cross-call/register/frame repair. Reject as frontier,
preserve exact source/replay. Earlier638/604 pair results were not assumed to
predict this outcome. No new stage capture or geometry claim. Same target active.


Read-only stack-map.md distinguishes original64B local region from current80B:
poly84/88 has original unknown word8C above it;628poly98/9C has none. UV50/54
versus90/94 and currentpage64 are actual accesses. Existing604 frontend declares
72B plus hidden4B switch home; optimizedDEF76 survives, four-byte rounding.
No628 frontend capture exists. First-declared-highest IDO rule explains current
layout, but original unused homes are unidentified; no arbitrary permutation
or padding proposed. Disjoint X0/right-coordinate roles are a possible real
same-type reuse lead, not measured or a complete original-layout explanation.


One constrained counter-first control on628 moves only existing var_s4 before
poly[2], with explicit FAKE unknown-original-order annotation. Prediction before
compile: unchanged76B homes/frameA0; poly94/98, UV1608C/UV2490, page60, counter
unused home9C above poly. All observed offsets match the prediction exactly;
628cost/856rows/592words/frameA0 unchanged. Arithmetic/register allocation is
unchanged. This confirms the declaration anchor mechanism, not identity of
original8C or a16B frame fix. No UV move, removed local, padding or sweep.
Original poly84/88 is still16bytes below the new array, so the unknown local
count/lifetime gap remains separate from this one-word anchor.


One disjoint common-X0 reuse on counter-first628 removes only temp_a1's s32
declaration and uses temp_v1 for its assignments/reads in cases1/3. Cases0/2
already use temp_v1 for the same X0 role. No value crosses arms/calls/iterations;
all formulas, types, stores and relative declarations remain fixed. Before
compile the predicted home count was76 to72 bytes and frameA0 to98, poly down8.
Measured628cost/856rows/592words/frame98, exactly that frame prediction.
Poly8C/90, UV16084/UV2488, page3205C; saves18..3C unchanged. Page home moves4,
not8, because removing the earlier declaration changes its negative offset.
No arithmetic or register allocation gain; one real source home removal yields
8bytes through alignment. Remaining8B frame excess and UV placement remain
separate unresolved questions. No further merge or layout control follows.
