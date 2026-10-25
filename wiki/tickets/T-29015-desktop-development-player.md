---
id: T-29015
title: Build a reproducible desktop development player
status: In Progress
assignee: root
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29008-assemble-playable-development-app]]"]
---

## Goal

Provide a repeatable code-only desktop development build using the pinned Docker engine/templates, with original content supplied externally. This makes integrated gameplay accessible beyond automated capture runs.

## Acceptance criteria

- [x] Export only reviewed project source/configuration using the pinned engine; record source and artifact hashes.
- [x] Keep original game data and development captures outside the package and Git.
- [ ] Provide explicit external input configuration and a usable local launch path; no host toolchain install or security-setting bypass.
- [ ] Review build/launch tooling and verify the actual exported application before claiming platform support.

## Notes

First target is the user's macOS desktop. Unsigned development export is separate from release signing/notarization and Windows/Linux acceptance. Preserve old exports. Root serializes application/build writes; current gameplay limitations remain explicit.

## Build

From the repository root:

```sh
sh tools/port/build_desktop.sh new-game .asset-cache/port/desktop-entry-new-build
```

Use `week` instead of `new-game` to select the captured-boundary development app. Each output directory must be new. The build records source hashes and stages code only, changes the main scene and universal texture-import setting only in that copy, then uses the pinned Docker engine/templates. External packs and runtime input configuration remain required; this command does not launch or install the app.

## Verification

2026-10-25: independent Standards and bounded correctness review pass in `.claude/orchestrator/state/T-29015/final-review.{md,json}`. Six invented-source tests pass. Actual v3 export at `.asset-cache/port/desktop-entry-90283b29d-v3/player.zip` has SHA256 `2a5670c4404bf171af842b9a10b1dec5f351748f7e2a0b5c8b3fa426fc68ee6d`; review verified 41 staged source hashes, 81 packaged resource mappings and both x86_64/arm64 Mach-O slices. Original game payloads are absent. Earlier failed v1 and successful v2 outputs are preserved. Host launch, external-input launcher and platform runtime acceptance remain unverified; the ticket stays In Progress.

## Current macOS export smoke (2026-10-25)

Refreshed code-only export contains 46 source files at `.asset-cache/port/desktop-current-60b0a94e6/player.zip`, SHA256 `a47d4c255d03e4e64689769f3144ecca9ff9dba9985ff7222dc3ae709740f603`. Native macOS executable launches headlessly and initializes Apple M4 OpenGL/Metal Compatibility rendering without a host toolchain install or security bypass. A visible development window reported rejected New Game inputs; same-bundle process/window attribution is not yet established, so full usable native launch remains unproved. Passed this observation to the New Game owner for controlled reproduction and clearer diagnostics. This export predates the rest-week integration.

2026-10-25: preparing a development-only direct-executable launcher under `.claude/orchestrator/state/T-29015/launcher-v1/`. It supplies external New Game inputs to a fresh process rather than relying on Launch Services to forward environment to an existing window. Strict content checks remain in the runtime. Native end-to-end verification and independent review are still required; the shipping first-launch installer remains T-29016.
