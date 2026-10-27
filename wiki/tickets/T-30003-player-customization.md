---
id: T-30003
title: Explore gender-aware player customization
status: Backlog
assignee:
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase4]]", "[[phase3]]", "[[tickets/T-30000-plan-phase4-modernization]]"]
---

## Goal

Explore and design gender-aware player customization for the native port — including playing as a girl, appearance/silhouette customization, pronoun and honorific adaptation, and contextual dialogue adjustments — as a Phase 4 stretch milestone (`P4.4`).

## Exclusive proposed paths

- `port/godot/core/player_customization_profile.gd`
- `port/godot/dialogue/gender_token_adapter.gd`
- `port/godot/tests/dialogue/run_player_customization_tests.gd`

## Acceptance criteria

- [ ] Defines a versioned `PlayerCustomizationProfile` (gender identity/presentation: `Default Male`, `Female`, `Custom`, pronoun set, default honorific override, room/SD avatar palette & silhouette variant) stored in save data and `GameState` extension fields without breaking native `4:3` or PS1 memory-card import compatibility.
- [ ] Audits and maps first-person/second-person pronouns, gendered nouns ("boy"/"girl", "boyfriend"/"girlfriend"), and name-entry honorific/nickname rules across both Japanese token substitutions and the `translations/en/` English script layer.
- [ ] Specifies explicit, non-destructive fallback policies for event CGs (`SCENES`/`ENDING` hands/silhouettes), SD sprites (`RPG_BAT`, `TAIIKU`, `SHUGAKU`), and voiced clips (`XA` name/honorific calls) when playing with a customized player profile.
- [ ] Automated test suite (`run_player_customization_tests.gd`) verifies profile persistence, dialogue token substitution across male/female/custom profiles, and zero regression on standard routes.
- [ ] Code review against `CODING_STANDARDS.md` passes with all findings resolved before moving to `Done`.
