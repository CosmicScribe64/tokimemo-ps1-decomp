---
id: T-30001
title: Add widescreen presentation
status: Ready
assignee: root
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase4]]", "[[phase3]]", "[[widescreen-settings]]", "[[tickets/T-30000-plan-phase4-modernization]]", "[[tickets/T-30005-widescreen-settings-feasibility]]"]
---

## Goal

Add native widescreen presentation (`16:9` `426x240` logical / `1280x720`+ display, `16:10`, and ultrawide `21:9` safe framing) to the Godot port while preserving an authentic `4:3` (`320x240`) option and never stretching original pixel art, portraits, or event CGs.

## Exclusive proposed paths

- `port/godot/presentation/widescreen_viewport_controller.gd`
- `port/godot/presentation/widescreen_backdrop_frame.gd`
- `port/godot/ui/widescreen_hud_layout.gd`
- `port/godot/tests/render/run_widescreen_presentation_tests.gd`

## Acceptance criteria

- [ ] Supports `16:9` (`426x240` logical canvas), `16:10`, and arbitrary desktop window sizes while keeping original `4:3` (`320x240`) selectable at runtime with zero scene reload loss.
- [ ] Preserves the three original-art safe-frame classes documented in `[[widescreen-settings]]`:
  - `320x88` weekday activity strip (`y=24..111` in `320x240`) with side pillar/frame extension that never reveals out-of-bounds sprite exit targets (`x < 0` or `x >= 320`).
  - `320x160` VN date/encounter/seasonal backdrop + `BUSTUP` portrait aperture (`y=0..159`) with themed side wings / ambient matting outside the `320px` art region.
  - `320x240` full-screen title, calendar, photo album, ending, and event CG plates centered at `1:1` aspect ratio with themed side framing.
- [ ] Reflows HUD panels, command bars, dialogue boxes, speaker plates, and choice menus responsively across `4:3` (`320x240`) and `16:9` (`426x240`) without overlapping portraits or clipping long translated strings.
- [ ] Supports both crisp integer-multiple nearest scaling (`TEXTURE_FILTER_NEAREST`) and smooth aspect-preserving fit (`TEXTURE_FILTER_LINEAR` at output composite) without seam bleeding on tile or glyph edges.
- [ ] Mouse hit-testing and cursor clamping convert accurately between window coordinates, `16:9` HUD regions, and `320x240`-bounded minigame/scene playfields (`TACO`, `RPG_BAT`, `TAIIKU`, `EN_NICHI`).
- [ ] Automated rendered and headless verification (`run_widescreen_presentation_tests.gd`) passes in Docker across `320x240`, `1280x720`, `1920x1080`, `1920x1200`, and `2560x1080`.
- [ ] Code review against `CODING_STANDARDS.md` passes with all findings resolved before moving to `Done`.

## Independent readiness follow-up (2026-10-26)

Widescreen geometry foundation is preserved under `.claude/orchestrator/state/T-30001/foundation-v1/`, tested in isolation but not integrated. User prioritizes baseline repair before expansion.
