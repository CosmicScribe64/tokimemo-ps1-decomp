---
id: T-11100
title: Big functions (Gemini) 33
status: In Review
assignee: g33
created: 2026-10-13
updated: 2026-10-14
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

2026-10-14 fix-up of the stalled `src/main/800674B0.c` drafts (the "no segment" build error was gone after merging main; the build then failed only on the SHA check):
- Matched: `func_80068938` (FAKE: loop stores go through a `GameState *` so uopt reloads `D_800B3748` per trip like the original, plus a `+0x800` base pointer `q`; marked in source) and `func_80068EC0` (draft was already right; it only needed the previous function's size). Full build `build/SLPM_86.053.bin: OK`, 27/27 OK, `headers OK`.
- Guarded under `NON_MATCHING` with `INCLUDE_ASM` fallback (reasons in the source comments): `func_80067870` (142 register diffs, original keeps D_800B5C08 in $a3), `func_800696DC` (44 structural diffs, frame 240 vs 248), `func_8006A2CC` (~115 words, store-triple order per branch; statement reorders only move the constant registers, a record pointer local did not help).
- `funcdiff --resolve` reports "linked function bytes differ" for every function of the unit while the unit does not match (also for untouched ones); use the object-level diff plus the full build.
- Review: pending (CODING_STANDARDS checklist).
