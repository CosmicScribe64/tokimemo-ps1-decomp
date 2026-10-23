---
id: T-24010
title: Coordinate isolated DATE and ETC matching
status: Done
assignee: Codex
created: 2026-10-17
updated: 2026-10-20
links: ["[[tickets/T-24000-complete-matching]]", "[[date-family-results]]", "[[etc-family-results]]"]
---

## Goal
Recover verified C in the two assigned families from c485bb71a. Parent integrates and pushes; this lane never edits main or integration.

## Acceptance criteria
- DATE T-24011 owns src/ovl/DATE/80141EE0.c and 8015A640.c; targets func_80144E20, func_801449D4, func_8015A640.
- ETC T-24012 owns src/ovl/ETC/80132880.c; targets func_80136448, func_80132C3C, func_80132FB0, func_801328A4.
- Docker only; setup and ninja -j1 per worker. Bound fruitless target searches near 25 minutes and record concrete blockers.
- Verified wins require default linked funcdiff --resolve --version default, affected checksums and header/global/prototype guards, then standards/spec review.
- Preserve source-only drafts and provenance. No copyrighted material, inline asm, toolchain exceptions, contributor messaging or scope expansion.

## Comments
Baseline 1,813,468 / 2,279,368 game bytes (79.6%). Two focused GPT-6.1 Sol medium workers authorized; Astra reserved for evidenced blockers. Shared headers require coordination.

Initial source query: [[gcc-objects]] settles DATE func_8015A640 as hand-written libgte assembly (register order, multu, hazard nops and local jump; multiple gcc versions previously failed). Older [[matching-notes]] suggestions of a compiler swap are superseded by that measured result. Worker skips repeat compiler sweeps and focuses on T-17207 dispatcher evidence. This remains assembly and does not count as a new C match; no denominator change is proposed.

Worker branches: codex/complete-date at /private/tmp/tokimemo-complete-date and codex/complete-etc at /private/tmp/tokimemo-complete-etc. Common ticket ancestor 4c5123f11. Parent chat notified of scope, setup and established libgte blocker.

Resource audit: tools/funcdiff.py invokes ninja without a job cap in freshness/header checks. Workers instructed to prebuild affected .ok with ninja -j1 and then use --no-build --resolve --version default (freshness and header checks retained), or an untracked PATH wrapper that appends -j1. No tracked toolchain or per-function exception is introduced.

ETC checkpoint: a0675cafe604146244c5d762cd915467b41a46a2 on codex/complete-etc verifies func_80136448, 1,280 literal linked bytes. Docker default funcdiff MATCH, ETC checksum OK, headers/globals OK, prototypes 0 new / 15 known; proofs under /private/tmp/tokimemo-complete-etc/build/etc24012/. The s32 no-return epilogue view is conservatively marked FAKE. Two-axis pinned review started against 4c5123f11; no integration or push performed. Remaining ETC targets retain assembly and ignored diagnostic-only drafts. DATE near misses remain unverified.

Pinned ETC checkpoint review: independent Standards PASS (0 actionable findings) and Spec PASS (0 actionable findings). Findings confirm the FAKE is explained and ticketed, own-overlay prototypes match existing void definitions, literal proof and checksum pass, and remaining work is explicitly pending. Parent sent exact commit and proof for integration; ETC worker resumed to document review and continue remaining targets. T-24012 stays open for its remaining three functions.

DATE concrete escalation: small func_801449D4 reaches exact frame 0x170 and table offset 0x2C using register GameState pointer plus three explicitly FAKE pad words. Eleven source-only added/removed resolved rows remain (constant-16 schedule, branch immediates, terminal padding). Bounded cast, byte-view, selector-layout and control-flow probes failed. Astra assigned only this evidenced blocker in scratch; Sol retains production ownership and continues large dispatcher. No C match claimed from diagnostics. ETC review documentation follow-on: 491c8b4ca, no production change.

ETC bounded pass completed at 920cbb61b921f4b901ddfefe8eea82022bcd2359 after review-doc commit 491c8b4ca. Production C/header exactly equal reviewed a0675cafe. Source-only drafts/T-24012/README.md records compiling nonmatches at diagnostic costs 217 (RGB buffer), 180 (quad), 150 (background), measured individually in the full TU; none promoted. Ticket/card In Review, one of four verified and three explicit blockers. Parent notified of complete commit chain. Draft/archive delta awaits independent review.

Final archive review: ETC a0675cafe...920cbb61b passes independent Standards and Spec (zero actionable findings). DATE archive 2f7bd397b adds no production source/header changes and passes Spec; Standards reports P2 in probe.py/stage.py: unexpected funcdiff subprocess failure could print zero rows and exit successfully. Worker fixing result validation and checking both failure and real diagnostic paths before acceptance. Astra concluded no improvement; observed constant hoisting at as1 does not establish original assembler-version mismatch.

## Bounded pass handoff

- Verified C: ETC func_80136448, 1,280 bytes, commit a0675cafe. Default linked proof, ETC checksum and all guards pass; Standards and Spec pass independently.
- ETC branch codex/complete-etc ends at a00f55090: review-doc 491c8b4ca, source-only archive 920cbb61b, final archive-review record a00f55090. Three targets remain assembly with full-TU diagnostic drafts 217/180/150, explicitly nonmatching.
- DATE branch codex/complete-date: source-only archive 2f7bd397b, failure-reporting fix 5c14a3c40, final review record 6dd5a1554. No production source/header changes and no new C bytes. Two drafts at full-TU 48/72 rows; helper remains established hand assembly. Checksum and guards pass.
- DATE P2 fixed and independently re-reviewed: Standards PASS and Spec PASS, zero remaining actionable findings. Five Docker tests cover twelve result contracts; full-TU harness remains 48/72 and stage reproducer succeeds twice at eleven isolated edit rows.
- Astra scratch investigation found no better candidate; as1-stage constant hoisting observed, original assembler mismatch not established. Six of seven functions remain assembly; no denominator or verified-byte inflation.
- All worktrees preserved, no integration/push or further file claims. Parent owns integration and continued 100-percent effort. T-24010/11/12 In Review reflects the reviewed bounded handoff with unmatched targets still pending, not completion of all matching goals.

- closing review (2026-10-20): work is on main (w2-good ff6c770b2). CODING_STANDARDS check of the touched sources: FAKE and NON_MATCHING comments carry ticket ids, no inline asm, no guard over 30 rows. Unmatched targets named here stay INCLUDE_ASM and remain in the matching work lists. Done.
