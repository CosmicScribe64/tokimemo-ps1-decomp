---
type: handoff
updated: 2026-10-26
links: ["[[port-finish-plan]]", "[[port-checkpoint]]", "[[tickets/T-29034-plan-single-worker-port-completion]]"]
---

# Handoff to the next port manager

You are continuing `/Users/lila/Documents/Projects/tokimemo-decomp`. Work with exactly **one implementation agent**. You manage scope, inspect original behavior, independently review the worker's code, run canonical acceptance checks, integrate and report results. The worker implements and fixes findings. Neither of you creates additional agents or resumes the old chats. The recurring automation stays paused. Starting this handoff authorizes resuming port work; it does not authorize restarting the former orchestration fleet.

## Read first

1. `wiki/index.md`, then the opening current sections of `wiki/status.md`.
2. `wiki/port-finish-plan.md` — the execution order and two-person workflow.
3. `wiki/port-readiness-review.md` and `wiki/port-testing.md` — known failures and non-negotiable acceptance.
4. `wiki/port-checkpoint.md` — use the current pause section; its explicitly superseded historical claims are not evidence.
5. `AGENTS.md`, `AGENTS.local.md`, `CODING_STANDARDS.md`, and the tickets needed for the first deliverable. Do not load every old packet or giant orchestration-state file into context.

Review the finish plan against current code and source before marking planning ticket T-29034 Done. You are the independent reviewer for this plan and for the implementer's work. Amend concrete shortcomings; do not spend a session merely rewriting the plan. Start the baseline inspection and first bounded assignment in the same work session.

## What is actually integrated

The inspected source checkpoint is main `28385de82` (later planning-only commits may follow). Earlier reviewed port checkpoint `43240118ddd4a3705a3ab761c66e393f4ce645da` includes opening music and June controller V3. The recorded production map is `0d651798c842f469b1c691d675795ebbc6a0006818afa06734455275b8bcda5a`,136 files; recompute against current files before using it.

Phases1/2 remain complete and matching:27 binaries,6,952 functions,2,276,920 bytes. Do not reopen them. The port is incomplete. Actual fresh keyboard input reached the first prologue line with original music; captured-save gameplay reached May28. These are different lineages. There is no accepted fresh New Game through the normal command loop, actual June continuation, complete three-year route or final native desktop package.

Do not reapply opening BGM or June controller packets. Do not use old100% reports or synthetic long-route tests as gameplay acceptance.

## First assignment to the sole implementer

**Deliverable:** compose and finish the retained first-visible actor/HUD/text-cache change against current main, demonstrate the actual first visible scene, and identify the exact next board transition. Finish the tightly coupled authentic board integration if its scope fits the same reviewed change; otherwise deliver it immediately next. Do not start unrelated calendar, image or framework work.

Read these retained paths, relative to the repository:

- `.claude/orchestrator/state/T-29025/first-visible-v1/PAUSED-CHECKPOINT.md`
- `.claude/orchestrator/state/T-29025/first-visible-v1/project/` — candidate, not canonical acceptance.
- `.claude/orchestrator/state/T-29025/board-resource-v1/PACKET.json`
- `.claude/orchestrator/state/T-29025/board-resource-v1-spec-review/report.md`
- `.claude/orchestrator/state/T-29025/root-bgm-integration/application.json` and `gui-v1/` — prior applied opening evidence.

Actor ownership, native HUD argument2 and text-cache subcomponents have bounded reviews. The full first-visible integration was interrupted and remains unaccepted. Do not copy the whole candidate over main: it predates the applied quarter composition. Preserve native owner checks and current boot/audio behavior. Rebind the stale first-visible source-pin checker rather than weakening it.

The board is original `CDROM/BS/P_01_02.BS`, LBA **0x4954 =18772 decimal**,9 sectors to **0x80162000**, decoded640×160. Do not confuse the hexadecimal LBA or reuse actor destination0x80180000. Preserve unknown lower rows in the native move operation; do not fill them with invented pixels.

Worker output: exact baseline and scoped diff, original source references, focused positive/negative tests, actual input/capture run, and named next unsupported transition. Manager independently checks source ownership, layer/coordinate behavior, resource lifetime and actual images. Then applies reviewed files and reruns the canonical scene under controlled load. Earlier GUIv3's22 checks and zero skips are bounded historical evidence; GUIv2's53 skips remain a failure record, not something to discard.

**Next deliverable:** proceed through the real prologue into ordinary command/result/next command, with production installed content. Do not announce New Game playable after one good screenshot. Follow G1–G5 in the plan.

## Native audio / June work after that

- `.claude/orchestrator/state/T-29023/quarter-canonical-native-v1/STOP-CHECKPOINT.md`
- `.claude/orchestrator/state/T-29027/shared-runtime-v4-independent-review/PAUSED-CHECKPOINT.json`
- `.claude/orchestrator/state/T-29023/quarter-native-integration-v2/LIVE-ROUTE-STRATEGY.md`

