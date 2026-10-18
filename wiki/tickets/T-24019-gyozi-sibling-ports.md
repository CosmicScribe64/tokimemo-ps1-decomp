---
id: T-24019
title: GYOZI sibling completion ports
status: In Progress
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[event-family-results]]", "[[shougatu-family-results]]"]
---

## Goal

Bounded matched-sibling ports for GYOZI func_8013907C, func_80139E4C, func_8013E824 and func_80142A8C. Only their four translation units are owned.

## Acceptance criteria

- [ ] Remove target INCLUDE_ASM only for independently verified default literal C matches; affected unit SHA-1 passes.
- [ ] Source-only failed forms and prior evidence retained.
- [ ] Header/global/prototype checks and independent review.

## Notes

Read wiki index, event-family-results (GYOZI family), shougatu-family-results, T-17600, T-13201, T-22022 and matched siblings. Earlier negatives include GYOZI39E4C split stores33/temporary mode view27 and E824 narrow result/split mask24; do not misreport default ASM fallback as recovered C. One Docker compiler/ninja-j1; bounded25min.

## Verified batch 1

func_80142A8C matched 392 default linked bytes. Direct port of DATE func_80152E34 uses existing GyoziWork, SDK POLY_G3, the local work pointer and ordinary comma colour assignments. No new FAKE, header edit or compiler change. Explicit target INCLUDE_ASM removed from src/ovl/GYOZI/801418B0.c; default no-build funcdiff reports linked bytes equal target, GYOZI SHA1 check passes, headers/globals pass, prototype branch check zero new/fifteen known disagreements.
