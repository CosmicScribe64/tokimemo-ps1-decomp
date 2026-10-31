---
type: verification
updated: 2026-10-30
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

## Evidence tiers and modes

Every reported result names its mode and its tier ([[phase4-design-2026-10-29]], Playtest). A result without both labels is not evidence for any claim.

| Tier | What it is | Typical producer | Never claims |
| --- | --- | --- | --- |
| T0 | Synthetic, original-free checks: unit tests, invented fixtures, schema and lint passes | `tools/port/run_offline_suite.sh`, `.github/workflows/port-offline.yml`, model route suites | Anything about the original game or a playable port |
| T1 | Headless port runs on the user's `.asset-cache` bundle | Godot `--script` routes in `tokimemo-port` | Equality with the original |
| T2 | Paired emulator-vs-port differential at a source-proven checkpoint | `compare_trace.py`, `compare_capture_pair.py`, `frame_pacing_report.py` | Unvisited routes; real-hardware timing |
| T3 | Real-input GUI routes under Xvfb (production `InputEvent`s) | `tokimemo-port-gui-test` routes | Desktop pacing or audio on the player's machine |
| T4 | Human review and native desktop sessions | the user, `wiki/playtest-pacing-checklist.md`, the visual review queue | Agent review; an agent verdict is never T4 |

| Mode | Settings | Held to |
| --- | --- | --- |
| F (fidelity) | Japanese, 4:3, native 320x240 canvas | Exact equality: state/RNG trace, native canvas RGBA (`compare_capture_pair.py`) |
| E (English) | F plus English text/art packs | State trace identical to F; pixels may differ only inside text/art regions declared from source |
| M (modern presentation) | F or E plus widescreen, filters, window sizes, frame-rate caps | Inner 320x240 canvas hash and trace identical to the same F/E cell; only pixels outside the canvas may change |

Model route suites (for example `run_multi_route_playtest_suite.gd`, whose harness assigns capabilities and injects resource completions) stay T0/T1 unit evidence and are never reported as playthroughs. The `0 / 6,000` hotspot audit (`run_emulator_vs_godot_audit.py`) is historical bounded T2-style evidence from an MAE threshold gate; it is not the exact RGBA gate, and its frames still need re-binding to synchronized checkpoints under [[tickets/T-39008-synchronized-differential-captures]]. Host pacing numbers from Xvfb are relative-only T3 signals; desktop pacing and macOS native audio need a T4 session ([[playtest-pacing-checklist]]).

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

### Long-route driver (input-script v2)

One deterministic script drives both sides from the first command select of golden set A (commands in `tools/port/README.capture-pair.md`, "Long-route driver"):

- `config/port/route-strategies/three_year_default.json` is the plan (a weekly study cycle per term, rest weeks, no club, default answers in scenes); `tools/port/route_strategy.py` compiles it to the expanded `config/port/input-scripts/three_year_default.json` (with `save_relaunch` every 13 weeks from week 4) and `three_year_default_continuous.json` (without them, for goldens).
- The boundary rule is one definition with three implementations that must agree: `tools/emu/route_script.py` (Rule, also `simulate` on a recorded trace), `port/godot/tests/route/route_script_player.gd` and `tools/emu/route_driver/player.lua`. A boundary is after an iteration's logic and before the next pad read; on the original that is 0x800421F0 in `boot_loop` (after `game_mode_dispatch`, before `frame_count += 1`). Segments wait on mode/stage/step/substep, calendar fields, `menu_cursor` and `icon_count`; `pulse` segments press until a predicate holds; checkpoints fire on rising edges and are observed only.
- Each press is checked against the pad mask the game sampled for its iteration. A missing press stops the run (input slip). The one exception is a `pulse` press missed right after a boundary state listed in the script's `unsampled_ok`; it is recorded in `unsampled` and the run continues. Each listed state is a loading state where the original was seen to skip the pad read, recorded with its evidence. Two are listed: 0x02 (continuous-v1, the boundary before 29520) and 0x46/1/0, the first iteration after `holiday_open_wait_bg` 0x46/0/1 (continuous-v2/v5/v6, the boundary before 31691). The comparer fails a port `unsampled` press that the native run lacks in the same checkpoint interval.
- A run passes only on script completion or on a stop pinned by an `unsupported` row for its side. The row must match both the exact stop text and `at_segment`, the segment the player was on when it stopped. The same text at a later week fails. Gaps that have no exact stop text yet (0x39 day notice, 0x30 save poll, `PRODUCTION_REJECTIONS` overlay modes) are in `known_gaps`, which never makes a run pass. `route_script.py verdict` recomputes a run's verdict from its `route.json`.

First results (2026-10-30; outputs under `.asset-cache/route-driver-2026-10-30/` and `.asset-cache/emu/route-golden-three_year_default_continuous-v5/`, `-v6/`):

