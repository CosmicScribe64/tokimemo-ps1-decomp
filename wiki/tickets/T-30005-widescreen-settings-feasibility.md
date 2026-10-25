---
id: T-30005
title: "Research widescreen art and modern settings feasibility"
status: In Progress
assignee: port-presentation-research
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-30001-modern-widescreen]]", "[[tickets/T-30002-modern-settings]]"]
---

## Goal

Research user concerns about stretched or degraded art at widescreen and modern resolutions, and recommend modern game settings conventions. Research only; no replacement art or modernization runtime changes.

## Acceptance criteria

- [ ] Inspect actual background, sprite, movie and 3D source dimensions/composition; distinguish output resolution, pixel aspect, scaling and newly visible scene area.
- [ ] Recommend per-scene 4:3, widescreen/reflow, crop or pillarbox policy with honest limits; no invented extra artwork or universal stretch.
- [ ] Compare nearest/integer scaling and optional filtering with original art examples where useful; label research mockups and keep them out of the live port gallery.
- [ ] Review current official Godot display/stretch/input/settings capabilities and cite primary documentation; specify reversible graphics defaults and platform limitations.
- [ ] Recommend accessible mouse/keyboard/controller settings navigation, clear categories, apply/revert countdown, remapping/conflict handling, per-category defaults, persistence, audio sliders and language/voice/translation options.
- [ ] Deliver concise source-backed recommendations and remaining design choices for independent review; implementation stays separate.

## Art extension preference (2026-10-25)

Research and both independent reviews are delivered under `.claude/orchestrator/state/T-30005/`. Report SHA256 `215f80028b12e0205c33ed5705f47900b961f617c26a687f76353b4946114bdd`. The source audit checked 49 TMD headers/local bounds and both exported movie streams; it does not establish unseen scenery, full rendering, CRT aspect or quality. Primary Godot and Xbox accessibility references are in the report. Review findings on accessible confirmation timing and platform-qualified V-sync were resolved. Root carried the concise recommendations into T-30001/2; final design and runtime verification remain separate.

User considers image generation/editing for wider backgrounds only as a last resort. Prefer faithful aspect-preserving scaling, layout reflow, verified existing source margins and original framing/pillarboxing. If none is satisfactory, propose optional scene-specific extension for review; do not make generated scenery the default or claim it is original game art. Preserve source assets and an original-art mode. No generation or replacement-art implementation is requested now.
