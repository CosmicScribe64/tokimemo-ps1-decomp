---
id: T-24021
title: Investigate dialogue argument scheduling
status: In Progress
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

### Explicit closure requirement (2026-10-17)

User: "do not stop until the dialogue_speak_part is a match". This is the sole active matching target. Call signature/local-lifetime probes did not close the gap. Investigate branch-derived constants and materialization placement; retain byte-exact verification gate.

### Closure work in progress

Additional full-TU probes tested branch-known word/short values, secondary constant uses, reused coordinate locals, aggregate argument pairs, and removable conditional barriers. None yet improves the verified baseline (2168 text instructions, aligned cost 4 / 16 printed rows). Wide integer shifting introduced runtime helpers and is rejected. Some coordinate variants score 12 but retain the same ordering gap, so they are not progress on the blocker. The captured unmodified binasm confirms the increment carries line 4204, while `li a0,-40` and all outgoing argument stores carry line 4205 (second site 4211/4212). Capture files remain ignored under `build/t24021-trace/`. Argument-internal conditional barriers are the active hypothesis.

External research consulted: [IDO field guide, own compiler experiments](https://github.com/akratch/n64-decomp-workbench/blob/main/docs/field-guide.md), especially conditional branch-to-next barriers. Its IDO 7.1 claims are hypotheses here, not evidence about our 5.3 compiler. No toolchain or source-production changes.
