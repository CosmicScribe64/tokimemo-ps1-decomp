---
id: T-29023
title: Verify original-backed routes through the connected app
status: In Progress
assignee: root
created: 2026-10-26
updated: 2026-10-26
links: ["[[port-readiness-review]]", "[[tickets/T-29019-independent-port-readiness-review]]"]
---

## Goal

Verify original-backed routes through the connected app against original PS1 behavior. Independent review found the prior completion claim unsupported.

## Acceptance criteria

- [ ] Implement real production behavior with original-backed inputs and explicit unsupported cases.
- [ ] Verify positive and negative cases through the actual app/service path; no synthetic substitutes presented as parity.
- [ ] Preserve simulation/RNG/save behavior and compare relevant original-game checkpoints.
- [ ] Independent code/standards review resolves findings before Done.

## Notes

Root serializes canonical edits. Candidate/proof directory: `.claude/orchestrator/state/T-29023/`. See the readiness review for exact findings and scope.

## 2026-10-26 actual two-week run

The applied T-29026 runtime reaches the second Sunday using actual input. Run `applied-two-weeks-v4` completes 50 bounded actions with 18 viewport captures. Initial and first-Sunday save/load checks pass, but second-Sunday and returned-command saves fail; their paired loads are deliberately not dispatched. The four failed checks remain acceptance blockers. This run starts at a captured boundary and does not establish normal New Game, native audio or whole-game coverage.

The readiness validator independently rejects the exact route hash `13e16982d211d216728c4a6d3909f7bdcca9e13069ef3df5889ebf6df9569120`; receipt: `.claude/orchestrator/state/T-29029/root-integration/two-weeks-v4-rejection.json`. Both independent reviewers rejected the first dynamic-Sunday save candidate: a sentinel comparison misclassifies the initial command boundary, and an unbounded calendar year can stall malformed-save validation. A separate corrected candidate and another actual-input run are required.

The corrected four-file v2 composition is now applied after fresh Spec and Standards acceptance. It derives Sunday save positions from validated date and current queue opcodes, checks the nonnegative sentinel and bounds the year before calendar traversal. Root verification passes 16 classifier checks, five core suites and original-media initial Boot/capture. The existing general save test is preserved; its proposed holiday-only assertion was excluded. Runtime hash `020df9fedabef2c5a46184b2e4b33dc779fbf8d64c4de1bf538704e9e897b1fb` is dispatched for a new independent actual-input run. Receipt: `.claude/orchestrator/state/T-29023/root-save-v2/application.json`. No actual later-Sunday success is claimed before that run.

Successor `applied-two-weeks-v5` now passes the bounded actual-input route: 50 actions, 18 viewport captures and five successful F5/F9 pairs, including Apr 16 queue indices 15 and 16. The saved state, RNG, clocks, input and pending-service equality checks pass. Root independently replayed raw failure validation against route SHA `3a8b875d5873a9a431dbc492eb8eb0135c7b6c22a83547e4c6583b2d187f4044`, with no failures. The producer retained the v4 observer and input packs; only the two reviewed production files changed. This is a captured-boundary two-week result, not normal New Game, phone/audio, native GPU parity or whole-port acceptance. The next diagnostic extends ordinary play to the first unsupported transition without skipping it.

A separate reviewed UI change removes permanent F5/F9 help from save notices and expires successful notices after 1.8 seconds. Errors and diagnostic metadata remain visible/preserved. Independent lifecycle checks cover stale timers, pause, input handling and state invariance; root reran nine focused checks and reviewed the source against the standards. Applied runtime `d815afe0a7462719882e1ce6f645dc8b1a4a9d811385287af9d4e977705b94fe` needs fresh actual UI inspection. The v5 route pass remains pinned to its earlier runtime and is not silently transferred.

The separate v6 ordinary-play diagnostic reaches April 29, 1995, a Saturday holiday, after passing the April 23 Sunday and returned-command menus. It stops deliberately before the holiday controller advances: mode `0x46`, stage/step `0/0`, queue index 12, event `0x10`. This is a newly reached, unverified boundary, not a demonstrated product failure. The run contains 21 captures and no app failure; its deliberate stop remains a failed coverage check. Source dispatch review and a bounded continuation are assigned separately.

Actual UI run `save-notice-ui-v7` passes 11 actions with eight viewport captures and no failed raw checks on d815afe0. Real F5/F9 notices expire after 2.2 seconds of observation; an actual unsupported-save error remains visible. The harness explicitly pauses SceneTree to test the UI timer, with state/RNG/clocks/input/services and metadata invariance checks. This is UI harness control, not verified player-pause gameplay. Root independently validates route SHA `a6fdbc2f8b0aaab2e2953a0b6887dcac04c28be014a6d3563df6ce008e6fa4cb` and inspects the clean post-load viewport. Wider route acceptance remains separate.

