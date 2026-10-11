---
id: T-9210
title: Wave-6 codegen shapes
status: In Progress
assignee: r6-shapes
created: 2026-10-10
updated: 2026-10-10
links: ["[[matching-notes]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Tooling round 6. For each codegen shape that wave 6 (T-9110..T-9180) left open, find the C idiom or a uniform, evidence-backed build rule (CODING_STANDARDS 7a), document it in [[decompile-workflow]], match the example functions and sweep [[data/t0018-cases]] for other rows with the same shape.

Shapes:
1. Constant multiplies that reuse the loaded constant (`li a0,56; multu` with `a0` a later call argument; `li k; multu` strides).
2. The stack-argument store in the `jal` delay slot (RPG_BAT/TACO `func_8014927C`, `func_80152110`).
3. Loop-invariant pointers or constants hoisted into `$s`/`$a` registers (TT `func_80145370`, TAIIKU `func_801449A0`; T-7020 phantom registers).
4. Narrow K&R parameter home slots (TT `func_80148B6C`, `func_80148C04`).
5. One `li v0,k` shared by two byte stores in scene inits; constants shared across separate stores.
6. Return value in `$v1` versus `$v0`.

## Acceptance criteria

- [ ] Each shape has a verdict (idiom, rule, or no C form) with evidence in [[matching-notes]] and a how-to in [[decompile-workflow]].
- [ ] Example functions matched where an idiom exists; sweep of [[data/t0018-cases]] done.
- [ ] Clean build 27/27 OK, headers OK, progress not lower than 4521 plus gains; no matched function changes bytes.
- [ ] Inline review against CODING_STANDARDS.md recorded below.

## Notes

## Comments
