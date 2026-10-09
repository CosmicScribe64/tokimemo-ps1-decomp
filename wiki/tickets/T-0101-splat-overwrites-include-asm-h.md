---
id: T-0101
title: Stop splat from overwriting include/include_asm.h
status: Backlog
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[build-system]]"]
---

## Goal

In a fresh checkout, the first `splat split` (run by `ninja`) replaces the committed `include/include_asm.h` with splat's generated version, which lacks the `__sgi` guard, so the IDO compile of `src/game.c` fails. Make the split leave the committed header alone (splat option or a different header path), so a clean `ninja` works first time.

## Acceptance criteria

- [ ] Fresh worktree: `tools/docker.sh sh -c 'python3 configure.py && ninja'` ends with `build/SLPM_86.053.bin: OK` and `git status` shows no change to `include/`.

## Notes

Found in T-0014. Workaround: `git checkout include/include_asm.h`, then `ninja` again ([[build-system]], Gotchas).

## Comments
