---
id: T-39230
title: Add a locked Parity mode and parity test gate
status: In Progress
assignee: Claude (worker)
created: 2026-10-30
updated: 2026-10-30
links: ["[[phase4-design-2026-10-29]]", "[[port-testing]]", "[[tickets/T-30002-modern-settings]]"]
---

## Goal

User request: a parity ("true") mode for playing and testing that guarantees core gameplay is unchanged, while modern features keep growing outside it.

## Acceptance criteria

- [ ] Top-level Parity mode, selectable at first launch, in settings and from the command line or environment. It locks every enhancement off (not just a preset), shows a visible badge, and is persisted.
- [ ] Feature registry: every modern feature declares a class (presentation-only, input/UX, or game-affecting, e.g. fast-forward, save manager, skip). Parity mode refuses anything active outside the registry. A test fails if a preference key or feature is unregistered.
- [ ] Parity gate: the opening frame-timing route, emulator differential runs, golden comparisons and the long-route playtest run in Parity mode by default and fail if parity is not active.
- [ ] Modern-vs-parity invariance: the same input script under "everything on" must produce identical game state and native frames. Game-affecting features are explicitly excluded or have their own declared invariants.
- [ ] Docs in the wiki; review against CODING_STANDARDS.md.

## Notes

## Comments
