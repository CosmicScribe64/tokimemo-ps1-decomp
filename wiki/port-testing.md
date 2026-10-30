---
type: verification
updated: 2026-10-29
---

# Port testing and acceptance

The Japanese baseline is not yet accepted as a complete game. See [[port-readiness-review]] for the reproduced failures and [[tickets/T-29029-port-readiness-evidence-gate]] for the evidence validator. Later modernization and translated content need their own regression policies.

## What each test proves

| Test | Useful evidence | Does not prove |
| --- | --- | --- |
| Unit and synthetic scenarios | Math, parsers, branches, malformed inputs, local contracts | Reachability or a playable game |
| Original instruction probes | Native arithmetic, memory effects and call behavior at a captured boundary | Complete original gameplay or presentation |
| Differential traces | Ordered state, RNG, input and service effects for the compared scenario | Unvisited routes or unmeasured hardware behavior |
| Actual application input runs | Connected production scenes, interaction, timing, saves and presentation | Normal New Game if launched from a captured boundary |
| Packaged-game playthroughs | Fresh install, normal start, persistence, complete route and ending on that package/platform | Every route or every supported original release |

Keep all five. Reuse `tools/port/run.sh`, `compare_trace.py`, and `verify_tile_diff.py` for their bounded purposes. A screenshot comparison needs the original capture provenance, exact app producer and matching gameplay checkpoint; two similar files alone do not establish game fidelity. Audio needs real output and native command/timing comparisons, not an `audio_verified` assertion or silent test driver.

## Differential testing

Run the original PS1 executable and the port with the same documented starting state and input sequence. Compare ordered mode/stage/step transitions, calendar and relationship effects, RNG state and draw count, input edges, and service/audio requests and completions. Use the original executable as the reference; a second implementation of the same formulas can repeat the same mistake.

`tools/port/compare_trace.py` reports the first divergent record and field. The readiness work adds a comparison report tied to the traces, so the gate can verify them and rerun the comparison. A `passed` flag alone is insufficient. Pin the projection, original executable, producer scripts, initial state and inputs. Missing or truncated traces, unequal results, and missing required fields fail. Do not resynchronize after divergent RNG or drop events to obtain a match. Record intentional adaptations separately from exact baseline behavior, with a reviewed projection or tolerance.

Compare visuals and audible output separately at corresponding gameplay checkpoints. Integer state and RNG should compare exactly; PS1/software-rendered color, mixing and presentation timing need specific justified tolerances. A numeric trace pass cannot waive missing graphics or audio, and a matching image cannot waive incorrect simulation. Captured-boundary differential cases complement normal-start routes without replacing them.

## Required coverage

### Paired baseline playthrough

The user explicitly requires running the game in both the emulator and the port to confirm baseline behavior and appearance. Start with Japanese original presentation and default gameplay timing, with translation packs and optional pacing adaptations disabled. Record the same player decisions through normal New Game, the complete route, ending and save/relaunch. Bounded captures help diagnose differences but do not replace this paired route.

Keep host elapsed time, emulator CPU cycles, vsync count, game-update count and CD/audio completion separate. Match checkpoints by ordered source state and input history, then inspect animation phase and displayed-buffer identity before comparing images. Measure different wait durations as discrepancies; do not silently delete wait frames, force service completion, reseed RNG or shift to a nicer-looking frame. Document intentional host adaptations separately. OpenBIOS can change hardware-service timing, so its timing is a measured reference with that limitation rather than a claim of exact console timing.

For translation verification, first establish the corresponding Japanese render, then compare the translated version against that baseline: only intended text/art regions may change. Original dialogue must not remain beneath an unrelated opaque panel. Modern settings need actual menu captures and keyboard/controller navigation review in addition to persistence and value tests.

### Preflight before long gameplay runs

