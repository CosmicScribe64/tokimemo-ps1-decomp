---
id: T-30001
title: "Add widescreen presentation"
status: Backlog
assignee:
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]"]
---

## Goal

Planned modernization; widescreen first. Track after the faithful Phase 3 port; no implementation started.

## Acceptance criteria

- [ ] Provide widescreen layouts with correctly positioned HUD, dialogue and menus; retain a selectable original-aspect presentation.
- [ ] Define per-scene framing for backgrounds, movies and 3D without blindly stretching art or revealing invalid scene edges.
- [ ] Check mouse/controller hit regions, text scaling and representative scenes at supported window sizes.

## Notes

Requested by the user on 2026-10-25. These additions do not replace full original-game port acceptance.

## Art extension preference (2026-10-25)

Reviewed research in T-30005 supports a separate responsive UI around aspect-preserved original art. Normal backgrounds use distinct 320x88, 320x160 and 320x240 apertures; larger images elsewhere do not establish extra room margins. Keep sprite proportions and original scene anchors. Movies and exceptional 256/512-wide display modes need their own presentation contracts. Higher output resolution adds no detail to existing pixels; offer integer nearest scaling and optional smooth fit, with visual testing before choosing defaults. Wider 3D cameras require scene-edge and gameplay checks.

User considers image generation/editing for wider backgrounds only as a last resort. Prefer faithful aspect-preserving scaling, layout reflow, verified existing source margins and original framing/pillarboxing. If none is satisfactory, propose optional scene-specific extension for review; do not make generated scenery the default or claim it is original game art. Preserve source assets and an original-art mode. No generation or replacement-art implementation is requested now.
