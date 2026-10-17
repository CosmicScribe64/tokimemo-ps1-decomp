---
id: T-11880
title: "DATE2: match the remaining nine functions (one long Opus run per group)"
status: Done
assignee: claude (diman8 fork)
created: 2026-10-17
updated: 2026-10-17
links: ["[[tickets/T-18800-types-first-date2]]", "[[tickets/T-11870-tt-batch-13]]", "[[decompile-workflow]]", "[[data/t0018-cases]]"]
---

## Goal
Outside-contributor claim CosmicScribe64/tokimemo-ps1-decomp#49: the nine remaining DATE2 functions (14,144 bytes),
with the method that finished TT (T-11840..T-11870): one Opus agent per group with a long budget, two at a time in
separate worktrees, never two on the same C file, earlier findings passed on as tips.
1. `80137D50.c`: `func_801388C0`, `func_801385CC`, `func_801395C8`
2. `80132DE0.c` part 1: `func_80136794`, `func_801330D4`, `func_80136860`, `func_801348F0`
3. `80132DE0.c` part 2 (after group 2): `func_801350A4`, `func_80133AF0`

## Acceptance criteria

- [x] All groups run; matches committed one per commit, near misses guarded with `T-11880`
- [x] Inline review against `CODING_STANDARDS.md` (FAKE comment on every fake trick, including every chain store)
- [x] `ninja`: 27/27 `OK`, headers/globals OK; `sync_protos.py --check-branch` and `migrate_globals.py --check` clean

## Results
8 of 9 matched (11,172 bytes), 1 guarded:

| Group | Result |
|---|---|
| 1 (`80137D50.c`) | `func_801388C0`, `func_801385CC` matched (data island `DATE2/80137D50`); `func_801395C8` guarded (84 rows, from 690) |
| 2 (`80132DE0.c` part 1) | `func_80136794`, `func_801330D4`, `func_80136860`, `func_801348F0` matched |
| 3 (`80132DE0.c` part 2) | `func_801350A4`, `func_80133AF0` matched (both new, from DATE twins) |

DATE2 101/102 functions, 91.9% of bytes (from 93/102, 57.8%); grand total `6419/6958 1782736/2279368 78.2%` after
merging current `main`. Notes in [[matching-notes]] ("DATE2 remainder"), 9 rows in [[data/t0018-cases]].

## Review
Inline review against `CODING_STANDARDS.md`: every fake trick carries a `FAKE ... T-11880` comment, including every
chain store used as a matching trick; colour copies `r = g = b = v` stay unmarked, as in the matched DATE twins.
Header changes: hand-written `MAIN_API_OVERRIDE_` guards in `main_api.h` (`D_800B3CFC`, `func_8004F870`,
`func_80083B24`), `s32` return types for three DATE2 prototypes, one block of DATE movie types in `DATE2.h` under
DATE.h's guard name (no file includes both). Config: data island `DATE2/80137D50`. Clean rebuild 27/27 OK,
headers/globals OK, `sync_protos.py --check-branch` and `migrate_globals.py --check` clean. No open findings.

## Comments

2026-10-17, maintainer review (PR #54): no findings; 15 FAKE lines name T-11880 with reasons, the main_api.h overrides follow section 8a, func_801395C8 stays guarded with its ticket. Verified: clean 27/27, headers, globals, prototypes OK, funcdiff MATCH for all 8 functions (11,172 bytes). Status Done.
