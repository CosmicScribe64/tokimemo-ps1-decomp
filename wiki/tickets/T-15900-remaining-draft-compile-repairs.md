---
id: T-15900
title: Compile the remaining mixed-overlay draft batch
status: In Review
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
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
