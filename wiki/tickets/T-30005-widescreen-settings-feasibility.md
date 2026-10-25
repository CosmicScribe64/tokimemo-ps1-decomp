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
