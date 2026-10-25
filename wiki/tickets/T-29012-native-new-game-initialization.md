---
id: T-29012
title: "Port native New Game initialization and profile rules"
status: In Progress
assignee: port-new-game-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29002-native-runtime-and-oracle]]"]
---

## Goal

Implement source-backed New Game initialization and profile-confirmation rules so the port can eventually start without a captured emulator state. This contributes to P3.3; it does not replace title, name-entry UI, voiced prologue or their acceptance gates.

## Owned scope

Additive `port/godot/rules/new_game.gd`, focused tests and any strict original-table adapter needed by those rules. Root owns canonical application; prepare an ignored immutable packet. Do not modify the shared controller, app, schemas, existing rules or translation pipeline.

## Acceptance

- Port actual `src/main/game_init.c` initialization and source-reachable profile rules, preserving integer widths, RNG calls/order and native behavior including unusual blood-type ranges.
- Read source tables through strict original-input adapters; no extracted data in Git or res:// and no invented production defaults.
- Produce an explicit complete owned-state projection and precise unsupported dependencies; do not pass a partial snapshot as a ready command boundary.
- Compare original initialization/profile-confirmation states and RNG endpoints through native evidence or reproducible emulator recipes. Synthetic tests supplement those comparisons.
- Reject invalid input and missing content before changing caller state; test independent initialization and replay.
- Independent Standards and native/Spec reviews, resolved findings and root integration are required before Done.

## Limits

Native Japanese name-entry limits remain the Phase 3 behavior. Later translation layout is separate. Gemini owns all translation wording and its pipeline. A completed initializer alone is not a complete New Game feature.
