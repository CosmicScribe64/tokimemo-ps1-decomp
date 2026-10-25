---
id: T-29001
title: "Create the pinned Godot project and test toolchain"
status: Done
assignee: Codex
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
---

## Goal

Establish the smallest reproducible Godot 4.7.2 standard-build project, scoped port standards and headless/rendered test entry points without original content.

## Owned paths

- `port/godot/project.godot`
- `port/godot/PORTING.md`
- `tools/port/Dockerfile`
- `tools/port/godot.lock.json`
- `tools/port/run.sh`
- `port/godot/tests/run_tests.gd`

## Acceptance criteria

- [x] Record official engine/template URLs, published-checksum verification, local SHA256s, CPU architecture, version output and base/result image digests; no invented pins.
- [x] Build and test only in Docker; source the engine from official releases and keep the matching toolchain unchanged.
- [x] A clean synthetic project parses and executes its test runner without game inputs; separate rendered execution from headless behavior tests.
- [x] Port standards define typed GDScript, source witnesses, native arithmetic/state, no original content in Git/player package and separate standards for the decompilation and port.
- [x] Exclude engine caches, original content and generated exports; verify protected source/header/config/build files are unchanged.
- [x] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested bounded owner: gpt-6-luna. This is the first implementation ticket. It establishes execution/test plumbing, not a playable game or a claimed native-behavior model. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: user authorized implementation alongside ongoing asset work. Root serializes live integration; bounded workers prepare isolated packets.

2026-10-25: prepared pinned Godot 4.7.2 standard/Linux arm64 toolchain and actual headless/Xvfb-Mesa proof. Independent reviews reproduced stale-screenshot and script-compile false-pass defects. Root prepared separate V3 resolving both; actual positive render and malformed-script/save-failure/stale-output negatives pass. Original reviewers are verifying resolution before live integration or Done.

2026-10-25: Done for the engine foundation only. Both independent review axes verify V4 resolves stale-capture and script/runtime false-pass findings. Actual installed Docker runner passes foundation+native two suites; separate actual Mesa/OpenGL capture is decoded and visible in the local viewer. Protected 899 existing files unchanged; no gameplay/parity claim. Evidence: ignored T-29001/{foundation-spec-resolution,foundation-standards-resolution,live-application}.json and live-headless.log.
