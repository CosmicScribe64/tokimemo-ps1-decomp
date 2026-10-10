---
id: T-9020
title: Wave-5 codegen shapes
status: Done
assignee: r5-shapes
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[game-state]]", "[[data/t0018-cases]]"]
---

## Goal
Tooling round 5. For each codegen shape that wave 5 (T-8010..T-8080) left open, find the C idiom or a uniform, evidence-backed build rule (CODING_STANDARDS 7a), document it in [[decompile-workflow]] and match the example functions.

1. `x * 0x44` as `sll/addu/sll` in the original, `li; multu` in IDO (OPTION `func_80134804` family).
2. A constant loaded once and stored several times (DATE / main `normal_date_*_select_init`).
3. ENDING reload after a zero test (`func_80137F64` and four siblings).
4. A constant shared between a store and a call argument in `$a0` (list 3).
5. Bit-field flag tests (`sll; bltz`) on GameState words.
6. TACO struct-by-value call frames with FAKE pad locals.
7. TACO `func_8015D270`: gcc-built SDK code inside an overlay (per-object compiler).
8. GameState members read as separate scalars (list 1), against the T-7010 verdict.

## Acceptance criteria
- [x] A verdict per shape, with evidence, in [[decompile-workflow]] / [[matching-notes]]
- [x] Example functions matched where an idiom exists
- [x] Clean build 27/27 OK, headers OK, progress >= 4227 plus gains
- [x] Inline review against CODING_STANDARDS.md

## Notes

## Answer
Verdicts (details in [[matching-notes]], "Wave-5 codegen shapes (T-9020)"; how-to in [[decompile-workflow]]):
1. `x * 0x44` shifts: uopt loop test replacement of `i == sel`; idiom = separate value counter. Matched OPTION `func_80134804`, `func_80134C00`, `func_80134E64`, `func_801368FC`.
2. Constant stored several times: chain assignment to array elements (copy propagation is not involved). Matched DATE `func_8013DCC0`, `func_80137FAC`, TAIIKU `func_80141964`, main `normal_date_two_select_init`; `D_800CA21C`..`D_800CA238` declared as four `s16[4]` rows.
3. ENDING reload: `if (D == 0) { } else if (D == 1)`, `D_8013C360` `u32`, `D_8013C35C` `s16`. Matched `func_80137F64`, `func_80138C70`, `func_80138A9C`, `func_80138DC8`.
4. Store + argument in `$a0`: register reproduced with an element/indirect target, `li`/`lui` order still off (as1); 0 matched.
5. Bit-fields: one `CharFlags`/`Rec38Flags` type replaces 13 file-local views (no byte changed).
6. TACO frames: no hidden struct; the family's `TcPos p0, p1, p2; s32 r;` locals (func_801563E4) replace the FAKE pads.
7. TACO `func_8015D270`: gcc output; per-object toolchain is simple in `configure.py` but the arm64 image has no old gcc. Not done, follow-up.
8. GameState "scalars": T-7010 holds; main `func_80042960` matched with the T-8080 view; the other three are register/scheduling rows in [[data/t0018-cases]].
Progress 4227 -> 4240 (13 functions). Clean build 27/27 OK, headers OK, globals OK, `sync_protos.py --check-branch` OK.

## Comments
- 2026-10-10 review (inline, CODING_STANDARDS.md): C89 only, `/* */` comments, ASCII. FAKE kept and marked on the one unused pad (`normal_date_two_select_init`). The TACO position locals are no longer marked FAKE because the same locals are used in full by `func_801563E4` (comment in the source cites it); a reviewer who reads 7a strictly may prefer FAKE, the bytes do not depend on it. `*(s16 *)&D_8011ECD0[k]` is an element of the declared `u8[]` table (section 8 allows it, the table type is not recovered yet). Declarations: main symbols only in `include/main_api.h`, ENDING symbols in `include/ovl/ENDING.h`, the OPTION record view is file-local like `Ev44`. No tooling changed, so no new tests. No open findings.
- Files owned by r5-data touched: `include/main_api.h` (data declarations: `D_800CA21C` rows, `Rec38Flags`) and `include/ovl/ENDING.h` (two types). `configure.py` not touched.
