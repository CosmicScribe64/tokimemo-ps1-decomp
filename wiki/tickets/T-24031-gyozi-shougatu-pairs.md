---
id: T-24031
title: Recover paired GYOZI and SHOUGATU particle handlers
status: In Review
assignee: Codex
created: 2026-10-17
updated: 2026-10-17
links: ["[[event-family-results]]", "[[shougatu-family-results]]", "[[tickets/T-24000-complete-matching]]"]
---

## Goal
Match exact opcode-structure pairs using the stronger recovered source and transfer:
- GYOZI func_80144B70 and SHOUGATU func_8014288C, each 1,300 literal bytes.
- GYOZI func_80144018 and SHOUGATU func_80141D20, each 1,204 literal bytes.
Exclusive C files src/ovl/GYOZI/80143AD0.c and src/ovl/SHOUGATU/80141780.c.

## Acceptance criteria
- Read prior type maps, drafts and negative evidence first; bounded novel pass near 25 minutes.
- Reuse built /private/tmp/tokimemo-complete-etc, codex/complete-gyozi-shougatu-pairs, preserving verified BUNKASAI ancestor 72cefc5d5 and all history.
- One GPT-6.1 Sol medium compiler total, Docker only, ninja -j1. No other source files or main_api.h edits without notification.
- Verified C removes target guard and INCLUDE_ASM and passes explicit normal-default linked proof, affected checksum and header/global/prototype guards.
- Preserve matched bodies and useful source-only drafts; send pinned wins promptly. Root owns independent review, all-27 checksums, integration and push.

## Prior evidence
T-17500/T-17600 typed arrays identify three 50-word work arrays, a short array and the shared Rec24/display records. Correct indexed array accesses are required before allocation experiments. T-24004 preserves repaired SHOUGATU func_80141D20 at 491 literal rows / 387 aligned edits; do not repeat uniform old m2c/redraft probes. No matched sibling exists yet in either target pair. Read current sibling setup handlers and faithful direct global mutations before choosing an iterative rewrite.

## Comments
Both target files are explicitly unclaimed by other active workers. Other GYOZI helper assignments remain excluded. No new worktree, history reset, host install, inline assembly or per-function compiler exception.

## Bounded pass result: four source-only drafts, no verified gain

**0 of 4 matched; zero new verified bytes.** All production targets retain their original INCLUDE_ASM paths. `drafts/T-24031/` preserves four compiling source-only bodies, a serial full-TU diagnostic reproducer and provenance. Actual default C proof is not claimed for assembly fallbacks. GYOZI's existing void func_80144018 header remains unchanged; archived GYOZI measurements use only a documented in-memory s32 view.

Archived resolved costs / printable rows, remeasured by `tools/docker.sh python3 drafts/T-24031/measure.py` in `build/pairs24031/archive-proof.log`:
- SHOUGATU func_80141D20: 264 / 264 (prior T-24004 baseline 387 / 491).
- GYOZI func_80144018: 266 / 266.
- SHOUGATU func_8014288C and GYOZI func_80144B70: each 412 / 508.

Recovered semantics: spawn Rec24[count+3], period/velocity[count-1], update work arrays[i-4], correct hide-record indexing, real GameState/GyoziWork byte arrays, and Fix32 low-halfword clear/high-halfword store/full-word destination copy. The original reset is sh zero, not the old draft's full-word zero. Direct shared counter mutation improves the smaller handler; legal entry-pointer indices improve it again. The larger handler reuses one induction variable and one period temporary, reducing frame 0x70 to 0x68 (target 0x50). Explicit per-entry pointers worsen to 426/frame 0x78. Consistent state pointers and true Rec24[4].unk_04 views preserve the best costs. No guessed padding, artificial aliases, broad permutation, shared-header, main API or production edits.

Remaining blockers are broad source live-range/induction allocation around RNG calls. The 1,204-byte frame matches 0x50 but its initial count is in a0 rather than a1 and later array ranges differ; the 1,300-byte frame still has 24 excess bytes and does not reproduce the original stride/array-base ranges. These results do not establish a compiler limitation. Rejected controls and reproduction are in the archive README.

Preservation checks: `ninja -j1 build/ovl/GYOZI.ok build/ovl/SHOUGATU.ok` in Docker passes both checksums (`build/pairs24031/final-build.log`). Header/global guards pass; prototype guard 0 new / 15 known, branch check OK (`final-{headers,globals,protos}.log`). These verify unchanged default assembly builds, not the archived C. A Git diff against 8ad16d265 confirms both owned production files, both overlay headers, main_api.h and the BUNKASAI win files are untouched. Ticket/card In Review for archive review and next disposition; all four matching targets remain pending.
