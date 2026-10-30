---
id: T-39050
title: Match the name-entry pointer origin to the native start
status: Backlog
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[playtest]]", "[[tickets/T-39018-playtest-differential-harness]]", "[[english-names]]"]
---

## Goal

The port name pointer starts at the desktop centre, while native NAME_ENT starts from the FILESEL cursor (or the name-ready-v1 state). Make the port origin match so the T-39018 WP-06 seed route draws the typed names instead of presets.

## Acceptance criteria

- [ ] The port pointer origin at NAME_ENT stage 1 equals the native origin, proved against an emulator capture (name-ready-v1 or a FILESEL-cursor run).
- [ ] The seed route (`tools/port/run_input_script_route.sh`) passes expect_state at family-name.
- [ ] Frame timing route pads 2 and 3 still report 0 mismatches.

## Notes

Proposed by playtest-diff v1/v2 (`.asset-cache/phase4-patches/playtest-diff.v2.notes.md`). Blocks like-for-like seed-route name-entry and Rei captures. Core-port sub-ticket of [[tickets/T-39018-playtest-differential-harness]].

## Comments
