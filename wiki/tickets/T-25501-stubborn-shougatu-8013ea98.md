---
id: T-25501
title: Stubborn function SHOUGATU func_8013EA98 (src/ovl/SHOUGATU/8013E330.c)
status: In Progress
assignee: wave-12 workflow
created: 2026-10-23
updated: 2026-10-23
links: [[[tickets/T-25500-stubborn-functions]], [[playbook]]]
---

## Goal

Match SHOUGATU func_8013EA98 (src/ovl/SHOUGATU/8013E330.c) after earlier passes failed (T-25046, T-25143, T-25305), using independent lenses, a synthesis pass and adversarial review ([[tickets/T-25500-stubborn-functions]]).

## Acceptance criteria

- [ ] Literal default-C match, 27/27, or a verdict naming the blocker and the change it needs.
- [ ] Review against CODING_STANDARDS.md (7 and 7a) before Done.

## Comments
- 2026-10-23 (synthesis, w12-shougatu-final): not matched; `INCLUDE_ASM` kept. Best body `drafts/T-25501/SHOUGATU/func_8013EA98/func_8013EA98.c`, 120 rows (`funcdiff --resolve --version c`), 230 words, frame 0x58. The whole selection loop now matches, registers included, from the cleanroom W1a loop form (13 blocks give the original's constant order) and the compiler lens's split mechanism (b1 `* 0` hole, alias forced store, free `* 0` references, a post-loop hole and a `>= 0` block for the selector's priority window). Left: the post-loop `lui v1`. It is a selector piece coloured v1 in the original; the alias form Z1 reproduces it but then loses the loop split, and under IDO 5.3 costs both can't hold together. Also left: the operand order of the FE4 == 9 compare. No tool or config change. Notes: `drafts/T-25501/SHOUGATU/func_8013EA98/NOTES.md`; wiki/matching-notes.md section "T-25501".
