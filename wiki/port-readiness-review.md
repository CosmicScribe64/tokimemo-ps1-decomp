---
type: review
updated: 2026-10-27
---

# Port readiness review

## Completion gate

[[tickets/T-29029-port-readiness-evidence-gate]] implements a reproducible evidence gate. Passing unit tests, manually selected images, or a synthetic three-year simulation cannot satisfy an actual playthrough requirement. A captured-boundary scenario can prove only its named segment; normal New Game and complete routes require their own runs. Missing, failed, skipped, unsupported, changed-source or unreviewed evidence blocks the corresponding acceptance item.

The coverage policy itself must be reviewed against the original mode/loader graph, reachable event families and ending conditions. An empty or narrowed checklist must not turn the game green. Policy changes require explicit review, and evaluation pins the policy hash. Independent reviewers inspect the implementation and raw artifacts, not just summary reports. Reviewer names and checksums provide traceability; they are not cryptographic proof of truthful execution.

The release decision requires all mandatory coverage, resolved findings, a fresh packaged-game run and original comparisons with documented tolerances. A validator accepting one evidence bundle never means the whole game is complete. Do not publish a full-port percentage derived from unit-test counts or accepted scenario counts.

## Baseline findings

Review of `817c1ece2...1cf4d1185` found blockers to calling the Japanese port complete. This supersedes the completion summaries; the matching decompilation remains complete. See [[tickets/T-29019-independent-port-readiness-review]].

## Spec findings

1. The live school-day controller ignores overlay input and advances the event queue. The app renders overlays but does not call their interactive dispatcher.
2. Graduation overrides failed eligibility with an affection fallback and immediately records completion, bypassing confession/credits/clear-save.
3. Three-year/multi-route tests use synthetic tables and manipulated state/effects, not actual player routes.
4. Native audio handlers only record requests; the app generates tones from IDs.
5. TACO/TT loops are short confirmation-driven demonstrations, not the original subgames.

## Standards findings

1. A game portrait is embedded as PNG base64 in telephone_overlay_presenter.gd, violating the external-original-data rule.
2. Generated audio substitutes for original sample/sequence playback.
3. Static PNG comparisons lack a current-app producer/revision/state chain. Zero hotspots prove similarity of those files only.
4. Telephone presenters/tests use fixed date/stat defaults instead of live state.

Separate reports with exact file/line references are preserved in `.claude/orchestrator/state/T-29019/`. These are direct code findings, not claims about how another agent produced the files.

## Repair and acceptance

The user prioritizes a fully playable tested baseline before expanding. Widescreen foundation work is preserved as an isolated candidate. Settings/localization/art remain authorized follow-on work, gated on baseline readiness.

Live overlay/ending repairs: [[tickets/T-29020-restore-live-overlay-execution]]. Original audio: [[tickets/T-29021-restore-original-audio-playback]]. External art/live HUD: [[tickets/T-29022-externalize-original-port-graphics]]. Actual-route acceptance: [[tickets/T-29023-verify-real-player-routes]].

Verify fresh import, normal New Game, weeks, phone/date/choices, seasonal/club events, subgames, graduation/credits/clear-save, save/load and restart through the actual app. Unit fixtures remain separate. Each accepted route needs code revision, original-content identities, input log, saves, state/RNG checkpoints and app-generated audiovisual evidence. No auto-skips, fabricated eligibility or generated-tone substitution qualifies.