Under [[tickets/T-29032-plan-port-by-reachable-boundaries]], distinguish **known unsupported**, **implemented but untested**, **regression**, and **verified for a bounded case**. Before extending a route, inspect its upcoming source predicates and list expected unsupported stops with their service dependencies and owners. For month-start, ordinary May coverage does not cover quarter June, September notice, year rollover or vacation bank selection. Target those branches with original-backed short cases before spending another full run rediscovering an explicit guard. Targeted setup is test evidence, not a normal-start playthrough.

Follow preflight with actual player-input runs to catch integration, lifetime and accumulated-state failures. Preflight cannot establish their absence. It must not skip a known unsupported branch, fabricate its service completion, shrink the readiness policy, or turn a failed continuation into an overall pass. Use original-source/native behavior as the reference. Tests that merely reproduce the implementation or image-generation template are insufficient.

The provisional policy at `port/verification/readiness-policy.json` is a starting checklist, not an exhaustive certification. Before acceptance, map original reachable modes, event conditions and endings to concrete scenarios, noting genuinely unreachable development code separately. Review that map against `src/main/game_mode.c`, the scheduler/loader, overlay source and [[phase3]]. Expand broad scenario families into named branch cases. No shrinking the checklist to match what happens to work.

Require a normal New Game success route through three years, the source-selected ending, credits, last photo, acknowledged clear-save and reload. Require the normal failure route separately: Yoshio13 may bypass credits and last photo, entering clear-save directly. Test every supported heroine's eligibility and ending, with source-designed boundary recipes supplementing full routes. Cover calendar, health, commands, encounters, dates, clubs, school/seasonal events, subgames, original media and desktop delivery. Rare-branch recipes must state their injected initial conditions and cannot stand in for a normal-start playthrough. Walkthroughs help choose actions; the PS1 executable remains the behavioral reference.

## Source coverage adoption priorities

The source proposal for T-29029 contains 235 unexecuted case records, not 235 accepted scenarios or an exhaustive reachability audit. Preserve every existing broad family and expand it with named source branches. Per-girl dialogue/date/seasonal callbacks, career and album paths, subgame caller/result families and exact media/service selectors still need closure before any audited-policy claim.

1. Preserve the existing three-year success route and its credits requirement. The proposal's conservative normal failure reference is useful separately; it cannot satisfy that success/credits route. Failure evidence must positively demonstrate the original C2-to-C4 bypass, rather than fabricate credits or waive a failed checkpoint.
2. Add Rei12 as a separate required ending family. The original selector preempts ordinary ranking when Shiori points < 500, Rei flags byte3 >= 70 and scene flag79 == 2. Preserve captured-boundary eligibility diagnostics and add actual normal New Game histories for all 13 supported heroine IDs 0..12 plus Yoshio failure13. Injected relationship/candidate/date fields do not establish earned route reachability.
3. Preserve successful-ending clear-save obligations and test the last-photo paths separately. Normal input 0x860 chooses clear-save; neutral timeout reaches album; replay/options_dirty returns title. Choose source input for the required save route and test timeout/replay separately. Require acknowledged game/system writes, restart, reopen/reload and matching continuation. Save counters changing in memory do not prove persistence.
4. Expand telephone stages, scheduler priorities, health transitions, seasonal/club callers and borrowed overlay/subgame returns from the source inventory. Imported callback tables and per-girl predicates must remain blocking gaps until resolved. Static loader/table word matches prove mapping constants only; function probes do not establish player reachability.

The proposed policy additions remain provisional and deliberately have no invented raw-check IDs, native snapshots, input hashes or executed receipts. Bind those only after reviewing real producers and captures. Actual state/RNG, corresponding viewport and audible output are separate mandatory evidence; no fixture, branch count or metadata assertion substitutes for them.

## Evidence and review

Every acceptance run records its runtime file inventory, engine/container identity, original-content hashes, producer script, starting state, input log, state/RNG checkpoints, service outcomes and output hashes. Raw screenshots come from the running viewport. Keep raw evidence under ignored `.asset-cache/` or `.claude/orchestrator/state/`; commit only tooling and policies.

