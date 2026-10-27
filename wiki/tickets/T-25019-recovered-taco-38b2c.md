---
id: T-25019
title: Recovered queue provenance and TACO func_80138B2C baseline
status: Done
assignee: codex
created: 2026-10-22
updated: 2026-10-26
links: ["[[tickets/T-25016-candidate-reconstruction]]"]
---

## Goal

Recover candidate provenance from the private T-25016 archive, reproduce the whole-TU resolved baseline for TACO func_80138B2C, and pursue at most three new causal hypotheses only if prior negatives leave a semantic, type or control gap.

## Acceptance criteria

- [x] Record candidate completeness and source identity.
- [x] Reproduce whole-file Docker baseline and classify its first divergence.
- [x] Record bounded next action and review against CODING_STANDARDS.md.

## Comments

Archive scripts are evidence only. No promotion of zeroed M2C_ERROR drafts, no production changes without literal match evidence.

### Provenance and baseline (2026-10-22)

Private recovery `/private/tmp/t25016-recovered-20261022` has 410 files, including 151 C body artifacts (150 unique basenames), 122 under `shape/beds/m2c`. This counts files, not unmatched functions. Only one body/header pair names func_80138B2C. Its body has no M2C_ERROR tokens. Census-m2c3 failed on obsolete D_800E62B7; census-m2c32 records packet canon 11/raw 56 and separately resolved 154. There is no func_80138B2C ledger entry. QUEUE.md omits that resolved counter. The archive preserves candidate evidence, not the original scoring execution state.

At base 7bc6d8d99, Docker `python3 tools/funcdiff.py --resolve TACO:func_80138B2C` reproduced current guarded full translation unit: 60 printed +/- lines = 30 positional instruction rows, REAL DIFFERENCE. First divergence is `lui a0` against `lui v0` at row 0; the count, zero-local and sum colour differences agree with T-16564/T-19015. Prior zero chains (T-20600) and T-24705 notes also supplied no untested semantic/type/control gap.

Compared recovered body once after only mechanical compatibility adaptations: map three old GameState scalar names to fields, score-word scalars to existing array elements, and void definition to existing s32 definition. Result: 103 printed +/- lines with unequal instruction spans, REAL DIFFERENCE; this is not a positional-row count and does not reproduce the historical 154 counter. Archived body restores the already rejected nested final branch and redundant temporaries. It is older than the guarded source. Restored source byte-for-byte; no source change retained and zero new causal hypotheses were run.

Private diagnostics and adapted source remain under build/t25019-* and /private/tmp/t25019-taco-*.txt. No game data or objects added to version control. Next reconstruction target is main func_8005215C in src/main/8004F870.c: Tier 2 recovered body fails at void-pointer field access, while matched xa_wait supplies the same prefix with signed 16-bit fields at buffer +2. Follow T-12050's wide F61+2 warning and check original call ABI before repairing k_disp_inc arguments.

Work is ready for independent review against CODING_STANDARDS.md. No match or coverage claim; register tail registered without another permutation search.

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