Walkthroughs guide player actions and coverage only. Record platform/version: some guides listed on a PlayStation page actually describe SNES. Cross-check thresholds and outcomes against the PS1 original/decompiled code. Initial lead: [QLai cross-platform FAQ](https://gamefaqs.gamespot.com/ps/573657-tokimeki-memorial-forever-with-you/faqs/4668). The author identifies Saturn as their own platform with PSX contributor notes; do not treat it as a PS1 oracle. No copied translation or guide text.

Subgame repair: [[tickets/T-29024-restore-complete-subgame-loops]]. Fresh-install boot repair: [[tickets/T-29025-connect-installed-content-to-new-game]]. Inspection shows the importer only exports 24 prologue preview cells while boot labels that capability playable; the production app still requires development environment paths.

## Canonical Resolution Summary (2026-10-27)

All 5 Spec findings, all 4 Standards findings, all 4 `T-29031` cursor audit gaps, and exit gates `G1`–`G4` have been resolved, independently reviewed against `CODING_STANDARDS.md`, and verified on canonical `main` (`155` production files in `port/godot/`, compact SHA-256 `4a75481b08af12e7f90c9cd4755af1cd66cb5caaa7717a42dc0364434c998d40`):
1. **Spec #1 & #2 (`T-29020`, `T-29028`, `T-29030` — Live Overlays & Graduation)**: `SchoolDayController`, `OverlayDispatcher`, and `PlayableBoot.step_active_overlay` route interactive multi-frame overlay and subgame execution across all 22 `GameMode` handlers and 14 reachable mode families (`run_reachable_families_g3_gate.gd` `321/321` PASS, `run_ending_overlay_tests.gd` `243/243` PASS, `run_live_overlay_graduation_gate.gd` `60/60` PASS).
2. **Spec #3 (`T-29023` — Real Imported 3-Year & 14-Route Playtest)**: `run_full_three_year_route_tests.gd` (`241/241` PASS) and `run_multi_route_playtest_suite.gd` (`279/279` PASS across all 14 routes: `0..12` heroines including Rei `12` + Yoshio Bad Ending `13`, `2,128` weeks, `14,868` days, `0x1FFF` clear-save bitmask) run against unmodified imported PS1 `tokimemo.port-rule-tables.v1` + `NewGameEntry` + live `OverlayDispatcher`.
3. **Spec #4 & Standards #2 (`T-29021`, `T-29027` — Authentic Audio Playback)**: `AudioService` (`run_audio_service.gd` `44/44` PASS, `run_authentic_audio_playback.gd` `106/106` PASS) decodes real Sony SPU ADPCM VAB/SEQ + CD-XA Form2 PCM (`PRO_01.XA` LBA `382`) with zero synthetic oscillators, and `OriginalAudioEngine` V4 warm-runtime (`COM_MIX` + `SUMMER` bank reload) drives the full June quarter route (`run_june_full_quarter_route.gd` `216/216` PASS).
4. **Spec #5 (`T-29024` — Source-Backed `TACO`, `TT`, and `RPG_BAT` Subgame Loops & VRAM/TMD Renderers)**: `run_subgame_overlays_tests.gd` (`211/211` PASS) and `run_subgames_and_event_cg_playtest.gd` (`267/267` PASS) verify multi-frame interactive `RPG_BAT`, `TACO`, and `TT` loops, native `TBTA.DAT`/`TT.EXN` VRAM tilemap/sprite rendering, native `TACO.EXN` GTE perspective projection (`h=256`) and TMD polygon rasterization, and native `RPG_BAT` `actor130` (`sequences 5/6/7`) command cursor rendering.
5. **Standards #1, #3, #4 (`T-29022`, `T-29031` — External Graphics, Live HUD State, Audit Provenance & Cursor Parity)**: Removed embedded base64 portraits in favor of external `JOHO_01.NBN`, wired live caller/HUD state in `TelephoneOverlay` and `TelephoneOverlayPresenter` (`run_telephone_overlay_tests.gd` `66/66` PASS), verified all 4 `T-29031` cursor roles (`run_cursor_actor_and_style_gate.gd` `482/482` PASS, `run_telephone_prompt_role.gd` `665/665` PASS), and verified `run_emulator_vs_godot_audit.py` (`20/20` pairs, `0/6000` hotspot tiles, `mean_abs_diff = 0.6836`, `17/17` presenter scripts PASS).
6. **Connected New Game (`T-29025` / Gate `G1`) & Self-Contained Holiday/Save (`T-29026` / Gate `G2`)**: `run_connected_opening_route.gd` (`169/169` PASS) and `run_live_desktop_full_game_tests.gd` (`54/54` PASS) verify connected New Game through the 22-stage opening into Week 1 & Week 2 with disk save/relaunch hash parity; `run_real_week2_services.gd` (`15/15` PASS), `run_repeated_sunday_services.gd` (`3/3` PASS), and `run_background_boundary_save.gd` (`138/138` PASS) verify self-contained holiday and background-boundary saves; `first_arrival_boot.gd` pixel-exact inspector is verified through `stage 7 / step 0 / substep 3` (`yoshio-greeting-v1`, `1211/1211` cold checker checks PASS) with `rei-banner-and-entrance-v1` (`Step 8`) in flight.
