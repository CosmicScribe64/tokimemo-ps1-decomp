---
id: T-11100
title: Big functions (Gemini) 33
status: In Progress
assignee: g33
created: 2026-10-13
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "src/ovl/TAIIKU/80138950.c", "src/main/8007C030.c", "src/main/8006CB30.c", "src/main/800674B0.c"]
---

## Goal

Match big functions from `astra33-list.txt` across `src/ovl/TAIIKU/80138950.c`, `src/main/8007C030.c`, `src/main/8006CB30.c`, and `src/main/800674B0.c`.

## Acceptance criteria

- [ ] Work through `astra33-list.txt` (biggest first), matching functions or keeping near misses (<30 lines diff) under `#ifdef NON_MATCHING`.
- [ ] `tools/docker.sh ninja` passes 27/27 OK with `build/headers.ok` and `build/globals.ok`.
- [ ] Code review against `CODING_STANDARDS.md` completed and recorded.

## Notes

Owned files:
- `src/ovl/TAIIKU/80138950.c`
- `src/main/8007C030.c`
- `src/main/8006CB30.c`
- `src/main/800674B0.c`

## Comments
