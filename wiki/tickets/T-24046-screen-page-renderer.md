---
id: T-24046
title: Recover the MAIN screen page renderer
status: Done
assignee: complete_render_family
created: 2026-10-18
updated: 2026-10-20
links: ['[[main-family-results]]']
---

## Goal

Make one novel bounded pass on srn_tpage_show in src/main/8004C3F0.c. Reuse typed screen records and SDK GPU packets; twenty-five minutes, one compiler.

## Acceptance criteria

- Any C win has no target INCLUDE_ASM, normal default literal proof and MAIN checksum.
- Preserve useful precise source-only negative evidence otherwise; independent standards review before Done.

## Notes

Read T-24006/T-17000 renderer negatives first. The prior SDK POLY_FT4/RECT draft costs1032 aligned edits (814 vs806 instructions), with a frame mismatch and repeated allocation/scheduling differences. Do not repeat that baseline as a new hypothesis. Only this source is owned; header changes require prior root type notice.

## Results

Source-only checkpoint; no production source/header changes and no new verified bytes. drafts/T-24046 preserves the current504-edit source, normal-toolchain scores and precise negative hypotheses. Built812 vs target806 instructions, exact120-byte frame.

Novel corrections: positive word texture offset (old s8 narrows128 to-128), unused argument5 ABI slot (old signature shifted clut/depth inputs), coherent declaration/mode assignment order, one reused SDK packet and compiler-generated common-expression spills, consecutive RGB initialization. Full current-context baseline1032; structural stages1026/637/562/524/504. Named-word-offset alternative523 rejected. No random search or header speculation.

Self-review: source/JSON/markdown only, no raw assets/instruction listings or toolchain exceptions. Production matching fallback preserved. Awaiting independent standards review.

Production preservation verified: ninja -j1 reports no work; MAIN SHA1 e823bd844a8f8fa4d05483b59c66bc54b8393b26 passes. Headers/globals/main_api/whitespace checks pass. These verify preservation of the ASM fallback, not a new C win.

## Comments

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
