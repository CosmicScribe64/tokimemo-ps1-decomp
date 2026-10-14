---
id: T-11130
title: Big functions (Gemini) 37
status: In Progress
assignee: astra37
created: 2026-10-14
updated: 2026-10-14
links: ["[[decompile-workflow]]", "[[register-cheatsheet]]", "[[matching-notes]]"]
---

## Goal

Match or produce clean guarded near-miss C (`< 30` diff lines) for the 52 functions in `astra37-list.txt` across owned files:
- `src/ovl/TAIIKU/80138950.c`
- `src/main/8007C030.c`
- `src/main/8006CB30.c`
- `src/ovl/NAME_ENT/80132000.c`
- `src/ovl/TAIIKU/80133C80.c`

## Acceptance criteria

- [ ] Work through `astra37-list.txt` (biggest first).
- [ ] Keep near misses under ~30 lines in `#ifdef NON_MATCHING` with `INCLUDE_ASM` fallback.
- [ ] All 27 targets build and match (`tools/docker.sh ninja`), `build/headers.ok` and `build/globals.ok` pass.
- [ ] Code review against `CODING_STANDARDS.md` completed.

## Notes

## Comments
