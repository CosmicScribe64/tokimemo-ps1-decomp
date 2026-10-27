---
type: review
updated: 2026-10-26
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
