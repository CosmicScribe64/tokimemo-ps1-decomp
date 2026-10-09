---
id: T-0012
title: Game code file boundaries and Shift-JIS strings
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]", "[[tickets/T-0011-game-code-file-boundaries-and-compiler]]"]
---

## Goal

Split `src/game.c` into per-source-file segments and decide how to represent Shift-JIS strings. Split out of [[tickets/T-0011-game-code-file-boundaries-and-compiler]].

## Acceptance criteria

- [ ] File boundaries found (rodata/data/bss ordering per object) and segments added to `config/SLPM_86.053.yaml`.
- [ ] Shift-JIS strings emitted readably without changing bytes.

## Notes

See [[toolchain]] and [[executable]].

## Comments
