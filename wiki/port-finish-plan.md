---
type: plan
updated: 2026-10-27
links: ["[[port-manager-handoff]]", "[[port-testing]]", "[[tickets/T-29034-plan-single-worker-port-completion]]"]
---

# Finish the original port: one manager, one implementer

The shortest credible route is to connect the existing source-backed components into real gameplay, then expand that connected route systematically. Keep the current core, use one implementation agent, and have the manager independently review and run each delivered change. Do not restart the old agent fleet or rebuild the project around a new architecture.

This execution plan supersedes the old parallel-lane instructions. It preserves the scope and acceptance requirements of [[phase3]] and [[port-testing]]. It was prepared from current code, recorded reviews and paused checkpoints, not a fresh gameplay run or an exhaustive audit of every historical conversation. The incoming manager must review the plan and resolve discrepancies before marking T-29034 Done. Development remains paused until that handoff starts.

## Actual starting point

Baseline inspected: `28385de82` on main. Phases1/2 are complete. The Japanese port is incomplete; it is not merely awaiting polish or a final test pass.

| Area | What exists | What is still missing or unproved |
| --- | --- | --- |
| Core | Ordered session stepping, fixed-width arithmetic, RNG/input ownership, rollback of owned logical state, route identity registry | Complete controllers and service/content coverage; a registered mode is not a playable mode |
| New Game | Real keyboard entry through the first prologue line, original music recorded in a bounded run | Continuous actors/HUD/board/dialogue through normal command gameplay; self-contained production import |
| Calendar | Captured-save route evidence through May28; June controller V3 applied with controlled checks | Actual played June transition, native audio continuation and restart; later branch families |
| Audio | Opening playback; reviewed finite warm-runtime V4 | Canonical composition, lifecycle/save handoff, sustained output, editor/platform qualification |
| Scenes and endings | Source, partial rules/controllers and isolated repair packets | Reachable interactive execution, callbacks/resources, correct returns and full ending persistence |
| Subgames | Bounded TACO combat/stage/enemy components | Connected original presentation, remaining mechanics/results and TT; synthetic previews do not qualify |
| Testing | Differential tools, actual-app observer, readiness gate and bounded month preflight | Reviewed comprehensive coverage map and accepted complete routes on the final package |
| Translation | User reports text translation finished; existing returns/worktree preserved | Image work and integration remain separate from Japanese baseline completion |

A historical 100% badge, 36-month simulation, large assertion count or old ticket marked Done cannot override these limits. The current registry still explicitly contains unsupported, preview-only and partial modes. Do not derive a percentage from its entry count.

## Lessons that change the workflow

| Observed problem | Required correction |
| --- | --- |
| Tests advanced a synthetic calendar while real overlays ignored input; fabricated affection fallback bypassed an ending. | Drive the production app with ordinary input. Check original eligibility and caller/return state. A missing scene stops with its source context; never skip it to keep a route running. |
| June1 was explicitly unsupported but discovered after repeated ordinary-week runs. | Before the next route segment, inspect its next calendar/event predicates and their dependencies. Probe branch families locally before the long run. Keep the actual route afterward. |
| Passing screenshots lacked a producer chain; TT used generated placeholder scenery. | Capture the running app at a named gameplay checkpoint. Compare to an independently sourced original checkpoint. Synthetic visual fixtures never enter the progress gallery. |
| Image alignment tests reused the same erroneous layout assumptions as generation. | Derive expected placement from native coordinates/draw order and inspect the actual menu. Check cursor hotspot, glyph bounds and highlight bounds separately; centering is not universally correct. |
| May7 saving rejected an equivalent background cache arrangement. | Validate source-visible identity while preserving physical cache state. Fix the invariant, not a whitelist for a date or one screenshot. |
| Native audio passed component tests but failed editor/owner transitions or underrun checks under load. | Qualify load, ownership, output, teardown and restart together. Measure timing serially under documented conditions, then test realistic load separately. Keep logical correctness and device/timing claims distinct. |
| Accepted pieces accumulated in stale isolated trees; composition changed their assumptions. | One live baseline and one candidate. Review the composed diff and run it in the canonical app. A component review transfers only to unchanged behavior and dependencies. |
| More receipts, wrappers and broad retests consumed time without extending gameplay. | Use existing evidence tooling. Add a mechanism only for a reproduced gap. Every implementation cycle must enable a named player transition or close a named acceptance blocker. |
| Cleanup removed old dirty bookkeeping before its backup ran. | Preserve before deleting, verify bytes and archive completeness, then remove from an explicit list. No broad cleanup during port integration or timing qualification. |

