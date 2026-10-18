---
id: T-24021
title: Investigate dialogue argument scheduling
status: In Review
assignee: Astra hard-function lane
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-24020-astra-hard-functions]]", "[[tickets/T-22001-w4-p01]]", "[[tickets/T-17001-w3-main-p01]]"]
---

## Goal

Match `dialogue_speak_part` in `src/main/8007C030.c` (8,980 progress bytes; 16 differing rows).

## Acceptance criteria

- [x] Test a concrete new hypothesis without repeating exhausted line/argument spelling forms.
- [x] Preserve evidence, keep default assembly fallback unless C is verified.
- [ ] Standards review before Done.

## Notes

T-17001 traces show the constant argument loses scheduling priority to earlier narrowing/string stores. T-22001 exhausted 18 further forms. New hypothesis: sharing the call path across branch arms can create a block-entry constant through uopt PRE, rather than changing expression spelling. Check a bounded set of natural source shapes; do not patch compiler inputs or flags.

## Comments

### 2026-10-17 bounded result

Six new variants plus the baseline scored with full-TU resolved comparisons. Baseline aligned insertion/deletion cost is 4 (the older 16 printed rows are a different metric). Backward labels around either/both incremented-Y calls and a do/goto block retain cost 4; moving call blocks before the switch costs 1,546; natural shared-tail coordinate/colour locals cost 518 for both s32 and s16 coordinates. No improvement and no production C change. Probe script and compact results are in `drafts/T-24021/`; generated objects remain ignored under `build/t24021/`. Stop this avenue and proceed to TEL. Review remains pending.
