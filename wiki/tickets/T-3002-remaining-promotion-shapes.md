---
id: T-3002
title: Register shapes left after the unsigned-load conversion pass
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-1321-register-promotion-build-step]]", "[[matching-notes]]", "[[data/t0018-cases]]"]
---

## Goal

Characterise the T-0018-family differences that still fail with `tools/cvt_pass.py` (T-1321) and find C forms or rules for them.

## Notes

From the T-1321 sample (cases under `build/t1321`, scratch):
- `if (D++ == K)` with `K != 0` (SHUGAKU `func_80134024`, `func_80134D8C`, GEKO `func_8013A5E8` on an `s32`): the original materialises the compare (`xori; sltiu; beqz`), IDO branches on the `xori` result (`bnez`). `== 0` matches.
- `u8` increment in an `if` arm that also returns the value (BUNKA_SD `func_80134540` family): original `addiu v0; andi t6,v0,0xff; or v0,t6,zero; sb t6`.
- Two globals swapped (`func_80086640`), constant 255 shared over three `u8` stores (`func_80061EFC`, see T-3001).
- Unsigned compares of `u8` globals: `sltiu` in the original where IDO emits `slti` for a `u8` against a signed constant (TACO `func_8013B990`).
- `$v1`/`$v0` choices of values that are not globals (the `regorder` rows of [[data/t0018-cases]]).

## Acceptance criteria

- [ ] Each shape has a C form, a rule, or a recorded verdict.

## Comments
- 2026-10-10 (T-9000): the `if (D++ == K)` shape (`xori; sltiu; beqz`) is an `s32` function without a return value; SHUGAKU `func_80134024`, GEKO `func_8013E56C`, `func_80140EBC`, SHOUGATU `func_80138158` match that way. The other shapes are covered by the T-9000 cluster verdicts in [[matching-notes]].
