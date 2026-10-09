---
id: T-0011
title: Game code file boundaries, compiler confirmation, Shift-JIS strings
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[overview]]"]
---

## Goal

Split `src/game.c` into per-source-file segments, confirm the compiler/assembler version on the first real C function, and decide how to represent Shift-JIS strings.

## Acceptance criteria

- [ ] File boundaries found (rodata/data/bss ordering per object) and segments added to `config/SLPM_86.053.yaml`.
- [ ] Compiler (2.7.2-psx provisional) and `--aspsx-version` confirmed by a matching function.
- [ ] Shift-JIS strings emitted readably without changing bytes.

## Notes

See [[toolchain]] and [[executable]].

## Comments
