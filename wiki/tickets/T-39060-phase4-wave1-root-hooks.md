---
id: T-39060
title: Integrate phase 4 wave 1 into main and wire the root hooks
status: In Review
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]", "[[tickets/T-39002-restore-port-runtime]]"]
---

## Goal

Merge `.asset-cache/phase4-patches/phase4-wave1-combined.v2.patch` into main by per-file three-way merges and implement the root hooks in `.asset-cache/phase4-patches/root-hooks.md` (display host in the three boots, modern text box, settings_runtime language wiring, WP-J/WP-I presenter binding, Latin name entry), keeping original-fidelity mode exact.

## Acceptance criteria

- [ ] Frame timing route pads 2 and 3 with default settings: 0 mismatches.
- [ ] Actual command route with default settings reaches command select with frames identical to r13.
- [ ] With English + wide + modern text box the route reaches command select with identical game state and native Image.
- [ ] Phase 4 suites and the core runner pass.
- [ ] Visual QA contact sheets at 1280x720, 1920x1080 and 960x720 inspected.
- [ ] Independent review against CODING_STANDARDS.md with findings resolved.

## Notes

Lane A (main checkout), 2026-10-30. Results are recorded in Comments. Agent review is not human approval.

## Comments

### 2026-10-30 lane A review fixes (agent review, not human approval)

Seven agent-review findings re-checked and fixed in the main checkout; outputs in `.asset-cache/lane-a-fix-2026-10-30/`.

- Modern box after the window clears (accepted, partly rejected): `first_arrival_boot.gd` now keys the box on `FirstArrivalPresenter.message_line()`, the same visibility test the presenter draws native text with (`message_visible` and not `prologue_text_cleared`). It re-binds the stores when `text_pack` or `translated_text` changes identity and on `preferences_changed` (`ModernDialogueOverlay.refresh_line()`). Rejected part: board-scroll, names-crossfade and stage2-fade-out do hold line 1 or 2 in the native window (default captures show the text), so the box is correct there. Known limit: the box shows the whole line while the native typewriter is still revealing it.
- Latin typing taking pad keys (accepted, one claim rejected): typing is now an explicit mode, entered with F3 (only while F3 is unbound) or a window-space toggle button. Esc and Enter leave it. EntryPad drops captured keys from its sample, and clicks on the toggle do not reach pointer confirm. Rejected claim: by default Enter is already START in the entry (`entry_pad.gd`), not confirm.
- Native English speaker inset (accepted): `translation_layout()` keeps the original speaker at the gnsx x (`PrologueTextWork.speaker_x_for`). The speaker stays Japanese because the bundle has no speaker strings, which is a data gap.
- Relaunch matrix (accepted): each relaunch phase mounts a live NativeDisplayHost and checks its layout, display rect, presented width and filter.
- Visual-QA settings draft (accepted): the harness resets `menu.draft` from the effective settings after each aspect change.
- Box slivers (accepted): the box covers the whole text band at the sides and bottom, with square corners.
- Source wings (accepted, plus a defect fix): the 0x34/0xFF/0/0 switch frame still shows the opening picture, and `_present` now frames it as 0x15 (black wings, no error). Before this change it fell back to black with `host.error` set. The modern route now asserts checker wings on all 224 reload frames.

Results: timing pad 2 and pad 3 give 0 mismatches. The default route matches r13 (140/140 native, 12/12 window, 11/11 JSON). The EN+wide+modern+source route at 1280x720 passes, with native frames and state identical to default. The QA route passes, the visual-QA screens pass (72), and the settings matrix passes except the known T-39045 XFAIL.

2026-10-30 English playthrough QA after the lane B merge (main working tree, not committed). Outputs in `.asset-cache/merge-laneb-2026-10-30`, contact sheet in `.asset-cache/visual-qa-2026-10-30/english-playthrough`.
- New observer `tests/qa/run_english_playthrough.gd` (`ROUTE_OBSERVER=playthrough`): the installed playtest with ROUTE_PREFS/ROUTE_WINDOW and, with PLAYTHROUGH_CAPTURES=1, paused window captures at each revealed opening line, each mode after command select and every PLAYTHROUGH_EVERY frames. The ROUTE_PREFS/ROUTE_WINDOW parsing and the duplicated `window_image` now live in `tests/qa/route_presentation_prefs.gd`, used by the modern route, the visual-QA screens and the new observer.
- Defect fixed: the modern box kept the opening's last line (42) over the command reload and command select, where the native window shows the command HUD prompt. `_present`/`_update_text_box` now take `prologue_frame` (the first-arrival presenter drew the Image); other frames clear the box. The modern route asserts the box is hidden on every reload/command frame (`hidden_command_frames`).
- Defect fixed: lines 17 and 21 read "…herself.)" with no opening parenthesis, because the source draws the monologue opener as a decoration piece (`parenthesis_open`, speaker 31). `text_translation_store.opened_parenthesis()` restores it when a text closes a parenthesis it never opened; run_english_bundle_open checks the rule and every bundled line (40/40 still fit).
- Equivalence: the EN + wide + modern playthrough (with captures) has state JSON 10/10 and native frames 125/125 identical to the default playtest (`stage2-fade-out.png` is a window capture); the final default playtest equals the pre-fix one (126/126, 10/10).
- Visible gaps left (data): lines 0 and 8 (player-name token) and 3 (native layout overflow) are not in the bundle; dynamic lines 100/101 (birthday, blood type) have no English; no English speaker labels; command prompts exist in ui_text (`c:main:8005a668:*`) but the box does not follow the command HUD message.
