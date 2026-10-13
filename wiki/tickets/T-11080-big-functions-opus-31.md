---
id: T-11080
title: Big functions (Opus via Antigravity) 31
status: In Progress
assignee: antigravity-opus (worktree o31)
created: 2026-10-13
updated: 2026-10-13
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