Run `apr29-holiday-v8` confirms the actual icon mismatch before input: Saturday, day kind 2, club NONE (11), but icon 4 enabled instead of the source-required disabled state. Its frozen route SHA is `4a69d87d0c5a8f9a06e06fb2d58de2f0c88db0b328d09dcae06f6cd4915be463`; the gate rejects it. The reviewed two-file Rules/HUD correction and one regression test are applied at runtime `b08d186b4f31744e877e2f2a34757a05189be69cfb91e81417c26decc321a111`. Root passes 1,700 assertions across 85 synthetic base cases and five core suites. A fresh actual-input replay is required to confirm the fix and continue through the holiday result to April 30. Saturday save policy remains unchanged.

Successor `apr29-holiday-v9` passes all 63 actions with 25 actual captures and no failed checks. The unchanged observer now confirms icon 4 disabled, the actual command 0 actor/result lifecycle, parameter changes and arrival at April 30's holiday menu. Root validates route SHA `acd76ea3a11d4ed0ca78009d92387f9cc5f849b4c0b69cfbe89292c21cab1145` with no raw failures and inspects the April 30 viewport. This remains captured-start, bounded gameplay evidence; normal New Game, audio, broader routes and full-game acceptance remain open. The next diagnostic extends toward the May boundary.

## Continuation after May 1 (2026-10-26)

User explicitly requested continued actual gameplay and a working fresh New Game flow. The actual-input owner is running successor V17 on unchanged runtime `e28bd42c6c4b34a0e2933be3e91ae63a14aca5adb20c3440fea09ffd6b136c80`, replaying before rollover and continuing the naturally reachable command-0 route toward May 7 or the first unsupported/error boundary. It must preserve failed traces, avoid forced dates/modes or fabricated services, and exercise a later save/load boundary if reached. This remains captured-boundary coverage, separate from fresh New Game. V16 Standards review passes; source/spec review remains pending.

## Actual May 7 save failure

V17 naturally reaches May 7 Sunday menu (mode 0x46, stage 1, step/substep 0, queue 15/type 0x10, logic 2067), then real F5 rejects with `Save unavailable: holiday command boundary lacks validated room background draw` (check-0232). The app remains runnable; no F9 is sent after its paired save fails. The route remains FAIL, with 66/70 actions and 59 captures reported by the producer. Earlier May arrivals and holiday observations do not waive this required failure. Root assigned a separate isolated source-backed save/presentation repair; it must preserve validation of wrong/missing backgrounds and rerun actual gameplay after integration.

V17 frozen failure handoff SHA `a47bdc8513af87e70f28d5e15945ff0669e3fbbe19fc3eb1d160c6d6f946eac5`, raw route `45f648a812e2c0958231972899348fdadbbcb5b40b2f24c2f33cabcfa8c2c01d`: 235 checks attempted, one failure, all trace/journal/draw consistency checks clear. The actual valid room is selected in physical slot 0 with cache `[16318,16131,0,0]`; the save guard assumed the opposite permutation and slot 1. Source background selection permits either physical slot. Repair remains isolated, with exact asset identities and negative cache tests required.

## May 7 fix integrated

Applied the one-file save adapter fix plus its regression test after both reviews passed and all packet/source/beforeimage hashes matched. Canonical original-backed replay of the diagnostic matrix passes 138 checks across 18 physical layouts. Production runtime is now `abadc538c74c46d280076cda7b9b3e98b6b16f7646f84cbbd85a43fe9e2b2e40` (98 files); receipt `T-29023/may7-save-v1/root-live/application.json`, log `root-live/run.log`. Fresh actual-input V18 through May7 F5/F9 is dispatched. V17 remains failed evidence; no repaired gameplay-route acceptance yet.

## May 7 actual checkpoint accepted

Fresh V18 passes independent Spec review and root raw-gate/viewport checks: 70/70 actual-input actions, 242 checks, six real save/load pairs and 333 evidence-finalizer checks. Both May7 save/load copies and on-disk quick save are byte-identical; all seven trace epochs validate and physical slot0/cache order survives unchanged. V17 remains frozen FAIL. Root accepts only the captured-start continuation through May7 F5/F9 on runtime `abadc538c74c46d280076cda7b9b3e98b6b16f7646f84cbbd85a43fe9e2b2e40`. Review: `T-29023/month-v18-spec-review/report.md`. After-load screenshot published. Further May gameplay, fresh New Game and native audiovisual parity remain open.

## V19 independently verified through May 14 (2026-10-26)

Independent Spec review accepts the frozen captured-start route through May 14: 73 actual actions, 298 checks, 54 input events, 86 captures; finalizer replay passes 410 checks. Exact V18 prefix, original calendar prefixes and six save/load pairs are verified. May 14 save/load was not attempted. Review `.claude/orchestrator/state/T-29023/month-v19-spec-review/report.md` SHA `6ecfbb49686688824f267c637c5530c4a6a1fbe657186b3300ddf674dfca6560`; handoff `2cb41f31ca7458b8e29a623c6c70638b9b5234cba2ba8da344f2d0f5d7fdce23`. Runtime remains frozen `abadc538`; the newer canonical phone presenter is separately tested, not accepted by inheritance. No New Game, full-May or full-game claim. Root inspected the genuine May 14 menu capture for the local progress page.