## Architecture: preserve the core, repair the joins

Keep `GameSession.step`, native math/RNG/clock/input, the mode identity registry, original readers, renderer primitives and existing test tooling. The current callbacks and explicit service phases provide a useful foundation. Do not replace them with an ECS, a general event bus, floating-point game rules or a second gameplay runtime.

For each enabled route, bind its source controller, input handling, original resources, service owners, renderer and return path together. Installed capability claims must reflect that complete binding. Source eligibility remains independent: unavailable content must not change which event the original selects.

Use prepare → validate logical intent → commit → publish physical effects → acknowledge completion. Keep one real audio/device owner through scene handoffs; do not replace native state with a new success-shaped object. Publication failure after commit must not rerun committed simulation. Keep prologue and SERIFU interpreters distinct where their native semantics differ; share physical resource/render/audio facilities only where their contracts agree.

After two real consumers pass, extract duplicated lifecycle code if it is causing a demonstrated bug or repeated implementation cost. Refactor one consumer at a time with the same input suffix and state/RNG/service comparisons. Do not make a broad refactor a prerequisite for the first playable New Game.

Save support must use legitimate stable boundaries and preserve the state required for continuation. Define which runtime state is serialized and which can be reconstructed equivalently, with a proof and a restart test. An old May14 debug save lacks native VM/SPU/queued-audio state; it cannot prove native recovery by adding fabricated fields. Original game saves and debug F5/F9 snapshots have different contracts.

## Ordered delivery milestones

These are completion gates, not calendar estimates. Each includes implementation and actual-app checks. If an unavoidable event blocks an earlier milestone, move that event's implementation forward; never suppress it to preserve this order.

| Gate | Work and existing tickets | Exit evidence |
| --- | --- | --- |
| G0: establish the live baseline | Inspect HEAD, protected files, active containers and retained candidates. Reproduce the smallest current first-scene run. Record each candidate as applied, reviewed-but-unapplied, or incomplete. Audit the native extension's editor/runtime load early. T-29025/27/29/30/32. | One current baseline, exact known stop, reproducible commands and a short dependency list. Failures remain visible. No acceptance of copied old PASS reports. |
| G1: fresh New Game reaches normal gameplay | Finish first-visible actor/HUD/text-cache composition; integrate authentic board loading; complete subsequent prologue steps and same-session command handoff. Make the normal import/installed launch generate the needed pack without developer environment paths. | Empty user-data directory + supported original input → New Game/name entry → prologue → ordinary command → first result → next selectable command. Input release/repeat/focus, actual original graphics/voice/BGM and save/relaunch at a legitimate boundary work. Publish real captures. |
| G2: first quarter is connected | Compose warm audio without losing opening guards; implement ownership/import dependencies. Preflight ordinary month, quarter, September notice, year rollover and vacation bank selection. Continue a fresh lineage through May and June1 to a subsequent controllable day. T-29023/27/32. | Source/native short cases for relevant predicates, real played June with audiovisual output, and restart/reload with matching continuation. The 3,832 controller checks remain supporting evidence only. |
| G3: complete the reachable gameplay families | Expand telephone/profile/invitation/date/choice/encounter, health/rest, clubs, seasonal/school events, borrowed overlays and menus, using the source coverage map. Finish original TACO/TT mechanics, rendering, audio, results and returns. T-29020/21/22/24/28/30/31. | Every reachable family has implemented dependencies and source-backed branch checks; each family has actual entry, interaction, outcome and return evidence. No auto-exit handlers or preview replacements. Cursor/highlight/render checks occur in these live contexts. |
| G4: complete earned routes and persistence | Run normal-start histories through all three years, success and failure endings, credits/last photo where required, clear-save and reload. Cover all13 heroine IDs including Rei and Yoshio failure per existing policy. | Actual source-eligible routes, no injected affection/date/queue/ending state. Original comparisons at branch checkpoints. Acknowledged disk writes, process restart and reopened game/system/album data. Rare-branch fixtures supplement rather than replace earned histories. |
| G5: release candidate | Freeze candidate runtime/content/policy. Run clean import and packaged gameplay on declared platforms; finish controls, file selection, audio/device/focus, error recovery and install/restart checks. | All mandatory policy items accepted, no open required failures/skips/unsupported behavior, independent manager review, source coverage audit, fresh package route and documented fidelity deviations. Only then call the declared baseline complete. |

