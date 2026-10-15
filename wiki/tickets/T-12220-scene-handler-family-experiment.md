---
id: T-12220
title: Test partial-family reconstruction on medium scene handlers
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[matching-breakthroughs]]", "[[tickets/T-12210-matching-bottleneck-research]]"]
---

## Goal

Test whether matched scene-handler structure can reconstruct medium-sized functions and then transfer to held-out siblings, yielding verified code beyond ordinary whole-function duplication.

## Acceptance criteria

- [x] Select targets outside cx-owned objects and TT; record baseline and source of reused structure.
- [x] Compare a fresh m2c draft with family-guided reconstruction under the same compiler.
- [x] Verify any wins using literal linked bytes and affected unit checksums.
- [x] Record gains, failed hypotheses, and transfer limits without extrapolating throughput from a small sample.
- [ ] Standards and requirements review before Done.

## Notes

Worktree /private/tmp/tokimemo-acceleration. BUNKASAI 80143770/8014DD90 are the initial object pair; both outside cx1-cx10 ownership. One-CPU Docker containers. No main or other worktree writes, no TT work, merges, pushes, rebases, resets, or stashes.

## Comments

Initial subjects BUNKASAI func_80150234 and func_80145930: fresh m2c drafts, with only absorbed GameState names migrated and switch-number comments removed, both score zero under the full-unit scorer (0.418 and 0.357 seconds for scoring only). This does not support crediting a special partial-family rewrite: ordinary fresh drafts suffice here. Verify them, then test a frozen cohort of 24 medium functions (first 12 by function name per BUNKAKEN/BUNKASAI, size 600–1,999) with the existing bulk pipeline, one m2c variant and zero hill-climb budget. Seed subjects excluded after verification. No additional solver/tool implementation is needed.

Final experiment: 20 functions / 16,272 literal linked bytes; 16/24 direct cohort matches in 260.18 seconds, plus two seeds and two existing-bitfield-idiom fixes. Comparison: fresh drafts scored 14 for both paired misses, bitfield reconstruction scored 0; literal proof 736/788 bytes. No special family reconstruction needed for seeds; broad transfer remains unproven. Full target/outcome table, scope and timing limits in [[matching-breakthroughs]]. Both overlay SHA1s pass; headers/globals/prototype branch guards pass. Evidence in build/family/final-proof.txt, pair-proof.txt, final-build.log, pair-build.log, progress-final.txt. Full-game build not run. Awaiting two-axis review against f1010b54.
