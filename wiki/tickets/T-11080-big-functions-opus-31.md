---
id: T-11080
title: Big functions (Opus via Antigravity) 31
status: Done
assignee: antigravity-opus (worktree o31)
created: 2026-10-13
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[matching-notes]]"]
---

## Goal

Match big functions from astra list 31 (48 functions, 65,120 bytes) in
`src/ovl/NAME_ENT/80139740.c`, `src/main/8006CB30.c`, `src/ovl/NAME_ENT/80132000.c`,
`src/ovl/TAIIKU/80133C80.c` and `src/ovl/DATE/801461B0.c`.

## Acceptance criteria

- [ ] Every listed function gets an m2c draft, a build and a `funcdiff --resolve`.
- [ ] Matches verified with `funcdiff --resolve` and `tools/docker.sh ninja` (27/27 OK, headers and globals OK).
- [ ] Near misses under ~30 lines kept in `#ifdef NON_MATCHING` with the INCLUDE_ASM fallback.
- [ ] Inline review against CODING_STANDARDS.md recorded here.

## Notes

## Comments

2026-10-17: Closing as superseded. The Opus run stalled with an empty ticket; the merge brought in two active NAME_ENT functions (func_8013F208, func_8014231C, both C on main under the 27/27 rebuild) and two NON_MATCHING drafts without ticket or line count, which later passes removed (T-11570, T-12020) and drafts/ archives. The rest of the 48-function list is covered by T-14500, T-11120 and the wave 1 and 3 packages (T-11570, T-11650, T-19004). Review: no findings in the active code; the stray repeated m2c comments in the same file come from T-11010 and are in the followup opened from T-11060.
