---
id: T-29001
title: "Create the pinned Godot project and test toolchain"
status: Ready
assignee:
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

- [ ] Record official engine/template URLs, published-checksum verification, local SHA256s, CPU architecture, version output and base/result image digests; no invented pins.
- [ ] Build and test only in Docker; source the engine from official releases and keep the matching toolchain unchanged.
- [ ] A clean synthetic project parses and executes its test runner without game inputs; separate rendered execution from headless behavior tests.
- [ ] Port standards define typed GDScript, source witnesses, native arithmetic/state, no original content in Git/player package and separate standards for the decompilation and port.
- [ ] Exclude engine caches, original content and generated exports; verify protected source/header/config/build files are unchanged.
- [ ] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested bounded owner: gpt-6-luna. This is the first implementation ticket. It establishes execution/test plumbing, not a playable game or a claimed native-behavior model. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.
