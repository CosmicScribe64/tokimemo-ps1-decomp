---
id: T-0101
title: Stop splat from overwriting include/include_asm.h
status: Done
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

## Resolution (2026-10-09, orchestrator)
Already fixed on main by T-0008: `generate_asm_macros_files: False` is set in config/SLPM_86.053.yaml and every config/overlays/*.yaml (checked with grep). No new code, so the review gate has nothing to check. Closed on merge.
