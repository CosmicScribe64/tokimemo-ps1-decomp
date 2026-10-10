---
id: T-9020
title: Wave-5 codegen shapes
status: In Progress
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
- [ ] A verdict per shape, with evidence, in [[decompile-workflow]] / [[matching-notes]]
- [ ] Example functions matched where an idiom exists
- [ ] Clean build 27/27 OK, headers OK, progress >= 4227 plus gains
- [ ] Inline review against CODING_STANDARDS.md

## Notes

## Comments
