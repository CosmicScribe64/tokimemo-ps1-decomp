---
id: T-29005
title: "Present original room, HUD and activity animation"
status: In Progress
assignee: Codex
created: 2026-10-25
updated: 2026-10-25
links: ["[[phase3]]", "[[tickets/T-29000-plan-godot-port]]"]
blocked-by: [T-29001, T-29002, T-29003]
---

## Goal

Display the first activity with original content, native logical layout and tick-driven composition that supplies correct completion events.

## Owned paths

- `port/godot/presentation/`
- `port/godot/ui/`
- `port/godot/app/`
- `port/godot/tests/render/`

## Acceptance criteria

- [ ] Load original required assets from the validated external route pack; no copyrighted payload in project or player packages.
- [ ] Implement the native logical canvas, ordering, crop/origin/parts/CLUT/bit15/flip semantics required by the slice; distinguish actual art from diagnostic masks.
- [ ] Prove selected SD sequence operations and completion events; preserve shared native RNG even if rendering is hidden or interpolated.
- [ ] Support actual keyboard/controller choice and native glyph HUD; keep semantic text/control/layout layers separate and exercise a synthetic long-string policy.
- [ ] Rendered comparisons use a real Docker rendering backend, original captures and bounded color tolerances; headless tests or placeholder rectangles do not establish presentation fidelity.
- [ ] Independent Standards and spec review findings resolved; actual evidence recorded before Done.

## Notes

Suggested owner: gpt-6.1-sol for the first composition/timing pass, with Luna on later bounded UI scenes. Defer widescreen, replacement art and translated content. Coordinate shared FrameResult/input contracts with T-29002 and activity gates with T-29004. Root serializes shared source applications, wiki, staging and commits. Original inputs stay immutable; real exports and captures remain ignored.

## Comments

2026-10-25: created from the independently reviewed Astra Phase 3 plan. No implementation or runtime validation is claimed by this ticket.

2026-10-25: start the first playable implementation in parallel, as the user requested faster progress. Reviewed native core V2 and real April 4, 1995 command boundary now provide fixed starting contracts; content work supplies only route-required assets. Parent runtime/content tickets retain broader open criteria, so full acceptance still waits on their applicable route gates. Workers prepare ignored disjoint packets; root alone integrates source.