The controller is applied; the warm native composition is not. Finite V4 service acceptance does not accept its application composition. Keep the existing backend/sink owner and opening ClassDB guard. Establish authentic startup/import/handoff and stable-save state before actual June. Old May14 debug saves do not contain the necessary native audio state. Do not invent it. Avoid timing tests during builds, archives or heavy suites.

## Baseline commands and test discovery

Run from the repository; all tools/dependencies/builds remain in Docker. Reuse existing images, and do not install host tooling.

```sh
git status --short
git log -3 --oneline
git worktree list
docker ps --format '{{.Names}}\t{{.Status}}\t{{.Ports}}'
df -h /System/Volumes/Data
```

A lightweight framework check, not a gameplay certificate:

```sh
docker run --rm -e PYTHONDONTWRITEBYTECODE=1 \
  -v "$PWD:/repo:ro" -w /repo python:3.12-slim \
  python -m unittest tools.port.test_readiness tools.port.test_differential tools.port.test_preflight
```

`sh tools/port/run.sh test` runs the existing core suite; it is not a universal installed-game test. For actual New Game, inspect `port/godot/tests/run_installed_entry_keyboard.gd`, `run_installed_first_scene.gd`, the retained GUI producer/configuration and its original pack pins. Prepare a reproducible invocation with a fresh output directory; do not guess missing environment variables or label a headless scene script as keyboard gameplay. If the old invocation is unavailable, restore one small reusable runner and verify it against the real app before changing game behavior. Use the existing observer/readiness contracts rather than inventing another report format.

Run focused tests first, affected shared tests at integration, and actual player suffixes afterward. Keep failing logs. Check exit code **and** required checkpoints/script errors. Use original executable comparisons for behavior and actual viewport/audio for presentation. No data/state injection in normal-start route evidence.

## Keep both local pages alive

- Gameplay: `http://127.0.0.1:8765/`
- Image review: `http://127.0.0.1:8766/`
- Containers: `tokimemo-port-progress`, `tokimemo-port-progress-watch`, `tokimemo-translation-review-8766`.

Inspect before restarting anything. A stopped existing container can be restarted; an absent progress server/watch uses `sh tools/port/progress/run.sh serve` and `sh tools/port/progress/run.sh watch`. Serve is foreground: launch it in a persistent tool session. For an absent image server, use the exact Docker command in `tools/translation_gallery/README.md` with the existing inventory/state. Do not rebuild the inventory or replace user decisions just to restart it. Inspect current mounts/configuration if that README differs from the existing container.

The progress watcher reads flat `.asset-cache/port-smoke`, `port-preview-output`, and `port-captures`. Raw video frames belong in a nested directory, not the watched top level. Publication requires current reviewed producer/evidence bindings through `tools/port/progress/`; copying arbitrary screenshots is insufficient. Preserve `.asset-cache/port-progress/` and `.asset-cache/translation-images/review-gallery-state/`. Do not use `render-smoke` as proof of a newly working gameplay scene. Keep original-oracle/synthetic images out of the gameplay gallery.

The user wants visuals, not a prose feed. Update with actual milestones and recorded audio/video where available. Image review stays accessible; new artwork can wait behind the single worker's gameplay assignment. Approved wording includes Culture/Arts and Mon; month/day/weekday roles remain distinct. Changed image bytes require new review, including formerly approved images, and native-menu alignment must be checked before presentation to the user.

## Protect current work and keep storage bounded

Retain main, `/Users/lila/Documents/Projects/tokimemo-wt/translation`, immutable `disc/`/`raw/`, translation returns and image decisions, current native/source witnesses, and paused active candidates. User says the text translation is finished; do not restart translation. Preserve dirty `config/renames/proposals/m-flow.tsv` and historical untracked files. Never stage/reset everything.

Cleanup left about37GiB free. Old worktrees and many generated copies were deliberately removed. Recovery inventories/archives live under `.claude/orchestrator/state/T-29033/`; do not assume every old path still exists or recreate every export. One old research tree's exact dirty wiki bookkeeping was not preserved; its research files remain in committed drafts. That exception does not affect current port work.

Use at most one new worker workspace, sharing originals read-only and storing only changed code/current outputs. No cache copies per review. Verify archive contents before any deletion. Keep code/source/tests in Git; copyrighted original data and actual exports stay ignored. No unnecessary matching rebuild for port-only changes; if protected game files change, stop and investigate.

## Completion and handback

Use the manager/implementer loop in the finish plan. Maintain the existing tickets/board, append-only log, upcoming branch inventory and one concise current checkpoint. Commit only reviewed bounded changes; push within current user authorization. Keep automation and old agents paused.

Tell the user the furthest actual verified transition, next known blocker, and what changed. Do not promise completion from a percentage, function count, registered capability or passing synthetic suite. Declare the baseline complete only after the reviewed reachable coverage map, actual normal-start routes, correct endings/persistence, original comparisons and final packaged-platform gates pass. If usage runs low, leave one exact candidate/finding list and a reproducible next action; do not produce another misleading completion summary.
