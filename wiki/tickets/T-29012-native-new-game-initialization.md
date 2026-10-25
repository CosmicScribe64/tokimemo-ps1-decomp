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

2026-10-25 v2 review checkpoint: original-function comparisons pass nineteen cases at author scope, but Standards review identified original girl birthday/blood arrays embedded in GDScript. The Phase 3 external-content boundary applies to those character initialization values; no exception is approved. Author is preparing a separate v3 that moves them into source/native-bound external content. Preserve v2 evidence and rerun endpoint comparisons after the adapter change. No canonical application or Done status.

2026-10-25: root integrated the four reviewed New Game initializer/original-table adapter files, composing v3 with the v4 leading-double-slash path correction. Both independent review axes pass. Actual installed code passes 109 Godot checks, 13 Python tests and all 19 preserved original full-modeled-state/profile/RNG comparisons; five existing core/content/rule/week suites pass. Exact hashes/review binding: `.claude/orchestrator/state/T-29012/v4-path-fix/live-application.json`. These are function-level native comparisons over 0x1A8C modeled bytes, not a fresh normal New Game capture or proof of the final four unowned GameState bytes. Profile-entry UI and opening/prologue routing continue separately; no captured-gameplay default is introduced.

2026-10-25: integrated seven independently reviewed additive files for the separate New Game name/profile entry scene. Exact packet `a6c643993e2c71aa14e0484ea9e9d016d2d236ab0455a1e4d4c8372eae9bb730`; installed receipt `.claude/orchestrator/state/T-29012/ui-route-v1-root-review/live-application.json`. Canonical checks pass: 63 controller cases, five invented-input adapter cases and eight actual Godot/Mesa UI checks. Independent Spec review additionally decoded all 34 kana-row pointers and 45 preset entries and checked actual input/focus behavior. Original text/font/presets remain authenticated external data. The scene performs name completion and birthday finalization at their source actions and preserves explicit unknown state; it stops at the prologue. This is a development action flow, not native UI layout/timing, a complete opening, or a ready gameplay session. The author continues source-backed prologue and optional explicit START-skip routing; normal story remains required.

2026-10-25: applied the finite four-file v2 correction discovered after the v1 integration. Native CIRCLE-on-DONE sets tab 4 and the next name initialization resets to hiragana; keyboard START retains the selected kana tab. The UI now distinguishes the Done button from START and consumes Enter before a focused Godot Button can activate itself. Independent Spec and root finite Standards review pass; actual canonical 85-controller and ten-rendered-UI checks pass. Exact receipt `.claude/orchestrator/state/T-29012/ui-route-v2/root-application.json`. Original decoders/content packs are unchanged; old captures/desktop exports remain historical v1 evidence, not proof of the corrected keyboard behavior.

## Bank0 loading control integrated (2026-10-25)

Applied the four-file prologue bank provider after independent Standards/source review PASS. The loader preserves original cache decisions, request ordering, byte-width counters, readiness/timeout predicates and ordered audio operations. Its content adapter verifies original source calls and ISO identities and exports authenticated metadata outside the project. Root installed tests pass: 24 Godot control checks and six Python checks in both normal and optimized execution. Review independently reproduced ten ISO bindings, two sequence headers and the external pack. Evidence: `.claude/orchestrator/state/T-29012/prologue-bank-provider-v1/{spec-standards-review,canonical-application}.json`.

Actual CD transport, driver readiness delivery, playback, native function timing and the connected normal prologue remain open. This component does not make New Game playable; T-29012 remains In Progress.

## Player-facing UI direction (2026-10-25)

User wants the finished app to feel like a game, not a developer tool. Remove default diagnostic headings, raw state/RNG/indices, implementation explanations and permanent control banners as scene integration proceeds. Debug information remains available only on request or in logs. Original game menus/dialogue remain; setup/settings/errors use concise useful copy. The current placeholder layout is not the target design. HUD and New Game owners have this requirement.

## Entry presentation cleanup integrated (2026-10-25)

Root applied the independently approved four-file v2 packet, manifest `74757a37da862316018747f9b517063476899c29a67b6ed681c82d8638e0ba58`. Normal entry screens omit persistent instructions, raw state/RNG and development paragraphs; technical diagnostics are opt-in or logged on load failure. Profile fields use readable values. Controller, initialization and original-data validation remain unchanged. Canonical execution passes eight UI/diagnostic checks, 85 controller checks, five shared suites and 13 actual Mesa-rendered UI checks. Evidence is `entry-ui-cleanup-v2/canonical-proof/` under the ignored T-29012 state directory. This does not complete the prologue or establish native macOS startup.