- Port: the actual installed route stops at the first confirm (`w001-confirm`) with the pinned capability stop. Three runs gave the same stop: port-r1, driver-r2 and driver-r3. In each, the circle is visible at select + 30 (frame 13893). driver-r3 is the shipped `run_route_driver.sh --port` with `ROUTE_NATIVE=…continuous-v5`, and its `compare.json` is the end-to-end report. The driver-r2 directory has a pre-final runner layout, and its `compare.json` is against continuous-v2; `compare/driver-r2-vs-native-v5.json` was a separate manual comparer call. `select-001` matches the original on 122 of 124 state fields; `girls[2].flags` and `scene_work` differ. Only one checkpoint pairs, so this is not evidence that the clocks agree, and drift cannot be measured (n/a). The timing evidence is the constant offset of 7470 (native 21333 vs port 13863 at select) and the confirm iteration below.
- The port's input owner sampled the circle for iteration 13893, but the failure surfaced while committing 13894. The original leaves 0x34 in the iteration that samples the press (21363 = select + 30). This one-iteration lag in the port's confirm path still needs checking once the 0x30 lane lands; the T-39002 playtest p01 shows the same lag.
- Original: the default strategy runs 9 weeks from 1995-04-04 through the May and June month starts. It writes month save states `month-001`/`month-002` for targeted setups (label them as such, not playthrough). It stops exactly in 0x72 (sports festival) stage 3 step 2 on 1995-06-03, where circle every 40 iterations does not finish the scene (pulse exhausted, w009). A TAIIKU-aware policy is the next strategy step.
- Rule parity: replaying the Python rule on the recorded traces gives exactly the delivered edges of both players (1346 edges over 27127 native boundaries; the port's 31). continuous-v6 reran the original with the segment-pinned, `unsampled_ok` Lua player. Its 1346 delivered edges and its checkpoint ids and frames are identical to v5, with the one tolerated miss at 31691. `route-golden-three_year_default-v2` (the save_relaunch script) stops at `w004-save` with the native pin, `passed=true`.

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

These check the framework, not the game's completeness. The wider T0 set (all original-free `tools/port` and `tools/emu` unittests, schema validation and the translation-asset lint) runs with `sh tools/port/run_offline_suite.sh` in `tokimemo-recovery`; see [[ci]]. The separate real-failure integration check requires the ignored T-29023 capture and is explicitly invoked as `tools.port.test_readiness.ReadinessTests.actual_red_receipts_never_pass`, with `ACTUAL_RED_RECEIPTS` set to that run's container path. Absence of the capture is a failure of that integration invocation, not a skipped success.

The phone-book digit table (`girl_phone_digits`, rows 0-12) follows the same pattern. `python3 -m unittest tools.port.test_phone_digits_executable` runs only the decoder tests. The byte check against the pinned executable is invoked explicitly as `tools.port.test_phone_digits_executable.ExecutableTests.executable_rows_match_port_table` in `tokimemo-content`. The pinned `disc/files/SLPM_86.053` must be mounted read-only and `TOKIMEMO_EXE` must name it; the module docstring gives the full command. If the variable or the file is missing, the check fails.

Use `python -m tools.port.readiness --help` and `python -m tools.port.differential --help` inside Docker for the evidence commands. Readiness requires the externally approved policy hash and separate receipt/review directories. Its content root must be an isolated input bundle exhaustively listed in the policy, not the entire asset cache. The current policy remains provisional because execution/content/projection pins and raw-check mappings still require the source coverage audit. No acceptance command should promote it automatically.

Installed verification on 2026-10-26: 41 offline framework tests pass without skips; the separate actual failed-phone-run rejection check passes. Independent Standards and Spec reviews accepted this bounded tooling. The expanded provisional policy requires 55 scenario families; none has accepted evidence. The pre-adoption checklist contained 38 broad families, all preserved in this expansion. That is a coverage-accounting result, not a claim that no individual game functions work.

Each audited checkpoint specifies its expected raw check IDs. Raw producer, route and artifact records must agree on hashes and starting scenario; a wrapper cannot invent a checkpoint from an unrelated passing check. Differential requirements also pin the initial state, input log and port producer, linked to the same checkpoint's actual-app evidence. Inspect `tools/port/test_readiness.py` for invented schema examples; they are not release receipts.

## Actual-run observer integrity

The T-29023 v13 observer tooling has independent Standards and Spec acceptance for its bounded consistency contract. It checks every trace epoch and append-only prefix, explicit successful save/restore lineage, complete ordered check attempts, and image format/dimensions/mip extents against the pinned Godot engine. Unsupported compressed image formats are rejected. Historical restored rows retain their provenance; current-at-drain input labels do not become reconstructed historical input. Earlier failed observer versions and their negative tests remain preserved.

This acceptance does not authenticate a dishonest producer or prove gameplay. Each new runtime/scenario still needs independently inspected source, original inputs and actual captures. The mode38/calendar-profile continuation is a separate scenario change, not automatically accepted by the observer review.
