---
id: T-39213
title: English WP-D: Latin name typing over the native Symbols page
status: In Review
assignee: Claude (phase 4 root, Lane A)
created: 2026-10-30
updated: 2026-10-30
links: ["[[english-names]]", "[[phase4-design-2026-10-29]]"]
---

## Goal

In English mode, typed Latin letters, digits and listed punctuation become native Symbols-page actions (tab, scroll, cell), so every stored byte is the original full-width code unit.

## Acceptance criteria

- [ ] `run_name_entry_latin_tests.gd` passes.
- [ ] new_game_entry_boot wires the hooks (root hook 7) and English mode reaches the Latin page in a live run.
- [ ] Independent review against CODING_STANDARDS.md with findings resolved.

## Notes

Root hook 7 is wired in `port/godot/app/new_game_entry_boot.gd` by the Lane A integration (2026-10-30); see [[tickets/T-39060-phase4-wave1-root-hooks]].

## Comments
