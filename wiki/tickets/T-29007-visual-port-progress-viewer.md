---
id: T-29007
title: "Show port progress through local screenshots and clips"
status: In Progress
assignee: Codex
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29001-godot-engine-foundation]]"]
---

## Goal

Provide an automatically updating local browser page that shows actual port screenshots and video clips, with audio where applicable. User explicitly selected a local page on this computer. Do not add written status updates, progress summaries or ticket text to the page.

## Acceptance criteria

- [ ] A working local browser URL displays actual port captures; no invented or generated game art.
- [ ] New captures appear automatically without restarting the page; refresh does not interrupt an actively playing clip.
- [ ] Video has playback/volume controls and retained source audio where available; never imply silent screenshots have audio.
- [ ] Media-only visual layout, responsive and keyboard-accessible; accessibility labels and media controls remain available.
- [ ] Viewer is loopback-only, Docker-run, with bounded file access and no arbitrary repository/disc browsing.
- [ ] Source/tooling may be committed; captures, clips and runtime manifests remain ignored under .asset-cache/port-progress/.
- [ ] Independently review source and verify real screenshot, refresh and clip playback paths before Done. No gameplay or parity claim from a foundation capture.

## Notes

Owned paths: tools/port/progress/. Root serializes live applications, wiki/Git and server launch. A bounded Luna worker prepares an isolated candidate. The existing port and asset lanes continue in parallel.

2026-10-25: user reported arrow directions reversed against the displayed thumbnail order. Swapped both visible arrow-button and keyboard ArrowLeft/ArrowRight deltas so they follow left-to-right thumbnail positions. Capture watcher now automatically imports stable port screenshots/clips from the three approved folders; actual activity renders appeared without manual publishing.

2026-10-25: actual browser video had readyState 4/no decoder error, but its CSS box was 1260px tall inside a 684px player frame, hiding controls below the viewport. Constrained both image/video elements to an absolutely positioned contain-fit media frame. Browser playback/layout recheck uses the real 33-frame Godot activity clip.
