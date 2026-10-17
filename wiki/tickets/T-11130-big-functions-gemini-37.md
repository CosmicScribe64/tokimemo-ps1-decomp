---
id: T-11130
title: Big functions (Gemini) 37
status: Done
assignee: astra37
created: 2026-10-14
updated: 2026-10-17
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

2026-10-17: Closed as superseded. No work was ever committed under T-11130, and its list file astra37-list.txt is not in the repo. The five files were worked by the wave package tickets: T-11590, T-16220 and T-17001 (main/8007C030), T-11670 and T-17004 (main/8006CB30), T-11650 (NAME_ENT/80132000), T-19004 (TAIIKU/80133C80), T-19008 and T-11280 (TAIIKU/80138950), plus the T-20900 type work. The functions still INCLUDE_ASM or guarded in those files fall under T-14500 (draft and score every remaining game function).
