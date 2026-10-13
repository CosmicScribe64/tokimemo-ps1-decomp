---
id: T-11060
title: Big functions (Gemini) 29
status: In Progress
assignee: g29
created: 2026-10-13
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "src/ovl/TAIIKU/80138950.c", "src/ovl/DATE/80132000.c", "src/main/8007C030.c", "src/main/800674B0.c"]
---

## Goal

Match big functions from `astra29-list.txt` across `src/ovl/TAIIKU/80138950.c`, `src/ovl/DATE/80132000.c`, `src/main/8007C030.c`, and `src/main/800674B0.c`.

## Acceptance criteria

- [ ] Work through `astra29-list.txt` (biggest first), matching functions or keeping near misses (<30 lines diff) under `#ifdef NON_MATCHING`.
- [ ] `tools/docker.sh ninja` passes 27/27 OK with `build/headers.ok` and `build/globals.ok`.
- [ ] Code review against `CODING_STANDARDS.md` completed and recorded.

## Notes

Owned files:
- `src/ovl/TAIIKU/80138950.c`
- `src/ovl/DATE/80132000.c`
- `src/main/8007C030.c`
- `src/main/800674B0.c`

## Comments
