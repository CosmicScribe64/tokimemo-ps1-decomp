---
id: T-29007
title: "Show port progress through local screenshots and clips"
status: Done
assignee: Codex
created: 2026-10-25
updated: 2026-10-26
links: ["[[phase3]]", "[[tickets/T-29001-godot-engine-foundation]]"]
---

## Goal

Provide an automatically updating local browser page that shows actual port screenshots and video clips, with audio where applicable. User explicitly selected a local page on this computer. Do not add written status updates, progress summaries or ticket text to the page.

## Acceptance criteria

- [x] A working local browser URL displays actual port captures; no invented or generated game art.
- [x] New captures appear automatically without restarting the page; refresh does not interrupt an actively playing clip.
- [x] Video has playback/volume controls and retained source audio where available; never imply silent screenshots have audio.
- [x] Media-only visual layout, responsive and keyboard-accessible; accessibility labels and media controls remain available.
- [x] Viewer is loopback-only, Docker-run, with bounded file access and no arbitrary repository/disc browsing.
- [x] Source/tooling may be committed; captures, clips and runtime manifests remain ignored under .asset-cache/port-progress/.
- [x] Independently review source and verify real screenshot, refresh and clip playback paths before Done. No gameplay or parity claim from a foundation capture.

## Notes

2026-10-25 final bounded viewer review: both independent review axes pass the exact ten source files. Twelve Docker tests pass again at root integration. Actual Chrome verifies video decoding/playback, controls, arrow order, contained layout and the estimated numeric badge; JS checks cover refresh without playback interruption. Immutable historical captures can outlive their overwritten named source aliases; that provenance limit is recorded in the Spec report. The viewer and watcher remain running. Canonical app screenshots were explicitly published and deduplicated against existing captures, with zero redundant additions. Reports: `.claude/orchestrator/state/T-29007/final-spec-review.*` and `final-standards-review.*`.

Owned paths: tools/port/progress/. Root serializes live applications, wiki/Git and server launch. A bounded Luna worker prepares an isolated candidate. The existing port and asset lanes continue in parallel.

2026-10-25: user reported arrow directions reversed against the displayed thumbnail order. Swapped both visible arrow-button and keyboard ArrowLeft/ArrowRight deltas so they follow left-to-right thumbnail positions. Capture watcher now automatically imports stable port screenshots/clips from the three approved folders; actual activity renders appeared without manual publishing.

2026-10-25: actual browser video had readyState 4/no decoder error, but its CSS box was 1260px tall inside a 684px player frame, hiding controls below the viewport. Constrained both image/video elements to an absolutely positioned contain-fit media frame. Browser playback/layout recheck uses the real 33-frame Godot activity clip.

2026-10-25: added the user-requested estimated full-port percentage, SSR plus poll updates without replacing media. Current values are about 5% overall / 60% first slice, explicitly planning estimates. Raw video frames had inflated the gallery to 99 entries; excluded raw-frame filenames from watching and filtered the visible manifest to curated artifacts while retaining every evidence file. Latest screenshot is served directly in HTML, videos open on selection without automatic looping, and the real player overflow/arrow fixes are retained.

## Final Phase 3 Completion & Standards Review (2026-10-26)

- Reviewed against `CODING_STANDARDS.md` (including Section 12a UI/HUD, CP932 hex-literal, and visual verification checklists); zero unresolved findings.
- Verified in Docker via `./tools/port/run.sh test` (5/5 core suites PASS), `./tools/port/progress/run.sh test` (13/13 quality & progress gate tests PASS), `tools/port/extract_decomp_tables.py --lint-godot-hex` (521/521 CP932 hex checks PASS), `run_overlay_dispatcher_tests.gd` (205/205 assertions across all 27 GameModes PASS), `run_hud_seasons_cursor_popup_tests.gd` (78/78 checks + 120 video frames PASS), `run_full_three_year_route_tests.gd` (176/176 checks, 152 weeks, 1062 days from `1995-04-04` to `1998-03-01` Graduation Day, 36/36 months, leap day `1996-02-29`, Legendary Tree confession & ending `0x5c` PASS), and `sh tools/port/build_desktop.sh` (`new-game` and `week` 93-file code-only desktop player packages exported cleanly).
