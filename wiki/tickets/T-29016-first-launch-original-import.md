---
id: T-29016
title: "Add first-launch original-game import"
status: In Progress
assignee: port-integration-agent
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[versions]]", "[[tickets/T-29003-port-content-adapter]]", "[[tickets/T-29015-desktop-development-player]]"]
---

## Goal

Add first-launch original-game import. User requirement recorded 2026-10-25; implementation and platform verification remain open.

## Acceptance criteria

- [ ] On first launch without a valid installed pack, offer a file/folder picker for the user's original PS1 game; identify the release and validate required data before enabling gameplay.
- [ ] Reuse existing readers through a packaged importer; accept supported container formats explicitly and report absent raw audio/sector data. Do not advertise every format or require users to install Docker, Python, dependencies or the Godot editor.
- [ ] Show import progress and cancellation; read originals without modification, stage output separately, validate all required components and atomically publish a versioned installed pack.
- [ ] Keep assets in a persistent per-user application-data location outside the executable/project, remember the validated pack and launch directly on subsequent runs without asking for the original again.
- [ ] Keep saves and settings independent from content; offer repair, re-import and location management, preserving the last working pack after cancellation, disk-full or validation errors.
- [ ] Build/package dependencies in Docker; verify the actual exported desktop app with no pre-existing cache or developer environment, then test restart/offline use with the original unavailable.

## Evidence and scope

The existing input reader supports extracted directories, raw MODE2/2352 data tracks and ZIP-contained tracks. Some formats require normalization and some sources lack audio/sector dependencies; a recognized container is not proof of complete game coverage. `wiki/versions.md` catalogs five PS1 discs: Rev 1 and Shokai have identical game files, later releases differ in executable/overlay and some data bytes, with moved LBAs. No new release support is claimed by this ticket.

2026-10-25: independent review reproduced three v1 flow/store defects: reentry abandons an active backend, source/install overlap permits source mutation, and interrupted location preference replacement fails to recover its backup. Author v2 addresses them with 55 assertions. Root actual visual inspection then found offscreen controls at 640x480; isolated v3 changes only the button container to wrapping layout. All five controls now fit and the 55 flow checks pass. Original reviewer re-verification is pending. Candidates remain ignored and unapplied; full original import still requires a packaged backend.

## Setup infrastructure integrated (2026-10-25)

Root applied seven additive v3 files after original-reviewer verification resolved the three flow/storage findings, copy and layout. Installed canonical execution passes 55 workflow assertions and five existing suites. Actual canonical 640x480 rendering keeps all five controls within x28..538 and bottom259. The candidate's synthetic project configuration was deliberately excluded; existing ContentStore and schema remain byte-identical. The first attempted apply stopped at that configuration mismatch before writing files; the successful scoped apply and tests are in `canonical-proof/` under the ignored T-29016 state directory.

The setup scene is additive and is not the default boot scene. No packaged importer or full-game readiness is claimed. Actual import backend, boot routing, complete capability policy and native offline/restart validation remain required. Independent resolution: `spec-resolution-v3.md/json`.
