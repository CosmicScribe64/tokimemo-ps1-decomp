---
id: T-29031
title: Verify native hand and triangle cursor selection
status: Done
assignee: root
created: 2026-10-26
updated: 2026-10-27
links: ["[[port-testing]]", "[[port-readiness-review]]"]
---

## Goal

## Reviewed prompt correction applied (2026-10-26)

Root applied the independently accepted three-file `prompt-role-v2` patch. Actual canonical Docker runs pass 665 focused checks and 42 legacy assertions with no script errors or warnings. The presenter now requires real committed text/voice/marker state and uses original actor2/frame1 for continuation; selection actor0 remains unchanged. Missing live phone state explicitly remains unsupported. No actual phone route or full visual parity is claimed. Application receipt and clean logs: `.claude/orchestrator/state/T-29031/prompt-role-v2/root-live/`. An initial patch-path check failed before writes and the premature missing-script test log is retained. Current production digest is `9afe1ec1e604f572d3f910365bf679971076c061dcca71e2e65a449021ab2e0e` (98 files); V19 retains its earlier `abadc538` snapshot.

## Original goal

Check the user's observation that the port frequently shows a hand cursor where the original may use a multicolored triangle. Establish source-backed cursor selection by screen and state before changing artwork or behavior.

## Acceptance criteria

- [x] Trace original cursor resource, selector, position, animation and visibility for command/holiday menus and relevant dialogue/choice screens.
- [x] Compare current port selectors and actual captures with the original source and authentic assets; distinguish confirmed behavior from unresolved cases.
- [x] Fix confirmed mismatches with focused selection regressions and actual-screen captures, without replacing genuine source art.
- [x] Use a serialized original-game comparison where static evidence cannot establish the distinction.
- [x] Independent review before closure; no universal hand/triangle assumption or broad visual-parity claim.

## Work

Independent cursor audit is running in `.claude/orchestrator/state/T-29031/cursor-audit-v1/`. May 7 save repair and fresh New Game work continue in parallel. No emulator lease is granted to this lane yet.

## User position references

Two user-provided 320x240 screenshots are preserved only in ignored `.asset-cache/port/cursor-user-references/`: `school-trip-choice.png` shows a hand near (275,191), beside the upper of two dialogue choices; `room-dialogue.png` shows a differently oriented hand near (281,202), at the dialogue box's lower-right. These are approximate visual locations, not yet native sprite origins/hotspots or proof of the prompt's state. Audit selection versus continuation roles, animation frame, orientation and box-relative anchoring separately from command-menu hand/triangle behavior. Original attachments remain unchanged.

User also supplied a calendar/date-selection highlight reference, preserved unchanged at ignored `.asset-cache/port/cursor-user-references/calendar-highlight.png`. Extend the audit to the highlighted date cell's coordinates, dimensions, color/alpha, draw order and movement, distinguishing the hand cursor from date eligibility/selection markers and panel borders. Do not infer correct geometry solely from the screenshot.

Additional user alignment references: ignored `help-highlight.png` (Q&A menu green selection rectangle and hand) and `battle-pointer.png` (RPG command menu yellow pointer beside the bottom row). Compare original rectangle extents, glyph baselines, sprite origins and selected-row spacing. Do not center by appearance without native evidence; distinguish preview renderer layout from actual original-game layout.

## All 4 Cursor Audit Gaps Resolved & Integrated (2026-10-27)

Integrated `cursor-actor-and-style-lifecycle-v1` (`.claude/orchestrator/state/T-29031/root-cursor-actor-and-style-lifecycle-integration/application.json`), `june-restart-reload-and-quarter-predicates-v1` (`.claude/orchestrator/state/T-29031/root-june-restart-reload-and-quarter-predicates-integration/application.json`), and `rpg-bat-authentic-actor130-and-vram-v1` (`.claude/orchestrator/state/T-29024/root-rpg-bat-authentic-actor130-integration/application.json`) onto canonical `main`:
1. **Gap #1 (`telephone_overlay_presenter.gd`)**: Continuation prompt uses `actor2/frame1` (`prompt-role-v2`, `665/665` PASS).
2. **Gap #2 (`dialogue_textbox_presenter.gd`)**: Choice selection uses `actor0` hand (`frame 0/156/160`) while text-wait uses `actor2` (`frame 1`) (`run_cursor_actor_and_style_lifecycle.gd` `482/482` PASS).
3. **Gap #3 (`command_hud_presenter.gd` + `playable_boot.gd`)**: Square button (`0x8000`) cycles `cursor_style` (`0 -> 1 -> 2 -> 0`, `cursor_frame_id` `0` hand / `156` cat / `160` triangle) across command and holiday menus (`482/482` PASS).
4. **Gap #4 (`subgame_overlays_presenter.gd`)**: `RPG_BAT` command menu uses authentic `actor130` (`ACT(96 + 0x22)`, `sequences 5/6/7`, hit rectangles `Rect2i(-43, 57/72/87, 35, 15)` -> screen `Rect2i(117, 177/192/207, 35, 15)`) decoded from `disc/files/CDROM/BATTLE/*.BIN`, visually verified against `.asset-cache/port/cursor-user-references/battle-pointer.png` (`run_subgame_overlays_tests.gd` `211/211` PASS).
- Reviewed against `CODING_STANDARDS.md`; zero unresolved findings.
