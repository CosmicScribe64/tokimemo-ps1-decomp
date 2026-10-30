---
id: T-30001
title: Add widescreen presentation
status: In Progress
assignee: root
created: 2026-10-25
updated: 2026-10-30
links: ["[[phase4]]", "[[phase4-design-2026-10-29]]", "[[phase3]]", "[[widescreen-settings]]", "[[tickets/T-30000-plan-phase4-modernization]]", "[[tickets/T-30005-widescreen-settings-feasibility]]"]
---

## Goal

Add native widescreen presentation (`16:9` `426x240` logical / `1280x720`+ display, `16:10`, and ultrawide `21:9` safe framing) to the Godot port while preserving an authentic `4:3` (`320x240`) option and never stretching original pixel art, portraits, or event CGs.

Amended 2026-10-30 per [[phase4-design-2026-10-29]] (revisable): widescreen is presentation only. The native producers keep emitting the same 320x240 Image, byte for byte, in every mode. A presentation composer builds a Wl x 240 frame (Wl 320..560) around that Image and window-space layers (modern text box, UI margins) draw above it. The `426x240` logical game canvas that the game would draw into is rejected; it is kept below only as the superseded original wording.

## Paths (amended 2026-10-30)

Work packages WP0-WP8 in [[phase4-design-2026-10-29]]. Implemented in the wide-core wave (not yet integrated or reviewed):

- `port/godot/presentation/native_frame_layout.gd` (WP1), `tests/render/run_native_frame_layout_tests.gd`
- `port/godot/presentation/scene_aperture_policy.gd`, `wide_frame_composer.gd` (WP2), `tests/render/run_wide_frame_composer_tests.gd`
- `port/godot/presentation/checker_wing_provider.gd` and the read-only `checker_tile()` in `hud_compositor.gd` (WP3), `tests/render/run_checker_wing_tests.gd`
- `port/godot/presentation/modern_dialogue_overlay.gd` (WP4), `tests/translation/run_modern_dialogue_overlay_tests.gd`
- `port/godot/presentation/native_display_host.gd` (WP6 host part), `tests/render/run_native_display_host_tests.gd`

Still open: WP5 preferences (`display.wing_fill`, `accessibility.text_box`), WP6 boot wiring (root-serialized), WP7 verification matrix, WP8 margin trace.

Superseded proposal (2026-10-25): `widescreen_viewport_controller.gd`, `widescreen_backdrop_frame.gd`, `ui/widescreen_hud_layout.gd`, `run_widescreen_presentation_tests.gd`.

## Acceptance criteria

Amended 2026-10-30 (these replace the superseded list below where they conflict):

- [ ] The native 320x240 Image (`last_rendered_image`), RNG call counts and GameState hashes are byte-identical for aspect {original, wide, auto} x filter {integer, nearest, linear} x the 8 window sizes of the design (640x480 .. 3440x1440).
- [ ] Widescreen is a presentation composer (native Image copied byte for byte into the centre of a Wl x 240 frame, wings from a provider) plus window-space layers. There is no 426x240 canvas that game code draws into.
- [ ] aspect=original reproduces the pre-T-30001 boot fit and is selectable at runtime; aspect=auto is wide when W/H >= 1.5.
- [ ] Wings are black by default. Class A (0x34, 0x40, 0x42) may continue the seasonal checker tile with `display.wing_fill=source` (seam-tested in all 4 seasons, fades 239 and 255). Class B and C are always pillarboxed. Room strip, actors (walk targets x=-16, x=340) and HUD stay clipped to the native 320 columns.
- [ ] The opt-in modern text box (`accessibility.text_box=modern`) is window space, shows only approved translations and never changes the native Image.
- [ ] Integer-mode window grabs cropped to the aperture and downsampled by k byte-match the native Image; aspect=original shows zero new diffs in the emulator-vs-godot audit.
- [ ] Code review against `CODING_STANDARDS.md` passes with all findings resolved before moving to `Done`.

Superseded 2026-10-25 list (kept for history):

- (superseded) Supports `16:9` (`426x240` logical canvas), `16:10`, and arbitrary desktop window sizes while keeping original `4:3` (`320x240`) selectable at runtime with zero scene reload loss.
- (superseded) Preserves the three original-art safe-frame classes documented in `[[widescreen-settings]]`:
  - `320x88` weekday activity strip (`y=72..159` in `320x240`; corrected 2026-10-30 from `y=24..111`, see `hud_compositor.gd:202-205`) with side pillar/frame extension that never reveals out-of-bounds sprite exit targets (`x < 0` or `x >= 320`).
  - `320x160` VN date/encounter/seasonal backdrop + `BUSTUP` portrait aperture (`y=0..159`) with themed side wings / ambient matting outside the `320px` art region.
  - `320x240` full-screen title, calendar, photo album, ending, and event CG plates centered at `1:1` aspect ratio with themed side framing.
- (superseded) Reflows HUD panels, command bars, dialogue boxes, speaker plates, and choice menus responsively across `4:3` (`320x240`) and `16:9` (`426x240`) without overlapping portraits or clipping long translated strings.
- (superseded) Supports both crisp integer-multiple nearest scaling (`TEXTURE_FILTER_NEAREST`) and smooth aspect-preserving fit (`TEXTURE_FILTER_LINEAR` at output composite) without seam bleeding on tile or glyph edges.
- (superseded) Mouse hit-testing and cursor clamping convert accurately between window coordinates, `16:9` HUD regions, and `320x240`-bounded minigame/scene playfields (`TACO`, `RPG_BAT`, `TAIIKU`, `EN_NICHI`).
- (superseded) Automated rendered and headless verification (`run_widescreen_presentation_tests.gd`) passes in Docker across `320x240`, `1280x720`, `1920x1080`, `1920x1200`, and `2560x1080`.
- (superseded) Code review against `CODING_STANDARDS.md` passes with all findings resolved before moving to `Done`.

## Independent readiness follow-up (2026-10-26)

Widescreen geometry foundation is preserved under `.claude/orchestrator/state/T-30001/foundation-v1/`, tested in isolation but not integrated. User prioritizes baseline repair before expansion.

2026-10-30: that path does not exist (checked during the phase4 design review). No foundation-v1 code is available; the reference is kept only as history.

## Comments

- 2026-10-30 (wide-core agent): WP0 amendment applied; WP1-WP4 and the WP6 host landed in a worktree patch with Docker tests. Agent review is not human approval; the review gate and status moves belong to the root agent.

- 2026-10-30 (Lane A root hooks, [[tickets/T-39060-phase4-wave1-root-hooks]]; agent work, not human approval): WP6 boot wiring landed. `new_game_entry_boot.gd`, `first_arrival_boot.gd` and `playable_boot.gd` frame their picture with NativeDisplayHost; the old `_layout_display` / `_layout_native_canvas` code and its resize and preference connections are gone. The entry keeps a native-coordinate hit layer aligned to the host aperture. `playable_boot` draws its canvas 1:1 into a 320x240 SubViewport that the host shows with `present_texture` (wide mode: black wings). First-arrival reload frames (mode 0x34) get source wings from `presentation/projection_checker_wing_provider.gd` (the prologue HUD checker, frame argument 0 only). The host now paints black bars itself. The modern text box follows the active prologue line, and while it is on the native Image keeps the original glyphs. WP7 and WP5 remain open as before.