A complete macOS/Linux/Windows claim requires actual testing of each target; a Linux/Dummy run cannot certify macOS audio or packaging. First get the user's macOS build playable and Linux automation reproducible. Preserve the broader declared desktop requirements; explicitly report an unavailable platform as blocked rather than silently dropping it. Likewise, start with the primary reference PS1 release; other release profiles require their own import/content/runtime checks before advertised support. Demos are not interchangeable with a complete release.

## Make the remaining work finite

Extend the existing preflight/readiness inventory rather than introducing a second tracker. Build the reachable coverage map incrementally from original dispatch/loader tables, scheduler predicates, per-girl callbacks, subgame callers and ending selection. Search for unsupported guards and placeholders as leads, not an exhaustive proof. Audit indirect/table dispatch and successful-looking no-op handlers as well.

For each case record: source function/predicate, real entry and caller, required content/services, expected return/state effects, implementation status, smallest original-backed test, actual-route evidence and next blocking dependency. Use four distinct states: missing implementation, implemented but unverified, failing regression, and verified for a named scope. Mark original dead/development code separately with evidence. Keep the existing policy's broad families; do not narrow it to fit the implementation.

At each milestone, reconcile three lists: source-reachable cases, implemented capability bindings, and accepted evidence. An unmatched item is work, not an implied pass. The current 55 scenario families and 235 proposed case records are starting material, not a complete or executed coverage inventory.

## Two-person operating loop

1. **Manager selects one deliverable.** Give the implementer the exact baseline, source entry/exit, permitted files, dependencies, reproduction and observable exit check. Normally choose the next blocked transition plus its tightly coupled dependencies. No large subsystem rewrite and no parallel tickets in flight.
2. **Implementer reproduces, implements and tests.** Use one reusable isolated worktree or candidate workspace; share immutable originals read-only. No nested agents. Supply the focused diff, original references, test commands/results, actual-app run where possible and explicit remaining limits. Do not duplicate entire asset caches or evidence trees.
3. **Manager reviews independently.** Read the actual code and original behavior; check lifecycle, arithmetic, failure behavior, real content, scope and standards. Inspect pixels/audio where relevant. Reproduce the original failure or a meaningful negative so the test cannot pass without the fix. Reviewers must not merely rerun the author's expected values.
4. **Implementer fixes findings.** Reuse the same candidate and finding list. Manager rechecks the changed behavior and affected seams. If two review cycles fail for the same reason, stop adding patches: agree on one minimal failing original-backed case and its expected contract first.
5. **Manager integrates and verifies.** Check current beforeimages; apply only reviewed files. Run affected tests and the actual canonical input suffix. If composition requires behavioral edits, send those back for implementation/review; the manager must not silently author an unreviewed workaround. Commit the bounded result and update one ticket, coverage status and log. Publication follows the user's authorized scope.
6. **Advance or report the precise blocker.** Publish current real captures, state the furthest proven player transition, and select the next deliverable. Do not keep rerunning an already-green week while a known downstream branch remains unimplemented.

Manager review is the independent review role; a third review agent is not required. The manager may inspect the next source boundary while the sole worker implements, but must not create a competing implementation. Keep at most one unapplied candidate. Do not resume old chats or automation.

## Efficient testing without weakening the finish line

Use a short ladder: focused unit/negative checks → original-backed boundary comparison → actual-app suffix from a valid save → fresh normal-start milestone replay → final complete routes/package. Run broad suites when shared contracts change and at milestone integration, not after every prose edit or repeated unchanged review.

Record a shared route prefix once and reuse legitimate saves for branch exploration. Each heroine history may fork from an earned prefix, but the resulting action/save lineage must still trace to New Game without injected progression fields. Replay the prefix on the final runtime when needed to establish current evidence. This avoids thirteen redundant opening investigations without pretending a synthetic eligible ending is an earned route.

For differential checks, compare exact mode/stage/step, state effects, RNG state/draw count, ordered inputs and services. Stop at the first divergence; never resynchronize by dropping inconvenient events. Native instruction probes accelerate diagnosis but do not replace original scene captures or full routes. Document renderer/audio tolerances before accepting differences. Walkthroughs can guide route actions; verify platform/version and use the PS1 original as the behavior authority.

