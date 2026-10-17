---
id: T-15900
title: Compile the remaining mixed-overlay draft batch
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-17
links: ["[[exhaustive-drafts]]"]
---

## Goal

Repair the thirteen reserved remaining scopes as genuine reconstructed C, reusing current verified production source before reconstruction. No stubs or ASM shortcuts. Retain full translation-unit compiler proof, diagnostics and provenance as separate alternatives.

## Scope

- BUNKAKEN:func_8013B510
- BUNKAKEN:func_80143DD0
- DATE2:func_801395C8
- ETC:func_80141BE8
- EVENT:func_80117D80
- EVENT:func_8011B848
- RPG_BAT:func_801328C4
- RPG_BAT:func_8013BBF8
- RPG_BAT:func_80153784
- RPG_BAT:func_80156064
- RPG_BAT:func_80159B40
- TEL:func_80136724
- TEL:func_8013D88C

## Acceptance criteria

Every selected alternative compiles through the default full-TU IDO path. Literal matching is a separate gate. Initial and prior repair artifacts remain immutable.

## Result

All thirteen scopes compile and score through the default full-TU IDO5.3 path. Evidence is in `drafts/T-14500/repairs/t15900/catalog.jsonl`, including complete source, preprocessed context, compiler diagnostics and per-scope provenance. `tools/replay_t15900.py` recompiled all thirteen and asserted each score. EVENT17D80 reuses already verified production C; its zero score adds no new production bytes. The other twelve scores are nonzero. Residual reconstruction flags remain a separate quality gate.

The original frozen census and automatic alternatives are unchanged. Awaiting coding-standards review.

## Comments

2026-10-17: Reviewed at 28d5f509. All 13 catalog rows compile through the default full-TU path, the bodies contain no asm or stubs, and commit 83d824543 only adds files, so the earlier archives are untouched; five RPG_BAT scopes have since been matched in production (T-17105, T-17107, T-17112, T-17115, T-17116). Trivial findings, accepted: tools/replay_t15900.py is a frozen one-off (no path arguments, hard-coded count, no test, run from baseline 48362009), and the result commit message writes T15900 without a hyphen. Moved to Done; the seven scopes still INCLUDE_ASM stay in the normal matching inventory.
