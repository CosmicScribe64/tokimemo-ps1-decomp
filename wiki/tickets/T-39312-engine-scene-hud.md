---
id: T-39312
title: Port the engine scene and HUD files (WP-B)
status: In Progress
assignee: worker agent
created: 2026-10-30
updated: 2026-10-30
links: ["[[tickets/T-39301-engine-api-core]]", "[[engine-porting-contract]]"]
---

## Goal

Port `scene_library.c`, `scene_display.c` and `hud_panels.c` onto the engine, following [[engine-porting-contract]].

## Acceptance criteria

- [ ] Every function of these files is ported, with function-differential tests from emulator captures, or listed as untested with a reason.
- [ ] The core suite, `run_engine_diff.sh --all` and the opening frame-timing gate pass.

## Notes

## Comments
