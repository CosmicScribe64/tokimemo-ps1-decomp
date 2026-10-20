---
id: T-24571
title: Test the first ENDING portrait16 alias view
status: In Review
assignee: Codex
created: 2026-10-20
updated: 2026-10-20
links: ["[[tickets/T-24512-ending]]", "[[tickets/T-24560-ending-slot]]", "[[families/ending-family-results]]"]
---

## Goal

Test the single mixed-view lead for `func_801387B0`: use canonical `D_8011F4DE` for the first portrait16 predicate like matched sibling `func_8013855C`, while retaining typed `DisplayActor44` views for portrait18 and portrait08.

## Acceptance criteria

- [x] Record fresh literal full-TU baseline and one trial score from Docker.
- [x] Preserve unrelated portrait18/08 accesses and keep production source unchanged unless the exact default-C match is proven.
- [x] Record exact score, diff rows and next evidence needed; no fallback ASM claim.

## Notes

Worktree `/private/tmp/tokimemo-t24560`, branch `codex/t24560-ending-slot`, clean at `b6e3f5057`. Historical T-24512 blanket typed views for portrait16/18/08 scored 148/177 and remained DIFF. T-18300 record/scalar drafts scored 156, with the promoted `D_8011F4DE` register noted as a blocker. This test isolates the untried hybrid source form suggested by matched `func_8013855C`; no shared-header or production changes.

## Result

One fresh Docker full-TU baseline (typed portrait16/18/08 fields in the first conjunction) and one trial (canonical `D_8011F4DE` for only the portrait16 equality, keeping typed portrait18/08 fields) both score 148 distance cost and 156 printable diff rows. The expected function has 187 instructions; both candidates have 185. The trial does not improve the result. No production file or shared header changed. Reproduction sources are `build/t24571_score.py`, `build/t24571_rows.py`, and the two generated full-TU files under `build/t24571/`.

The target frame is exactly 0x28 (`asm/ovl/ENDING/nonmatchings/ENDING/80137340/func_801387B0.s`, stack adjustment at 0x801387B8); the compiled baseline/trial also use 0x28 (`build/t24571/typed-baseline/guarded.o`, function offset 0x1478). Frame size is already aligned. Remaining evidence is a broad register/control-flow mismatch: the first guard uses target `li v1,1; bne v0,v1` while the candidate uses `li a1,1; bne v0,a1`, with downstream branch targets displaced by two instructions. Keep the function unmatched; no fallback assembly claim.

## Comments