Pin the runtime, content, inputs and producer for each accepted run. A changed relevant dependency makes a run stale. Unchanged component proofs can be reused by an explicit dependency check; final end-to-end claims require the final runtime. Do not invalidate and rerun every parser test for an unrelated label, or carry old full-game claims across a new runtime.

Keep one current run and the first useful failing regression per issue plus final acceptance evidence. Preserve original inputs, active saves, translation returns and user image decisions. Archive only after verified preservation; never delete evidence that supports the current review. Run timing qualification without concurrent builds, archives or heavy test jobs, then separately measure expected normal load. Do not mask underruns by freezing gameplay or enlarging queues without checking latency.

## Scope and communication

Finish baseline gameplay before expanding widescreen, settings, customization or achievements. Preserve those plans and all translation/image work. With only one implementer, image generation is queued rather than a hidden second lane. Keep the image-review page available with its existing decisions; do not invent new drafts or approvals to make it look active.

The gameplay page receives only current actual-app captures, with video/audio when recorded. Keep diagnostic/original-oracle images out of that gallery. Report progress as the furthest verified route, the newly enabled behavior, the next known blocker and remaining mandatory families. Avoid another unsupported full-port percentage. After G2, use measured completed transition work to offer a range for the next milestone, explicitly excluding unknown event families; do not promise an overall completion date from test counts.

The manager may declare a milestone complete only from its exit evidence. Full completion requires G5 and the unchanged substantive obligations of [[port-testing]], not merely reaching graduation once. See [[port-manager-handoff]] for the exact resumption assignment and local server instructions.

## 2026-10-27 Parallel Lane Execution & Gate Status (`G1`–`G4` Verified on Canonical `main`)

Per user direction on 2026-10-27, the orchestrator parallelized the remaining work across 5 strictly file-disjoint subagent lanes while retaining single-applicator canonical integration and independent code/visual review on `main` (`155` production files in `port/godot/`, compact SHA-256 `4a75481b08af12e7f90c9cd4755af1cd66cb5caaa7717a42dc0364434c998d40`, `25` integrated slices):
- **`G1` (Connected Playable Boot -> Opening -> Week 1 & Week 2 + Save/Load + Real Audio)**: **VERIFIED** (`run_connected_opening_route.gd` `169/169` PASS, `run_live_desktop_full_game_tests.gd` `54/54` PASS; `first_arrival_boot.gd` pixel-exact inspector verified through `stage 7 / step 0 / substep 3` in `yoshio-greeting-v1`, with `rei-banner-and-entrance-v1` in flight).
- **`G2` (First-Quarter Route -> Full June + Warm Audio + Holiday Week 2 + Background Save)**: **VERIFIED** (`run_june_full_quarter_route.gd` `216/216` PASS through July 1, `run_real_week2_services.gd` `15/15` PASS, `run_repeated_sunday_services.gd` `3/3` PASS, `run_background_boundary_save.gd` `138/138` PASS).
- **`G3` (Reachable Gameplay Families, Interactive Overlays, Source-Backed `TACO`/`TT`/`RPG_BAT` & Cursor Parity)**: **VERIFIED** (`run_reachable_families_g3_gate.gd` `321/321` PASS, `run_subgame_overlays_tests.gd` `211/211` PASS, `run_subgames_and_event_cg_playtest.gd` `267/267` PASS, `run_telephone_overlay_tests.gd` `66/66` PASS, `run_cursor_actor_and_style_gate.gd` `482/482` PASS, `run_emulator_vs_godot_audit.py` `20/20` pairs / `0/6000` hotspot tiles).
- **`G4` (3-Year Campaign, All 13 Heroine Endings + Bad Ending, Clear-Save `0x1FFF` Persistence)**: **VERIFIED** (`run_multi_route_playtest_suite.gd` `279/279` PASS across all 14 routes, `2,128` weeks, `14,868` days; `run_full_three_year_route_tests.gd` `241/241` PASS).
- **Master Test & Tooling Suites**: `run_tests.gd` (`5/5` suites PASS), `run_render_smoke.gd` (`PASS`), and all 23 `tools.port.test_*` + `tools/port/progress/tests/run_tests.py` (`258/258` Python tests PASS).
