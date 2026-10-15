---
id: T-12230
title: Broaden medium and large function coverage across unclaimed objects
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-breakthroughs]]"]
---

## Goal

Increase literal-verified matched bytes by extending the successful zero-search pipeline beyond BUNKAKEN/BUNKASAI, including larger functions. Preserve all other workers' ownership and main checkout.

## Acceptance criteria

- [x] Freeze a broad target manifest excluding known claimed objects and TT.
- [x] Run existing draft/exact-rescore pipeline with bounded resources and preserve failures.
- [x] Verify new matches with literal linked comparison and binary checksums; check shared headers.
- [x] Record measured gains, sampling limits and follow-up opportunities.
- [ ] Complete standards and requirements review before Done.

## Notes

Baseline 6d1e9cf2: 5,756 functions / 1,306,708 game bytes (57.3%). Work only in /private/tmp/tokimemo-acceleration. Docker capped at one CPU, 1 GiB. No tool defaults changed, no merge or push. Claims from cx package files plus changed source files in all existing worktrees are conservatively excluded; TT excluded wholesale.

## Comments

Frozen 101 targets / 150,524 list bytes across 18 units. Per unit: up to eight largest eligible 600–1,999-byte targets and three largest eligible 2,000–7,999-byte targets. Excluded previous 24-target cohort, TT, cx1–cx10 object lists, changed source objects in all accessible worktrees, and filesystem differences in three stale w17 worktrees whose Git metadata is missing. Conservative exclusion leaves 190 eligible functions. Manifest /private/tmp/tokimemo-broad-out/cohort.json; exclusions build/broad/claimed.txt. Run existing bulk_m2c with --time 0 --score-jobs 1 --jobs 1 --no-order --no-regdiff. No mutation search.

Final: 190 targets across 18 units, 235,624 list bytes; 3 direct matches, two targeted fixes, total five literal matches / 5,036 bytes. Progress 57.5% (+0.221 percentage points), not a large jump. Five variants on nine close drafts yielded no new win; known bitfield/call-convention fixes were more productive. All accepted functions pass literal linked verification, both affected SHA1s and header/global/prototype guards pass; no full-game build. Complete outcome ledger, failed hypotheses and reproducibility notes in [[broad-coverage-results]]. Review baseline 6d1e9cf2; awaiting standards/spec review.
