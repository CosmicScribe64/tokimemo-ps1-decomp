---
id: T-24022
title: Recover TEL large handler allocation
status: Done
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-20
links: ["[[tickets/T-24020-astra-hard-functions]]", "[[tickets/T-23001-w5-big-p01]]"]
---

## Goal

Investigate `func_80136724` (7,044 bytes) in exclusively reserved `src/ovl/TEL/80132000.c`, after dialogue.

## Acceptance criteria

- [x] Read archived frame recovery and negative evidence, test a new hypothesis.
- [x] Preserve source provenance; no win claimed, unchanged unit SHA verified.
- [x] Standards review before Done.

## Notes

T-23001 recovered the frame. New hypothesis: explicit conditional global assignment (rather than unconditional local writeback) changes the store/reload cost of the month range and restores its original global register allocation. Test equivalent forms of month normalization first, then inspect structural differences. One compiler maximum across this lane.

## Comments

### 2026-10-17 bounded result

TEL baseline SHA1 passes. Full-TU baseline has aligned cost 1038, 1643 instructions against 1645, printed rows 1048. Seven normalization forms (including baseline), eight access/local-type forms and eight structural forms compiled serially. The conditional local/global store improves aligned cost to 990 but worsens positional rows to 1746 and emits stores inside the branch, unlike the original unconditional call-delay-slot store. It does not restore month promotion. Unsigned store/read views, GsWord stores, unsigned local, explicit local reloads and bit-field grouping do not change the baseline; a state pointer for the full case-6 tail worsens it to 1056; a diagnostic volatile local costs 1044 and is rejected. Prior frame recovery remains intact; no source promoted.

Scripts, compact results, alternative conditional-store source and interpretation: `drafts/T-24022/`. Production source/header/config unchanged. Stop the bounded month-range avenue; proceed to EVENT. Review pending.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
