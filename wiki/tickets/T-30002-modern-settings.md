---
id: T-30002
title: Provide comprehensive modern settings
status: Ready
assignee:
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase4]]", "[[phase3]]", "[[widescreen-settings]]", "[[tickets/T-30000-plan-phase4-modernization]]", "[[tickets/T-30005-widescreen-settings-feasibility]]"]
---

## Goal

Provide a comprehensive modern desktop settings menu (`tokimemo.port-settings.v1`) covering display/graphics, audio buses, keyboard/controller/mouse remapping, accessibility/text presentation, and independent language/translation-pack and voice-pack selection.

## Exclusive proposed paths

- `port/godot/services/user_preferences_service.gd`
- `port/godot/services/input_remap_service.gd`
- `port/godot/services/translation_pack_service.gd`
- `port/godot/ui/modern_settings_menu.gd`
- `port/godot/tests/services/run_modern_settings_tests.gd`

## Acceptance criteria

- [ ] Implements versioned user preferences (`tokimemo.port-settings.v1`) persisted atomically in user data, separate from PS1 memory-card `OptionConfig` (`stereo_mode`, `voice_enabled`, `mouth_anim_mode`) while syncing compatible flags cleanly.
- [ ] **Display & Graphics tab**: Windowed / Borderless Fullscreen / Exclusive Fullscreen mode, resolution & monitor selector, aspect ratio (`4:3 Original`, `16:9 Widescreen`, `Auto`), scaling filter (`Integer Nearest`, `Sharp Nearest Fit`, `Smooth Linear`), VSync, frame rate cap (`60`/`120`/`144`/`Uncapped`), and a `15s` Keep/Revert confirmation timer (`60s` on first launch) plus crash-recovery boot sentinel (`pending_display_boot_check`).
- [ ] **Audio tab**: Independent `0..100%` linear sliders mapped via `linear_to_db` + mute toggles for Master, Voice (`XA`), BGM (`SEQ`), SFX, and Movie (`STR`), output device selection, stereo/mono toggle, background-unfocused mute toggle, and live test-tone/voice preview.
- [ ] **Controls & Input Remapping tab**: Keyboard, Controller, and Mouse rebinding via `InputMap` for all gameplay/menu actions, Confirm/Cancel (`O`/`X` vs. `A`/`B`) swap with dynamic button-glyph prompts, controller deadzone and menu key-repeat sliders, conflict detection, and one-click Reset to Default.
- [ ] **Accessibility & Gameplay QoL tab**: Text reveal speed (`Slow`, `Normal`, `Fast`, `Instant`), auto-advance delay, skip mode (`Read Only` vs. `All`), UI/dialogue font scale (`100%–150%`), dialogue box backdrop opacity, and screen-flash/shake reduction.
- [ ] **Language & Voice Pack tab**: Independent text language / translation-pack selector (`Japanese (Original)`, `English (translations/en)`, or external custom pack directory/manifest) and voice language selector (`Japanese (Original)` or custom voice pack), with hot-reload fallback to original Japanese for missing entries.
- [ ] Automated test suite (`run_modern_settings_tests.gd`) verifies schema persistence, 15s/60s display revert and crash-sentinel recovery, audio bus dB math, input conflict resolution, and translation pack hot-switching in Docker.
- [ ] Code review against `CODING_STANDARDS.md` passes with all findings resolved before moving to `Done`.
