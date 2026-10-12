---
id: T-10310
title: "Wave 12: list 1"
status: Done
assignee: w12-1
created: 2026-10-11
updated: 2026-10-12
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match as many as possible of the 163 functions assigned in scratchpad/wave12-list-1.txt, within the 49 owned C files.

## Acceptance criteria

- [x] First pass (m2c draft, build, diff) over the list; deep passes on near misses.
- [x] Final incremental build passes all 27 unit checks and header/global guards.
- [x] Inline review against CODING_STANDARDS.md.

## Comments

- 2026-10-12: Result: 2 matches, BUNKA_SD `func_80132708` (220 bytes, FAKE dead test) and SHUGAKU `func_80139740` (304 bytes, FAKE `s32 pad[2]`, narrow `u8 t` found by the permuter); 9 guarded near misses (RPG_BAT 4 lines, GEKO 7, TACO 9 and 12, ETC 20, main 18 and 34, TACO 29); details and T-0018 rows in [[matching-notes]] and [[data/t0018-cases]]. Data islands built for `main/8007C030` and `TAIIKU/80138950` (`D_800CA2A4` is now `u8[4]`, `D_80149984` an `s16[2]` defined in the file).
- Inline review against CODING_STANDARDS.md: C89 declarations at block tops, `/* */` comments only, every `NON_MATCHING` block carries T-10310 and its INCLUDE_ASM fallback, every trick (dead test, pad locals, narrow temporary) is marked FAKE, no main-exe declarations outside `main_api.h` (changed there: `D_800CA2A4[4]` only), overlay declarations appended to `include/ovl/*.h`. All 27 unit checksums, `build/headers.ok`, `build/globals.ok`, `sync_protos --check-branch` and `migrate_globals --check` pass. No findings open.