Missing, failed, skipped, unsupported, stale or unreviewed required results block acceptance. Synthetic evidence cannot satisfy real-input requirements. Required checkpoints must all be reached; a clean process exit is insufficient. A changed runtime or asset invalidates the prior run. A bounded passing rerun does not erase another unresolved mandatory failure.

Freeze and externally pin the reviewed policy hash when running the gate. Reviewers must inspect raw artifacts and the implementation, bind their decision to the exact receipt/policy/runtime hashes, and resolve findings. Producer and reviewer must differ. The coordinator verifies who performed the review; names in JSON are audit metadata, not authentication. Checksums detect changed evidence, not false evidence.

The gate reports accepted evidence for its named scope and missing coverage. It does not set the roadmap to Done or calculate a full-port percentage. Final completion also requires an independently reviewed reachability audit, all mandatory scenarios, packaged builds on the declared desktop targets and a reviewed list of fidelity deviations. Neither an agent's summary nor a large passing assertion count can replace that decision.

## Reproduced baseline failure

T-29023's `baseline-phone-v1` used the actual application, normal input events and viewport capture. It reached Sunday and Yoshio's profile, then observed the wrong mode and an exit after neutral input; save/load checks also failed. The receipt records those failures and separately marks native audio, normal New Game and full routes untested. Those screenshots are useful failure evidence, not completion evidence.

## Running the framework checks

The offline tests contain invented fixtures and need no original game files. Run them from the repository root:

```sh
docker run --rm -e PYTHONDONTWRITEBYTECODE=1 \
  -v "$PWD:/repo:ro" -w /repo python:3.12-slim \
  python -m unittest tools.port.test_readiness tools.port.test_differential
```

These check the framework, not the game's completeness. The separate real-failure integration check requires the ignored T-29023 capture and is explicitly invoked as `tools.port.test_readiness.ReadinessTests.actual_red_receipts_never_pass`, with `ACTUAL_RED_RECEIPTS` set to that run's container path. Absence of the capture is a failure of that integration invocation, not a skipped success.

Use `python -m tools.port.readiness --help` and `python -m tools.port.differential --help` inside Docker for the evidence commands. Readiness requires the externally approved policy hash and separate receipt/review directories. Its content root must be an isolated input bundle exhaustively listed in the policy, not the entire asset cache. The current policy remains provisional because execution/content/projection pins and raw-check mappings still require the source coverage audit. No acceptance command should promote it automatically.

Installed verification on 2026-10-26: 41 offline framework tests pass without skips; the separate actual failed-phone-run rejection check passes. Independent Standards and Spec reviews accepted this bounded tooling. The expanded provisional policy requires 55 scenario families; none has accepted evidence. The pre-adoption checklist contained 38 broad families, all preserved in this expansion. That is a coverage-accounting result, not a claim that no individual game functions work.

Each audited checkpoint specifies its expected raw check IDs. Raw producer, route and artifact records must agree on hashes and starting scenario; a wrapper cannot invent a checkpoint from an unrelated passing check. Differential requirements also pin the initial state, input log and port producer, linked to the same checkpoint's actual-app evidence. Inspect `tools/port/test_readiness.py` for invented schema examples; they are not release receipts.

## Actual-run observer integrity

The T-29023 v13 observer tooling has independent Standards and Spec acceptance for its bounded consistency contract. It checks every trace epoch and append-only prefix, explicit successful save/restore lineage, complete ordered check attempts, and image format/dimensions/mip extents against the pinned Godot engine. Unsupported compressed image formats are rejected. Historical restored rows retain their provenance; current-at-drain input labels do not become reconstructed historical input. Earlier failed observer versions and their negative tests remain preserved.

This acceptance does not authenticate a dishonest producer or prove gameplay. Each new runtime/scenario still needs independently inspected source, original inputs and actual captures. The mode38/calendar-profile continuation is a separate scenario change, not automatically accepted by the observer review.
